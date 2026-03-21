TEST_NAME = $$basename(PWD)
include(../../../../common/TestConfig.pri)

include(../../../../common/UiHelpers.pri)
include(../../../../common/SettingsTraits.pri)

QT += widgets

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

include(../../../../../modules/UiBricks.pri)

SOURCES += \
    SettingsValueUserInputTest.cpp

