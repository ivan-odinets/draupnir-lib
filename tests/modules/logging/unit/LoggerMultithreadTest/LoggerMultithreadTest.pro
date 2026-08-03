TEST_NAME = $$basename(PWD)
include(../../../../common/TestConfig.pri)

QT += widgets concurrent

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

include(../../../../common/ConcurrentTestHelpers.pri)
include(../../../../common/MessageReceiverMock.pri)

include(../../../../../modules/Logging.pri)

SOURCES +=  \
    LoggerMultithreadTest.cpp

