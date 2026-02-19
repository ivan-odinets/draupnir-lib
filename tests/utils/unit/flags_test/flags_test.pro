QT += testlib
QT += core

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++20
CONFIG += c++20

include(../../../../modules/Utils.pri)

SOURCES += \
    FlagsTest.cpp
