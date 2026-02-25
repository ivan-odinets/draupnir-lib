!contains(DEFINES, DRAUPNIR_APPLICATION_TEMPLATE_ENABLED) {
    DEFINES += DRAUPNIR_APPLICATION_TEMPLATE_ENABLED

    !contains(CONFIG, c++20) { CONFIG += c++20 }
    !contains(QT, gui) { QT += gui }
    !contains(QT, widgets) { QT += widgets }

    include(MessageSystem.pri)

    INCLUDEPATH += $$PWD/../include/application_template

    HEADERS += \
        $$PWD/../include/application_template/draupnir/ApplicationTemplate.h \
        $$PWD/../include/application_template/draupnir/application_template/utils/MenuBarHandlersHelper.h \
        $$PWD/../include/application_template/draupnir/application_template/utils/UiMainEntryHelper.h
}




