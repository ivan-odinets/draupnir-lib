!contains(DEFINES, DRAUPNIR_SETTINGS_RESOURCES_ENABLED) {
    DEFINES += DRAUPNIR_SETTINGS_RESOURCES_ENABLED

    !contains(CONFIG, c++20) { CONFIG += c++20 }

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




