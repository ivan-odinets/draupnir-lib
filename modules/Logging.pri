!contains(DEFINES, DRAUPNIR_LOGGING_ENABLED) {
    DEFINES += DRAUPNIR_LOGGING_ENABLED

    !contains(QT, gui) { QT += gui }
    !contains(QT, widgets) { QT += widgets }

    include(Containers.pri)
    include(DraupnirVersion.pri)
    include(SettingsRegistry.pri)
    include(UiBricks.pri)

    INCLUDEPATH += $$PWD/../include/logging

    SOURCES +=

    HEADERS += \
        $$PWD/../include/logging/draupnir/logging/messages/Message.h \
        $$PWD/../include/logging/draupnir/logging/Logger.h \
        $$PWD/../include/logging/draupnir/logging/core/AbstractMessageHandler.h \
        $$PWD/../include/logging/draupnir/logging/core/AbstractMessageViewIconProvider.h \
        $$PWD/../include/logging/draupnir/logging/messages/MessageCategories.h \
        $$PWD/../include/logging/draupnir/logging/messages/MessageGroup.h \
        $$PWD/../include/logging/draupnir/logging/messages/MessageLevels.h \
        $$PWD/../include/logging/draupnir/logging/messages/MessageTypes.h \
        $$PWD/../include/logging/draupnir/logging/messages/MessageViewItem.h \
        $$PWD/../include/logging/draupnir/logging/messages/MessageViewItemFields.h \
        $$PWD/../include/logging/draupnir/logging/models/MessageListModel.h \
        $$PWD/../include/logging/draupnir/logging/models/MessageListProxyModel.h \
        $$PWD/../include/logging/draupnir/logging/traits/categories/DefaultMessageCategory.h \
        $$PWD/../include/logging/draupnir/logging/ui/widgets/MessageDisplayWidget.h \
        $$PWD/../include/logging/draupnir/logging/ui/widgets/MessageListView.h \
        $$PWD/../include/logging/draupnir/logging/ui/windows/MessageDisplayDialog.h

    DISTFILES += \
        $$PWD/../docs/pages/Logging.dox

    SOURCES += \
        $$PWD/../src/logging/draupnir/Logger.cpp \
        $$PWD/../src/logging/draupnir/core/AbstractMessageViewIconProvider.cpp \
        $$PWD/../src/logging/draupnir/messages/MessageViewItem.cpp \
        $$PWD/../src/logging/draupnir/models/MessageListModel.cpp \
        $$PWD/../src/logging/draupnir/models/MessageListProxyModel.cpp \
        $$PWD/../src/logging/draupnir/ui/widgets/MessageDisplayWidget.cpp \
        $$PWD/../src/logging/draupnir/ui/widgets/MessageListView.cpp \
        $$PWD/../src/logging/draupnir/ui/windows/MessageDisplayDialog.cpp
}
