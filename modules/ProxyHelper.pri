!contains(DEFINES, DRAUPNIR_PROXY_HELPER_ENABLED) {
    DEFINES += DRAUPNIR_PROXY_HELPER_ENABLED

    !contains(CONFIG, c++23) {
        error("C++23 is required for the whole draupnir-lib. Add CONFIG += c++20 to your *.pro or *.pri file.")
    }

    !contains(QT, network) {
        error("QtNetwork module is required for the ProxyHelper. Add QT += network to your *.pro or *.pri file.")
    }

    include(DraupnirVersion.pri)

    INCLUDEPATH += $$PWD/../include/proxy_helper

    HEADERS += \
        $$PWD/../include/proxy_helper/draupnir/utils/ProxySerializer.h

    SOURCES += \
        $$PWD/../src/proxy_helper/draupnir/utils/ProxySerializer.cpp

    contains(QT, widgets) {
        HEADERS += \
            $$PWD/../include/proxy_helper/draupnir/ui/widgets/ProxyEditWidget.h \
            $$PWD/../include/proxy_helper/draupnir/ui/windows/ProxyEditDialog.h

        SOURCES += \
            $$PWD/../src/proxy_helper/draupnir/ui/widgets/ProxyEditWidget.cpp \
            $$PWD/../src/proxy_helper/draupnir/ui/windows/ProxyEditDialog.cpp
    }

    DISTFILES += \
        $$PWD/../docs/pages/ProxyHelper.dox
}
