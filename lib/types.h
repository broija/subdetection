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

#include <opencv2/core/core.hpp>

namespace SubDetection
{

typedef cv::Rect Rect;
typedef std::vector<Rect> RectVector;

typedef cv::Point Point;
typedef std::vector<Point> PointVector;

typedef cv::Size Size;

typedef cv::Scalar Scalar;

typedef cv::Mat Mat;

typedef int Thresh;

typedef double MatchRatio;

typedef std::vector<Point> Contour;
typedef std::vector<Contour> ContourVector;

typedef Contour Curve;

typedef cv::Vec4i HierarchyItem;
typedef std::vector<HierarchyItem> Hierarchy;

typedef ContourVector::size_type Index;
typedef std::vector<Index> IndexVector;

}//namespace SubDetection

#endif // SUBDETECTION_TYPES_H
