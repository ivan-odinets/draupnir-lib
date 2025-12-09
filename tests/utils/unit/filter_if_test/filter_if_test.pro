QT += testlib
QT += core

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

include(../../../../modules/Utils.pri)

SOURCES += \
    FilterIfTest.cpp
