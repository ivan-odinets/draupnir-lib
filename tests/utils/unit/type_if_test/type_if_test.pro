QT += testlib
QT += core gui widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

include(../../../common/TypeHelpers.pri)

include(../../../../modules/Utils.pri)

SOURCES += \
    TypeIfTest.cpp
