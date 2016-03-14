/*!
    Copyright 2015 Broija

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

#ifndef SUBDETECTION_BLOB_H
#define SUBDETECTION_BLOB_H

#include <QSharedPointer>

#include "subdetection_global.h"

#include "types.h"

namespace SubDetection
{

/*! Base class made for gathering information on a blob. It will loop through each pixel of it.*/
class SUBDETECTIONSHARED_EXPORT Blob
{
public:
    typedef QSharedPointer<Blob> Pointer;

    typedef QSharedPointer<PointVector> PointVectorPtr;

    Blob();
    virtual ~Blob(){}

    bool isValid() const {return m_valid;}///< Returns true if Blob is valid.

    void setPointVector(PointVectorPtr & _pPoints);
    void resetPointVector();
    void clearPointVector();

    void update(const Mat & _mat, Rect & _boundingRect, Contour & _contour, ContourVector & _children);
    void update(const Mat & _mat, Rect & _boundingRect, Contour & _contour, ContourVector & _children, PointVectorPtr & _pPoints);

protected:
    Mat croppedMat() const;

    virtual void initialize(){}///< Initialization operations before pixel loop. Called if Blob is valid.

    virtual void inside(const Point &){}///< Called when pixel is inside Blob. Original mat origin.
    virtual void inHole(const Point &){}///< Called when pixel is in a hole of Blob. Original mat origin.
    virtual void outside(const Point &){}///< Called when pixel is outside Blob. Original mat origin.

    virtual void finalize(){}///< Finalization operations after pixel loop. Called if Blob is valid.

    int xOffset() const {return m_xOffset;}///< Returns X offset regarding Orignal Mat.
    int yOffset() const {return m_yOffset;}///< Returns Y offset regarding Orignal Mat.

    Point toBlobOrigin(const Point & _point) const;
    Point toMatOrigin(const Point & _point) const;

    Rect m_bounding;
    Contour m_contour;
    Point m_massCenter;
    ContourVector m_children;

private:
    void setMat(const Mat & _mat);
    void releaseMat();

    Mat m_mat;///< Original Mat

    bool m_valid;

    int m_xOffset;
    int m_yOffset;

    QSharedPointer<PointVector> m_pPoints;

    Q_DISABLE_COPY(Blob)
};//Blob

}//namespace Blob

#endif // SUBDETECTION_BLOB_H
