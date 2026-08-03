TEST_NAME = $$basename(PWD)
include(../../../../common/TestConfig.pri)

QT += widgets

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM
DEFINES += DRAUPNIR_LOGGING_SYSTEM_SINGLETHREAD

include(../../../../common/SettingsBackendMockTemplate.pri)
include(../../../../common/SettingsSourceMockTemplate.pri)

include(../../../../../modules/Logging.pri)

SOURCES += \
    MessageReceiverTemplateIT.cpp

