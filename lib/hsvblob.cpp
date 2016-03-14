/*!
    Copyright 2016 Broija

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

#include <opencv2/imgproc/imgproc.hpp>

#include "statistical_tools.h"

#include "hsvblob.h"

namespace SubDetection
{

void HsvBlob::initialize()
{
    //Cropping
    Mat tmpMat = croppedMat();

    cv::cvtColor(tmpMat,m_hsvMat,cv::COLOR_BGR2HSV);//HSV conversion

    m_tmpMin.toMax();
    m_tmpMax.toMin();
}//initialize

//-------------------------

void HsvBlob::inside(const Point & _pixel)
{
    Hsv tmp;

    //retrieving HSV parameters in blob coordinate system.
    tmp.from(m_hsvMat.at<cv::Vec3b>(toBlobOrigin(_pixel)));

    m_tmpMin.saveMinima(tmp);
    m_tmpMax.saveMaxima(tmp);

    m_list.append(tmp);
}//inside

//-------------------------

void HsvBlob::finalize()
{
    m_min = m_tmpMin;
    m_max = m_tmpMax;

    m_median = median(m_list);
}//finalize

//-------------------------

}//SubDetection
