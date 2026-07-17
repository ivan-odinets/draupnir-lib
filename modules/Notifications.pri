!contains(DEFINES, DRAUPNIR_NOTIFICATIONS_ENABLED) {
    DEFINES += DRAUPNIR_NOTIFICATIONS_ENABLED

    !contains(QT, gui) { QT += gui }
    !contains(QT, widgets) { QT += widgets }

    include(DraupnirVersion.pri)
    include(Logging.pri)

    INCLUDEPATH += $$PWD/../include/notifications

    HEADERS += \
        $$PWD/../include/notifications/draupnir/notifications/traits/notifications/DialogNotificationTrait.h \
        $$PWD/../include/notifications/draupnir/notifications/traits/notifications/TrayNotificationTrait.h \
        $$PWD/../include/notifications/draupnir/notifications/utils/NotificationTypesSerializer.h \
        $$PWD/../include/notifications/draupnir/notifications/concepts/NotificationTypeConcept.h \
        $$PWD/../include/notifications/draupnir/notifications/core/NotificationTypeSerializerInterface.h \
        $$PWD/../include/notifications/draupnir/notifications/core/NotificationTypes.h \
        $$PWD/../include/notifications/draupnir/notifications/core/NotificationTypesSerializerTemplate.h \
        $$PWD/../include/notifications/draupnir/notifications/ui/menus/AbstractNotificationTypesSelectorMenu.h \
        $$PWD/../include/notifications/draupnir/notifications/ui/menus/NotificationTypesSelectorMenuTemplate.h \
        $$PWD/../include/notifications/draupnir/notifications/concepts/MessageTypeTraitConcept.h \
        $$PWD/../include/notifications/draupnir/notifications/concepts/NotificationHandlerConcepts.h \
        $$PWD/../include/notifications/draupnir/notifications/handlers/DialogNotificationHandler.h \
        $$PWD/../include/notifications/draupnir/notifications/handlers/MessageHandlerTemplate.h \
        $$PWD/../include/notifications/draupnir/notifications/handlers/NotificationDispatcherTemplate.h \
        $$PWD/../include/notifications/draupnir/notifications/handlers/NotificationHandlerTemplate.h \
        $$PWD/../include/notifications/draupnir/notifications/handlers/TrayNotificationHandler.h \
        $$PWD/../include/notifications/draupnir/notifications/traits/messages/MessageTypeTraitTemplate.h \
        $$PWD/../include/notifications/draupnir/notifications/traits/settings/MessageNotificationSettingTrait.h
}
