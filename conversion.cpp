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

#include "conversion.h"

#include <QImage>
#include <QPixmap>
#include <QByteArray>

#include <opencv2/core/core.hpp>

#include "hsv.h"

namespace SubDetection
{

//-------------------------

/*!
 * \brief getGray8RgbTable
 * \return A color table used for grayscale QImage. Don't forger to call "init" early, specifically if you use threads in your program.
 */
const RgbTable & getGray8RgbTable()
{
    static bool s_init = false;
    static RgbTable s_table(256);

    if (!s_init)
    {
        s_init = true;

        for (int i = 0; i < s_table.size(); ++i)
        {
            s_table.replace(i,qRgb(i,i,i));
        }//for (int i = 0; i < s_table.size(); ++i)
    }//if (!s_init)

    return s_table;
}//getGray8RgbTable

//-------------------------

template<>
void imageToMat<RGB32>(QImage & _image, cv::Mat & _mat, bool _shareMem)
{
//    Q_ASSERT_X(_image.format() == QImage::Format_RGB32,"imageToMat<RGB32>","Invalid format.");

    _mat = cv::Mat(_image.height(),_image.width(),
                   CV_8UC4,
                   static_cast<uchar*>(_image.bits()),
                   _image.bytesPerLine());

    if (!_shareMem) _mat = _mat.clone();
}//imageToMat RGB32

//-------------------------

template<>
void pixmapToMat<RGB32>(const QPixmap & _pixmap, cv::Mat & _mat)
{
    QImage tmpImage(_pixmap.toImage());

    imageToMat<RGB32>(tmpImage,_mat);
}//pixmapToMat RGB32

//-------------------------

template<>
void imageToMat<ARGB32>(QImage & _image, cv::Mat & _mat, bool _shareMem)
{
//    Q_ASSERT_X(_image.format() == QImage::Format_RGB32,"imageToMat<ARGB32>","Invalid format.");

    _mat = cv::Mat(_image.height(),_image.width(),
                   CV_8UC4,
                   static_cast<uchar*>(_image.bits()),
                   _image.bytesPerLine());

    if (!_shareMem) _mat = _mat.clone();
}//imageToMat ARGB32

//-------------------------

template<>
void pixmapToMat<ARGB32>(const QPixmap & _pixmap, cv::Mat & _mat)
{
    QImage tmpImage(_pixmap.toImage());

    imageToMat<ARGB32>(tmpImage,_mat);
}//pixmapToMat ARGB32

//-------------------------

template <>
void byteArrayToMat<RGB32>(const QByteArray & _data, cv::Mat & _mat)
{
    QImage tmpImage;
    tmpImage.loadFromData(_data);

    imageToMat<RGB32>(tmpImage,_mat);
}//byteArrayToMat

//-------------------------

template <>
void matToImage<RGB32>(const cv::Mat & _mat, QImage & _image)
{
    _image = QImage(_mat.data,_mat.cols,_mat.rows,_mat.step,QImage::Format_RGB32);
}//matToImage RGB32

//-------------------------

template <>
void matToPixmap<RGB32>(const cv::Mat & _mat, QPixmap & _pixmap)
{
    QImage tmpImage;
    matToImage<RGB32>(_mat,tmpImage);

    _pixmap = QPixmap::fromImage(tmpImage);
}//matToPixmap RGB32

//-------------------------

template <>
void matToImage<ARGB32>(const cv::Mat & _mat, QImage & _image)
{
    _image = QImage(_mat.data,_mat.cols,_mat.rows,_mat.step,QImage::Format_ARGB32);
}//matToImage ARGB32

//-------------------------

template <>
void matToPixmap<ARGB32>(const cv::Mat & _mat, QPixmap & _pixmap)
{
    QImage tmpImage;
    matToImage<ARGB32>(_mat,tmpImage);

    _pixmap = QPixmap::fromImage(tmpImage);
}//matToPixmap ARGB32

//-------------------------

template <>
void byteArrayToMat<ARGB32>(const QByteArray & _data, cv::Mat & _mat)
{
    QImage tmpImage;
    tmpImage.loadFromData(_data);

    imageToMat<ARGB32>(tmpImage,_mat);
}//byteArrayToMat

//-------------------------

template <>
void matToImage<Gray8>(const cv::Mat & _mat, QImage & _image)
{
    _image = QImage(_mat.data,_mat.cols,_mat.rows,_mat.step,QImage::Format_Indexed8);
    _image.setColorTable(getGray8RgbTable());
}//matToImage Gray8

//-------------------------

template <>
void matToPixmap<Gray8>(const cv::Mat & _mat, QPixmap & _pixmap)
{
    QImage tmpImage;
    matToImage<Gray8>(_mat,tmpImage);

    _pixmap = QPixmap::fromImage(tmpImage);
}//matToPixmap RGB32

//-------------------------

void hsvToColor(const Hsv & _hsv, QColor & _color)
{
    _color.setHsv(_hsv.hue() * 2,_hsv.saturation(),_hsv.value());
}//hsvToColor

//-------------------------

}//namespace SubDetection
