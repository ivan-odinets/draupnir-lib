INCLUDEPATH += $$PWD/include/trayIcon

!defined(DRAUPNIR_TRAY_ICON_PRI_INCLUDED) {
    DEFINES += DRAUPNIR_TRAY_ICON_PRI_INCLUDED

    QT          += core
    CONFIG      += c++20

    HEADERS += \
        $$PWD/include/trayIcon/TrayIcon.h

    SOURCES += \
        $$PWD/src/trayIcon/TrayIcon.cpp

}
