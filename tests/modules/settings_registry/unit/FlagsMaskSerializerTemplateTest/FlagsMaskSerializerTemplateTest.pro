TEST_NAME = $$basename(PWD)
include(../../../../common/TestConfig.pri)

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

include(../../../../../modules/SettingsRegistry.pri)

SOURCES += \
    FlagsMaskSerializerTemplateTest.cpp

