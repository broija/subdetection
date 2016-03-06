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

#ifndef SUBDETECTION_HSVBLOB_H
#define SUBDETECTION_HSVBLOB_H

#include "subdetection_global.h"

#include "blob.h"

#include <QList>
#include "hsv.h"
#include "hsvlist.h"

namespace SubDetection
{

class SUBDETECTIONSHARED_EXPORT HsvBlob : public Blob
{
public:
    typedef QSharedPointer<HsvBlob> Pointer;

    HsvBlob(){}
    virtual ~HsvBlob(){}

    Hsv minHsv() const {return m_min;}///< Returns minimum Hsv.
    Hsv maxHsv() const {return m_max;}///< Returns maximum Hsv.
    Hsv medianHsv() const {return m_median;}///< Returns median Hsv.

protected:
    virtual void initialize();

    virtual void inside(const Point & _pixel);

    virtual void finalize();

    Mat m_hsvMat;

    Hsv m_min;
    Hsv m_max;

    Hsv m_tmpMin;
    Hsv m_tmpMax;

    Hsv m_median;

    HsvList m_list;

private:
    Q_DISABLE_COPY(HsvBlob)
};//HsvBlob

}//SubDetection

#endif // SUBDETECTION_HSVBLOB_H
