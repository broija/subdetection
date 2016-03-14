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

#ifndef SUBDETECTION_CONVERSION_H
#define SUBDETECTION_CONVERSION_H

#include "subdetection_global.h"

class QImage;
class QPixmap;
class QByteArray;
class QColor;

template <typename>
class QVector;

class Mat;
class Hsv;
class RgbTable;

namespace SubDetection
{

//class Hsv;

enum ImageFormat {RGB32,///< RGB - 32bit
                  ARGB32,///< ARGB - 32bit
                  Gray8///< Grayscale - 8bit
                 };

const RgbTable & getGray8RgbTable();

//Qt to OpenCV
template <ImageFormat format_>
void SUBDETECTIONSHARED_EXPORT imageToMat(QImage & _image, Mat & _mat, bool _shareMem = false);///< No check is done. Be sure that the template format matches '_image' format.
template <ImageFormat format_>
void SUBDETECTIONSHARED_EXPORT pixmapToMat(const QPixmap & _pixmap, Mat & _mat);///< No check is done. Be sure that the template format matches '_pixmap' format.
template <ImageFormat format_>
void SUBDETECTIONSHARED_EXPORT byteArrayToMat(const QByteArray & _data, Mat & _mat);///< No check is done. Be sure that the template format matches '_data' format.

//OpenCV to Qt
template <ImageFormat format_>
void SUBDETECTIONSHARED_EXPORT matToImage(const Mat & _mat, QImage & _image);///< No check is done. Be sure that the template format matches '_mat' format.
template <ImageFormat format_>
void SUBDETECTIONSHARED_EXPORT matToPixmap(const Mat & _mat, QPixmap & _pixmap);///< No check is done. Be sure that the template format matches '_mat' format.

void SUBDETECTIONSHARED_EXPORT hsvToColor(const Hsv & _hsv, QColor & _color);///< Converts a Hsv to QColor

}//namespace SubDetection

#endif // SUBDETECTION_CONVERSION_H
