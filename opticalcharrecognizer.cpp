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

#include "opticalcharrecognizer.h"

#include "deepdebug.h"

namespace SubDetection
{

OpticalCharRecognizer::OpticalCharRecognizer(const QString & _tessdataParentPath, const QString & _lang)
{
    m_tess.Init(_tessdataParentPath.toLocal8Bit().constData(), _lang.toLocal8Bit().constData(), tesseract::OEM_DEFAULT);
    // m_tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
    m_tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);
}//OpticalCharRecognizer

//-------------------------

OpticalCharRecognizer::~OpticalCharRecognizer()
{
    m_tess.End();
}//~OpticalCharRecognizer

//-------------------------

void OpticalCharRecognizer::setImage(const uchar * _image, int _width, int _height, int _bytes_per_px, int _bytes_per_line)
{
    m_tess.SetImage(_image,_width,_height,_bytes_per_px,_bytes_per_line);
}//setImage

//-------------------------

void OpticalCharRecognizer::setImage(const cv::Mat & _image)
{
    m_tess.SetImage((uchar*)_image.data,_image.cols,_image.rows,_image.elemSize(),_image.elemSize() * _image.cols);
}//setImage

//-------------------------

void OpticalCharRecognizer::setRectangle(int _x, int _y, int _width, int _height)
{
    m_tess.SetRectangle(_x,_y,_width,_height);
}//setRectangle

//-------------------------

void OpticalCharRecognizer::setRectangle(const cv::Rect &_rect)
{
    m_tess.SetRectangle(_rect.x,_rect.y,_rect.width,_rect.height);
}//setRectangle

//-------------------------

QString OpticalCharRecognizer::getUtf8Text()
{
    const char * text = m_tess.GetUTF8Text();

    QString sText(text);

    deepDebug("OpticalCharRecognizer::getUtf8Text : %s",text);

    delete [] text;

    return sText.trimmed();//Removes \n at the end of the line
}//getUtf8Text

//-------------------------

}//namespace SubDetection
