!contains(DEFINES, DRAUPNIR_PROXY_HELPER_ENABLED) {
    DEFINES += DRAUPNIR_PROXY_HELPER_ENABLED

    !contains(CONFIG, c++20) { CONFIG += c++20 }
    !contains(QT, gui) { QT += gui }
    !contains(QT, network) { QT += network }
    !contains(QT, widgets) { QT += widgets }

    INCLUDEPATH += $$PWD/../include/proxy_helper

    HEADERS += \
        $$PWD/../include/proxy_helper/draupnir/ui/widgets/ProxyEditWidget.h \
        $$PWD/../include/proxy_helper/draupnir/ui/windows/ProxyEditDialog.h \
        $$PWD/../include/proxy_helper/draupnir/utils/ProxySerializer.h

    SOURCES += \
        $$PWD/../src/proxy_helper/draupnir/ui/widgets/ProxyEditWidget.cpp \
        $$PWD/../src/proxy_helper/draupnir/ui/windows/ProxyEditDialog.cpp \
        $$PWD/../src/proxy_helper/draupnir/utils/ProxySerializer.cpp

    DISTFILES += \
        $$PWD/../docs/pages/ProxyHelper.dox
}
