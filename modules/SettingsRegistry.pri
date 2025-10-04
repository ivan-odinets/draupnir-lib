!contains(DEFINES, DRAUPNIR_SETTINGS_REGISTRY_ENABLED) {
    DEFINES += DRAUPNIR_SETTINGS_REGISTRY_ENABLED

    # This define enable using QSettings as a source of settings for the SettingsRegistry module
    # DEFINES += DRAUPNIR_SETTINGS_USE_QSETTINGS

    # This define enable using AppSettings as a source of settings for the SettingsRegistry module
    # DEFINES += DRAUPNIR_SETTINGS_USE_APPSETTINGS

    # This define enable using custom settings backends as a source of settings for the SettingsRegistry
    # module
    # DEFINES  += DRAUPNIR_SETTINGS_USE_CUSTOM

    include(DraupnirVersion.pri)
    include(Utils.pri)

    INCLUDEPATH += $$PWD/../include/settings_registry

    contains(DEFINES, DRAUPNIR_SETTINGS_USE_APPSETTINGS) {
        HEADERS += \
            $$PWD/../include/settings_registry/draupnir/core/AppSettings.h

        SOURCES += \
            $$PWD/../src/settings_registry/draupnir/core/AppSettings.cpp
    }

    contains(DEFINES, DRAUPNIR_SETTINGS_USE_CUSTOM) {
        HEADERS += \
            $$PWD/../include/settings_registry/draupnir/core/SettingsBackendInterface.h
    }

    HEADERS += \
        $$PWD/../include/settings_registry/draupnir/SettingsBundleMerge.h \
        $$PWD/../include/settings_registry/draupnir/SettingsBundleTemplate.h \
        $$PWD/../include/settings_registry/draupnir/SettingsRegistryTemplate.h \
        $$PWD/../include/settings_registry/draupnir/core/SettingTemplate.h \
    $$PWD/../include/settings_registry/draupnir/traits/settings/CentralWidgetIndexSetting.h \
        $$PWD/../include/settings_registry/draupnir/traits/settings/LastUsedDirectorySetting.h \
    $$PWD/../include/settings_registry/draupnir/traits/settings/MainWindowMinimizeOnCloseSetting.h \
    $$PWD/../include/settings_registry/draupnir/traits/settings/MainWindowMinimizeToTraySetting.h \
    $$PWD/../include/settings_registry/draupnir/traits/settings/MainWindowSizeSetting.h \
    $$PWD/../include/settings_registry/draupnir/traits/settings/MainWindowStartHiddenSetting.h \
        $$PWD/../include/settings_registry/draupnir/traits/settings/RecentFilesListSetting.h \
        $$PWD/../include/settings_registry/draupnir/traits/settings/StandartSettingTraitTemplates.h \
        $$PWD/../include/settings_registry/draupnir/utils/SettingTraitSerializer.h \
        $$PWD/../include/settings_registry/draupnir/utils/SettingTraitValidator.h \
        $$PWD/../include/settings_registry/draupnir/utils/SettingTraitsConcat.h \
        $$PWD/../include/settings_registry/draupnir/utils/ValueSerializer.h
}
