QT += testlib
QT += gui widgets
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++20

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

# Mocks
include(../../../common/DummyFileContext.pri)
include(../../../common/MockSettingsTemplate.pri)

# Modules
include(../../../../modules/UiBricks.pri)


SOURCES +=  \
    FileMenuEntriesHandlerIT.cpp
