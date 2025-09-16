INCLUDEPATH += $$PWD/include/settings

# This define enable using QSettings as a source of settings for the Settings module
#DEFINES += DRAUPNIR_SETTINGS_USE_QSETTINGS

# This define enable using AppSettings as a source of settings for the Settings module
DEFINES += DRAUPNIR_SETTINGS_USE_APPSETTINGS

contains(DEFINES, DRAUPNIR_SETTINGS_USE_APPSETTINGS) {

    HEADERS += \
        $$PWD/include/settings/AppSettings.h

    SOURCES += \
        $$PWD/src/settings/AppSettings.cpp

}

HEADERS += \
    $$PWD/include/settings/SettingTemplate.h \
    $$PWD/include/settings/SettingTraitForEntry.h \
    $$PWD/include/settings/SettingTraitSerializer.h \
    $$PWD/include/settings/SettingTraitValidator.h \
    $$PWD/include/settings/SettingTraitsConcat.h \
    $$PWD/include/settings/SettingsBundle.h \
    $$PWD/include/settings/SettingsBundleMerge.h \
    $$PWD/include/settings/SettingsRegistry.h \
    $$PWD/include/settings/ValueSerializer.h \
    $$PWD/include/settings/traits/ActiveWidgetIndex.h \
    $$PWD/include/settings/traits/LastUsedDirectory.h \
    $$PWD/include/settings/traits/MinimizeOnClose.h \
    $$PWD/include/settings/traits/MinimizeToTray.h \
    $$PWD/include/settings/traits/RecentFilesList.h \
    $$PWD/include/settings/traits/SettingTraitTemplate.h \
    $$PWD/include/settings/traits/StartHidden.h \
    $$PWD/include/settings/traits/WindowSize.h
