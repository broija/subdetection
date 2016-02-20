#-------------------------------------------------
#
# Project created by QtCreator 2016-02-14T13:09:43
#
#-------------------------------------------------

QT += testlib

SUBDETECTIONPATH = d:/prog/c++/projets/subdetection/
SUBDETECTIONVERSION = 3

IMAGE_PROCESSING_DIR = D:/prog/traitement_image
OPENCV_DIR = $$IMAGE_PROCESSING_DIR/build/opencv
OPENCV_INC_DIR = $$OPENCV_DIR/build/include

TARGET = tst_subdetection
CONFIG   += console
CONFIG   -= app_bundle

CONFIG += release

TEMPLATE = app

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += .
INCLUDEPATH += $$SUBDETECTIONPATH
INCLUDEPATH += $$OPENCV_INC_DIR
DEPENDPATH = $$INCLUDEPATH

# Input
#Libs
build_pass:CONFIG(debug, debug|release) {
     win32:LIBS += $$SUBDETECTIONPATH/debug/subdetectiond3.dll
} else {
     win32:LIBS += $$SUBDETECTIONPATH/release/subdetection3.dll
}

HEADERS += tst_subdetection.h
SOURCES += tst_subdetection.cpp
