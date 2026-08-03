!contains(DEFINES, DRAUPNIR_MESSAGES_ENABLED) {
    DEFINES += DRAUPNIR_MESSAGES_ENABLED

    !contains(QT, gui) { QT += gui }
    !contains(QT, widgets) { QT += widgets }

    include(Containers.pri)
    include(DraupnirVersion.pri)
    include(SettingsRegistry.pri)
    include(UiBricks.pri)

    INCLUDEPATH += $$PWD/../include/messages

    SOURCES +=

    HEADERS += \
    $$PWD/../include/messages/draupnir/messages/core/MessageTypeIconProvider.h \
        $$PWD/../include/messages/draupnir/messages/traits/MessageTypeTraitTemplate.h \
        $$PWD/../include/messages/draupnir/messages/concepts/MessageTypeTraitConcept.h \
        $$PWD/../include/messages/draupnir/messages/concepts/MessageCategoryTraitConcept.h \
        $$PWD/../include/messages/draupnir/messages/core/Message.h \
        $$PWD/../include/messages/draupnir/messages/core/AbstractMessageHandler.h \
        $$PWD/../include/messages/draupnir/messages/core/MessageLevels.h \
        $$PWD/../include/messages/draupnir/messages/core/MessageType.h \
        $$PWD/../include/messages/draupnir/messages/core/MessageViewItem.h \
        $$PWD/../include/messages/draupnir/messages/core/MessageViewItemFields.h \
        $$PWD/../include/messages/draupnir/messages/categories/MessageCategories.h \
        $$PWD/../include/messages/draupnir/messages/models/MessageListModel.h \
        $$PWD/../include/messages/draupnir/messages/models/MessageListProxyModel.h \
        $$PWD/../include/messages/draupnir/messages/ui/widgets/MessageDisplayWidget.h \
        $$PWD/../include/messages/draupnir/messages/ui/widgets/MessageListView.h \
        $$PWD/../include/messages/draupnir/messages/ui/windows/MessageDisplayDialog.h \
        $$PWD/../include/messages/draupnir/messages/traits/categories/DefaultMessageCategoryTrait.h \
        $$PWD/../include/messages/draupnir/messages/traits/categories/NetworkMessageCategoryTrait.h \
        $$PWD/../include/messages/draupnir/messages/ui/menus/MessageLevelsSelectorMenu.h \
        $$PWD/../include/messages/draupnir/messages/ui/menus/MessageViewItemFieldsSelectorMenu.h \
        $$PWD/../include/messages/draupnir/messages/ui/menus/AbstractMessageCategoriesSelectorMenu.h \
        $$PWD/../include/messages/draupnir/messages/ui/menus/MessageCategoriesSelectorMenuTemplate.h \
        $$PWD/../include/messages/draupnir/messages/categories/MessageCategoriesSerializerInterface.h \
        $$PWD/../include/messages/draupnir/messages/utils/MessageCategoriesSerializer.h \

    DISTFILES += \
        $$PWD/../docs/pages/Logging.dox

    SOURCES += \
    $$PWD/../src/messages/draupnir/core/MessageTypeIconProvider.cpp \
        $$PWD/../src/messages/draupnir/core/MessageViewItem.cpp \
        $$PWD/../src/messages/draupnir/core/MessageViewItemFields.cpp \
        $$PWD/../src/messages/draupnir/models/MessageListModel.cpp \
        $$PWD/../src/messages/draupnir/models/MessageListProxyModel.cpp \
    $$PWD/../src/messages/draupnir/ui/widgets/MessageDisplayWidget.cpp \
        $$PWD/../src/messages/draupnir/ui/widgets/MessageListView.cpp \
        $$PWD/../src/messages/draupnir/ui/windows/MessageDisplayDialog.cpp \
        $$PWD/../src/messages/draupnir/ui/menus/MessageViewItemFieldsSelectorMenu.cpp \
        $$PWD/../src/messages/draupnir/ui/menus/MessageLevelsSelectorMenu.cpp \
        $$PWD/../src/messages/draupnir/core/MessageLevels.cpp
}

HEADERS += \
    $$PWD/../include/messages/draupnir/messages/categories/MessageCategoriesSerializerTemplate.h
