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

#ifndef SUBDETECTION_HSV_H
#define SUBDETECTION_HSV_H

#include "subdetection_global.h"

#include "types.h"

namespace SubDetection
{

typedef cv::Scalar Scalar;

/*!
 * \brief The Hsv class. Defines a color in HSV format.
 */
class SUBDETECTIONSHARED_EXPORT Hsv
{
public:
    typedef int Type;

    Hsv();
    Hsv(Type _h, Type _s, Type _v);

    Type hue() const {return m_hue;}
    Type saturation() const {return m_sat;}
    Type value() const {return m_val;}

    void setHue(Type _h);
    void setSaturation(Type _s);
    void setValue(Type _v);

    Scalar toScalar() const;

protected:
    Type m_hue;
    Type m_sat;
    Type m_val;
};//Hsv

}//namespace SubDetection

#endif // SUBDETECTION_HSV_H
