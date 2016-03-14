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

#ifndef SUBDETECTION_CONTOURMANAGER_H
#define SUBDETECTION_CONTOURMANAGER_H

#include "subdetection_global.h"

#include "types.h"

namespace SubDetection
{

class SUBDETECTIONSHARED_EXPORT ContourManager
{
public:
    enum SelectionFlag
    {
        SFNone = 0x00,
        SFBoundings = 0x01,
        SFHierarchy = 0x02,
        SFMassCenters = 0x04
    };//SelectionFlag

    Q_DECLARE_FLAGS(SelectionFlags,SelectionFlag);

    static void children(const Hierarchy & _hierarchy, int _parentIndex, IndexVector & _children);
    static void children(const ContourVector & _contours, const Hierarchy & _hierarchy, int _parentIndex, ContourVector & _children);
    static int childCount(const Hierarchy & _hierarchy, int _parentIndex);
    static void massCenter(const Contour & _contour, Point & _massCenter);

    /// Helper to lighten methods and functions signatures.
    struct Attributes
    {
        ContourVector contours;
        Hierarchy hierarchy;
        RectVector boundings;
        PointVector massCenters;

        SelectionFlags flags;
    };//Attributes

    ContourManager();
    ContourManager(Thresh _binThresh);
    ContourManager(int _cvRetrievalMode, int _cvApproxMethod);
    ContourManager(Thresh _binThresh, int _cvRetrievalMode, int _cvApproxMethod);

    void setBinThresh(Thresh _binThresh);

    void setRetrievalMode(int _cvRetrievalMode);
    void setApproxMethod(int _cvApproxMethod);

    void setHierarchySearch(bool _enabled);

    void contours(ContourVector & _contours) const;
    void contours(ContourVector & _contours, Hierarchy & _hierarchy) const;
    void boundingRects(RectVector & _boundings) const;
    void massCenters(PointVector & _massCenters) const;

    void attributes(Attributes & _attributes) const;

    void process(Mat & _mat, SelectionFlags _flags);
    void process(const Mat & _mat, SelectionFlags _flags);

protected:
    void setImage(Mat & _mat);
//    void setImage(const Mat & _mat);

    void buildContours(ContourVector & _contours);
    void buildContours(ContourVector & _contours, Hierarchy &_hierarchy);
    void buildBoundings(const ContourVector & _contours, RectVector & _boundings);
    void buildMassCenters(const ContourVector & _contours, PointVector & _massCenters);

    typedef quint32 Flag;

    Thresh m_binThresh;

    int m_cvRetrievalMode;
    int m_cvApproxMethod;

    Mat m_originalMat;
    Mat m_grayMat;
    Mat m_contourMat;

    ContourVector m_tempContours;
    RectVector m_tempBoundings;
    PointVector m_tempMassCenters;
    Hierarchy m_tempHierarchy;

    SelectionFlags m_lastFlags;
};//ContourManager

}//namespace SubDetection

Q_DECLARE_OPERATORS_FOR_FLAGS(SubDetection::ContourManager::SelectionFlags)

#endif // SUBDETECTION_CONTOURMANAGER_H
