QT += testlib
CONFIG += console testcase c++23
QMAKE_CXXFLAGS += -std=c++23
TEMPLATE = app

isEmpty(TEST_NAME) {
    error("TEST_NAME variable must be specified by the test *.pro file");
}

isEmpty(JOB_NAME) {
    BUILD_DIR = $$clean_path($$PWD/../../tests/build/default)
} else {
    BUILD_DIR = $$clean_path($$PWD/../../tests/build/$$JOB_NAME)
}

DESTDIR     = $$BUILD_DIR/bin
OBJECTS_DIR = $$BUILD_DIR/obj/$$TEST_NAME
MOC_DIR     = $$BUILD_DIR/moc/$$TEST_NAME
RCC_DIR     = $$BUILD_DIR/rcc/$$TEST_NAME
UI_DIR      = $$BUILD_DIR/ui/$$TEST_NAME
