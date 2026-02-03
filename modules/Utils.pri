!contains(DEFINES, DRAUPNIR_UTILS_ENABLED) {
    DEFINES += DRAUPNIR_UTILS_ENABLED

    !contains(CONFIG, c++20) { CONFIG += c++20 }

    include(DraupnirVersion.pri)

    INCLUDEPATH += $$PWD/../include/utils

    HEADERS += \
        $$PWD/../include/utils/draupnir/utils/advance_enum.h \
        $$PWD/../include/utils/draupnir/utils/index_of.h \
        $$PWD/../include/utils/draupnir/utils/sfinae_detector_macro.h \
        $$PWD/../include/utils/draupnir/utils/template_constructors.h \
        $$PWD/../include/utils/draupnir/utils/template_detectors.h \
        $$PWD/../include/utils/draupnir/utils/type_presense.h \
        $$PWD/../include/utils/draupnir/utils/filter_if.h \
        $$PWD/../include/utils/draupnir/utils/tuple_like_merge.h \
        $$PWD/../include/utils/draupnir/utils/type_extractors.h \
        $$PWD/../include/utils/draupnir/utils/type_list.h \
        $$PWD/../include/utils/draupnir/utils/type_name_utils.h \
        $$PWD/../include/utils/draupnir/utils/class_marcos.h \
        $$PWD/../include/utils/draupnir/utils/type_if.h \
        $$PWD/../include/utils/draupnir/utils/template_adapters.h \
        $$PWD/../include/utils/draupnir/utils/type_detectors.h

    DISTFILES += \
        $$PWD/../docs/pages/Utils.dox
}
