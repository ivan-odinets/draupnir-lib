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
            $$PWD/../include/settings_registry/draupnir/settings_registry/core/AppSettings.h

        SOURCES += \
            $$PWD/../src/settings_registry/draupnir/core/AppSettings.cpp
    }

    contains(DEFINES, DRAUPNIR_SETTINGS_USE_CUSTOM) {
        HEADERS += \
            $$PWD/../include/settings_registry/draupnir/settings_registry/core/SettingsBackendInterface.h
    }

    HEADERS += \
        $$PWD/../include/settings_registry/draupnir/SettingsRegistry.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/SettingsBundleTemplate.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/SettingsRegistryTemplate.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/core/SettingTemplate.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/traits/settings/files/LastUsedDirectorySetting.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/traits/settings/files/RecentFilesListSetting.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/traits/settings/templates/SettingTraitTemplate.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/utils/SettingTraitSerializer.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/traits/settings/templates/SizeSettingTraitTemplate.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/utils/SettingsTraitsConcatenator.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/utils/ValueSerializer.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/concepts/SettingTraitConcept.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/utils/OptionalSettingsBundle.h

    DISTFILES += \
        $$PWD/../docs/pages/SettingsRegistry.dox
}
