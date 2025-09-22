# ---------------------------------------------------------------------------------------------------------------------
# Build config options.
# Note: Better to enable/disable sth not in this file, but in the main *.pro

# ---------------------------------------------------------------------------------------------------------------------

QT          += core widgets
CONFIG      += c++20

# ---------------------------------------------------------------------------------------------------------------------

INCLUDEPATH += $$PWD/include/templateMenus

# ---------------------------------------------------------------------------------------------------------------------

SOURCES += \
    $$PWD/src/templateMenus/menus/RecentFilesMenu.cpp

HEADERS += \
    $$PWD/include/templateMenus/menus/MenuBarTemplate.h \
    $$PWD/include/templateMenus/menus/MenuEntriesContainer.h \
    $$PWD/include/templateMenus/menus/MenuTemplate.h \
    $$PWD/include/templateMenus/menus/RecentFilesMenu.h \
    $$PWD/include/templateMenus/traits/entries/FileMenuEntries.h \
    $$PWD/include/templateMenus/traits/entries/HelpMenuEntries.h \
    $$PWD/include/templateMenus/traits/entries/HelperEntries.h \
    $$PWD/include/templateMenus/traits/entries/SettingsMenuEntry.h \
    $$PWD/include/templateMenus/traits/entries/SubmenuEntries.h
