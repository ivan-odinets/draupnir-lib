QT += testlib
QT += gui widgets
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

QMAKE_CXXFLAGS += -std=c++20
CONFIG      += c++20

# Modules
include(../../../../modules/UiBricks.pri)

# Mocks
include(../../../common/MockSettingsTemplate.pri)
include(../../../common/MockUiElements.pri)

SOURCES +=  ViewManagerTest.cpp

