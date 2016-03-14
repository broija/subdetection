#-------------------------------------------------
#
# Project created by QtCreator 2016-02-14T13:09:43
#
#-------------------------------------------------

include($${PWD}/../../common.pri)
include($${SUBDETECTION_ROOT_DIR}dependencies.pri)
include($${SUBDETECTION_ROOT_DIR}lib/lib.pri)

QT += testlib

TARGET = subdetection_unit_tests
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += .
INCLUDEPATH += $${SUBDETECTION_ROOT_DIR}/lib
INCLUDEPATH += $$OPENCV_INC_DIR
DEPENDPATH = $$INCLUDEPATH

HEADERS += tst_subdetection.h
SOURCES += tst_subdetection.cpp
