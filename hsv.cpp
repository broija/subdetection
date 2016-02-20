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

#include <QString>

namespace SubDetection
{

namespace
{
    Hsv::Hue MIN_HUE = 0;
    Hsv::Hue MAX_HUE = 179;

    Hsv::Saturation MIN_SAT = 0;
    Hsv::Saturation MAX_SAT = 255;

    Hsv::Value MIN_VAL = 0;
    Hsv::Value MAX_VAL = 255;

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

Hsv::Hsv(Hue _h, Saturation _s, Value _v)
{
    setHue(_h);
    setSaturation(_s);
    setValue(_v);
}//Hsv

//-------------------------

void Hsv::setHue(Hue _h)
{
    m_hue = constrainedValue(_h,MIN_HUE,MAX_HUE);
}//setHue

//-------------------------

void Hsv::setSaturation(Saturation _s)
{
    m_sat = constrainedValue(_s,MIN_SAT,MAX_SAT);
}//setSaturation

//-------------------------

void Hsv::setValue(Value _v)
{
    m_val = constrainedValue(_v,MIN_VAL,MAX_VAL);
}//setValue

//-------------------------

/*!
 * \brief Hsv::saveMinima Copying Hue, saturation, value from _other if they are lesser than current ones.
 * \param _other
 */
void Hsv::saveMinima(const Hsv & _other)
{
    if (m_hue > _other.hue()) m_hue = _other.hue();
    if (m_sat > _other.saturation()) m_sat = _other.saturation();
    if (m_val > _other.value()) m_val = _other.value();
}//saveMinima

//-------------------------

/*!
 * \brief Hsv::saveMaxima Copying hue, saturation, value from _other if they are greater than current ones.
 * \param _other
 */
void Hsv::saveMaxima(const Hsv & _other)
{
    if (m_hue < _other.hue()) m_hue = _other.hue();
    if (m_sat < _other.saturation()) m_sat = _other.saturation();
    if (m_val < _other.value()) m_val = _other.value();
}//saveMaxima

//-------------------------

/*!
 * \brief Hsv::toMin Sets hue, saturation and value to their minimum.
 */
void Hsv::toMin()
{
    m_hue = MIN_HUE;
    m_sat = MIN_SAT;
    m_val = MIN_VAL;
}//toMin

//-------------------------

/*!
 * \brief Hsv::toMax Sets hue, saturation and value to their maximum.
 */
void Hsv::toMax()
{
    m_hue = MAX_HUE;
    m_sat = MAX_SAT;
    m_val = MAX_VAL;
}//toMax

//-------------------------

/*!
 * \brief Hsv::from cv::Vec3b Sets hue, saturation and value according to _source.
 */
void Hsv::from(cv::Vec3b & _source)
{
    setHue(_source[0]);
    setSaturation(_source[1]);
    setValue(_source[2]);
}//from cv::Vec3b

//-------------------------

Scalar Hsv::toScalar() const
{
    return Scalar(m_hue,m_sat,m_val);
}//toScalar

//-------------------------

QString Hsv::toString() const
{
    return QString("H[%1]S[%2]V[%3]")
            .arg(m_hue)
            .arg(m_sat)
            .arg(m_val);
}//toString

//-------------------------

Hsv & Hsv::operator =(const Hsv & _other)
{
    setHue(_other.hue());
    setSaturation(_other.saturation());
    setValue(_other.value());

    return *this;
}//operator =

//-------------------------

bool Hsv::operator ==(const Hsv & _other) const
{
    return (m_hue == _other.hue()
         && m_sat == _other.saturation()
         && m_val == _other.value());
}//operator ==

//-------------------------

bool Hsv::operator !=(const Hsv & _other) const
{
    return (m_hue != _other.hue()
         || m_sat != _other.saturation()
         || m_val != _other.value());
}//operator !=

//-------------------------

}//SubDetection
