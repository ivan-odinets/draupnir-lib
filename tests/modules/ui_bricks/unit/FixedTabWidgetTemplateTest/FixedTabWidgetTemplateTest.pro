TEST_NAME = $$basename(PWD)
include(../../../../common/TestConfig.pri)

QT += widgets

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

include(../../../../common/MockSettingsTemplate.pri)
include(../../../../common/SettingsTraits.pri)
include(../../../../common/WidgetTraits.pri)

include(../../../../../modules/UiBricks.pri)

SOURCES +=  \
    FixedTabWidgetTemplateTest.cpp

