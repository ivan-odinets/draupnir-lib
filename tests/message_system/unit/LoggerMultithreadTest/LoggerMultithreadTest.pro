QT += testlib
QT += gui widgets concurrent
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++20
CONFIG += c++20

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

# Mocks
include(../../../common/MockSettingsTemplate.pri)
include(../../../common/DummyMessageHandler.pri)

# Modules
include(../../../../modules/MessageSystem.pri)

SOURCES +=  LoggerMultithreadTest.cpp

