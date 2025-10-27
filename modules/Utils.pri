!contains(DEFINES, DRAUPNIR_UTILS_ENABLED) {
    DEFINES += DRAUPNIR_UTILS_ENABLED

    !contains(CONFIG, c++20) { CONFIG += c++20 }

    include(DraupnirVersion.pri)

    INCLUDEPATH += $$PWD/../include/utils

    HEADERS += \
        $$PWD/../include/utils/draupnir/utils/advance_enum.h \
        $$PWD/../include/utils/draupnir/utils/index_of.h \
        $$PWD/../include/utils/draupnir/utils/template_constructors.h \
        $$PWD/../include/utils/draupnir/utils/template_detectors.h \
        $$PWD/../include/utils/draupnir/utils/type_presense.h

    DISTFILES += \
        $$PWD/../docs/pages/Utils.dox
}
