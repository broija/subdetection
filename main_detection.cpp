/*!
    Copyright 2014 Broija

    This file is part of subdetection library.

    subdetection is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    subdetection is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with subdetection library.  If not, see <http://www.gnu.org/licenses/>.
*/

/*!
Simple test file for subdetection library.
*/

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <QMap>
#include <QSharedPointer>
#include <QStringList>
#include <QFileInfo>

#include "hsv.h"
#include "subdetection_init.h"
#include "opticalcharrecognizer.h"
#include "parameters.h"
#include "detector.h"
#include "parametermanager.h"
#include "statistical_tools.h"
#include "blob.h"

#include "deepdebug.h"

namespace
{
const int KEY_NONE = -1;
const int KEY_ESCAPE = 27;
const int KEY_D = 100;
const int KEY_H = 104;
const int KEY_L = 108;
const int KEY_R = 114;
const int KEY_S = 115;

const char * DEFAULT_TESSERACT_PARENT_PATH = ".";
const char * DEFAULT_LANGUAGE = "eng";

const char * DEFAULT_CONFIG_FILE_PATH = "./subdetection_default.ini";
}//namespace

struct CallbackHelper
{
    CallbackHelper(){}
    CallbackHelper(const QString & _tessdataParentPath, const QString & _lang):detector(_tessdataParentPath,_lang){}

    cv::Mat mat;
    SubDetection::Detector detector;

//    SubDetection::Hsv selectedHsv;
};//CallbackHelper

//---------------------------------------------------------------------------

//void cannyCallback(int, void * _pHelper);
void mouseCallback(int _event, int _x, int _y, int, void * _pHelper);

void displayMessage(const QString & _message, bool _endl = true);
void displayHelp();

bool loadSettings(SubDetection::ParameterManager & _paramManager, QString & _config_filename);
void setDefaultSettings(QSharedPointer<SubDetection::Parameters> & _pParams, const cv::Mat & _mat);

bool fileExists(QString & _filename);

//---------------------------------------------------------------------------

int main(int argc, char** argv)
{
    SubDetection::init();//Init first!

    SubDetection::ParameterManager paramManager;
//    CallbackHelper callbackHelper;
    CallbackHelper callbackHelper(DEFAULT_TESSERACT_PARENT_PATH,DEFAULT_LANGUAGE);

    QString image_filename;
    QString config_filename;

    bool paramsLoaded = false;

    QFileInfo defaultCfgFileInfo(DEFAULT_CONFIG_FILE_PATH);
    QString defaultCfgPath = defaultCfgFileInfo.absoluteFilePath();

    switch (argc)
    {
    default:
    case 3:
        config_filename = argv[2];

        paramsLoaded = loadSettings(paramManager,config_filename);

        image_filename = argv[1];

        callbackHelper.mat = cv::imread(qPrintable(image_filename),1);

        if (callbackHelper.mat.data == NULL)
        {
            std::cerr << "Image could not be read." << std::endl;
            return 2;
        }//if (callbackHelper.mat.data == NULL)

        break;
    case 2:
    case 1:
        std::cerr << argv[0] << " path/to/image/file path/to/config/file" << std::endl;
        return 1;
    }//switch (argc)

//    cv::namedWindow("Control", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Original",cv::WINDOW_NORMAL);

    cv::setMouseCallback("Original",mouseCallback,&callbackHelper);

    QSharedPointer<SubDetection::Parameters> pParams;

    if (paramsLoaded)
    {
        pParams = paramManager.parameters();
    }//if (paramsLoaded)
    else
    {
         setDefaultSettings(pParams,callbackHelper.mat);
    }//if (paramsLoaded)...else

    displayMessage("Use Canny trackbar to detect text.");
    displayMessage("Be sure that all detection parameters fit the type of subtitle in your image.");
    displayMessage("A window should pop with a blue rectangle, representing the detection zone.");
    displayMessage("If text has been detected, you should see red rectangles in this blue rectangle.");

    displayMessage(QString("Text detection zone: x: %1, y: %2, w: %3, h: %4")
                   .arg(pParams->zone.x)
                   .arg(pParams->zone.y)
                   .arg(pParams->zone.width)
                   .arg(pParams->zone.height));

    displayHelp();

    callbackHelper.detector.setParameters(pParams);

    cv::imshow("Original", callbackHelper.mat); //show the original image

    QStringList textLines;
    int lineIndex;

    bool end = false;

    while (!end)
    {
        int key = cv::waitKey(500);

        switch (key)
        {
        case KEY_ESCAPE://escape key
            {
                end = true;
                break;
            }//KEY_ESCAPE
        case KEY_D://'D' key
            {
                textLines.clear();

                SubDetection::Detector::ReturnCode result = callbackHelper.detector.detect(callbackHelper.mat,textLines);

                switch (result)
                {
                case SubDetection::Detector::RC_NO_CHANGE:
                    displayMessage("Change parameters and reload them before launching a detection.");
                    break;
                default:
                    break;
                case SubDetection::Detector::RC_OK:
                    displayMessage(" +++ Text detected +++");

                    lineIndex = 0;
                    foreach (QString textLine, textLines)
                    {
                        displayMessage(QString("Line %1: %2")
                                       .arg(lineIndex++)
                                       .arg(textLine));
                    }//foreach (QString textLine, textLines)
                    break;
                }//switch (result)

                break;
            }//KEY_D
        case KEY_H://'H' key
            {
                displayHelp();
                break;
            }//KEY_H
        case KEY_R:
            {
                loadSettings(paramManager,config_filename);
                break;
            }//KEY_R
        case KEY_S:
            {
                displayMessage(QString("Saving default config to %1").arg(defaultCfgPath));

                setDefaultSettings(pParams,callbackHelper.mat);
                callbackHelper.detector.setParameters(pParams);

                paramManager.saveSettings(defaultCfgPath);

                break;
            }//KEY_R
        case KEY_NONE://no key pressed
            {
                break;
            }//KEY_NONE
        default:
            {
                deepDebug("KEY PRESSED %d",key);
                break;
            }//default
        }//switch (key)
    }//while (!end)

    return 0;
}//main

//---------------------------------------------------------------------------

void mouseCallback(int _event, int _x, int _y, int , void * _pHelper)
{
    CallbackHelper * pHelper = (CallbackHelper *)_pHelper;

    switch (_event)
    {
    case cv::EVENT_LBUTTONDOWN:
        {
            qDebug("MLB X %d Y %d",_x,_y);
            cv::Point pixel;
            pixel.x = _x;
            pixel.y = _y;
            SubDetection::Blob blob;

            SubDetection::Detector::ReturnCode result = pHelper->detector.getPointedBlob(pHelper->mat,pixel,blob);

            switch (result)
            {
            case SubDetection::Detector::RC_OK:
                {
                    qDebug("Median HSV %s",qPrintable(blob.hsvMedian.toString()));
                    break;
                }//RC_OK
            case SubDetection::Detector::RC_AMBIGUOUS:
                {
                    qDebug("Ambiguous result.");
                    break;
                }//RC_AMBIGUOUS
            case SubDetection::Detector::RC_INCONSISTENT:
                {
                    qDebug("Inconsistent result.");
                    break;
                }//RC_INCONSISTENT
            default:
                break;
            }//switch (result)

            break;//EVENT_LBUTTONDOWN
        }////EVENT_LBUTTONDOWN
    case cv::EVENT_MOUSEMOVE:
        {
//            qDebug("X %d Y %d",_x,_y);
            break;
        }//EVENT_MOUSEMOVE
    }//switch (_event)
}//mouseCallback

//---------------------------------------------------------------------------

void displayMessage(const QString & _message, bool _endl)
{
    std::cout << qPrintable(_message);

    if (_endl) std::cout << std::endl;
}//displayMessage

//---------------------------------------------------------------------------

void displayHelp()
{
    displayMessage("");
    displayMessage("******* HELP *******");
    displayMessage("When focus is on image window, following commands are available by pressing associated key:");
    displayMessage(" - text detection : 'D'.");
    displayMessage(" - reload settings : 'R'.");
    displayMessage(" - save default settings file : 'S'.");
    displayMessage(" - quit : 'Esc'.");
    displayMessage(" - display this help message : 'H'.");
    displayMessage("******* !HELP *******");
}//displayHelp

//---------------------------------------------------------------------------

bool loadSettings(SubDetection::ParameterManager & _paramManager, QString & _config_filename)
{
    displayMessage("*** *** ***");
    displayMessage(QString("Loading config file: %1").arg(_config_filename));
    displayMessage("*** *** ***");

    bool result = _paramManager.loadSettings(_config_filename);

    if (!result)
    {
        qWarning("Error while loading config file.");
    }//if (!result)

    return result;
}//loadSettings

//---------------------------------------------------------------------------

void setDefaultSettings(QSharedPointer<SubDetection::Parameters> & _pParams, const cv::Mat & _mat)
{
    _pParams.reset(new SubDetection::Parameters);

    _pParams->hsvMin.setHue(20);
    _pParams->hsvMax.setHue(32);

    _pParams->hsvMin.setSaturation(155);
    _pParams->hsvMax.setSaturation(255);

    _pParams->hsvMin.setValue(160);
    _pParams->hsvMax.setValue(255);

    _pParams->charMaxSize = cv::Size(20,20);
    _pParams->yTolerance = _pParams->charMaxSize.height;
    _pParams->xTolerance = 25;

    int x, y, width, height;

    x = static_cast<int>(_mat.cols * 0.10);
    y = static_cast<int>(_mat.rows * 0.85);
    width = static_cast<int>(_mat.cols * 0.8);
    height = static_cast<int>(_mat.rows * 0.15);

    _pParams->zone = cv::Rect(x,y,width,height);
    _pParams->thresh = 65;
}//setDefaultSettings

//---------------------------------------------------------------------------

bool fileExists(QString & _filename)
{
    QFileInfo fileInfo(_filename);

    bool result = fileInfo.exists();

    if (!result)
    {
        displayMessage(QString("File %1 doesn't exist.").arg(_filename));
    }//if (!result)

    return result;
}//fileExists

//---------------------------------------------------------------------------

