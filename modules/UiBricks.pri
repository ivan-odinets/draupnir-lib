!contains(DEFINES, DRAUPNIR_UI_BRICKS_ENABLED) {
    DEFINES += DRAUPNIR_UI_BRICKS_ENABLED

    !contains(CONFIG, c++20) { CONFIG += c++20 }
    !contains(QT, gui) { QT += gui }
    !contains(QT, widgets) { QT += widgets }

    include(DraupnirVersion.pri)
    include(SettingsRegistry.pri)
    include(DraupnirResources.pri)

    DEFINES += DRAUPNIR_RELAXED_MENU_TEMPLATE_HANDLERS

    INCLUDEPATH += $$PWD/../include/ui_bricks

    HEADERS += \
        $$PWD/../include/ui_bricks/draupnir/UiBricks.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/animations/AnimationBuilder.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/animations/FadeOut.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/animations/ParralelAnimationTemplate.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/animations/ShrinkToCenter.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/concepts/FileManagerConcept.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/core/ViewManager.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/settings_menu/SettingsMenuHandlerTemplate.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/templates/ActionHandlerTemplate.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/templates/GenericMenuEntryHandlerTemplate.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/templates/MenuHandlerTemplate.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/features/UseCentralWidget.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/features/UseMenuBar.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/features/UseTrayIcon.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/ui/TrayIcon.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/settings/CentralWidgetIndexSetting.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/settings/main_window/MinimizeOnCloseSetting.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/settings/main_window/MinimizeToTraySetting.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/settings/main_window/WindowSizeSetting.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/settings/main_window/StartHiddenSetting.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/utils/CheckableUiElementHelper.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/ui/widgets/FixedCentralTabWidgetTemplate.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/ui/widgets/FixedTabWidget.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/ui/widgets/FixedTabWidgetTemplate.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/ui/widgets/AnimationOverlayWidget.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/ui/windows/MainWindowTemplate.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/features/AnimatedMinimizableToTray.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/features/ExtraSettings.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/features/MinimizableOnClose.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/features/MinimizableToTray.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/features/RememberWindowSize.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/FileMenuEntries.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/HelpMenuEntries.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/SettingsMenuEntries.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/decoration/SectionEntry.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/decoration/SeparatorEntry.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/templates/ActionEntry.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/templates/CheckableEntry.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/templates/CustomMenuEntryTrait.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/templates/DynamicCustomMenu.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/templates/MenuTemplateEntry.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/ui/menus/MenuBarTemplate.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/ui/menus/MenuTemplate.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/ui/menus/RecentFilesMenu.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/core/MenuEntriesContainer.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/submenus/HelpMenuTemplate.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/menu_entries/submenus/SettingsMenuTemplate.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/file_menu/FileCloseEntryHandler.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/file_menu/FileExitEntryHandler.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/file_menu/FileMenuEntriesHandler.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/file_menu/FileNewEntryHandler.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/file_menu/FileOpenEntryHandler.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/file_menu/FileRecentEntryHandler.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/file_menu/FileSaveAsEntryHandler.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/file_menu/FileSaveEntryHandler.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/help_menu/AboutAppEntryHandler.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/help_menu/AboutDraupnirLibEntryHandler.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/help_menu/AboutQtEntryHandler.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/help_menu/HelpEntryHandler.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/help_menu/HelpMenuEntriesHandler.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/concepts/FileContextConcept.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/concepts/MenuEntryConcept.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/file_menu/FileMenuContext.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/settings_menu/SettingsContext.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/handlers/settings_menu/SettingsMenuEntryHandlerTemplate.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/utils/MenuEntriesConcatenator.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/utils/MenuEntryToTraitMapper.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/utils/SettingsValueUserInput.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/concepts/ActionHandlerConcept.h \
        $$PWD/../include/ui_bricks/draupnir/ui_bricks/traits/features/ClosureConfirmation.h

    SOURCES += \
        $$PWD/../src/ui_bricks/draupnir/ui/TrayIcon.cpp \
        $$PWD/../src/ui_bricks/draupnir/ui/widgets/AnimationOverlayWidget.cpp \
        $$PWD/../src/ui_bricks/draupnir/ui/menus/RecentFilesMenu.cpp

    DISTFILES += \
        $$PWD/../docs/pages/UiBricks.dox
}
