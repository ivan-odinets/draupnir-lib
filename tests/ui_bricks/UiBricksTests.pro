TEMPLATE = subdirs
CONFIG += ordered warn_on qt debug_and_release

SUBDIRS += \
     integration/HelpMenuEntriesHandlerIT \
     integration/FileMenuEntriesHandlerIT \
     integration/MenuHandlerTemplateIT \
     integration/SettingsMenuEntriesHandlerIT \
     unit/FileCloseEntryHandlerTest \
     unit/FileNewEntryHandlerTest \
     unit/FileOpenEntryHandlerTest \
     unit/CheckableUiElementHelperTest \
     unit/FileSaveAsEntryHandlerTest \
     unit/FileSaveEntryHandlerTest \
     unit/FixedTabWidgetTemplateTest \
     unit/MainWindowTemplateTest \
     unit/MenuBarTemplateTest \
     unit/MenuEntriesConcatenatorTest \
     unit/MenuEntriesContainerTest \
     unit/MenuTemplateTest \
     unit/RecentFilesMenuTest \
     unit/ViewManagerTest
