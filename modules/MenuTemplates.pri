!contains(DEFINES, DRAUPNIR_TEMPLATE_MENUS_ENABLED) {
    DEFINES += DRAUPNIR_TEMPLATE_MENUS_ENABLED

    !contains(CONFIG, c++20) { CONFIG += c++20 }
    !contains(QT, gui) { QT += gui }
    !contains(QT, widgets) { QT += widgets }

    include(DraupnirVersion.pri)
    include(DraupnirResources.pri)
    include(Utils.pri)

    INCLUDEPATH += $$PWD/../include/ui_bricks

    HEADERS += \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/core/MenuEntriesContainer.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/FileMenuEntries.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/HelpMenuEntries.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/SettingsMenuEntries.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/ui/menus/MenuBarTemplate.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/ui/menus/MenuTemplate.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/ui/menus/RecentFilesMenu.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/templates/ActionEntry.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/templates/CheckableEntry.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/templates/CustomMenuEntryTrait.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/templates/DynamicCustomMenu.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/templates/MenuTemplateEntry.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/decoration/SectionEntry.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/decoration/SeparatorEntry.h

    SOURCES += \
        $$PWD/../src/ui_bricks/draupnir/ui/menus/RecentFilesMenu.cpp

    DISTFILES += \
        $$PWD/../docs/pages/MenuTemplates.dox
}


