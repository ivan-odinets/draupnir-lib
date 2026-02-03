QT += testlib network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

QMAKE_CXXFLAGS += -std=c++20
CONFIG += c++20

TEMPLATE = app

include(../../../../modules/ProxyHelper.pri)

SOURCES += \
    ProxySerializerTest.cpp
