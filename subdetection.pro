#-------------------------------------------------
#
# Project created by QtCreator 2014-10-21T00:04:06
#
#-------------------------------------------------

VERSION = 4.0.0

#No C++ 11
#QMAKE_CXXFLAGS += -std=c++0x

QT += gui

#CONFIG += release
CONFIG += debug

DEFINES += "DEBUG_LEVEL=0x1"

#Uncomment this to build an executable and test the library
#CONFIG += TEST_PARAM_DETECTION
CONFIG += TEST_DETECTION

TEST_PARAM_DETECTION | TEST_DETECTION {
    DEFINES += SUBDETECTION_APP
    TEMPLATE = app
} else {
    TARGET = subdetection
    TEMPLATE = lib

    DEFINES += SUBDETECTION_LIBRARY
}

build_pass:CONFIG(debug, debug|release) {
    unix: TARGET = $$join(TARGET,,,_debug)
    else: TARGET = $$join(TARGET,,,d)
}

DEFINES += _tagBLOB_DEFINED

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

TEST_PARAM_DETECTION {
    TARGET = test_subdetection_param
    #to be defined
    SOURCES += main_param_detection.cpp
}

TEST_DETECTION {
    TARGET = test_subdetection
    SOURCES += main_detection.cpp
}

SOURCES += blob.cpp \
    contourmanager.cpp \
    conversion.cpp \
    detector.cpp \
    drawnblob.cpp \
    hash.cpp \
    hsv.cpp \
    hsvblob.cpp \
    opticalcharrecognizer.cpp \
    parametermanager.cpp \
    statistical_tools.cpp \
    subdetection_init.cpp \
    hsvlist.cpp

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
