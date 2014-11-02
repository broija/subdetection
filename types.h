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

#ifndef SUBDETECTION_TYPES_H
#define SUBDETECTION_TYPES_H

#include <vector>
#include <QtGlobal>

#include <opencv2/core/core.hpp>

#include "hsv.h"

namespace SubDetection
{

typedef std::vector<cv::Point> Contour;
typedef std::vector<Contour> ContourVector;

typedef Contour ContourCurve;

typedef cv::Rect Rect;
typedef std::vector<Rect> RectVector;

typedef cv::Point Point;
typedef std::vector<Point> PointVector;

typedef cv::Point MassCenter;

typedef cv::Size Size;

typedef cv::Scalar Scalar;

typedef cv::Mat Mat;

typedef std::vector<cv::Vec4i> Hierarchy;

typedef int Thresh;

typedef double MatchRatio;

}//namespace SubDetection

#endif // SUBDETECTION_TYPES_H
