INCLUDEPATH += $$PWD/include/appSettings

!defined(APP_SETTINGS_PRI_INCLUDED) {
    DEFINES += APP_SETTINGS_PRI_INCLUDED

    QT          += core
    CONFIG      += c++20

    HEADERS += \
        $$PWD/include/appSettings/AppSettings.h

    SOURCES += \
        $$PWD/src/appSettings/AppSettings.cpp

}
