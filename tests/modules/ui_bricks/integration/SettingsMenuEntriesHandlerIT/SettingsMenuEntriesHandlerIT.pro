TEST_NAME = $$basename(PWD)
include(../../../../common/TestConfig.pri)

QT += widgets

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM

include(../../../../common/MenuEntryTraits.pri)
include(../../../../common/SettingsTraits.pri)
include(../../../../common/UiHelpers.pri)

include(../../../../../modules/UiBricks.pri)

SOURCES +=  \
    SettingsMenuEntriesHandlerIT.cpp
