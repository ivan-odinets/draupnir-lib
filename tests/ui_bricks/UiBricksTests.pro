TEMPLATE = subdirs
CONFIG += ordered warn_on qt debug_and_release

SUBDIRS += \
    unit/CheckableUiElementHelperTest \
    unit/FixedTabWidgetTemplateTest \
    unit/MainWindowTemplateTest \
    unit/MenuBarTemplateTest \
    unit/MenuEntriesContainerTest \
    unit/MenuTemplateTest \
    unit/RecentFilesMenuTest \
    unit/ViewManagerTest
