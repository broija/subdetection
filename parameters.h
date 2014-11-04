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

#ifndef SUBDETECTION_PARAMETERS_H
#define SUBDETECTION_PARAMETERS_H

#include "subdetection_global.h"

#include "hsv.h"
#include "types.h"

namespace SubDetection
{

/*!
 * \brief The Parameters struct. Used to store detection parameters.
 */
struct SUBDETECTIONSHARED_EXPORT Parameters
{
    Parameters():zone(0,0,1,1),
                 charMaxSize(5,5),
                 thresh(85),
                 xTolerance(1),
                 yTolerance(0),
                 matchRatio(0.05){}

    Hsv hsvMin;
    Hsv hsvMax;

    Rect zone;///< Subtitle zone.

    Size charMaxSize;

    Thresh thresh;///< Edge detection thresh

    Point::value_type xTolerance;///< Delta X max between mass centers on a single line.
    Point::value_type yTolerance;///< Delta Y max between mass centers on a single line.

    MatchRatio matchRatio;///< Ratio to consider that 2 text zones are similar. 0 < ratio < 1.
};//Parameters

}//namespace SubDetection

#endif // SUBDETECTION_PARAMETERS_H
