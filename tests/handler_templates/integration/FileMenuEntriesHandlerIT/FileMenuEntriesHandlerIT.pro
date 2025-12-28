QT += testlib
QT += gui widgets
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

# Modules
include(../../../../modules/UiBricks.pri)
include(../../../../modules/HandlerTemplates.pri)

SOURCES +=  \
    FileMenuEntriesHandlerIT.cpp
