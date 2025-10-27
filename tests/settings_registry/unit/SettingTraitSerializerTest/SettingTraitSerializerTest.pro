QT += testlib
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

# Modules
include(../../../../modules/SettingsRegistry.pri)

# Mocks
include(../../../common/MockSettingsTemplate.pri)
include(../../../common/SomeRandomSettingsTraits.pri)

SOURCES +=  SettingTraitSerializerTest.cpp

