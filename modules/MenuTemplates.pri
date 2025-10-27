!contains(DEFINES, DRAUPNIR_TEMPLATE_MENUS_ENABLED) {
    DEFINES += DRAUPNIR_TEMPLATE_MENUS_ENABLED

    !contains(CONFIG, c++20) { CONFIG += c++20 }
    !contains(QT, gui) { QT += gui }
    !contains(QT, widgets) { QT += widgets }

    include(DraupnirVersion.pri)
    include(DraupnirResources.pri)
    include(Utils.pri)

    INCLUDEPATH += $$PWD/../include/menu_templates

    HEADERS += \
        $$PWD/../include/menu_templates/draupnir/core/MenuEntriesContainer.h \
        $$PWD/../include/menu_templates/draupnir/traits/entries/FileMenuEntries.h \
        $$PWD/../include/menu_templates/draupnir/traits/entries/HelpMenuEntries.h \
        $$PWD/../include/menu_templates/draupnir/traits/entries/SettingsMenuEntries.h \
        $$PWD/../include/menu_templates/draupnir/ui/menus/MenuBarTemplate.h \
        $$PWD/../include/menu_templates/draupnir/ui/menus/MenuTemplate.h \
        $$PWD/../include/menu_templates/draupnir/ui/menus/RecentFilesMenu.h \
        $$PWD/../include/menu_templates/draupnir/traits/entries/templates/ActionEntry.h \
        $$PWD/../include/menu_templates/draupnir/traits/entries/templates/CheckableEntry.h \
        $$PWD/../include/menu_templates/draupnir/traits/entries/templates/CustomMenuEntryTrait.h \
        $$PWD/../include/menu_templates/draupnir/traits/entries/templates/DynamicCustomMenu.h \
        $$PWD/../include/menu_templates/draupnir/traits/entries/templates/MenuTemplateEntry.h \
        $$PWD/../include/menu_templates/draupnir/traits/entries/decoration/SectionEntry.h \
        $$PWD/../include/menu_templates/draupnir/traits/entries/decoration/SeparatorEntry.h

    SOURCES += \
        $$PWD/../src/menu_templates/draupnir/ui/menus/RecentFilesMenu.cpp

    DISTFILES += \
        $$PWD/../docs/pages/MenuTemplates.dox
}


