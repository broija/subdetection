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

using namespace cv;
using namespace std;

void canny_callback(int, void * _pDetector);

Mat imgOriginal;
Mat imgThresholded;
Mat imgGray;
Mat imgMasked;
int max_thresh = 255;
int ratio = 4;
int max_ratio = 5;
int kernel_size = 3;

int main(int argc, char** argv)
{
//    Logger logger(true,false);
    SubDetection::init();//Init first!
    if (argc != 2)
    {
        std::cerr << "Image file name must be passed as first and only argument." << std::endl;
        return 1;
    }//if (argc != 2)

    const char * filename = argv[1];

    namedWindow("Control", CV_WINDOW_AUTOSIZE);
    namedWindow("Original",WINDOW_NORMAL);
//    namedWindow("Threshold",WINDOW_NORMAL);
//    namedWindow("Edges",WINDOW_NORMAL);
//    namedWindow("Masked",WINDOW_NORMAL);

    SubDetection::Parameters params;

    params.hsvMin.setHue(20);
    params.hsvMax.setHue(32);

    params.hsvMin.setSaturation(155);
    params.hsvMax.setSaturation(255);

    params.hsvMin.setValue(160);
    params.hsvMax.setValue(255);

    params.charMaxSize = Size(20,20);
    params.yTolerance = params.charMaxSize.height;
    params.xTolerance = 25;

    imgOriginal = imread(filename,1);

    if (imgOriginal.data == NULL)
    {
        std::cerr << "Image could not be read." << std::endl;
        return 2;
    }//if (imgOriginal.data == NULL)

    std::cout << "Use Canny trackbar to detect text." << std::endl
              << "Be sure that all detection parameters fit the type of subtitle in your image." << std::endl
              << "A window should pop with a blue rectangle, representing the detection zone." << std::endl
              << "If text has been detected, you should see red rectangles in this blue rectangle." << std::endl;

    int x = static_cast<int>(imgOriginal.cols * 0.10),
        y = static_cast<int>(imgOriginal.rows * 0.85),
        width = static_cast<int>(imgOriginal.cols * 0.8),
        height = static_cast<int>(imgOriginal.rows * 0.15);

    std::cout << "x:" << x << ", "
              << "y:" << y << ", "
              << "w:" << width << ", "
              << "h:" << height << std::endl;

    params.zone = Rect(x,y,width,height);
    params.thresh = 65;

    SubDetection::Detector detector(params);

    createTrackbar("Canny:", "Control", &(params.thresh), max_thresh, canny_callback, &detector );
    imshow("Original", imgOriginal); //show the original image

    bool end = false;

    while (!end)
    {
        int key = waitKey(500);

        switch (key)
        {
        case 27:
            end = true;
            break;
        //Put your own key behaviour here...
        }//switch (key)
    }//while (!end)

    SubDetection::ParameterManager paramManager(params);
    paramManager.saveSettings("params.ini");

    return 0;
}//main

//---------------------------------------------------------------------------

void canny_callback(int, void * _pDetector)
{
    SubDetection::Detector * pDetector = (SubDetection::Detector *)_pDetector;

    QStringList textLines;

    pDetector->detect(imgOriginal,textLines);

    int lineIndex = 0;

    foreach (QString textLine, textLines)
    {
        qDebug("Line %d: %s",lineIndex,qPrintable(textLine));
        ++lineIndex;
    }//foreach (QString textLine, textLines)
}//canny_callback



