TEST_NAME = $$basename(PWD)
include(../../../../common/TestConfig.pri)

QT += widgets

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM
DEFINES += DRAUPNIR_LOGGING_SINGLETHREAD

include(../../../../common/MessageHandlerMock.pri)

include(../../../../../modules/Logging.pri)

SOURCES +=  \
    LoggerTest.cpp

