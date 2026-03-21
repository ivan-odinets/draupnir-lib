TEST_NAME = $$basename(PWD)
include(../../../../common/TestConfig.pri)

QT += widgets concurrent

DEFINES += DRAUPNIR_SETTINGS_USE_CUSTOM
DEFINES += DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD

include(../../../../common/MockSettingsTemplate.pri)
include(../../../../common/MessageTraits.pri)

include(../../../../../modules/MessageSystem.pri)

SOURCES +=  \
    MessageSystemIT.cpp


