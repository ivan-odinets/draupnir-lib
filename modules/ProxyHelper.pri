!contains(DEFINES, DRAUPNIR_PROXY_HELPER_ENABLED) {
    DEFINES += DRAUPNIR_PROXY_HELPER_ENABLED

    QT          += core network widgets
    CONFIG      += c++20

    INCLUDEPATH += $$PWD/../include/proxy_helper

    HEADERS += \
        $$PWD/../include/proxy_helper/draupnir/ui/widgets/ProxyEditWidget.h \
        $$PWD/../include/proxy_helper/draupnir/ui/windows/ProxyEditDialog.h \
        $$PWD/../include/proxy_helper/draupnir/utils/ProxySerializer.h

    SOURCES += \
        $$PWD/../src/proxy_helper/draupnir/ui/widgets/ProxyEditWidget.cpp \
        $$PWD/../src/proxy_helper/draupnir/ui/windows/ProxyEditDialog.cpp \
        $$PWD/../src/proxy_helper/draupnir/utils/ProxySerializer.cpp
}
