{
    runtime_tests => [
        File::Spec->catdir("containers","unit"),
        File::Spec->catdir("draupnir_resources","unit"),
        File::Spec->catdir("loptr","unit"),
        File::Spec->catdir("message_system","integration"),
        File::Spec->catdir("message_system","unit"),
        File::Spec->catdir("proxy_helper","unit"),
        File::Spec->catdir("settings_registry","integration"),
        File::Spec->catdir("settings_registry","unit"),
        File::Spec->catdir("ui_bricks","integration"),
        File::Spec->catdir("ui_bricks","unit"),
        File::Spec->catdir("utils","unit")
    ],
    compile_tests => [
        File::Spec->catdir("utils","compile")
    ],
    must_crash_tests => [ "TerminateIfEvenTest" ]
}
