subdetection
============

Subtitle detection library based on Tesseract-OCR and Qt5.

Version used:
 - tesseract-ocr v3.02.02
 - Qt 5.1

You can check out my tutorials if you have no clue how to compile Tesseract on Windows:

http://broija.blogspot.fr/2014/10/build-tesseract-ocr-30202-with-qt-51.html

http://broija.blogspot.fr/2014/10/compile-tesseract-ocr-30202-with-mingw.html

Description
-----------

This library contains several classes, structures and functions to help detect subtitles in images.

All the parameters are tuned in the *Parameters* structure. You can also use a *ParameterManager* to save or load these parameters in a file
or use the signal/slot mechanism.

The *Detector* class is the one to use for text detection.

Parameters
----------

 - *hsvMin* and *hsvMax* are set to define the color range in which the subtitles are printed.
 - *zone* defines the zone were the subtitles will appear.
 - *charMaxSize* helps the detector guess what could be a character and what is too big to be one.
 - *thresh* is used in the contour detection process.
 - *xTolerance* is the maximum spacing between two characters along X axis.
 - *yTolerance* is the maximum spacing between characters on a line along Y axis.
 - *matchRatio* represents the similariy of the "cleaned" text zones between consecutively loaded images. The closer from 0, the more similar.

Tests
-----

Simply uncomment *#CONFIG += SUB_APP* in **subdetection.pro** file and build the project. It will generate an executable.

You can pass it two arguments:
 - the image file you want to extract the subtitles from.
 - the configuration file (optional).