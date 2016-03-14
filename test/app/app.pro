#-------------------------------------------------
#
# Project created by QtCreator 2014-10-21T00:04:06
#
#-------------------------------------------------

include($${PWD}/../../common.pri)
include($${SUBDETECTION_ROOT_DIR}tagblob.pri)
include($${SUBDETECTION_ROOT_DIR}dependencies.pri)
include($${SUBDETECTION_ROOT_DIR}lib/lib.pri)

TEMPLATE = app

CONFIG += console

INCLUDEPATH += .
INCLUDEPATH += $${SUBDETECTION_ROOT_DIR}/lib
DEPENDPATH = $$INCLUDEPATH

TARGET = subdetection_test_app

SOURCES += main_detection.cpp
