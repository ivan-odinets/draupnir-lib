!contains(DEFINES, DRAUPNIR_LOGGING_ENABLED) {
    DEFINES += DRAUPNIR_LOGGING_ENABLED

    !contains(QT, gui) { QT += gui }
    !contains(QT, widgets) { QT += widgets }

    include(Containers.pri)
    include(DraupnirVersion.pri)
    include(Messages.pri)
    include(SettingsRegistry.pri)
    include(UiBricks.pri)

    INCLUDEPATH += $$PWD/../include/logging

    HEADERS += \
        $$PWD/../include/logging/draupnir/logging/core/Logger.h \
        $$PWD/../include/logging/draupnir/logging/core/MessageGroupId.h \
        $$PWD/../include/logging/draupnir/logging/core/MessageReceiverTemplate.h \
        $$PWD/../include/logging/draupnir/logging/traits/settings/log_widget/DisplayedMessageLevelsSettingTrait.h \
        $$PWD/../include/logging/draupnir/logging/ui/widgets/LogWidget.h \
        $$PWD/../include/logging/draupnir/logging/core/AbstractMessageReceiver.h \
        $$PWD/../include/logging/draupnir/logging/traits/settings/log_widget/DisplayedMessageCategoriesSettingTrait.h \
        $$PWD/../include/logging/draupnir/logging/traits/settings/log_widget/DisplayedMessageViewItemFieldsSettingTrait.h \
        $$PWD/../include/logging/draupnir/logging/traits/settings/log_widget/IconSizeSetting.h

    DISTFILES += \
        $$PWD/../docs/pages/Logging.dox

    SOURCES += \
       $$PWD/../src/logging/draupnir/core/Logger.cpp \
        $$PWD/../src/logging/draupnir/ui/widgets/LogWidget.cpp
}

