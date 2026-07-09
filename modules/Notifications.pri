!contains(DEFINES, DRAUPNIR_NOTIFICATIONS_ENABLED) {
    DEFINES += DRAUPNIR_NOTIFICATIONS_ENABLED

    !contains(QT, gui) { QT += gui }
    !contains(QT, widgets) { QT += widgets }

    include(DraupnirVersion.pri)
    include(Logging.pri)

    INCLUDEPATH += $$PWD/../include/notifications

    HEADERS += \
        $$PWD/../include/notifications/draupnir/notifications/concepts/NotificationTypeConcept.h \
        $$PWD/../include/notifications/draupnir/notifications/core/NotificationTypeSerializerInterface.h \
        $$PWD/../include/notifications/draupnir/notifications/core/NotificationTypes.h \
        $$PWD/../include/notifications/draupnir/notifications/core/NotificationTypesSerializerTemplate.h \
        $$PWD/../include/notifications/draupnir/notifications/traits/DialogNotificationTrait.h \
        $$PWD/../include/notifications/draupnir/notifications/traits/NoNotificationTrait.h \
        $$PWD/../include/notifications/draupnir/notifications/traits/TrayNotificationTrait.h \
        $$PWD/../include/notifications/draupnir/notifications/ui/menus/AbstractNotificationTypesSelectorMenu.h \
        $$PWD/../include/notifications/draupnir/notifications/ui/menus/NotificationTypesSelectorMenuTemplate.h
}
