TEMPLATE = subdirs
CONFIG += ordered warn_on qt debug_and_release

SUBDIRS += \
    integration/MessageSystemIT \
    integration/MessageUiBuilderTemplateIT \
    unit/LoggerMultithreadTest \
    unit/LoggerTest \
    unit/MessageFieldsSelectorWidgetTest \
    unit/MessageGroupTest \
    unit/MessageHandlerTemplateTest \
    unit/MessageListProxyModelTest \
    unit/MessageListViewTest \
    unit/MessageListViewConfigMenuTemplateTest \
    unit/MessageTypesSelectorWidgetTemplateTest \
    unit/NotificationTypeComboBoxTest \
    unit/NotificationTypeMenuTest
