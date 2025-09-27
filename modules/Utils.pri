!contains(DEFINES, DRAUPNIR_UTILS_ENABLED) {
    DEFINES += DRAUPNIR_UTILS_ENABLED

    INCLUDEPATH += $$PWD/../include/utils

    HEADERS += \
        $$PWD/../include/utils/draupnir/utils/advance_enum.h \
        $$PWD/../include/utils/draupnir/utils/common.h \
        $$PWD/../include/utils/draupnir/utils/template_constructors.h
}


