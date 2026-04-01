TEST_NAME = $$basename(PWD)
include(../../../../common/TestConfig.pri)
include(../../../../common/DummyFile.pri)
include(../../../../common/FileTestHelpers.pri)

QT += widgets

include(../../../../../modules/DraupnirFiles.pri)

SOURCES += \
    ScopedFileTemplateTest.cpp
