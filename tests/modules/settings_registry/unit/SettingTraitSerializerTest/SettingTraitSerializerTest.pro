TEST_NAME = $$basename(PWD)
include(../../../../common/TestConfig.pri)

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

include(../../../../common/MockSettingsTemplate.pri)
include(../../../../common/SettingsTraits.pri)

include(../../../../../modules/SettingsRegistry.pri)

SOURCES +=  \
    SettingTraitSerializerTest.cpp

