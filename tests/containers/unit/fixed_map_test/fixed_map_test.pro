QT += testlib
QT += core gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

include(../../../../modules/Containers.pri)

SOURCES +=  \
    FixedMapTest.cpp
