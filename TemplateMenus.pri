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
    $$PWD/include/templateMenus/entries/FileMenuEntries.h \
    $$PWD/include/templateMenus/entries/HelpMenuEntries.h \
    $$PWD/include/templateMenus/entries/HelperEntries.h \
    $$PWD/include/templateMenus/entries/SettingsMenuEntry.h \
    $$PWD/include/templateMenus/entries/SubmenuEntries.h \
    $$PWD/include/templateMenus/handlers/AbstractHandlers.h \
    $$PWD/include/templateMenus/handlers/GenericMenuEntriesHandler.h \
    $$PWD/include/templateMenus/menus/MenuBarTemplate.h \
    $$PWD/include/templateMenus/menus/MenuEntriesContainer.h \
    $$PWD/include/templateMenus/menus/MenuTemplate.h \
    $$PWD/include/templateMenus/menus/RecentFilesMenu.h
