TEST_NAME = $$basename(PWD)
include(../../../../common/TestConfig.pri)

QT += widgets

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM
DEFINES += DRAUPNIR_LOGGING_SINGLETHREAD

include(../../../../../modules/Notifications.pri)
include(../../../../common/SettingsSourceMockTemplate.pri)
include(../../../../common/UiHelpers.pri)

SOURCES += \
    MessageHandlerTemplateIT.cpp

