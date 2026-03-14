TEST_NAME = $$basename(PWD)
include(../../../../common/TestConfig.pri)

QT += widgets

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

include(../../../../../modules/UiBricks.pri)

SOURCES +=  \
    MenuTemplateTest.cpp

