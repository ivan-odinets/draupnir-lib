!contains(DEFINES, DRAUPNIR_MESSAGE_SYSTEM_ENABLED) {
    DEFINES += DRAUPNIR_MESSAGE_SYSTEM_ENABLED

    !contains(CONFIG, c++20) { CONFIG += c++20 }
    !contains(QT, gui) { QT += gui }
    !contains(QT, widgets) { QT += widgets }

    include(Containers.pri)
    include(DraupnirVersion.pri)
    include(SettingsRegistry.pri)
    include(UiBricks.pri)

    INCLUDEPATH += $$PWD/../include/message_system

    SOURCES += \
        $$PWD/../src/message_system/draupnir/Logger.cpp \
        $$PWD/../src/message_system/draupnir/core/AbstractMessageHandler.cpp \
        $$PWD/../src/message_system/draupnir/core/Notification.cpp \
        $$PWD/../src/message_system/draupnir/models/MessageListModel.cpp \
        $$PWD/../src/message_system/draupnir/models/MessageListProxyModel.cpp \
        $$PWD/../src/message_system/draupnir/ui/menus/AbstractMessageListViewConfigMenu.cpp \
        $$PWD/../src/message_system/draupnir/ui/menus/NotificationTypeMenu.cpp \
        $$PWD/../src/message_system/draupnir/ui/widgets/AbstractNotificationSettingsWidget.cpp \
        $$PWD/../src/message_system/draupnir/ui/widgets/LogWidget.cpp \
        $$PWD/../src/message_system/draupnir/ui/widgets/MessageDisplayWidget.cpp \
        $$PWD/../src/message_system/draupnir/ui/widgets/MessageListView.cpp \
        $$PWD/../src/message_system/draupnir/ui/widgets/NotificationTypeComboBox.cpp \
        $$PWD/../src/message_system/draupnir/ui/windows/MessageDisplayDialog.cpp \
        $$PWD/../src/message_system/draupnir/ui/widgets/MessageFieldsSelectorWidget.cpp \
        $$PWD/../src/message_system/draupnir/ui/windows/MessageSystemConfigDialog.cpp

    HEADERS += \
        $$PWD/../include/message_system/draupnir/Logger.h \
        $$PWD/../include/message_system/draupnir/MessageGroup.h \
        $$PWD/../include/message_system/draupnir/MessageSystemTemplate.h \
        $$PWD/../include/message_system/draupnir/core/AbstractMessageHandler.h \
        $$PWD/../include/message_system/draupnir/core/AbstractMessageUiBuilder.h \
        $$PWD/../include/message_system/draupnir/core/Message.h \
        $$PWD/../include/message_system/draupnir/core/MessageHandlerTemplate.h \
        $$PWD/../include/message_system/draupnir/core/MessageType.h \
        $$PWD/../include/message_system/draupnir/core/MessageUiBuilderTemplate.h \
        $$PWD/../include/message_system/draupnir/core/Notification.h \
        $$PWD/../include/message_system/draupnir/models/MessageListModel.h \
        $$PWD/../include/message_system/draupnir/models/MessageListProxyModel.h \
        $$PWD/../include/message_system/draupnir/traits/messages/DefaultMessageTraits.h \
        $$PWD/../include/message_system/draupnir/traits/settings/LogWidgetSettingsTraits.h \
        $$PWD/../include/message_system/draupnir/traits/settings/MessageTypeSettingsTrait.h \
        $$PWD/../include/message_system/draupnir/ui/menus/MessageListViewConfigMenuTemplate.h \
        $$PWD/../include/message_system/draupnir/ui/menus/NotificationTypeMenu.h \
        $$PWD/../include/message_system/draupnir/ui/widgets/AbstractNotificationSettingsWidget.h \
        $$PWD/../include/message_system/draupnir/ui/widgets/LogWidget.h \
        $$PWD/../include/message_system/draupnir/ui/widgets/MessageDisplayWidget.h \
        $$PWD/../include/message_system/draupnir/ui/widgets/MessageListView.h \
        $$PWD/../include/message_system/draupnir/ui/widgets/NotificationTypeComboBox.h \
        $$PWD/../include/message_system/draupnir/ui/windows/MessageDisplayDialog.h \
        $$PWD/../include/message_system/draupnir/utils/MessageTraitsHelper.h \
        $$PWD/../include/message_system/draupnir/core/MessageFieldsSelectorBase.h \
        $$PWD/../include/message_system/draupnir/core/MessageTypeSelectorBase.h \
        $$PWD/../include/message_system/draupnir/traits/messages/DebugMessageTrait.h \
        $$PWD/../include/message_system/draupnir/traits/messages/ErrorMessageTrait.h \
        $$PWD/../include/message_system/draupnir/traits/messages/InfoMessageTrait.h \
        $$PWD/../include/message_system/draupnir/traits/messages/WarningMessageTrait.h \
        $$PWD/../include/message_system/draupnir/traits/widgets/MessageSystemDialogWidgetTraits.h \
        $$PWD/../include/message_system/draupnir/ui/menus/AbstractMessageListViewConfigMenu.h \
        $$PWD/../include/message_system/draupnir/ui/widgets/AbstractMessageTypesSelectorWidget.h \
        $$PWD/../include/message_system/draupnir/ui/widgets/MessageFieldsSelectorWidget.h \
        $$PWD/../include/message_system/draupnir/ui/widgets/MessageTypesSelectorWidgetTemplate.h \
        $$PWD/../include/message_system/draupnir/ui/widgets/NotificationSettingsWidgetTemplate.h \
        $$PWD/../include/message_system/draupnir/ui/windows/MessageSystemConfigDialog.h \
        $$PWD/../include/message_system/draupnir/utils/CheckableUiElementHelper.h

    DISTFILES += \
        $$PWD/../docs/pages/MessageSystem.dox
}
