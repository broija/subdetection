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

#include <opencv2/core/core.hpp>

class QString;

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
    typedef Type Hue;
    typedef Type Saturation;
    typedef Type Value;

    Hsv();
    Hsv(Hue _h, Saturation _s, Value _v);

    Hue hue() const {return m_hue;}
    Saturation saturation() const {return m_sat;}
    Value value() const {return m_val;}

    void setHue(Hue _h);
    void setSaturation(Saturation _s);
    void setValue(Value _v);

    void saveMinima(const Hsv & _other);
    void saveMaxima(const Hsv & _other);

    void toMin();
    void toMax();

    void from(cv::Vec3b & _source);

    Scalar toScalar() const;
    QString toString() const;

    Hsv & operator =(const Hsv & _other);
    bool operator ==(const Hsv & _other) const;
    bool operator !=(const Hsv & _other) const;

protected:
    Hue m_hue;
    Saturation m_sat;
    Value m_val;
};//Hsv

}//namespace SubDetection

#endif // SUBDETECTION_HSV_H
