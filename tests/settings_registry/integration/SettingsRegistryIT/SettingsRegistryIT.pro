QT += testlib
QT += gui
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

# Modules
include(../../../../modules/SettingsRegistry.pri)

# Mocks & Test infrastructure
include(../../../common/MockSettingsTemplate.pri)
include(../../../common/SomeRandomSettingsTraits.pri)

SOURCES +=  \
    SettingsRegistryIT.cpp

