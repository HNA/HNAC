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

unix { 
    # VARIABLES
    isEmpty(PREFIX):PREFIX = /usr/local
    BINDIR = $$PREFIX/bin
    DATADIR = $$PREFIX/share
    DEFINES += DATADIR=\"$$DATADIR\" \
        PKGDATADIR=\"$$PKGDATADIR\"
    
    # MAKE INSTALL
    INSTALLS += target 
    target.path = $$BINDIR
}
