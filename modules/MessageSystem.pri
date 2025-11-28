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
        $$PWD/../include/message_system/draupnir/message_system/core/AbstractMessageHandler.h \
        $$PWD/../include/message_system/draupnir/message_system/core/AbstractMessageUiBuilder.h \
        $$PWD/../include/message_system/draupnir/message_system/core/Message.h \
        $$PWD/../include/message_system/draupnir/message_system/core/MessageFieldsSelectorBase.h \
        $$PWD/../include/message_system/draupnir/message_system/core/MessageGroup.h \
        $$PWD/../include/message_system/draupnir/message_system/core/MessageHandlerTemplate.h \
        $$PWD/../include/message_system/draupnir/message_system/core/MessageSystemTemplate.h \
        $$PWD/../include/message_system/draupnir/message_system/core/MessageType.h \
        $$PWD/../include/message_system/draupnir/message_system/core/MessageTypeSelectorBase.h \
        $$PWD/../include/message_system/draupnir/message_system/core/MessageUiBuilderTemplate.h \
        $$PWD/../include/message_system/draupnir/message_system/core/Notification.h \
        $$PWD/../include/message_system/draupnir/message_system/models/MessageListModel.h \
        $$PWD/../include/message_system/draupnir/message_system/models/MessageListProxyModel.h \
        $$PWD/../include/message_system/draupnir/message_system/traits/messages/DebugMessageTrait.h \
        $$PWD/../include/message_system/draupnir/message_system/traits/messages/DefaultMessageTraits.h \
        $$PWD/../include/message_system/draupnir/message_system/traits/messages/ErrorMessageTrait.h \
        $$PWD/../include/message_system/draupnir/message_system/traits/messages/InfoMessageTrait.h \
        $$PWD/../include/message_system/draupnir/message_system/traits/messages/WarningMessageTrait.h \
        $$PWD/../include/message_system/draupnir/message_system/traits/settings/LogWidgetSettingsTraits.h \
        $$PWD/../include/message_system/draupnir/message_system/traits/settings/MessageTypeSettingsTrait.h \
        $$PWD/../include/message_system/draupnir/message_system/traits/widgets/MessageTypesSelectorTrait.h \
        $$PWD/../include/message_system/draupnir/message_system/ui/menus/AbstractMessageListViewConfigMenu.h \
        $$PWD/../include/message_system/draupnir/message_system/ui/menus/MessageListViewConfigMenuTemplate.h \
        $$PWD/../include/message_system/draupnir/message_system/ui/menus/NotificationTypeMenu.h \
        $$PWD/../include/message_system/draupnir/message_system/ui/widgets/AbstractMessageTypesSelectorWidget.h \
        $$PWD/../include/message_system/draupnir/message_system/ui/widgets/AbstractNotificationSettingsWidget.h \
        $$PWD/../include/message_system/draupnir/message_system/ui/widgets/LogWidget.h \
        $$PWD/../include/message_system/draupnir/message_system/ui/widgets/MessageDisplayWidget.h \
        $$PWD/../include/message_system/draupnir/message_system/ui/widgets/MessageFieldsSelectorWidget.h \
        $$PWD/../include/message_system/draupnir/message_system/ui/widgets/MessageListView.h \
        $$PWD/../include/message_system/draupnir/message_system/ui/widgets/MessageTypesSelectorWidgetTemplate.h \
        $$PWD/../include/message_system/draupnir/message_system/ui/widgets/NotificationSettingsWidgetTemplate.h \
        $$PWD/../include/message_system/draupnir/message_system/ui/widgets/NotificationTypeComboBox.h \
        $$PWD/../include/message_system/draupnir/message_system/ui/windows/MessageDisplayDialog.h \
        $$PWD/../include/message_system/draupnir/message_system/ui/windows/MessageSystemConfigDialog.h \
        $$PWD/../include/message_system/draupnir/message_system/utils/CheckableUiElementHelper.h \
        $$PWD/../include/message_system/draupnir/message_system/utils/MessageTraitsHelper.h

    DISTFILES += \
        $$PWD/../docs/pages/MessageSystem.dox
}

HEADERS += \
    $$PWD/../include/message_system/draupnir/MessageSystem.h \
    $$PWD/../include/message_system/draupnir/message_system/traits/widgets/MessageFieldsSelectorTrait.h \
    $$PWD/../include/message_system/draupnir/message_system/traits/widgets/NotificationSettingsEditorWidgetTrait.h
