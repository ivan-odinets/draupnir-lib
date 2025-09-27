!contains(DEFINES, DRAUPNIR_CONTAINERS_ENABLED) {
    DEFINES += DRAUPNIR_CONTAINERS_ENABLED

    include(Utils.pri)

    INCLUDEPATH += $$PWD/../include/containers

    HEADERS += \
        $$PWD/../include/containers/draupnir/containers/fixed_map.h \
        $$PWD/../include/containers/draupnir/containers/fixed_tuple_map.h
}
