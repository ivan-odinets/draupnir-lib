!contains(DEFINES, DRAUPNIR_MESSAGE_SYSTEM_ENABLED) {
    DEFINES += DRAUPNIR_MESSAGE_SYSTEM_ENABLED

    !contains(CONFIG, c++20) { CONFIG += c++20 }
    !contains(QT, gui) { QT += gui }
    !contains(QT, widgets) { QT += widgets }

    include(Containers.pri)
    include(DraupnirVersion.pri)
    include(SettingsRegistry.pri)

    INCLUDEPATH += $$PWD/../include/message_system

    SOURCES += \
        $$PWD/../src/message_system/draupnir/Logger.cpp \
        $$PWD/../src/message_system/draupnir/core/MessageHandler.cpp \
        $$PWD/../src/message_system/draupnir/core/Notification.cpp \
        $$PWD/../src/message_system/draupnir/models/MessageListModel.cpp \
        $$PWD/../src/message_system/draupnir/models/MessageListProxyModel.cpp \
        $$PWD/../src/message_system/draupnir/traits/messages/DefaultMessageTraits.cpp \
        $$PWD/../src/message_system/draupnir/ui/menus/MessageViewConfigMenu.cpp \
        $$PWD/../src/message_system/draupnir/ui/menus/NotificationTypeMenu.cpp \
        $$PWD/../src/message_system/draupnir/ui/widgets/LogWidget.cpp \
        $$PWD/../src/message_system/draupnir/ui/widgets/MessageDisplayWidget.cpp \
        $$PWD/../src/message_system/draupnir/ui/widgets/MessageListView.cpp \
        $$PWD/../src/message_system/draupnir/ui/widgets/MessageNotificationSettingsWidget.cpp \
        $$PWD/../src/message_system/draupnir/ui/widgets/NotificationTypeComboBox.cpp \
        $$PWD/../src/message_system/draupnir/ui/windows/MessageDisplayDialog.cpp \
        $$PWD/../src/message_system/draupnir/ui/windowsMessageViewConfigDialog.cpp

    HEADERS += \
        $$PWD/../include/message_system/draupnir/Logger.h \
        $$PWD/../include/message_system/draupnir/MessageGroup.h \
        $$PWD/../include/message_system/draupnir/MessageSystemInterface.h \
        $$PWD/../include/message_system/draupnir/MessageSystemTemplate.h \
        $$PWD/../include/message_system/draupnir/core/Message.h \
        $$PWD/../include/message_system/draupnir/core/MessageHandlerInterface.h \
        $$PWD/../include/message_system/draupnir/core/MessageHandlerTemplate.h \
        $$PWD/../include/message_system/draupnir/core/MessageType.h \
    $$PWD/../include/message_system/draupnir/core/MessageUiBuilderInterface.h \
        $$PWD/../include/message_system/draupnir/core/MessageUiBuilderTemplate.h \
        $$PWD/../include/message_system/draupnir/core/Notification.h \
        $$PWD/../include/message_system/draupnir/models/MessageListModel.h \
        $$PWD/../include/message_system/draupnir/models/MessageListProxyModel.h \
        $$PWD/../include/message_system/draupnir/traits/messages/DefaultMessageTraits.h \
        $$PWD/../include/message_system/draupnir/traits/settings/LogWidgetSettingsTraits.h \
        $$PWD/../include/message_system/draupnir/traits/settings/MessageTypeSettingsTrait.h \
        $$PWD/../include/message_system/draupnir/ui/menus/MessageViewConfigMenu.h \
        $$PWD/../include/message_system/draupnir/ui/menus/NotificationTypeMenu.h \
        $$PWD/../include/message_system/draupnir/ui/widgets/LogWidget.h \
        $$PWD/../include/message_system/draupnir/ui/widgets/MessageDisplayWidget.h \
        $$PWD/../include/message_system/draupnir/ui/widgets/MessageListView.h \
        $$PWD/../include/message_system/draupnir/ui/widgets/MessageNotificationSettingsWidget.h \
        $$PWD/../include/message_system/draupnir/ui/widgets/NotificationTypeComboBox.h \
        $$PWD/../include/message_system/draupnir/ui/windows/MessageDisplayDialog.h \
        $$PWD/../include/message_system/draupnir/ui/windows/MessageViewConfigDialog.h \
        $$PWD/../include/message_system/draupnir/utils/MessageTraitsHelper.h

    DISTFILES += \
        $$PWD/../docs/pages/MessageSystem.dox
}

HEADERS += \
    $$PWD/../include/message_system/draupnir/ui/windows/MessageViewConfigDialogTemplate.h
