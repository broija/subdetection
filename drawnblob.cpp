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

#include "drawnblob.h"

namespace SubDetection
{

DrawnBlob::DrawnBlob():
    m_bgColor(0,0,0)
{
}//DrawnBlob

//-------------------------

void DrawnBlob::setBackgroundColor(cv::Scalar & _color)
{
    m_bgColor = _color;
}//setBackgroundColor

//-------------------------

void DrawnBlob::initialize()
{
    m_drawnMat = croppedMat();
}//initialize

//-------------------------

/// Pixel is in a hole of DrawnBlob. Painting it with background color.
void DrawnBlob::inHole(const Point & _pixel)
{
    //Drawing pixel with background color
    cv::circle(m_drawnMat,toBlobOrigin(_pixel),0,m_bgColor);
}//inHole

//-------------------------

/// Pixel is outside DrawnBlob. Painting it with background color.
void DrawnBlob::outside(const Point & _pixel)
{
    //Drawing pixel with background color
    cv::circle(m_drawnMat,toBlobOrigin(_pixel),0,m_bgColor);
}//outside

//-------------------------

}//SubDetection
