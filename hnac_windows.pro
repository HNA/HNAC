CONFIG += warn_on \
	qt

TEMPLATE = subdirs

SUBDIRS += hnacservice

#!unix {
    SUBDIRS += HnacConfig
#}

win32 {
   RC_FILE = hnac.rc
}
