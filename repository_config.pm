{
    name => "draupnir-lib",
    jobs => "jobs",
    tests => File::Spec->catdir("tests","modules"),
    tests_build => File::Spec->catdir("tests","build")
}
