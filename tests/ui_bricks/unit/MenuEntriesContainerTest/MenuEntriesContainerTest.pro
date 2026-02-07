QT += testlib
QT += gui widgets
CONFIG += qt warn_on depend_includepath testcase

QMAKE_CXXFLAGS += -std=c++20
CONFIG      += c++20

TEMPLATE = app

# Modules
include(../../../../modules/MenuTemplates.pri)

include(../../../common/TypeHelpers.pri)

SOURCES +=  MenuEntriesContainerTest.cpp

