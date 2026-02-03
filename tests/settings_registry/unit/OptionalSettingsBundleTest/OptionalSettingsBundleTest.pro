QT += testlib
CONFIG += qt warn_on depend_includepath testcase
QMAKE_CXXFLAGS += -std=c++20
CONFIG += c++20

TEMPLATE = app

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

# Modules
include(../../../../modules/SettingsRegistry.pri)

# Mocks
include(../../../common/SomeRandomSettingsTraits.pri)

SOURCES +=  \
    OptionalSettingsBundleTest.cpp

