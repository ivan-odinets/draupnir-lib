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
    $$PWD/include/templateMenus/handlers/fileMenu/FileCloseEntryHandler.h \
    $$PWD/include/templateMenus/handlers/fileMenu/FileEntriesHandlersContext.h \
    $$PWD/include/templateMenus/handlers/fileMenu/FileExitEntryHandler.h \
    $$PWD/include/templateMenus/handlers/fileMenu/FileManagerValidator.h \
    $$PWD/include/templateMenus/handlers/fileMenu/FileMenuEntriesHandler.h \
    $$PWD/include/templateMenus/handlers/fileMenu/FileNewEntryHandler.h \
    $$PWD/include/templateMenus/handlers/fileMenu/FileOpenEntryHandler.h \
    $$PWD/include/templateMenus/handlers/fileMenu/FileRecentEntryHandler.h \
    $$PWD/include/templateMenus/handlers/fileMenu/FileSaveAsEntryHandler.h \
    $$PWD/include/templateMenus/handlers/fileMenu/FileSaveEntryHandler.h \
    $$PWD/include/templateMenus/handlers/helpMenu/AboutAppEntryHandler.h \
    $$PWD/include/templateMenus/handlers/helpMenu/AboutQtEntryHandler.h \
    $$PWD/include/templateMenus/handlers/helpMenu/HelpEntryHandler.h \
    $$PWD/include/templateMenus/handlers/helpMenu/HelpMenuEntriesHandler.h \
    $$PWD/include/templateMenus/handlers/settingsMenu/MinimizeOnCloseEntryHandler.h \
    $$PWD/include/templateMenus/menus/MenuBarTemplate.h \
    $$PWD/include/templateMenus/menus/MenuEntriesContainer.h \
    $$PWD/include/templateMenus/menus/MenuTemplate.h \
    $$PWD/include/templateMenus/menus/RecentFilesMenu.h
