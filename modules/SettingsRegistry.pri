!contains(DEFINES, DRAUPNIR_SETTINGS_REGISTRY_ENABLED) {
    DEFINES += DRAUPNIR_SETTINGS_REGISTRY_ENABLED

    # -----------------------------------------------------------------------------------------------------------------
    # Configuring Backend to be used within SettingsRegistry module
    # -----------------------------------------------------------------------------------------------------------------

    # This define enable using QSettings as a source of settings for the SettingsRegistry module
    # DEFINES += DRAUPNIR_SETTINGS_USE_QSETTINGS

    # This define enable using AppSettings as a source of settings for the SettingsRegistry module
    # DEFINES += DRAUPNIR_SETTINGS_USE_APPSETTINGS

    # This define enable using custom settings backends as a source of settings for the SettingsRegistry
    # module
    # DEFINES  += DRAUPNIR_SETTINGS_USE_CUSTOM

    # -----------------------------------------------------------------------------------------------------------------
    # Configuring default settings traits
    # -----------------------------------------------------------------------------------------------------------------

    !contains(DEFINES, DRAUPNIR_CUSTOM_LAST_USED_DIRECTORY_SETTING) {
        DEFINES += DRAUPNIR_DEFAULT_LAST_USED_DIRECTORY_SETTING
    }
    # Add DRAUPNIR_MW_START_HIDDEN_DEFAULT_SETTING somewhere to your *.pro file to override the default
    # setting of the main_window/start_hidden setting
    !contains(DEFINES, DRAUPNIR_MW_START_HIDDEN_DEFAULT_SETTING) {
        DEFINES += DRAUPNIR_MW_START_HIDDEN_DEFAULT_SETTING=false
    }

    !contains(CONFIG, c++20) { CONFIG += c++20 }

    # -----------------------------------------------------------------------------------------------------------------
    # Dependencies
    # -----------------------------------------------------------------------------------------------------------------

    include(DraupnirVersion.pri)
    include(Utils.pri)

    # -----------------------------------------------------------------------------------------------------------------
    # includepath
    # -----------------------------------------------------------------------------------------------------------------

    INCLUDEPATH += $$PWD/../include/settings_registry

    # -----------------------------------------------------------------------------------------------------------------
    # *.h and *.cpp files
    # -----------------------------------------------------------------------------------------------------------------

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
        $$PWD/../include/settings_registry/draupnir/traits/settings/files/LastUsedDirectorySetting.h \
        $$PWD/../include/settings_registry/draupnir/traits/settings/files/RecentFilesListSetting.h \
        $$PWD/../include/settings_registry/draupnir/traits/settings/main_window/MinimizeOnCloseSetting.h \
        $$PWD/../include/settings_registry/draupnir/traits/settings/main_window/MinimizeToTraySetting.h \
        $$PWD/../include/settings_registry/draupnir/traits/settings/main_window/StartHiddenSetting.h \
        $$PWD/../include/settings_registry/draupnir/traits/settings/main_window/WindowSizeSetting.h \
        $$PWD/../include/settings_registry/draupnir/traits/settings/templates/SettingTraitTemplate.h \
        $$PWD/../include/settings_registry/draupnir/utils/SettingTraitSerializer.h \
        $$PWD/../include/settings_registry/draupnir/utils/SettingTraitValidator.h \
        $$PWD/../include/settings_registry/draupnir/utils/SettingTraitsConcat.h \
        $$PWD/../include/settings_registry/draupnir/utils/ValueSerializer.h

    DISTFILES += \
        $$PWD/../docs/pages/SettingsRegistry.dox
}

HEADERS += \
    $$PWD/../include/settings_registry/draupnir/traits/settings/templates/SizeSettingTraitTemplate.h
