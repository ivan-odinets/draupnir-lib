#!/usr/bin/perl

use Cwd;
use Getopt::Long;
use File::Basename;
use File::Find;
use File::Path qw(make_path);
use File::Path qw(remove_tree);
use File::Spec;
use feature 'say';
use strict;
use warnings;

# =====================================================================================================================
# General helper function declarations

sub say_empty;
sub say_result;

sub say_debug;
sub say_info;
sub say_warning;
sub say_error;
sub say_fatal;

# =================================================================================================
# Script command executors & general helpers

sub get_repo;
sub get_repo_subpath;
sub git;
sub list_jobs;
sub help;

# =================================================================================================
# Global constants

use constant {
    JOB_SUCCESS        => 0,
    JOB_FAILURE        => 1,
    TOTAL_TESTS        => 'total',
    TEST_PASSED        => 'passed',
    TEST_FAILED        => 'failed',
    TEST_MISCONFIGURED => 'misconfigured',
    TEST_SKIPPED       => 'skipped',
};

# =================================================================================================
# Possible script command line arguments & script state

my $debug                  = 0;
my $job_list_requested     = 0;
my $cleanup_before         = 0;
my $job_name               = '';
my $repository_config_path = '';

my $repo = undef;

# =================================================================================================
# "sub main"

Getopt::Long::Configure('no_auto_abbrev', 'no_ignore_case', 'bundling');
GetOptions(
    'config|c=s'      => \$repository_config_path,
    'debug|d'         => \$debug,
    'job|j=s'         => \$job_name,
    'cleanup-before'  => \$cleanup_before,
    'list-jobs|l'     => \$job_list_requested,
    'help|h'          => sub { help(); exit 0; }
) or do {
    help();
    exit 1;
};

say_fatal "Either -j / --job or -l / list-jobs parameter must be specified."
    if ( (!$job_list_requested) && ($job_name eq '') );

my $error_message = undef;

($repo, $error_message) = get_repo($repository_config_path);
say_fatal($error_message)
    if defined $error_message;

# Lets print some information about the repository.
say_info "Repository:   ".$repo->{name};
say_info "Branch:       ".git "rev-parse --abbrev-ref HEAD";
say_info "Last commit:  ".git "rev-parse HEAD";
say "";

# And even more information if -d / --debug was specified
if ($debug) {
    say_debug "Using following repo config: ".$repo->{config};
    say_empty "Repository root: ".$repo->{absolute_path};
    say_empty "Jobs:            ".get_repo_subpath($repo->{jobs});
    say_empty "Tests:           ".get_repo_subpath($repo->{tests});
    say_empty "Tests builds:    ".get_repo_subpath($repo->{tests_build});
    say "";
}

if ($job_list_requested) {
    list_jobs();
    exit 0;
}

# By now if -l is not specified, we have -j <job> specified. So we should run a job
exit(run_test_job($job_name));

# =================================================================================================
# Two functions related to the current repository:
# - sub get_repo - uses repository config specified via command line and reads it.
# - get_repo_subpath - transforms specified absolute path into the subpath within current repo.

sub get_repo {
    my ($repository_config_path) = @_;
    my $git_repo_path = git "rev-parse --show-toplevel";

    if ($repository_config_path eq '') {
        $repository_config_path = File::Spec->catfile($git_repo_path,"repository_config.pm");
        say_info "Repository config was not specified. Using: ".$repository_config_path;
    }

    return (undef, "Repository config not found under $repository_config_path.") unless -f $repository_config_path;

    # Repo config found, but is it correct?
    my $repo_config = do $repository_config_path;

    return (undef, "Syntax error in '$repository_config_path': $@") if ($@);
    return (undef, "Could not read '$repository_config_path': $!") unless defined $repo_config;
    return (undef, "Config file '$repository_config_path' did not return a hash ref.") if ref($repo_config) ne 'HASH';
    return (undef, "Config  file '$repository_config_path' did not contain a valid repository name.") unless defined $repo_config->{name};
    return (undef, "Config field 'name' must be a non-empty string.") if $repo_config->{name} eq "";

    my $result = {};
    $result->{name} = $repo_config->{name};
    $result->{config} = $repository_config_path;

    if (basename($git_repo_path) ne $result->{name}) {
        say_debug "git rev-parse --show-toplevel: ".$git_repo_path if $debug;
        return (undef, "This script should be executed within the ".$result->{name}." source tree.");
    };

    $result->{absolute_path} = $git_repo_path;
    return (undef, "Repository path: '".$result->{absolute_path}."' not found.")
        unless ( -d $result->{absolute_path});

    # Path is valid. What about some other dirs:
    # For example jobs?
    if (!defined $repo_config->{jobs}) {
        say_debug "Jobs dir is not defined. Will use default: <repo-name>/jobs" if $debug;
        $repo_config->{jobs} = "jobs";
    } elsif ($repo_config->{jobs} eq "") {
        return (undef, "Config field 'jobs' must be a non-empty string.");
    }

    $result->{jobs} = File::Spec->catdir($result->{absolute_path}, $repo_config->{jobs});
    return (undef, "Job path: '".$result->{jobs}."' not found.") unless (-d $result->{jobs});

    # Or tests?
    if (!defined $repo_config->{tests}) {
        say_debug "Tests dir is not defined. Will use default: <repo-name>/tests" if $debug;
        $repo_config->{tests} = "tests";
    } elsif ($repo_config->{tests} eq "") {
        return (undef, "Config field 'tests' must be a non-empty string.");
    }

    $result->{tests} = File::Spec->catdir($result->{absolute_path}, $repo_config->{tests});
    return (undef, "Tests path: '".$result->{tests}."' not found.") unless (-d $result->{tests});

    # Or test builds?
    if (!defined $repo_config->{tests_build}) {
        say_debug "Tests build dir is not defined. Will use default: <repo-name>/tests/build" if $debug;
        $repo_config->{tests_build} = File::Spec->catdir("tests","build");
    } elsif ($repo_config->{tests_build} eq "") {
        return (undef, "Config field 'tests_build' must be a non-empty string.");
    }

    $result->{tests_build} = File::Spec->catdir($result->{absolute_path}, $repo_config->{tests_build});

    return ($result, undef);
};

sub get_repo_subpath { return File::Spec->abs2rel($_[0],$repo->{absolute_path}); };

# =====================================================================================================================
# Reading and parsing job files:
# - sub read_job_file - reads specified *.pm job file and validate its structure
# - sub prepare_job - uses job file which was parsed by 'read_job_file' and updates it to contain valid paths and
#                     config. Repository details are obtained from global $repo variable.

sub read_job_file {
    my ($job_file) = @_;
    return (undef, "File $job_file is not existing.") unless -f $job_file;

    my $result = do $job_file;
    return (undef, "Syntax error in '$job_file': $@") if $@;
    return (undef, "Could not read '$job_file': $!") unless defined $result;
    return (undef, "Job file '$job_file' did not return a hash ref.") if (ref($result) ne 'HASH');

    return (undef, "Job file '$job_file' field 'runtime_tests' must be an array.")
        if (defined $result->{runtime_tests} && ref($result->{runtime_tests}) ne 'ARRAY');

    return (undef, "Job file '$job_file' field 'compile_tests' must be an array.")
        if (defined $result->{compile_tests} && ref($result->{compile_tests}) ne 'ARRAY');

    return (undef, "Job file '$job_file' field 'skipped_tests' must be an array.")
        if (defined $result->{skipped_tests} && ref($result->{skipped_tests}) ne 'ARRAY');

    return (undef, "Job file '$job_file' field 'must_crash_tests' must be an array.")
        if (defined $result->{must_crash_tests} && ref($result->{must_crash_tests}) ne 'ARRAY');

    return (undef, "Job file '$job_file' should contain at least one of 'runtime_tests' or 'compile_tests' arrays.")
        unless (defined $result->{runtime_tests} || defined $result->{compile_tests});

    return ($result, undef);
}

sub prepare_job {
    my ($job_name) = @_;
    my ($job_config, $error_message) = read_job_file(File::Spec->catfile($repo->{jobs},$job_name.".pm"));
    return (undef, $error_message) unless defined($job_config);

    $job_config->{name} = $job_name;

    my $absolute_logs_directory = File::Spec->catdir($repo->{tests_build},$job_name,"logs");
    $job_config->{logs}->{dir} = $absolute_logs_directory;
    make_path($absolute_logs_directory) unless -d $absolute_logs_directory;

    if (defined($job_config->{compile_tests})) {
        $_ = File::Spec->catdir($repo->{tests},$_) for @{$job_config->{compile_tests}};

        $job_config->{logs}->{compile_tests} = File::Spec->catdir($absolute_logs_directory,"compile");
        make_path($job_config->{logs}->{compile_tests}) unless -d $job_config->{logs}->{compile_tests};
    }

    if (defined($job_config->{runtime_tests})) {
        $_ = File::Spec->catdir($repo->{tests},$_) for @{$job_config->{runtime_tests}};

        $job_config->{logs}->{runtime_failed_build} = File::Spec->catfile($absolute_logs_directory, "runtime_failed_build.log");
        $job_config->{logs}->{runtime_failed_execution} = File::Spec->catfile($absolute_logs_directory, "runtime_failed_execution.log");
    }

    return ($job_config, undef);
}

# =====================================================================================================================
# sub run_test_job

sub run_test_job {
    (my $job_name) = @_;

    say_info "Running job: $job_name";
    if ($cleanup_before) {
        my $job_build_directory = File::Spec->catdir($repo->{tests_build},$job_name);
        say_info "Cleaning build directory: ".get_repo_subpath($job_build_directory);
        remove_tree($job_build_directory)
            or say_fatal "Failed to remove directory '$job_build_directory': $!";
        make_path($job_build_directory)
            or say_fatal "Failed to recreate directory '$job_build_directory': $!";
    }
    say "" ;

    (my $job_config, my $error_message) = prepare_job($job_name);
    say_fatal $error_message unless defined $job_config;

    if ($debug) {
        say_debug "Job config:";

        if (defined($job_config->{compile_tests})) {
            say_empty "Compilation tests paths: ".join(',',(map { get_repo_subpath($_) } @{$job_config->{compile_tests}}));
            say_empty "Compilation tests logs:  ".get_repo_subpath($job_config->{logs}->{compile_tests});
        } else {
            say_empty "No compilation tests configured.";
        }

        if (defined($job_config->{runtime_tests})) {
            say_empty "Runtime tests paths:              ".join(',',(map { get_repo_subpath($_) } @{$job_config->{runtime_tests}}));
            say_empty "Runtime tests build failures log: ".get_repo_subpath($job_config->{logs}->{runtime_failed_build});
            say_empty "Runtime tests run failures log:   ".get_repo_subpath($job_config->{logs}->{runtime_failed_execution});
        } else {
            say_empty "No runtime tests configured.";
        }
        say "";
    };

    my $job_results = {};

    $job_results->{compile_tests} = run_compile_tests($job_config)
        if (defined($job_config->{compile_tests}));

    $job_results->{runtime_tests} = run_runtime_tests($job_config)
        if (defined($job_config->{runtime_tests}));

    my $failed_subtasks = 0;

    say_info "Job results:";
    if (defined($job_results->{compile_tests})) {
        say_info "Compilation tests.";
        say_empty "Total:         ".$job_results->{compile_tests}->{TOTAL_TESTS()};
        say_empty "Passed:        ".$job_results->{compile_tests}->{TEST_PASSED()};
        say_empty "Failed:        ".$job_results->{compile_tests}->{TEST_FAILED()};
        say_empty "Misconfigured: ".$job_results->{compile_tests}->{TEST_MISCONFIGURED()};
        if (($job_results->{compile_tests}->{TEST_MISCONFIGURED()} != 0) || ($job_results->{compile_tests}->{TEST_FAILED()} != 0)) {
            $failed_subtasks++;
        }
    } else {
        say_info "No compilation tests configured.";
    }

    if (defined($job_results->{runtime_tests})) {
        say_info "Runtime tests:";
        say_empty "Total:         ".$job_results->{runtime_tests}->{TOTAL_TESTS()};
        say_empty "Passed:        ".$job_results->{runtime_tests}->{TEST_PASSED()};
        say_empty "Failed:        ".$job_results->{runtime_tests}->{TEST_FAILED()};
        say_empty "Skipped:       ".$job_results->{runtime_tests}->{TEST_SKIPPED()};
        say_empty "Misconfigured: ".$job_results->{runtime_tests}->{TEST_MISCONFIGURED()};
        if (($job_results->{runtime_tests}->{TEST_MISCONFIGURED()} != 0) || ($job_results->{runtime_tests}->{TEST_FAILED()} != 0)) {
            $failed_subtasks++;
        }
    } else {
        say_info "No runtime tests configured.";
    }
    say "";

    say_result($failed_subtasks == 0, "Result.");
    return ($failed_subtasks == 0 ? JOB_SUCCESS() : JOB_FAILURE());
}

# =====================================================================================================================
# sub run_runtime_tests - will build and run all runtime tests

sub run_runtime_tests {
    my ($job_config) = @_;
    say_info "Running runtime tests.";

    my @all_tests = map {
        my @files;
        say_fatal "Test directory ".get_repo_subpath($_)." is not existing. " unless -d $_;
        find( sub { return unless (-f $_) && (/(?:Test|IT)\.pro$/i); push @files, $File::Find::name; }, $_ );
        @files;
    } @{ $job_config->{runtime_tests} };

    my %all_tests_names_set  = map { basename(dirname($_)) => 1 } @all_tests;
    my %skipped_tests_set    = map { $_ => 1 } @{$job_config->{skipped_tests}};
    my %must_crash_tests_set = map { $_ => 1 } @{$job_config->{must_crash_tests}};

    my $result = {
        TOTAL_TESTS()        => 0,
        TEST_PASSED()        => 0,
        TEST_FAILED()        => 0,
        TEST_MISCONFIGURED() => 0,
        TEST_SKIPPED()       => 0
    };

    say_warning("Test $_ is present within 'skipped_tests', but was not found within 'runtime_tests'."), $result->{TEST_MISCONFIGURED()}++
        for grep { !exists($all_tests_names_set{$_}) } keys %skipped_tests_set;
    say_warning("Test $_ is present within 'must_crash_tests', but was not found within 'runtime_tests'."), $result->{TEST_MISCONFIGURED()}++
        for grep { !exists($all_tests_names_set{$_}) } keys %must_crash_tests_set;

    if ($result->{TEST_MISCONFIGURED()} != 0) {
        say_warning "There are misconfigured runtime tests. Skipping.";
        return $result;
    }

    my $binaries_dir = File::Spec->catdir($repo->{tests_build},$job_config->{name},"bin");
    my $runtime_failed_build_handle     = undef;
    my $runtime_failed_execution_handle = undef;

    $result->{TOTAL_TESTS()}  = scalar(@all_tests);
    $result->{TEST_SKIPPED()} = scalar(@{$job_config->{skipped_tests}});

    # As within for loop we will do some chdir-ing - lets same current direcrtory and restore it later.
    my $old_cwd = getcwd();

    for my $test_path (@all_tests) {
        my $test_name = basename($test_path) =~ s/\.[^.]+$//r;
        if (exists $skipped_tests_set{$test_name}) {
            say_info "Skipping:  $test_name";
            next;
        }

        my $test_dir  = dirname($test_path);
        chdir $test_dir or say_fatal "Cannot chdir to '$test_dir': $!";

        my $output = `( qmake JOB_NAME="$job_config->{name}" "$test_name".pro && make ) 2>&1`;
        my $exit_code = $? >> 8;
        say_result($exit_code == 0, "Building: $test_name");
        if ($exit_code != 0) {
            $result->{TEST_FAILED()}++;
            open($runtime_failed_build_handle, '>', $job_config->{logs}->{runtime_failed_build})
                or say_fatal "Cannot open '".$job_config->{logs}->{runtime_failed_build}."': $!"
                unless defined $runtime_failed_build_handle;

            say {$runtime_failed_build_handle} "********* Building $test_name *********";
            say {$runtime_failed_build_handle} $output;
            say {$runtime_failed_build_handle} "********* End of qmake && make output *********";
            next;
        };

        my $binary = File::Spec->catfile($binaries_dir,$test_name);
        $output = `$binary 2>&1`;

        $exit_code = $? >> 8;
        my $signal    = $? & 127;

        # exists $must_crash_tests_set{}
        my $should_crash = exists $must_crash_tests_set{$test_name};
        my $passed = (
            ($exit_code == 0 && $signal == 0 && (!$should_crash)) ||
            ((($exit_code != 0) || ($signal != 0)) && $should_crash)
        );

        say_result($passed,"Running:  $test_name");
        $result->{$passed ? TEST_PASSED() : TEST_FAILED()}++;

        if (!$passed) {
            open($runtime_failed_execution_handle, '>', $job_config->{logs}->{runtime_failed_execution})
                or say_fatal "Cannot open '".$job_config->{logs}->{runtime_failed_execution}."': $!"
                unless defined($runtime_failed_execution_handle);

            say {$runtime_failed_execution_handle} $output;
        }
    }

    say "";

    close($runtime_failed_build_handle)
        or say_fatal "Cannot close '".$job_config->{logs}->{runtime_failed_build}."': $!"
        if defined $runtime_failed_build_handle;

    close($runtime_failed_execution_handle)
        or say_fatal "Cannot close '".$job_config->{logs}->{runtime_failed_execution}."': $!"
        if defined $runtime_failed_execution_handle;

    chdir($old_cwd) or say_fatal("Cannot chdir back to $old_cwd");

    return $result;
}

sub read_compile_test_config {
    my ($compile_test_file) = @_;
    return (undef, "Compile test config file '$compile_test_file' does not exist.")
        unless -f $compile_test_file;

    my $compile_test_config = do $compile_test_file;
    return (undef, "Syntax error in '$compile_test_file': $@")
        if $@;
    return (undef, "Could not read '$compile_test_file': $!")
        unless defined $compile_test_config;
    return (undef, "Compile test config file '$compile_test_file' did not return a hash ref.")
        unless (ref($compile_test_config) eq 'HASH');
    return (undef, "Compile test config file '$compile_test_file' does not contain 'test_cases' hash.")
        unless defined($compile_test_config->{test_cases});
    return (undef, "'test_cases' must be a hash")
        unless (ref($compile_test_config->{test_cases}) eq 'HASH');

    return ($compile_test_config, undef);
};

sub run_compile_test_case {
    (my $job_config, my $compile_test_config, my $test_case) = @_;

    my $test_case_directory = File::Spec->catdir($compile_test_config->{path},$test_case);
    if (! -d $test_case_directory) {
        say_warning "Test case dir: ".get_repo_subpath($test_case_directory)." does not exist";
        return TEST_MISCONFIGURED();
    }

    # Compile test case is considered valid if:
    # - It contains main.cpp file
    do { say_warning "Test case $test_case does not contain 'main.cpp' file."; return TEST_MISCONFIGURED(); }
        unless -f File::Spec->catfile($test_case_directory,"main.cpp");
    # - It contains $test_case.pro file
    do { say_warning "Test case $test_case does not contain '$test_case.pro' file."; return TEST_MISCONFIGURED(); }
        unless -f File::Spec->catfile($test_case_directory,"$test_case.pro");
    # - If test case config has niether error string nor error array - we are failed
    my $failure_message = $compile_test_config->{test_cases}->{$test_case};
    do { say_warning "Test case $test_case config does not contain niether error string nor error array"; return TEST_MISCONFIGURED(); }
        unless (defined($failure_message) && (!ref($failure_message) || ref($failure_message) eq 'ARRAY'));
    # - Well and if we cannot enter the test case directory - this is also wrong
    my $old_cwd = getcwd();
    do { say_warning "Cannot chdir into $test_case_directory"; return TEST_MISCONFIGURED(); }
        unless chdir($test_case_directory);

    # Lets run the test.
    my $output = `( qmake && make ) 2>&1`;
    my $exit_code = $? >> 8;

    chdir $old_cwd or say_fatal "Cannot go back to directory $old_cwd";

    # And check what we received:
    my $expected_failure = defined $failure_message;
    my $compilation_failed = ($exit_code != 0);

    my $output_is_valid = 0;
    if (ref($failure_message) eq 'ARRAY') {
        my $matched_parts = 0;
        for my $part (@{$failure_message}) {
            $matched_parts++ if (index($output, $part) != -1);
        }
        $output_is_valid = ($matched_parts == scalar(@{$failure_message}));
    } elsif (ref($failure_message) ne 'HASH') {
        $output_is_valid = ($failure_message eq "") ? 1 :
            index($output, $failure_message) != -1;
    }

    my $passed =
        (!$expected_failure && !$compilation_failed) ||
        ($expected_failure &&  $compilation_failed && $output_is_valid);

    # Cleanup. For Compilation tests we must have main.cpp and *.pro file. All other stuff can be removed.
    opendir(my $dh, $test_case_directory) or die "Cannot open '$test_case_directory': $!";
    unlink map { File::Spec->catfile($test_case_directory, $_) }
           grep { $_ ne '.' && $_ ne '..' && $_ ne 'main.cpp' && $_ ne "$test_case.pro" }
           readdir($dh);
    closedir($dh);

    say_result($passed,$test_case) if $debug;
    if (!$passed) {
        my $log_file = File::Spec->catfile($compile_test_config->{log_dir},$test_case.".log");
        open(my $handle, '>', $log_file) or say_fatal "Cannot open '$log_file': $!";
        say {$handle} $output;
        close($handle);
        return TEST_FAILED();
    } else {
        return TEST_PASSED();
    }
}

sub run_compile_test {
    (my $job_config, my $compile_test) = @_;
    my $relative_test_path = get_repo_subpath($compile_test);

    # Running discovered compile test
    my ($compile_test_config, $error_message) = read_compile_test_config($compile_test);
    unless (defined($compile_test_config)) {
        say_warning "Wrong compile test config: ".get_repo_subpath($compile_test);
        return TEST_MISCONFIGURED();
    }
    my $compile_test_directory = dirname($compile_test);

    # Lets make sure that all test cases are configured properly
    opendir(my $dh, $compile_test_directory) or die "Cannot open '$compile_test_directory': $!";
    my %actual_test_cases =
        map { $_ => 1 }
        grep { $_ ne '.' && $_ ne '..' && -d File::Spec->catdir($compile_test_directory, $_) }
        readdir($dh);
    closedir($dh);

    my $misconfigured_test_cases = 0;
    say_warning("Test case $_ is configured within $relative_test_path, but not present within test directory."), $misconfigured_test_cases++
        for grep { !exists $actual_test_cases{$_} } keys %{$compile_test_config->{test_cases}};
    say_warning("Test case $_ is present within test directory, but not configured within $relative_test_path."), $misconfigured_test_cases++
        for grep { !exists $compile_test_config->{test_cases}->{$_} } keys %actual_test_cases;

    if ($misconfigured_test_cases != 0) {
        say_warning("Test $relative_test_path has $misconfigured_test_cases misconfigured test cases. Skipping.");
        return TEST_MISCONFIGURED();
    }

    # Lets update test_config
    $compile_test_config->{name} = basename($compile_test) =~ s/\.[^.]+$//r;
    $compile_test_config->{path} = $compile_test_directory;
    $compile_test_config->{log_dir} = File::Spec->catdir($job_config->{logs}->{compile_tests},$compile_test_config->{name});
    make_path($compile_test_config->{log_dir}) unless -d $compile_test_config->{log_dir};

    my $result = {
        TOTAL_TESTS()        => scalar keys %{$compile_test_config->{test_cases}},
        TEST_PASSED()        => 0,
        TEST_FAILED()        => 0,
        TEST_MISCONFIGURED() => 0
    };

    say_debug "Testing: ".get_repo_subpath($compile_test) if $debug;
    $result->{ run_compile_test_case($job_config,$compile_test_config,$_) }++
        for keys %{$compile_test_config->{test_cases}};

    my $passed =
        ($result->{TEST_FAILED()} == 0) &&
        ($result->{TEST_MISCONFIGURED()} == 0) &&
        ($result->{TOTAL_TESTS()} != 0) &&
        ($result->{TOTAL_TESTS()} == ($result->{TEST_PASSED()} + $result->{TEST_FAILED()} + $result->{TEST_MISCONFIGURED()}));

    say_result $passed,"Compilation test: ".$compile_test_config->{name};
    if ($debug) {
        say_empty "Cases passed:        ".$result->{TEST_PASSED()};
        say_empty "Cases failed:        ".$result->{TEST_FAILED()};
        say_empty "Cases misconfigured: ".$result->{TEST_MISCONFIGURED()};
        say "";
    }

    return TEST_PASSED() if $passed;
    return TEST_MISCONFIGURED() if ($result->{TEST_MISCONFIGURED()} != 0);
    return TEST_FAILED();
}

# =====================================================================================================================
# 1. Compile test structure:
# <test_name>/          <- test root directory
#   <test_name>.pm      <- test description (e.g. collection of test_cases and their expected statuses)
#   <test_case1>/       <- test case directory
#     main.cpp          <- main.cpp file
#     <test_case1>.pro  <- qmake file for building the tests.
#   <test_case2>/
#     ...
#
# 2. Compile test location(s)
# Within job.pm file there may be an array of 'compile_tests' relative paths. These paths should be relative to the
# 'tests' directory specified in repo config. When calling 'sub run_compile_tests' job_config provided should contain
# array with absolute paths to the directories with Compilation tests.

sub run_compile_tests {
    my ($job_config) = @_;

    say_info "Compilation tests.";

    # Lets find all available Compilation tests. They are defined by the *.pm files
    my @compile_tests = ();
    for my $dir (@{$job_config->{compile_tests}}) {
        my $absolute_dir = File::Spec->catdir($dir);
        say_fatal "Compilation tests directory does not exist: ".get_repo_subpath($absolute_dir) unless -d $absolute_dir;
        find( sub { return unless -f $_ && /\.pm$/i; push @compile_tests, $File::Find::name; }, $absolute_dir );
    }

    # Some counters, to be used within the job stats printout and so on.
    my $result = {
        TOTAL_TESTS()          => scalar(@compile_tests),
        TEST_PASSED()          => 0,
        TEST_FAILED()          => 0,
        TEST_MISCONFIGURED()   => 0
    };

    # run_compile_test will return one of: TEST_PASSED / TEST_FAILED / TEST_MISCONFIGURED
    $result->{ run_compile_test($job_config, $_) }++
        for @compile_tests;

    return $result;
};

# =====================================================================================================================
# sub list_jobs - lists jobs available within <repo-root>/jobs directory.

sub list_jobs {
    my @job_files = glob(File::Spec->catfile($repo->{jobs}, '*.pm'));
    return say_info "No jobs found." unless @job_files;

    say_info "Available jobs:";
    say_empty(
        (defined((read_job_file($_))[0]) ? basename($_) : "\e[31m".basename($_)."\e[0m")
    ) for (@job_files);
}

# =====================================================================================================================
# sub help - print help and exit 0.
# TODO: Extend help message

sub help {
    say "Usage:";
    say "    ".basename($0)." -config <path/to/config.pm> --job <job-name> [--debug]";
    say "";
    say "Options:";
    say "  -c, --config           Specifies path to the repository config.";
    say "  -j, --job              Job name";
    say "  --cleanup-before       When specified - will clean the job build directory before ";
    say "                         starting building the job.";
    say "  -l, --list-jobs        List jobs";
    say "  -d, --debug            Enable debug output for this script";
    say "  -h, --help             Show this help message";
}

# =====================================================================================================================
# sub git - helper to call git in smart manner

sub git {
    my $command = join(' ', @_);
    my $output = `git $command 2>&1`;
    my $exit_code = $? >> 8;

    say_fatal "Failed to call 'git $command': $output" if $exit_code != 0;

    chomp $output;
    return $output;
}

# =====================================================================================================================
# General helper function implementations

sub say_empty      { say                      "          ".join(' ',@_); }
sub say_result     { say (($_[0] ? "\e[1m\e[32m[OK]\e[0m      " : "\e[1m\e[31m[FAILURE]\e[0m ") . $_[1]); }

sub say_debug      { say "\e[1m[DEBUG]\e[0m   " . join(' ', @_); }
sub say_info       { say "\e[1m[INFO]\e[0m    " . join(' ', @_); }
sub say_warning    { say "\e[1m\e[33m[WARNING]\e[0m " . join(' ', @_); }
sub say_error      { say "\e[1m\e[31m[ERROR]\e[0m   " . join(' ', @_); }
sub say_fatal      { say "\e[1m\e[31m[FATAL]\e[0m   " . join(' ', @_); exit 1; }
