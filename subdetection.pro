# broija 2016-03-13
# Project file for subdetection
# Regrouping:
#  - subdetection lib
#  - subdetection test app
#  - subdetection unit tests

TEMPLATE = subdirs

CONFIG += ordered
CONFIG += functions

CONFIG += TEST_APP
CONFIG += UNIT_TESTS

SUBDIRS = Library

Library.subdir = lib

TEST_APP {
  SUBDIRS += Test_app

  Test_app.subdir = test/app
  Test_app.depends = library
}

UNIT_TESTS {
  SUBDIRS += Unit_tests

  Unit_tests.subdir = test/unit_tests
  Unit_tests.depends = library
}
