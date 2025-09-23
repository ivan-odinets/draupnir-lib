# ---------------------------------------------------------------------------------------------------------------------
# Build config options.
# Note: Better to enable/disable sth not in this file, but in the main *.pro

# ---------------------------------------------------------------------------------------------------------------------

QT          += core widgets
CONFIG      += c++20

# ---------------------------------------------------------------------------------------------------------------------

INCLUDEPATH += $$PWD/include/templateHandlers

# ---------------------------------------------------------------------------------------------------------------------

HEADERS += \
    $$PWD/include/templateHandlers/handlers/AbstractHandlers.h \
    $$PWD/include/templateHandlers/handlers/GenericMenuEntriesHandler.h \
    $$PWD/include/templateHandlers/handlers/fileMenu/FileCloseEntryHandler.h \
    $$PWD/include/templateHandlers/handlers/fileMenu/FileExitEntryHandler.h \
    $$PWD/include/templateHandlers/handlers/fileMenu/FileMenuEntriesHandler.h \
    $$PWD/include/templateHandlers/handlers/fileMenu/FileNewEntryHandler.h \
    $$PWD/include/templateHandlers/handlers/fileMenu/FileOpenEntryHandler.h \
    $$PWD/include/templateHandlers/handlers/fileMenu/FileRecentEntryHandler.h \
    $$PWD/include/templateHandlers/handlers/fileMenu/FileSaveAsEntryHandler.h \
    $$PWD/include/templateHandlers/handlers/fileMenu/FileSaveEntryHandler.h \
    $$PWD/include/templateHandlers/handlers/helpMenu/AboutAppEntryHandler.h \
    $$PWD/include/templateHandlers/handlers/helpMenu/AboutDraupnirLibEntryHandler.h \
    $$PWD/include/templateHandlers/handlers/helpMenu/AboutQtEntryHandler.h \
    $$PWD/include/templateHandlers/handlers/helpMenu/HelpEntryHandler.h \
    $$PWD/include/templateHandlers/handlers/helpMenu/HelpMenuEntriesHandler.h \
    $$PWD/include/templateHandlers/handlers/settingsMenu/MinimizeOnCloseEntryHandler.h \
    $$PWD/include/templateHandlers/handlers/settingsMenu/MinimizeToTrayEntryHandler.h \
    $$PWD/include/templateHandlers/handlers/settingsMenu/SettingsCheckableEntryHandler.h \
    $$PWD/include/templateHandlers/handlers/settingsMenu/SettingsMenuEntriesHandler.h \
    $$PWD/include/templateHandlers/handlers/settingsMenu/StartHiddenEntryHandler.h \
    $$PWD/include/templateHandlers/utils/FileManagerValidator.h \
    $$PWD/include/templateHandlers/utils/SettingTraitForEntry.h
