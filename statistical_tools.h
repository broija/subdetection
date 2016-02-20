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
#ifndef SUBDETECTION_STAT_TOOLS_H
#define SUBDETECTION_STAT_TOOLS_H

#include "subdetection_global.h"

#include "hsv.h"
#include "types.h"

namespace SubDetection
{

SUBDETECTIONSHARED_EXPORT Hsv average(const Hsv & _first, const Hsv & _second);
SUBDETECTIONSHARED_EXPORT Hsv average(const HsvList & _list);
SUBDETECTIONSHARED_EXPORT Hsv median(HsvList & _list);

}//namespace SubDetection


#endif // SUBDETECTION_STAT_TOOLS_H
