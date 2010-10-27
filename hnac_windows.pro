CONFIG += qt warn_on

TEMPLATE = subdirs

SUBDIRS += hnacservice

win32 {
   SUBDIRS += HnacConfig
   RC_FILE = hnac.rc
}
