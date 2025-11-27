QT += testlib
QT += gui widgets concurrent
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM
DEFINES += DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD

# Mocks
include(../../../common/SomeRandomMessageTraits.pri)
include(../../../common/DummyMessageHandler.pri)

# Modules
include(../../../../modules/MessageSystem.pri)

SOURCES +=  LoggerTest.cpp

