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

#ifndef SUBDETECTION_DRAWNBLOB_H
#define SUBDETECTION_DRAWNBLOB_H

#include "subdetection_global.h"

#include "blob.h"

namespace SubDetection
{

class SUBDETECTIONSHARED_EXPORT DrawnBlob : public Blob
{
public:
    typedef QSharedPointer<DrawnBlob> Pointer;

    DrawnBlob();
    virtual ~DrawnBlob(){}

    void setBackgroundColor(cv::Scalar & _color);
    cv::Scalar backgroundColor() const {return m_bgColor;}

    /// Returns Mat used to draw the blob.
    Mat & drawnMat(){return m_drawnMat;}

protected:
    virtual void initialize();

    virtual void inHole(const Point & _pixel);
    virtual void outside(const Point & _pixel);

    Mat m_drawnMat;
    cv::Scalar m_bgColor;

private:
    Q_DISABLE_COPY(DrawnBlob)
};//DrawnBlob

}//SubDetection

#endif // SUBDETECTION_DRAWNBLOB_H
