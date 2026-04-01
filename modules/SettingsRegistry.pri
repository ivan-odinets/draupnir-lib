!contains(DEFINES, DRAUPNIR_SETTINGS_REGISTRY_ENABLED) {
    DEFINES += DRAUPNIR_SETTINGS_REGISTRY_ENABLED

    !contains(CONFIG, c++23) {
        error("C++23 is required for the whole draupnir-lib. Add CONFIG += c++20 to your *.pro or *.pri file.")
    }

    # -----------------------------------------------------------------------------------------------------------------
    # Configuring Backend to be used within SettingsRegistry module
    # -----------------------------------------------------------------------------------------------------------------

    # This define enable using QSettings as a source of settings for the SettingsRegistry module
    # DEFINES += DRAUPNIR_SETTINGS_USE_QSETTINGS

    # This define enable using AppSettings as a source of settings for the SettingsRegistry module
    # DEFINES += DRAUPNIR_SETTINGS_USE_APPSETTINGS

    # This define enable using custom settings backends as a source of settings for the SettingsRegistry module
    # DEFINES  += DRAUPNIR_SETTINGS_USE_CUSTOM

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
        $$PWD/../include/settings_registry/draupnir/settings_registry/concepts/SettingsBackendConcept.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/concepts/SettingsBundleConcept.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/concepts/SettingTraitConcept.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/core/SettingTemplate.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/traits/settings/files/LastUsedDirectorySetting.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/traits/settings/files/RecentFilesListSetting.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/traits/settings/templates/SettingTraitTemplate.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/traits/settings/templates/SizeSettingTraitTemplate.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/utils/EnumFlagsSerializer.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/utils/OptionalSettingsBundle.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/utils/SettingsTraitsConcatenator.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/utils/SettingTraitPrinter.h \
        $$PWD/../include/settings_registry/draupnir/settings_registry/utils/SettingTraitSerializer.h

    DISTFILES += \
        $$PWD/../docs/pages/SettingsRegistry.dox
}
