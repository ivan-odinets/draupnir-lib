QT += testlib
CONFIG += qt warn_on depend_includepath testcase
QMAKE_CXXFLAGS += -std=c++20
CONFIG += c++20

TEMPLATE = app

# Modules
include(../../../../modules/SettingsRegistry.pri)

# Mocks
include(../../../common/MockSettingsTemplate.pri)
include(../../../common/SomeRandomSettingsTraits.pri)

SOURCES +=  SettingTraitSerializerTest.cpp

