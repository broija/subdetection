#-------------------------------------------------
#
# Project created by QtCreator 2014-10-21T00:04:06
#
#-------------------------------------------------

include($${PWD}/../common.pri)
include($${SUBDETECTION_ROOT_DIR}tagblob.pri)
include($${SUBDETECTION_ROOT_DIR}dependencies.pri)

VERSION = 4.0.0

MAJOR_VERSION = $$split(VERSION, .)
MAJOR_VERSION = $$member(MAJOR_VERSION, 0)

TARGET = subdetection
TEMPLATE = lib
CONFIG += dll

LIB_MAJOR_VERSION = "SUBDETECTION_MAJOR_VERSION = $${MAJOR_VERSION}"

#Writing major version for other projects dependencies
write_file(version.pri,LIB_MAJOR_VERSION)

DEFINES += SUBDETECTION_LIBRARY
DEFINES += "DEBUG_LEVEL=0x1"

build_pass:CONFIG(debug, debug|release) {
    unix: TARGET = $$join(TARGET,,,_debug)
    else: TARGET = $$join(TARGET,,,d)
}

INCLUDEPATH += .
DEPENDPATH = $$INCLUDEPATH

SOURCES += blob.cpp \
    contourmanager.cpp \
    conversion.cpp \
    detector.cpp \
    drawnblob.cpp \
    hash.cpp \
    hsv.cpp \
    hsvblob.cpp \
    hsvlist.cpp \
    opticalcharrecognizer.cpp \
    parametermanager.cpp \
    statistical_tools.cpp \
    subdetection_init.cpp

HEADERS += blob.h \
    contourmanager.h \
    conversion.h \
    deepdebug.h \
    detector.h \
    drawnblob.h \
    hash.h \
    hsv.h \
    hsvtypes.h \
    hsvblob.h \
    hsvlist.h \
    opticalcharrecognizer.h \
    parametermanager.h \
    parameters.h \
    rgbtable.h \
    statistical_tools.h \
    subdetection_global.h \
    subdetection_init.h \
    types.h
