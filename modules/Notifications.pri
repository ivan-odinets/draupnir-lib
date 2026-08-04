!contains(DEFINES, DRAUPNIR_NOTIFICATIONS_ENABLED) {
    DEFINES += DRAUPNIR_NOTIFICATIONS_ENABLED

    !contains(QT, gui) { QT += gui }
    !contains(QT, widgets) { QT += widgets }

    include(DraupnirVersion.pri)
    include(Logging.pri)

    INCLUDEPATH += $$PWD/../include/notifications

    HEADERS += \
        $$PWD/../include/notifications/draupnir/notifications/channels/DialogNotificationChannel.h \
        $$PWD/../include/notifications/draupnir/notifications/channels/TrayNotificationChannel.h \
        $$PWD/../include/notifications/draupnir/notifications/concepts/NotificationChannelConcept.h \
        $$PWD/../include/notifications/draupnir/notifications/core/NotificationDispatcherTemplate.h \
        $$PWD/../include/notifications/draupnir/notifications/core/NotificationTypesSerializerInterface.h \
        $$PWD/../include/notifications/draupnir/notifications/traits/settings/MessageNotificationSettingTraitTemplate.h \
        $$PWD/../include/notifications/draupnir/notifications/utils/NotificationTypesSerializer.h \
        $$PWD/../include/notifications/draupnir/notifications/core/NotificationTypes.h \
        $$PWD/../include/notifications/draupnir/notifications/core/NotificationTypesSerializerTemplate.h \
        $$PWD/../include/notifications/draupnir/notifications/ui/menus/AbstractNotificationTypesSelectorMenu.h \
        $$PWD/../include/notifications/draupnir/notifications/ui/menus/NotificationTypesSelectorMenuTemplate.h \
        $$PWD/../include/notifications/draupnir/notifications/handlers/MessageHandlerTemplate.h \
        $$PWD/../include/notifications/draupnir/notifications/traits/notifications/NoneNotificationTrait.h

    SOURCES += \
        $$PWD/../src/notifications/draupnir/channels/DialogNotificationChannel.cpp \
        $$PWD/../src/notifications/draupnir/channels/TrayNotificationChannel.cpp
}


