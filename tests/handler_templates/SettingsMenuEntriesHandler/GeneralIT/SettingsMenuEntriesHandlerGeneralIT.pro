QT += testlib
QT += gui
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

# Modules
include(../../../../modules/SettingsRegistry.pri)
include(../../../../modules/MenuTemplates.pri)
include(../../../../modules/HandlerTemplates.pri)

c
include(../../../common/SomeRandomHandlers.pri)

SOURCES +=  \
    SettingsMenuEntriesHandlerGeneralIT.cpp
