!contains(DEFINES, DRAUPNIR_UI_BRICKS_ENABLED) {
    DEFINES += DRAUPNIR_UI_BRICKS_ENABLED

    include(DraupnirVersion.pri)
    include(SettingsRegistry.pri)

    INCLUDEPATH += $$PWD/../include/ui_bricks

    HEADERS += \
    $$PWD/../include/ui_bricks/draupnir/core/UiManager.h \
        $$PWD/../include/ui_bricks/draupnir/ui/widgets/CentralTabbedWidegt.h \
        $$PWD/../include/ui_bricks/draupnir/ui/TrayIcon.h

    SOURCES += \
        $$PWD/../src/ui_bricks/draupnir/ui/TrayIcon.cpp
}

HEADERS += \
    $$PWD/../include/ui_bricks/draupnir/ui/widgets/CentralTabbedWidgetTemplate.h


