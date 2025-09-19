INCLUDEPATH += $$PWD/include/settings

DEFINES += DRAUPNIR_SETTINGS_ENABLED

# This define enable using QSettings as a source of settings for the Settings module
#DEFINES += DRAUPNIR_SETTINGS_USE_QSETTINGS

# This define enable using AppSettings as a source of settings for the Settings module
DEFINES += DRAUPNIR_SETTINGS_USE_APPSETTINGS

contains(DEFINES, DRAUPNIR_SETTINGS_USE_APPSETTINGS) {

    HEADERS += \
        $$PWD/include/settings/core/AppSettings.h

    SOURCES += \
        $$PWD/src/settings/AppSettings.cpp

}

HEADERS += \
    $$PWD/include/settings/SettingTemplate.h \
    $$PWD/include/settings/SettingsBundle.h \
    $$PWD/include/settings/SettingsBundleMerge.h \
    $$PWD/include/settings/SettingsRegistry.h \
    $$PWD/include/settings/traits/ActiveWidgetIndexSetting.h \
    $$PWD/include/settings/traits/LastUsedDirectorySetting.h \
    $$PWD/include/settings/traits/MinimizeOnCloseSetting.h \
    $$PWD/include/settings/traits/MinimizeToTraySetting.h \
    $$PWD/include/settings/traits/RecentFilesListSetting.h \
    $$PWD/include/settings/traits/StandartSettingTraitTemplates.h \
    $$PWD/include/settings/traits/StartHiddenSetting.h \
    $$PWD/include/settings/traits/WindowSizeSetting.h \
    $$PWD/include/settings/utils/SettingTraitSerializer.h \
    $$PWD/include/settings/utils/SettingTraitValidator.h \
    $$PWD/include/settings/utils/SettingTraitsConcat.h \
    $$PWD/include/settings/utils/ValueSerializer.h
