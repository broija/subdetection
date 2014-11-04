#-------------------------------------------------
#
# Project created by QtCreator 2014-10-21T00:04:06
#
#-------------------------------------------------

VERSION = 1.0.0

QT += gui

TARGET = subdetection

CONFIG += release
#CONFIG += debug

#Uncomment this to build an executable and test the library
#CONFIG += SUB_APP

SUB_APP {
DEFINES += SUBDETECTION_APP
TEMPLATE = app
} else {
TEMPLATE = lib
}

DEFINES += SUBDETECTION_LIBRARY
DEFINES += _tagBLOB_DEFINED

#DEFINES += DEEP_DEBUG

PROJECT_DIR = D:/prog/c++/projets
IMAGE_PROCESSING_DIR = D:/prog/traitement_image

OPENCV_DIR = $$IMAGE_PROCESSING_DIR/build/opencv
OPENCV_INC_DIR = $$OPENCV_DIR/build/include
OPENCV_BIN_DIR = $$OPENCV_DIR/bin
OPENCV_LIB_DIR = $$OPENCV_BIN_DIR/bin

OCR_DIR = $$IMAGE_PROCESSING_DIR/ocr

TESSERACT_DIR = $$OCR_DIR/tesseract-ocr
TESSERACT_INC_DIR = $$TESSERACT_DIR
TESSERACT_LIB_DIR = $$OCR_DIR/tesseract_output/bin

win32 {
CONFIG += console

LIBS += $$IMAGE_PROCESSING_DIR/tuto/tesseract3.dll

LIBS += $$OPENCV_LIB_DIR/libopencv_core249.dll
LIBS += $$OPENCV_LIB_DIR/libopencv_imgproc249.dll
LIBS += $$OPENCV_LIB_DIR/libopencv_highgui249.dll
}

INCLUDEPATH += .
INCLUDEPATH += $$OPENCV_INC_DIR
INCLUDEPATH += $$TESSERACT_INC_DIR
INCLUDEPATH += $$TESSERACT_INC_DIR/ccmain
INCLUDEPATH += $$TESSERACT_INC_DIR/ccstruct
INCLUDEPATH += $$TESSERACT_INC_DIR/ccutil
DEPENDPATH = $$INCLUDEPATH

SUB_APP {
SOURCES += main.cpp
}
SOURCES += conversion.cpp
SOURCES += detector.cpp
SOURCES += hsv.cpp
SOURCES += opticalcharrecognizer.cpp
SOURCES += parametermanager.cpp
SOURCES += subdetection_init.cpp

HEADERS += conversion.h
HEADERS += deepdebug.h
HEADERS += detector.h
HEADERS += hsv.h
HEADERS += opticalcharrecognizer.h
HEADERS += parametermanager.h
HEADERS += parameters.h
HEADERS += subdetection_global.h
HEADERS += subdetection_init.h
HEADERS += types.h
