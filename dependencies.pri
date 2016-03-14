#broija 2016-03-13
#subdetection dependency vars

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
    LIBS += $$IMAGE_PROCESSING_DIR/tuto/tesseract3.dll

    LIBS += $$OPENCV_LIB_DIR/libopencv_core249.dll
    LIBS += $$OPENCV_LIB_DIR/libopencv_imgproc249.dll
    LIBS += $$OPENCV_LIB_DIR/libopencv_highgui249.dll
}

INCLUDEPATH += $$OPENCV_INC_DIR
INCLUDEPATH += $$TESSERACT_INC_DIR
INCLUDEPATH += $$TESSERACT_INC_DIR/ccmain
INCLUDEPATH += $$TESSERACT_INC_DIR/ccstruct
INCLUDEPATH += $$TESSERACT_INC_DIR/ccutil
