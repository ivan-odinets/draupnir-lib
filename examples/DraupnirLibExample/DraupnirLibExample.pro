QT += core gui widgets

QMAKE_CXXFLAGS += -std=c++20
CONFIG      += c++20

QMAKE_LFLAGS += -fuse-ld=lld

DEFINES += DRAUPNIR_SETTINGS_USE_QSETTINGS

DEFINES += DRAUPNIR_RELAXED_MENU_TEMPLATE_HANDLERS

DEFINES += DRAUPNIR_LIB_ENABLE_EASTER_EGG

include(../../modules/DraupnirApplicationTemplate.pri)


SOURCES += \
    main.cpp \
    ui/ButtonsWidget.cpp

HEADERS += \
    core/ExampleHelpSource.h \
    traits/DraupnirTemplatesDefinitions.h \
    traits/menu_entries/MySettingsMenuEntries.h \
    traits/messages/MyMessageTrait.h \
    traits/settings/MySettingTraits.h \
    traits/widgets/ButtonsWidgetTrait.h \
    ui/ButtonsWidget.h
