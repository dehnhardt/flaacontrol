#-------------------------------------------------
#
# Project created by QtCreator 2018-01-21T21:16:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = flaacontrol
TEMPLATE = app

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/main.cpp \
    src/tracks/tracks.cpp \
    src/handler/FLCRepositoryModuleHandler.cpp \
    src/handler/FLCPingHandler.cpp \
    src/handler/FLCModuleInstancesHandler.cpp \
    src/model/FLCRepositoryModule.cpp \
    src/model/FLCModuleInstancesModel.cpp \
    src/model/FLCRepositoryModuleModel.cpp \
    src/model/FLCModuleInstance.cpp \
    src/Flaacontrol.cpp \
    src/settings/SessionSettings.cpp \
    src/settings/SettingsModel.cpp \
    src/structure/FLCModuleWidget.cpp \
    src/structure/FlowControl.cpp \
    src/MainWindow.cpp

HEADERS += \
    src/tracks/tracks.h \
    src/handler/FLCRepositoryModuleHandler.h \
    src/handler/FLCPingHandler.h \
    src/handler/FLCModuleInstancesHandler.h \
    src/model/FLCRepositoryModule.h \
    src/model/FLCRepositoryModuleModel.h \
    src/model/FLCModuleInstancesModel.h \
    src/model/FLCModuleInstance.h \
    src/Flaacontrol.h \
    src/settings/SessionSettings.h \
    src/settings/XmlSettings.h \
    src/settings/SettingsModel.h \
    src/structure/FLCModuleWidget.h \
    src/structure/FlowControl.h \
    src/MainWindow.h

FORMS += \
    src/mainwindow.ui \
    src/structure/flowcontrol.ui \
    src/settings/SessionSettings.ui

RESOURCES += \
    icons.qrc

DISTFILES += \
        .astylerc \
    resources/ethernetjack.xpm \
    resources/midijack_svg.xpm \
    resources/readfromdevice.xpm \
    resources/restore.xpm \
    resources/savetodevice.xpm \
    resources/usbhostjack.xpm \
    resources/usbjack.xpm \
    resources/audiocombojack.xpm \
    src/flaaoscsdk/README.md \
    src/flaaoscsdk/LICENSE \
    src/flaaoscsdk/README.md \
    src/flaaoscsdk/LICENSE \
    src/flaaoscsdk/README.md \

include( src/flaaoscsdk/flaaoscsdk.pri )

INCLUDEPATH += $$PWD/../flaarlib/src

