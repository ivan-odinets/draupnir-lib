!contains(DEFINES, DRAUPNIR_CONTAINERS_ENABLED) {
    DEFINES += DRAUPNIR_CONTAINERS_ENABLED

    !contains(CONFIG, c++20) {
        error("C++20 is required for the whole draupnir-lib. Add CONFIG += c++20 to your *.pro or *.pri file.")
    }

    include(DraupnirVersion.pri)
    include(Utils.pri)

    INCLUDEPATH += $$PWD/../include/containers

    HEADERS += \
        $$PWD/../include/containers/draupnir/containers/fixed_map.h \
        $$PWD/../include/containers/draupnir/containers/fixed_tuple_map.h

    DISTFILES += \
        $$PWD/../docs/pages/Containers.dox
}

