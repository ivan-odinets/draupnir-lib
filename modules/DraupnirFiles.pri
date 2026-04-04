!contains(DEFINES, DRAUPNIR_FILES_ENABLED) {
    DEFINES += DRAUPNIR_FILES_ENABLED

    !contains(CONFIG, c++23) {
        error("C++23 is required for the whole draupnir-lib. Add CONFIG += c++23 to your *.pro or *.pri file.")
    }

    include(DraupnirVersion.pri)
    include(Utils.pri)

    INCLUDEPATH += $$PWD/../include/files

    HEADERS += \
        $$PWD/../include/files/draupnir/files/ScopedFileTemplate.h \
        $$PWD/../include/files/draupnir/files/concepts/FileConcept.h \
        $$PWD/../include/files/draupnir/files/file_types/AbstractFile.h \
        $$PWD/../include/files/draupnir/files/file_types/AbstractJsonFile.h \
        $$PWD/../include/files/draupnir/files/file_types/AbstractTextFile.h \
        $$PWD/../include/files/draupnir/files/file_types/AbstractXmlFile.h \
        $$PWD/../include/files/draupnir/files/managers/AbstractFileManager.h \
        $$PWD/../include/files/draupnir/files/managers/SingleFileManagerTemplate.h

    SOURCES += \
        $$PWD/../src/files/file_types/AbstractFile.cpp
}
