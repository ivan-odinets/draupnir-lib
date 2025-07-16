QT += testlib
QT += gui
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

DEFINES += DRAUPNIR_MSGSYS_CUSTOM_SETTINGS

include(../../../MessageSystem.pri)

SOURCES +=  MessageSystemGeneralIT.cpp

HEADERS += \
    ../mocks/MockSettings.h
