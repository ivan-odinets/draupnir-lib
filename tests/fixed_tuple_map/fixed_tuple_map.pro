QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    FixedTupleMapTest.cpp

HEADERS += \
    ../../containers/fixed_map.h \
    ../../containers/fixed_tuple_map.h \
    ../../core/common.h \
    ../../core/template_constructors.h
