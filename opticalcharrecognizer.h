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

#ifndef SUBDETECTION_OCR_H
#define SUBDETECTION_OCR_H

#include "api/baseapi.h"

#include <QtGlobal>
#include <QString>

#include <opencv2/core/core.hpp>

#include "subdetection_global.h"

class QString;

namespace SubDetection
{

/*!
 * \brief The OpticalCharRecognizer class. Based on Tesseract-OCR.
 */
class SUBDETECTIONSHARED_EXPORT OpticalCharRecognizer
{
public:
    OpticalCharRecognizer(const QString & _tessdataParentPath, const QString & _lang);
    ~OpticalCharRecognizer();

    void setImage(const uchar * _image, int _width, int _height, int _bytes_per_px, int _bytes_per_line);
    void setImage(const cv::Mat & _image);

    void setRectangle(int _x, int _y, int _width, int _height);
    void setRectangle(const cv::Rect & _rect);

    QString getUtf8Text();

protected:
    tesseract::TessBaseAPI m_tess;
};//OpticalCharRecognizer

}//namespace SubDetection

#endif//SUBDETECTION_OCR_H
