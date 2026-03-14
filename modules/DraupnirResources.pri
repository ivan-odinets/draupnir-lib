!contains(DEFINES, DRAUPNIR_SETTINGS_RESOURCES_ENABLED) {
    DEFINES += DRAUPNIR_SETTINGS_RESOURCES_ENABLED

    !contains(CONFIG, c++20) {
        error("C++20 is required for the whole draupnir-lib. Add CONFIG += c++20 to your *.pro or *.pri file.")
    }

    include(DraupnirVersion.pri)

    INCLUDEPATH += $$PWD/../include/resources

    HEADERS += \
        $$PWD/../include/resources/draupnir/utils/ResourceHelper.h

    SOURCES += \
        $$PWD/../src/resources/draupnir/utils/ResourceHelper.cpp

    RESOURCES += \
        $$PWD/../resources/draupnir_resources.qrc

    DISTFILES += \
        $$PWD/../docs/pages/DraupnirResources.dox
}




