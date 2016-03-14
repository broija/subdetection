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
#include <QHash>
#include <opencv2/core/core.hpp>

#include "hash.h"

namespace cv
{

uint qHash(const Point & _point)
{
    // :: to help compiler understand that he must seek Qt qHash function elsewhere
    return ::qHash(_point.x + _point.y);
}//qHash

}//cv
