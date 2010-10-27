CONFIG += qt warn_on

TEMPLATE = subdirs

SUBDIRS += hnacservice
SUBDIRS += HnacConfig
win32 {

   RC_FILE = hnac.rc
}
