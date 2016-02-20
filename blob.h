/*!
    Copyright 2015 Broija

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

#ifndef SUBDETECTION_BLOB_H
#define SUBDETECTION_BLOB_H

#include "hsv.h"
#include "types.h"

namespace SubDetection
{

struct SUBDETECTIONSHARED_EXPORT Blob
{
    Hsv hsvMin;
    Hsv hsvMax;
    Hsv hsvMedian;

    Rect bounding;

    Point massCenter;
/*
    ContourVector contours;
    Hierarchy hierarchy;
*/

    Mat mat;

    Blob & operator =(const Blob & _other);
};//Blob

}//namespace Blob

#endif // SUBDETECTION_BLOB_H
