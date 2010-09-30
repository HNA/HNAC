QT += network
TARGET = hnac
TEMPLATE = app
SOURCES += main.cpp \
    login.cpp \
    hnaclient.cpp \
    logout.cpp \
    mainwindow.cpp
HEADERS += login.h \
    hnaclient.h \
    logout.h \
    mainwindow.h

RESOURCES += res.qrc

TRANSLATIONS = hnac_ru.ts

MOC_DIR = .moc
OBJECTS_DIR = .obj
DESTDIR = bin

win32 {
   RC_FILE = hnac.rc
}
# install
#target.path = /usr/bin
#INSTALLS += target
