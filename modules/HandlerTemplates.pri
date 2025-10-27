!contains(DEFINES, DRAUPNIR_HANDLER_TEMPLATES_ENABLED) {
    DEFINES += DRAUPNIR_HANDLER_TEMPLATES_ENABLED

    !contains(CONFIG, c++20) { CONFIG += c++20 }
    !contains(QT, gui) { QT += gui }
    !contains(QT, widgets) { QT += widgets }

    include(DraupnirVersion.pri)
    include(DraupnirResources.pri)
    include(MenuTemplates.pri)
    include(SettingsRegistry.pri)

    INCLUDEPATH += $$PWD/../include/handler_templates/

    HEADERS += \
        $$PWD/../include/handler_templates/draupnir/handlers/file_menu/FileCloseEntryHandler.h \
        $$PWD/../include/handler_templates/draupnir/handlers/file_menu/FileExitEntryHandler.h \
        $$PWD/../include/handler_templates/draupnir/handlers/file_menu/FileMenuEntriesHandler.h \
        $$PWD/../include/handler_templates/draupnir/handlers/file_menu/FileNewEntryHandler.h \
        $$PWD/../include/handler_templates/draupnir/handlers/file_menu/FileOpenEntryHandler.h \
        $$PWD/../include/handler_templates/draupnir/handlers/file_menu/FileRecentEntryHandler.h \
        $$PWD/../include/handler_templates/draupnir/handlers/file_menu/FileSaveAsEntryHandler.h \
        $$PWD/../include/handler_templates/draupnir/handlers/file_menu/FileSaveEntryHandler.h \
        $$PWD/../include/handler_templates/draupnir/handlers/help_menu/AboutAppEntryHandler.h \
        $$PWD/../include/handler_templates/draupnir/handlers/help_menu/AboutDraupnirLibEntryHandler.h \
        $$PWD/../include/handler_templates/draupnir/handlers/help_menu/AboutQtEntryHandler.h \
        $$PWD/../include/handler_templates/draupnir/handlers/help_menu/HelpEntryHandler.h \
        $$PWD/../include/handler_templates/draupnir/handlers/help_menu/HelpMenuEntriesHandler.h \
        $$PWD/../include/handler_templates/draupnir/handlers/settings_menu/MinimizeOnCloseEntryHandler.h \
        $$PWD/../include/handler_templates/draupnir/handlers/settings_menu/MinimizeToTrayEntryHandler.h \
        $$PWD/../include/handler_templates/draupnir/handlers/settings_menu/SettingsCheckableEntryHandler.h \
        $$PWD/../include/handler_templates/draupnir/handlers/settings_menu/SettingsMenuEntriesHandler.h \
        $$PWD/../include/handler_templates/draupnir/handlers/settings_menu/StartHiddenEntryHandler.h \
    $$PWD/../include/handler_templates/draupnir/handlers/templates/GenericMenuEntriesHandler.h \
        $$PWD/../include/handler_templates/draupnir/utils/FileManagerValidator.h \
        $$PWD/../include/handler_templates/draupnir/utils/SettingTraitForEntry.h
}

HEADERS += \
    $$PWD/../include/handler_templates/draupnir/handlers/templates/ActionHandler.h \
    $$PWD/../include/handler_templates/draupnir/handlers/templates/CheckableActionHandler.h \
    $$PWD/../include/handler_templates/draupnir/handlers/templates/CustomMenuHandler.h \
    $$PWD/../include/handler_templates/draupnir/handlers/templates/GenericMenuEntryHandler.h
