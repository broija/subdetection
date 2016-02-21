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

const char * DEFAULT_TESSERACT_PARENT_PATH = ".";
const char * DEFAULT_LANGUAGE = "eng";
}//namespace

struct CallbackHelper
{
    CallbackHelper(){}
    CallbackHelper(const QString & _tessdataParentPath, const QString & _lang):detector(_tessdataParentPath,_lang){}

    cv::Mat mat;
    SubDetection::Detector detector;

//    SubDetection::Hsv selectedHsv;
};//CallbackHelper

//void cannyCallback(int, void * _pHelper);
void mouseCallback(int _event, int _x, int _y, int, void * _pHelper);

void displayHelp();

int main(int argc, char** argv)
{
    SubDetection::init();//Init first!

    SubDetection::ParameterManager paramManager;
//    CallbackHelper callbackHelper;
    CallbackHelper callbackHelper(DEFAULT_TESSERACT_PARENT_PATH,DEFAULT_LANGUAGE);

    const char * image_filename;
    const char * config_filename;

    bool paramsLoaded = false;

    switch (argc)
    {
    default:
    case 3:
        config_filename = argv[2];
        paramsLoaded = paramManager.loadSettings(config_filename);

        if (!paramsLoaded)
        {
            std::cout << "Error while loading config file." << std::endl;
        }//if (!paramsLoaded)

        image_filename = argv[1];

        callbackHelper.mat = cv::imread(image_filename,1);

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
        pParams.reset(new SubDetection::Parameters);

        pParams->hsvMin.setHue(20);
        pParams->hsvMax.setHue(32);

        pParams->hsvMin.setSaturation(155);
        pParams->hsvMax.setSaturation(255);

        pParams->hsvMin.setValue(160);
        pParams->hsvMax.setValue(255);

        pParams->charMaxSize = cv::Size(20,20);
        pParams->yTolerance = pParams->charMaxSize.height;
        pParams->xTolerance = 25;

        int x, y, width, height;

        x = static_cast<int>(callbackHelper.mat.cols * 0.10);
        y = static_cast<int>(callbackHelper.mat.rows * 0.85);
        width = static_cast<int>(callbackHelper.mat.cols * 0.8);
        height = static_cast<int>(callbackHelper.mat.rows * 0.15);

        pParams->zone = cv::Rect(x,y,width,height);
        pParams->thresh = 65;
    }//if (paramsLoaded)...else

    std::cout << "Use Canny trackbar to detect text." << std::endl
              << "Be sure that all detection parameters fit the type of subtitle in your image." << std::endl
              << "A window should pop with a blue rectangle, representing the detection zone." << std::endl
              << "If text has been detected, you should see red rectangles in this blue rectangle." << std::endl;

    std::cout << "Text detection zone: "
              << "x:" << pParams->zone.x << ", "
              << "y:" << pParams->zone.y << ", "
              << "w:" << pParams->zone.width << ", "
              << "h:" << pParams->zone.height << std::endl << std::endl;

    displayHelp();

    callbackHelper.detector.setParameters(pParams);

    cv::imshow("Original", callbackHelper.mat); //show the original image

    QStringList textLines;

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
                callbackHelper.detector.detect(callbackHelper.mat,textLines);

                int lineIndex = 0;

                foreach (QString textLine, textLines)
                {
                    qDebug("Line %d: %s",lineIndex,qPrintable(textLine));
                    ++lineIndex;
                }//foreach (QString textLine, textLines)
                break;
            }//KEY_D
        case KEY_H://'H' key
            {
                displayHelp();
                break;
            }//KEY_H
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

    paramManager.saveSettings(config_filename);

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

void displayHelp()
{
    std::cout << "When focus is on image window:" << std::endl;
    std::cout << " - press 'D' to detect text." << std::endl;
    std::cout << " - press 'Esc' to quit." << std::endl;
    std::cout << " - press 'H' to display this help message." << std::endl;
}//displayHelp

//---------------------------------------------------------------------------
