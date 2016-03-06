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

#define SD_TEST_DRAW_CONTOUR_SEPARATE 0

#include <opencv2/imgproc/imgproc.hpp>

#include "contourmanager.h"

#include "blob.h"

namespace
{
/*!
 * \brief isNotNullPVP Checks if _ptr is not a NULL pointer.
 * \param _ptr
 * \return
 */
inline bool isNotNullPVP(const SubDetection::Blob::PointVectorPtr & _ptr)
{
    return (_ptr.data() != 0);
}//isNotNullPVP

//-------------------------

/*!
 * \brief resetPVP Resets pointed PointVector.
 * \param _ptr
 */
inline void resetPVP(SubDetection::Blob::PointVectorPtr & _ptr)
{
    _ptr.reset();
}//resetPVP

//-------------------------

/*!
 * \brief copyPVP
 * \param _ptr
 * \return
 */
inline void copyPVP(const SubDetection::Blob::PointVectorPtr & _ptr, SubDetection::Blob::PointVectorPtr & _target)
{
    _target = _ptr;
}//copyPVP

//-------------------------

}//

namespace SubDetection
{

Blob::Blob():
    m_valid(false),
    m_xOffset(0),
    m_yOffset(0),
    m_pPoints(0)
{
}//Blob bool

//-------------------------

/*!
 * \brief Blob::setPointVector Defines the PointVector filled by update.
 *        It will contain all points belonging to the Blob.
 * \param _pPoints Will be filled when update has returned.
 * \warning This function must be called before update.
 * \warning pointed PointVector is not cleared. If you need to clear it after a call to setPointVector
 *          simply call clearPointVector.
 */
void Blob::setPointVector(PointVectorPtr & _pPoints)
{
    m_pPoints = _pPoints;
}//setPointVector

//-------------------------

/*!
 * \brief Blob::resetPointVector
 */
void Blob::resetPointVector()
{
    resetPVP(m_pPoints);
}//resetPointVector

//-------------------------

/*!
 * \brief Blob::clearPointVector Clears pointed PointVector.
 */
void Blob::clearPointVector()
{
    if (isNotNullPVP(m_pPoints)) m_pPoints->clear();
}//clearPointVector

//-------------------------

/*!
 * \brief Blob::update Calls setMat then initialize. Loops through all Blob pixels
 *  calling inside, outside, inhole according to pixel position in the Blob.
 *  At the end, finalize is called.
 * \param _mat Input image.
 * \param _boundingRect Input Rect.
 * \param _contour Input Contour.
 * \param _children Input _contour children.
 */
void Blob::update(const Mat & _mat, Rect & _boundingRect, Contour & _contour, ContourVector & _children)
{
    //Can we retrieve blob attributes?
    if (_mat.cols && _mat.rows)
    {
        setMat(_mat);

        m_bounding = _boundingRect;
        m_contour = _contour;
        m_children = _children;

        ContourManager::massCenter(m_contour,m_massCenter);

        m_xOffset = m_bounding.tl().x;
        m_yOffset = m_bounding.tl().y;

        m_valid = true;

        initialize();

        IndexVector::size_type childCount = _children.size();

        bool isInHole;
        Point pixel;

        for (int col = m_xOffset; col <= m_bounding.br().x; ++col)
        {
            pixel.x = col;

            for (int row = m_yOffset; row <= m_bounding.br().y; ++row)
            {
                pixel.y = row;

                int polygonTest = static_cast<int>(cv::pointPolygonTest(m_contour,pixel,false));

                switch (polygonTest)
                {
                case -1:
                    //Outside contour
                    outside(pixel);

                    break;
                case 1:
                    //Inside contour
                    isInHole = false;

                    for (ContourVector::size_type i = 0; i < childCount && !isInHole; ++i)
                    {
                        isInHole = (cv::pointPolygonTest(m_children[i],pixel,false) >= 0.);
                    }//for (ContourVector::size_type i = 0; i < childCount && !isInHole; ++i)

                    if (isInHole)
                    {
                        inHole(pixel);
                    }//if (isInHole)
                    else
                    {
                        //If PointVector pointer is defined
                        if (isNotNullPVP(m_pPoints))
                        {
                            //Fills PointVector.
                            m_pPoints->push_back(pixel);
                        }//if (isNotNullPVP(m_pPoints))

                        inside(pixel);
                    }//if (isInHole)...else

                    break;
                }//switch (polygonTest)
            }//for (int row = m_yOffset; row <= m_bounding.br().y; ++row)
        }//for (int col = m_xOffset; col <= m_bounding.br().x; ++col)

#if SD_TEST_DRAW_CONTOUR_SEPARATE
        cv::namedWindow(WINDOW_NAME_SELECTED_CONTOUR, CV_WINDOW_NORMAL);
        cv::imshow(WINDOW_NAME_SELECTED_CONTOUR, m_mat);
#endif//SD_TEST_DRAW_CONTOUR_SEPARATE

        finalize();

        releaseMat();
    }//if (_mat.cols && _mat.rows)
}//update

//-------------------------

/*!
 * \brief Blob::update Calls setMat then initialize. Loops through all Blob pixels
 *  calling inside, outside, inhole according to pixel position in the Blob.
 *  At the end, finalize is called.
 *  Convenience function that calls setPointVector before update.
 * \param _mat Input image.
 * \param _boundingRect Input Rect.
 * \param _contour Input Contour.
 * \param _children Input _contour children.
 * \param _pPoints Output points contained in the Blob.
 */
void Blob::update(const Mat & _mat, Rect & _boundingRect, Contour & _contour, ContourVector & _children, PointVectorPtr & _pPoints)
{
    setPointVector(_pPoints);
    update(_mat,_boundingRect,_contour,_children);
}//update

//-------------------------

/// Sets original Mat.
void Blob::setMat(const Mat & _mat)
{
    m_mat = _mat;
}//setMat

//-------------------------

/// Releases Original Mat.
void Blob::releaseMat()
{
    if (m_mat.data != NULL)
    {
        m_mat.release();
    }//if (m_mat.data != NULL)
}//releaseMat

//-------------------------

/*! Returns cropped orignal Mat to bounding rect dimensions.
    This function must be called for a valid blob only. There is no control done.*/
Mat Blob::croppedMat() const
{
    return m_mat(m_bounding);
}//croppedMat

//-------------------------

/*! Makes an origin shift from original Mat origin to blob origin.
    This function must be called for a valid Blob only. There is no control done.*/
Point Blob::toBlobOrigin(const Point & _point) const
{
    return Point(_point.x - m_xOffset,_point.y - m_yOffset);
}//toBlobOrigin

//-------------------------

/*! Makes an origin shift from blob origin to original Mat origin.
    This function must be called for a valid Blob only. There is no control done.*/
Point Blob::toMatOrigin(const Point & _point) const
{
    return Point(_point.x + m_xOffset,_point.y + m_yOffset);
}//toMatOrigin

//-------------------------

}//SubDetection
