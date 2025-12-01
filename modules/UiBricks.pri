!contains(DEFINES, DRAUPNIR_UI_BRICKS_ENABLED) {
    DEFINES += DRAUPNIR_UI_BRICKS_ENABLED

    !contains(CONFIG, c++20) { CONFIG += c++20 }
    !contains(QT, gui) { QT += gui }
    !contains(QT, widgets) { QT += widgets }

    include(DraupnirVersion.pri)
    include(SettingsRegistry.pri)

    INCLUDEPATH += $$PWD/../include/ui_bricks

    HEADERS += \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/core/ViewManager.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/ui/TrayIcon.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/ui/widgets/FixedCentralTabWidgetTemplate.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/ui/widgets/FixedTabWidget.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/ui/widgets/FixedTabWidgetTemplate.h

    SOURCES += \
        $$PWD/../src/ui_bricks/draupnir/ui/TrayIcon.cpp

    DISTFILES += \
        $$PWD/../docs/pages/UiBricks.dox
}



