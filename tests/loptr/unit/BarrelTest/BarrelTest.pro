QT += testlib gui widgets
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

# Modules
include(../../../../modules/Loptr.pri)

SOURCES +=  \
    BarrelTest.cpp

