#broija 2016-03-14

include($$PWD/../common.pri)
include($${SUBDETECTION_ROOT_DIR}lib/version.pri)

LIB_PATH = ''
LIB_NAME = ''
LIB_SUFFIX = ''
LIB_SUBDIR = ''
LIB_EXT = ''
SD_VERSION_KEYWORD = '%SD_VERSION%'

win32:CONFIG(debug, debug|release) {
  LIB_SUFFIX = 'd'
  LIB_SUBDIR = 'debug'
} else {
  LIB_SUBDIR = 'release'
}

LIB_DIR = \"$${SUBDETECTION_ROOT_DIR}lib/$${LIB_SUBDIR}/\"

win32 {
  LIB_EXT = '.dll'
  LIB_NAME = subdetection$${LIB_SUFFIX}$${SD_VERSION_KEYWORD}
}

LIB_NAME ~= s/$${SD_VERSION_KEYWORD}/$${SUBDETECTION_MAJOR_VERSION}

LIB_PATH = $${LIB_DIR}$${LIB_NAME}$${LIB_EXT}

win32:LIBS += $$LIB_PATH
