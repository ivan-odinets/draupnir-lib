!contains(DEFINES, DRAUPNIR_LOPTR_ENABLED) {
    DEFINES += DRAUPNIR_LOPTR_ENABLED

    !contains(CONFIG, c++20) { CONFIG += c++20 }

    INCLUDEPATH += $$PWD/../include/loptr

    HEADERS += \
        $$PWD/../include/loptr/draupnir/utils/Terminate.h

    DISTFILES += \
        $$PWD/../docs/pages/Loptr.dox
}
