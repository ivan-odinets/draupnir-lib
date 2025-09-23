# ---------------------------------------------------------------------------------------------------------------------

QT          += core widgets

# ---------------------------------------------------------------------------------------------------------------------

INCLUDEPATH += $$PWD/include/messages

# ---------------------------------------------------------------------------------------------------------------------

SOURCES += \
    $$PWD/src/messages/Logger.cpp \
    $$PWD/src/messages/core/MessageHandler.cpp \
    $$PWD/src/messages/core/Notification.cpp \
    $$PWD/src/messages/models/MessageListModel.cpp \
    $$PWD/src/messages/models/MessageListProxyModel.cpp \
    $$PWD/src/messages/traits/messages/DefaultMessageTraits.cpp \
    $$PWD/src/messages/ui/menus/MessageViewConfigMenu.cpp \
    $$PWD/src/messages/ui/menus/NotificationTypeMenu.cpp \
    $$PWD/src/messages/ui/widgets/LogWidget.cpp \
    $$PWD/src/messages/ui/widgets/MessageDisplayWidget.cpp \
    $$PWD/src/messages/ui/widgets/MessageListView.cpp \
    $$PWD/src/messages/ui/widgets/MessageNotificationSettingsWidget.cpp \
    $$PWD/src/messages/ui/widgets/NotificationTypeComboBox.cpp \
    $$PWD/src/messages/ui/windows/MessageDisplayDialog.cpp \
    $$PWD/src/messages/ui/windows/MessageViewConfigDialog.cpp

HEADERS += \
    $$PWD/include/messages/MessageGroup.h \
    $$PWD/include/messages/Logger.h \
    $$PWD/include/messages/MessageSystemInterface.h \
    $$PWD/include/messages/MessageSystemTemplate.h \
    $$PWD/include/messages/core/Message.h \
    $$PWD/include/messages/core/MessageHandler.h \
    $$PWD/include/messages/core/MessageHandlerTemplate.h \
    $$PWD/include/messages/core/MessageTemplate.h \
    $$PWD/include/messages/core/MessageType.h \
    $$PWD/include/messages/core/MessageUiBuilder.h \
    $$PWD/include/messages/core/MessageUiBuilderTemplate.h \
    $$PWD/include/messages/core/Notification.h \
    $$PWD/include/messages/models/MessageListModel.h \
    $$PWD/include/messages/models/MessageListProxyModel.h \
    $$PWD/include/messages/traits/messages/DefaultMessageTraits.h \
    $$PWD/include/messages/traits/settings/LogWidgetSettingsTraits.h \
    $$PWD/include/messages/traits/settings/MessageTypeSettingsTrait.h \
    $$PWD/include/messages/ui/menus/MessageViewConfigMenu.h \
    $$PWD/include/messages/ui/menus/NotificationTypeMenu.h \
    $$PWD/include/messages/ui/widgets/LogWidget.h \
    $$PWD/include/messages/ui/widgets/MessageDisplayWidget.h \
    $$PWD/include/messages/ui/widgets/MessageListView.h \
    $$PWD/include/messages/ui/widgets/MessageNotificationSettingsWidget.h \
    $$PWD/include/messages/ui/widgets/NotificationTypeComboBox.h \
    $$PWD/include/messages/ui/windows/MessageDisplayDialog.h \
    $$PWD/include/messages/ui/windows/MessageViewConfigDialog.h \
    $$PWD/include/messages/utils/MessageTraitsHelper.h
