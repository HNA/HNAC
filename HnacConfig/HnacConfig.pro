QT       += core gui

TARGET = HnacConfig
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32 {
   RC_FILE = hnac.rc
}

#TRANSLATIONS = translation/HnacConfig_ru.ts

MOC_DIR = .moc
OBJECTS_DIR = .obj
DESTDIR = ../bin

include(../service/qtservice.pri)

RESOURCES += res.qrc
