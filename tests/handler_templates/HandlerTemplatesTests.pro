TEMPLATE = subdirs

CONFIG += c++20

SUBDIRS += \
    integration/FileMenuEntriesHandlerIT \
    integration/HelpMenuEntriesHandlerIT \
    integration/SettingsMenuEntriesHandlerIT \
    unit/FileCloseEntryHandlerTest \
    unit/FileNewEntryHandlerTest \
    unit/FileOpenEntryHandlerTest \
    unit/FileSaveAsEntryHandlerTest \
    unit/FileSaveEntryHandlerTest

