!contains(DEFINES, DRAUPNIR_LOPTR_ENABLED) {
    DEFINES += DRAUPNIR_LOPTR_ENABLED

    !contains(CONFIG, c++20) { CONFIG += c++20 }

    INCLUDEPATH += $$PWD/../include/loptr

    HEADERS += \
        $$PWD/../include/loptr/draupnir/loptr/ui/widgets/RotateScreenOverlay.h \
        $$PWD/../include/loptr/draupnir/loptr/utils/Terminate.h \
        $$PWD/../include/loptr/draupnir/loptr/utils/Barrel.h

    SOURCES += \
        $$PWD/../src/loptr/ui/widgets/RotateScreenOverlay.cpp \
        $$PWD/../src/loptr/utils/Barrel.cpp

    DISTFILES += \
        $$PWD/../docs/pages/Loptr.dox
}
