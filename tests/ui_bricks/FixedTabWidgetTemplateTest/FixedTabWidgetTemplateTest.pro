QT += testlib
QT += gui widgets
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

# Modules
include(../../../modules/UiBricks.pri)

# Mocks
include(../../common/MockSettingsTemplate.pri)
include(../../common/SomeRandomSettingsTraits.pri)
include(../../common/SomeRandomWidgetTraits.pri)

SOURCES +=  FixedTabWidgetTemplateTest.cpp

