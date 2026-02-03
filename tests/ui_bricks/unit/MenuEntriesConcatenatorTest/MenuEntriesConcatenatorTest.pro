QT += testlib
QT += gui widgets
CONFIG += qt warn_on depend_includepath testcase

QMAKE_CXXFLAGS += -std=c++20
CONFIG      += c++20

TEMPLATE = app

# Modules
include(../../../../modules/UiBricks.pri)

# Test
include(../../../common/TypeHelpers.pri)

SOURCES +=  \
    MenuEntriesConcatenatorTest.cpp

