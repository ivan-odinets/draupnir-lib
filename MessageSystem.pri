# ---------------------------------------------------------------------------------------------------------------------
# Build config options.
# Note: Better to enable/disable sth not in this file, but in the main *.pro

# ---------------------------------------------------------------------------------------------------------------------
# Multithreading
DEFINES += DRAUPNIR_MSGSYS_MULTITHREAD

# ---------------------------------------------------------------------------------------------------------------------
# Settings storage

# This define enable using AppSettings as a source of settings for the MessageSystem
#DEFINES += DRAUPNIR_MSGSYS_APP_SETTINGS

# This define enable using custom settings object as a source of settings for the MessageSystem
#DEFINES += DRAUPNIR_MSGSYS_CUSTOM_SETTINGS

# ---------------------------------------------------------------------------------------------------------------------

QT          += core widgets
CONFIG      += c++20

# ---------------------------------------------------------------------------------------------------------------------

INCLUDEPATH += $$PWD/include/messages

# ---------------------------------------------------------------------------------------------------------------------

contains(DEFINES, DRAUPNIR_MSGSYS_APP_SETTINGS) {
    include($$PWD/AppSettings.pri)
}

contains(DEFINES, DRAUPNIR_MSGSYS_CUSTOM_SETTINGS) {
    HEADERS += \
        $$PWD/include/messages/MessageSettingsInterface.h
}

SOURCES += \
    $$PWD/src/messages/core/DefaultMessageTraits.cpp \
    $$PWD/src/messages/core/Logger.cpp \
    $$PWD/src/messages/core/MessageHandler.cpp \
    $$PWD/src/messages/core/Notification.cpp \
    $$PWD/src/messages/menus/MessageViewConfigMenu.cpp \
    $$PWD/src/messages/menus/NotificationTypeMenu.cpp \
    $$PWD/src/messages/models/MessageListModel.cpp \
    $$PWD/src/messages/models/MessageListProxyModel.cpp \
    $$PWD/src/messages/widgets/LogWidget.cpp \
    $$PWD/src/messages/widgets/MessageDisplayWidget.cpp \
    $$PWD/src/messages/widgets/MessageListView.cpp \
    $$PWD/src/messages/widgets/MessageNotificationSettingsWidget.cpp \
    $$PWD/src/messages/widgets/NotificationTypeComboBox.cpp \
    $$PWD/src/messages/windows/MessageDisplayDialog.cpp \
    $$PWD/src/messages/windows/MessageViewConfigDialog.cpp

HEADERS += \
    $$PWD/include/containers/fixed_map.h \
    $$PWD/include/containers/fixed_tuple_map.h \
    $$PWD/include/messages/MessageGroup.h \
    $$PWD/include/messages/Logger.h \
    $$PWD/include/messages/MessageSystemInterface.h \
    $$PWD/include/messages/MessageSystemTemplate.h \
    $$PWD/include/messages/MessageType.h \
    $$PWD/include/messages/MessageUiBuilder.h \
    $$PWD/include/messages/Notification.h \
    $$PWD/include/utils/advance_enum.h \
    $$PWD/include/utils/common.h \
    $$PWD/include/utils/template_constructors.h \
    $$PWD/src/messages/core/DefaultMessageTraits.h \
    $$PWD/src/messages/core/Message.h \
    $$PWD/src/messages/core/MessageHandler.h \
    $$PWD/src/messages/core/MessageHandlerTemplate.h \
    $$PWD/src/messages/core/MessageTemplate.h \
    $$PWD/src/messages/core/MessageTraitsHelper.h \
    $$PWD/src/messages/core/MessageUiBuilderTemplate.h \
    $$PWD/src/messages/menus/MessageViewConfigMenu.h \
    $$PWD/src/messages/menus/NotificationTypeMenu.h \
    $$PWD/src/messages/models/MessageListModel.h \
    $$PWD/src/messages/models/MessageListProxyModel.h \
    $$PWD/src/messages/widgets/LogWidget.h \
    $$PWD/src/messages/widgets/MessageDisplayWidget.h \
    $$PWD/src/messages/widgets/MessageListView.h \
    $$PWD/src/messages/widgets/MessageNotificationSettingsWidget.h \
    $$PWD/src/messages/widgets/NotificationTypeComboBox.h \
    $$PWD/src/messages/windows/MessageDisplayDialog.h \
    $$PWD/src/messages/windows/MessageViewConfigDialog.h
