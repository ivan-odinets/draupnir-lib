TEST_NAME = $$basename(PWD)
include(../../../../common/TestConfig.pri)

QT += widgets concurrent

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

include(../../../../common/MessageHandlerMock.pri)

include(../../../../../modules/Logging.pri)

SOURCES +=  \
    LoggerMultithreadTest.cpp

