!contains(DEFINES, DRAUPNIR_MESSAGE_SYSTEM_ENABLED) {
    DEFINES += DRAUPNIR_MESSAGE_SYSTEM_ENABLED

    !contains(QT, gui) { QT += gui }
    !contains(QT, widgets) { QT += widgets }

    include(DraupnirVersion.pri)
    include(Logging.pri)
    include(Notifications.pri)

    INCLUDEPATH += $$PWD/../include/message_system

    HEADERS += \
        $$PWD/../include/message_system/draupnir/message_system/core/AbstractMessageHandler.h \
        $$PWD/../include/message_system/draupnir/message_system/core/MessageHandlerTemplate.h \
        $$PWD/../include/message_system/draupnir/message_system/core/MessageSystemTemplate.h \
        $$PWD/../include/message_system/draupnir/message_system/core/MessageSystemTypeBuilder.h
}
