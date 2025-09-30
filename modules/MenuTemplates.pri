!contains(DEFINES, DRAUPNIR_TEMPLATE_MENUS_ENABLED) {
    DEFINES += DRAUPNIR_TEMPLATE_MENUS_ENABLED

    include(DraupnirVersion.pri)
    include(Utils.pri)

    INCLUDEPATH += $$PWD/../include/menu_templates

    HEADERS += \
        $$PWD/../include/menu_templates/draupnir/core/MenuEntriesContainer.h \
        $$PWD/../include/menu_templates/draupnir/traits/entries/FileMenuEntries.h \
        $$PWD/../include/menu_templates/draupnir/traits/entries/HelpMenuEntries.h \
        $$PWD/../include/menu_templates/draupnir/traits/entries/HelperEntries.h \
        $$PWD/../include/menu_templates/draupnir/traits/entries/SettingsMenuEntries.h \
        $$PWD/../include/menu_templates/draupnir/traits/entries/SubmenuEntries.h \
        $$PWD/../include/menu_templates/draupnir/ui/menus/MenuBarTemplate.h \
        $$PWD/../include/menu_templates/draupnir/ui/menus/MenuTemplate.h \
        $$PWD/../include/menu_templates/draupnir/ui/menus/RecentFilesMenu.h

    SOURCES += \
        $$PWD/../src/menu_templates/draupnir/ui/menus/RecentFilesMenu.cpp
}
