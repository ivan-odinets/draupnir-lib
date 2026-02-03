QT += testlib gui widgets
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++20
CONFIG += c++20

# Modules
include(../../../../modules/Loptr.pri)

SOURCES +=  \
    BarrelTest.cpp

