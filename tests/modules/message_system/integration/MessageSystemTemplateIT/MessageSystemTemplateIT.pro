TEST_NAME = $$basename(PWD)
include(../../../../common/TestConfig.pri)

QT += widgets

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM
DEFINES += DRAUPNIR_LOGGING_SINGLETHREAD

include(../../../../../modules/MessageSystem.pri)
include(../../../../common/MessageDisplayDialogHelpers.pri)
include(../../../../common/SettingsSourceMockTemplate.pri)
include(../../../../common/UiHelpers.pri)

SOURCES += \
    MessageSystemTemplateIT.cpp

