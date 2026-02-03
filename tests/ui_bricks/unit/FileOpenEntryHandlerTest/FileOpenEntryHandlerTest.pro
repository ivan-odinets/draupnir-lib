QT += testlib
QT += gui widgets
CONFIG += qt warn_on depend_includepath testcase
QMAKE_CXXFLAGS += -std=c++20
CONFIG += c++20

TEMPLATE = app

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

# Mocks
include(../../../common/DummyFileContext.pri)

# Modules
include(../../../../modules/UiBricks.pri)

SOURCES += \
    FileOpenEntryHandlerTest.cpp
