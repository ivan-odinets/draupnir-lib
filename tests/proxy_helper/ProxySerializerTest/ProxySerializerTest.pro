QT += testlib network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

include(../../../modules/ProxyHelper.pri)

SOURCES += \
    ProxySerializerTest.cpp
