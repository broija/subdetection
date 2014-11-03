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

#include "subdetection_init.h"
#include "opticalcharrecognizer.h"
#include "parameters.h"
#include "detector.h"
#include "parametermanager.h"

struct CallbackHelper
{
    cv::Mat mat;
    SubDetection::Detector detector;
};//CallbackHelper

void canny_callback(int, void * _pHelper);

int main(int argc, char** argv)
{
    SubDetection::init();//Init first!

    SubDetection::ParameterManager paramManager;
    CallbackHelper callbackHelper;

    const char * image_filename;
    const char * config_filename = "params.ini";

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

        //no break
    case 2:
        image_filename = argv[1];

        callbackHelper.mat = cv::imread(image_filename,1);

        if (callbackHelper.mat.data == NULL)
        {
            std::cerr << "Image could not be read." << std::endl;
            return 2;
        }//if (callbackHelper.mat.data == NULL)

        break;
    case 1:
        std::cerr << "Image file name must be passed as first and mandatory argument. Config file is an optional, second argument." << std::endl;
        return 1;
    }//switch (argc)

    cv::namedWindow("Control", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Original",cv::WINDOW_NORMAL);

    SubDetection::Parameters * pParams = 0;

    if (paramsLoaded)
    {
        pParams = &paramManager.parameters();
    }//if (paramsLoaded)
    else
    {
        pParams = new SubDetection::Parameters();

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
              << "h:" << pParams->zone.height << std::endl;

    callbackHelper.detector.setParameters(*pParams);

    cv::createTrackbar("Canny:", "Control", &(pParams->thresh), 255, canny_callback, &callbackHelper);
    cv::imshow("Original", callbackHelper.mat); //show the original image

    bool end = false;

    while (!end)
    {
        int key = cv::waitKey(500);

        switch (key)
        {
        case 27:
            end = true;
            break;
        //Put your own key behaviours here...
        }//switch (key)
    }//while (!end)

    paramManager.saveSettings(config_filename);

    return 0;
}//main

//---------------------------------------------------------------------------

void canny_callback(int, void * _pHelper)
{
    CallbackHelper * pHelper = (CallbackHelper *)_pHelper;

    QStringList textLines;

    pHelper->detector.detect(pHelper->mat,textLines);

    int lineIndex = 0;

    foreach (QString textLine, textLines)
    {
        qDebug("Line %d: %s",lineIndex,qPrintable(textLine));
        ++lineIndex;
    }//foreach (QString textLine, textLines)
}//canny_callback

//---------------------------------------------------------------------------



