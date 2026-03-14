TEST_NAME = $$basename(PWD)
include(../../../../common/TestConfig.pri)

QT += widgets

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

include(../../../../common/DummyFileContext.pri)

include(../../../../../modules/UiBricks.pri)

SOURCES +=  \
    FileSaveAsEntryHandlerTest.cpp
