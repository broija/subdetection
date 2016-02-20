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

#include "statistical_tools.h"

#include "types.h"

namespace
{

inline bool hsvCompare(const SubDetection::Hsv & _first, const SubDetection::Hsv & _second)
{
    if (_first.hue() < _second.hue()) return true;
    if (_first.hue() > _second.hue()) return false;

    //Here _first and _second have the same Hue
    if (_first.saturation() < _second.saturation()) return true;
    if (_first.saturation() > _second.saturation()) return false;

    //Here _first and _second have the same Hue and Saturation
    if (_first.value() < _second.value()) return true;

    return false;
}//hsvCompare

}//namespace

namespace SubDetection
{

/*! Returns the a Hsv with Hue, Saturation and Value being averages of the ones from _first and _second.*/
Hsv average(const Hsv & _first, const Hsv & _second)
{
    Hsv result;

    result.setHue((_first.hue() + _second.hue())/2);
    result.setSaturation((_first.saturation() + _second.saturation())/2);
    result.setValue((_first.value() + _second.value())/2);

    return result;
}//average Hsv

//-------------------------

/*! Returns the a Hsv with Hue, Saturation and Value being averages of the ones from Hsv in _table.*/
Hsv average(const HsvList &_list)
{
    Hsv result;

    HsvList::size_type count = _list.size();

    if (count)
    {
        Hsv::Type h = 0;
        Hsv::Type s = 0;
        Hsv::Type v = 0;

        for (HsvList::const_iterator it = _list.begin(); it != _list.end(); ++it )
        {
            h += (*it).hue();
            s += (*it).saturation();
            v += (*it).value();
        }//for (HsvList::const_iterator it = _list.begin(); it != _list.end(); ++it )

        result.setHue(h / count);
        result.setSaturation(s / count);
        result.setValue(v / count);
    }//if (count)

    return result;
}//average HsvList

//-------------------------

/*! Returns the median value of the list. T must have an associated operator <.
    The list is sorted:
     - the middle item is returned if there are an odd count of items.
     - the average value of the two midlle items is returned in case of even count.*/
Hsv median(HsvList & _list)
{
    Hsv result;

    int count = _list.size();

    if (count)
    {
        qSort(_list.begin(),_list.end(),hsvCompare);
//        qSort(_list);

        int odd = count %2;
        int medianIndex = count/2;

        if (odd)
        {
            result = _list.at(medianIndex);
        }//if (odd)
        else
        {
            result = average(_list.at(medianIndex-1),_list.at(medianIndex));
        }//if (odd)
    }//if (count)

    return result;
}//median

}//SubDetection
