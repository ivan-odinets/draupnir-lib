CONFIG += c++20

TEMPLATE = subdirs
CONFIG += ordered warn_on qt debug_and_release

QMAKE_CXXFLAGS += -std=c++20
CONFIG += c++20

SUBDIRS += \
    TerminateIfEvenTest
