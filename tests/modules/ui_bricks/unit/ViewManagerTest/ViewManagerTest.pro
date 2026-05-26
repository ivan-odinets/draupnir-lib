TEST_NAME = $$basename(PWD)
include(../../../../common/TestConfig.pri)

QT += widgets

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

include(../../../../common/SettingsSourceMockTemplate.pri)
include(../../../../common/MockUiElements.pri)

include(../../../../../modules/UiBricks.pri)

SOURCES +=  \
    ViewManagerTest.cpp

