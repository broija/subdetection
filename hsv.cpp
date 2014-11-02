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

#include "hsv.h"

#include <algorithm>

namespace SubDetection
{

namespace
{
    Hsv::Type MIN_HUE = 0;
    Hsv::Type MAX_HUE = 179;

    Hsv::Type MIN_SAT = 0;
    Hsv::Type MAX_SAT = 255;

    Hsv::Type MIN_VAL = 0;
    Hsv::Type MAX_VAL = 255;

    inline Hsv::Type constrainedValue(Hsv::Type _val, Hsv::Type _min, Hsv::Type _max)
    {
        return std::max(_min,std::min(_val,_max));
    }//constrainedValue
}//namespace

Hsv::Hsv():
    m_hue(MIN_HUE),
    m_sat(MIN_SAT),
    m_val(MIN_VAL)
{
}//Hsv

//-------------------------

Hsv::Hsv(Type _h, Type _s, Type _v)
{
    setHue(_h);
    setSaturation(_s);
    setValue(_v);
}//Hsv

//-------------------------

void Hsv::setHue(Type _h)
{
    m_hue = constrainedValue(_h,MIN_HUE,MAX_HUE);
}//setHue

//-------------------------

void Hsv::setSaturation(Type _s)
{
    m_sat = constrainedValue(_s,MIN_SAT,MAX_SAT);
}//setSaturation

//-------------------------

void Hsv::setValue(Type _v)
{
    m_val = constrainedValue(_v,MIN_VAL,MAX_VAL);
}//setValue

//-------------------------

Scalar Hsv::toScalar() const
{
    return Scalar(m_hue,m_sat,m_val);
}//toScalar

//-------------------------

}//SubDetection
