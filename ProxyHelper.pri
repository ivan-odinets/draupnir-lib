QT          += core network widgets
CONFIG      += c++20

INCLUDEPATH += $$PWD/include/proxy

HEADERS += \
    $$PWD/include/proxy/ProxyEditDialog.h \
    $$PWD/include/proxy/ProxyEditWidget.h \
    $$PWD/include/proxy/ProxyHelper.h

SOURCES += \
    $$PWD/src/proxy/ProxyEditDialog.cpp \
    $$PWD/src/proxy/ProxyEditWidget.cpp \
    $$PWD/src/proxy/ProxyHelper.cpp

