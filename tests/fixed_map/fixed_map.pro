QT += testlib
QT += core gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    FixedMapTest.cpp

HEADERS += \
    ../../containers/fixed_map.h \
    ../../core/common.h \
    ../../core/template_constructors.h \
    ../../src/common/Common.h \
    ../../src/common/fixed_map.h
