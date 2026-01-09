QT += testlib
QT += core gui widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

include(../../../../modules/Utils.pri)
include(../../../common/TypeHelpers.pri)

SOURCES += \
    TypeListTest.cpp
