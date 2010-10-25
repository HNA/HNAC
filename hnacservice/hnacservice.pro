QT -= gui
QT += network

CONFIG   += qt warn_on

TARGET = hnac
TEMPLATE = app

MOC_DIR = .moc
OBJECTS_DIR = .obj
DESTDIR = ../bin

HEADERS += \
    hnaclient.h \
    service.h

SOURCES += \
    main.cpp \
    hnaclient.cpp \
    service.cpp

RESOURCES += res.qrc

include(../service/qtservice.pri)

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
