TEMPLATE = subdirs
CONFIG += ordered warn_on qt debug_and_release

SUBDIRS += \
    unit/MessageListProxyModelTest \
    unit/MessageGroupTest \
    integration/MessageSystemIT
