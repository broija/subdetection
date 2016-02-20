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

#include "contourmanager.h"

#include <opencv2/imgproc/imgproc.hpp>

#define SD_DETECT_EDGE_CANNY 0
#define SD_DETECT_EDGE_BLUR 0
#define SD_APPROX_CURVES 0

namespace SubDetection
{

namespace
{

void prepareContourSearch(Mat & _mat, Thresh _binThresh, Mat & _grayMat, Mat & _contourMat)
{
    cv::cvtColor(_mat,_grayMat,CV_BGR2GRAY);
#if SD_DETECT_EDGE_BLUR
    cv::blur(_grayMat,_grayMat,Size(3,3));
#endif//SD_DETECT_EDGE_BLUR

//----Detecting edges
#if SD_DETECT_EDGE_CANNY
    cv::Canny(_grayMat, _contourMat, edge_thresh, edge_thresh * ratio, kernel_size);
#else
    cv::threshold(_grayMat, _contourMat, _binThresh, 255, cv::THRESH_BINARY);
#endif
}//prepareContourSearch

//-------------------------

const int HIERARCHY_INDEX_NEXT = 0;
const int HIERARCHY_INDEX_PREVIOUS = 1;
const int HIERARCHY_INDEX_FIRST_CHILD = 2;
const int HIERARCHY_INDEX_PARENT = 3;

const Thresh DEFAULT_BIN_THRESH = 75;

const int DEFAULT_CONTOUR_RETRIEVAL_MODE = CV_RETR_EXTERNAL;
const int DEFAULT_CONTOUR_APPROX_METHOD = CV_CHAIN_APPROX_SIMPLE;

}//namespace

//-------------------------

/*!
 * \brief children Populate a children list for the given contour. No control is made.
 * \param _hierarchy OpenCV contour hierarchy used to find children.
 * \param _parentIndex Index in _hierarchy of the given contour. _parentIndex must exist in _hierarchy.
 * \param _children Output list of the found children.
 * \return number of children.
 */
void ContourManager::children(const Hierarchy & _hierarchy, int _parentIndex, IndexVector & _children)
{
    _children.clear();

    int currentChild = _hierarchy[_parentIndex][HIERARCHY_INDEX_FIRST_CHILD];//First child

    while (currentChild >= 0)
    {
        _children.push_back(currentChild);

        currentChild = _hierarchy[currentChild][HIERARCHY_INDEX_NEXT];
    }//while (currentChild >= 0)
}//children

//-------------------------

/*!
 * \brief childCount Returns the number of children for the given contour. No control is made.
 * \param _hierarchy OpenCV contour hierarchy used to find children.
 * \param _parentIndex Index in _hierarchy of the given contour. _parentIndex must exist in _hierarchy.
 * \return number of children.
 */
int ContourManager::childCount(const Hierarchy & _hierarchy, int _parentIndex)
{
    int result = 0;

    int currentChild = _hierarchy[_parentIndex][HIERARCHY_INDEX_FIRST_CHILD];//First child

    while (currentChild >= 0)
    {
        ++result;
        currentChild = _hierarchy[currentChild][HIERARCHY_INDEX_NEXT];
    }//while (currentChild >= 0)

    return result;
}//childCount

//-------------------------

/*!
 * \brief massCenter Use this function to retrieve the mass centers of _contour.
 * \param _contour Input contour.
 * \param _massCenter Output mass center.
 */
void ContourManager::massCenter(const Contour & _contour, Point & _massCenter)
{
    cv::Moments moments = cv::moments(_contour);

    _massCenter.x = lround(moments.m10/moments.m00);
    _massCenter.y = lround(moments.m01/moments.m00);
}//massCenter

//-------------------------

ContourManager::ContourManager():
    m_binThresh(DEFAULT_BIN_THRESH),
    m_cvRetrievalMode(DEFAULT_CONTOUR_RETRIEVAL_MODE),
    m_cvApproxMethod(DEFAULT_CONTOUR_APPROX_METHOD)
{
}//ContourManager

//-------------------------

/*!
 * \brief ContourManager::ContourManager
 * \param _binThresh Binarization thresh for contour detection.
 */
ContourManager::ContourManager(Thresh _binThresh):
    m_binThresh(_binThresh),
    m_cvRetrievalMode(DEFAULT_CONTOUR_RETRIEVAL_MODE),
    m_cvApproxMethod(DEFAULT_CONTOUR_APPROX_METHOD)
{
}//ContourManager Thresh

//-------------------------

/*!
 * \brief ContourManager::ContourManager
 * \param _cvRetrievalMode OpenCV contour retrieval mode. See OpenCV documentation on findContours for more details.
 * \param _cvApproxMethod OpenCV contour approximation method. See OpenCV documentation on findContours for more details.
 */
ContourManager::ContourManager(int _cvRetrievalMode, int _cvApproxMethod):
    m_binThresh(DEFAULT_BIN_THRESH),
    m_cvRetrievalMode(_cvRetrievalMode),
    m_cvApproxMethod(_cvApproxMethod)
{
}//ContourManager int int

//-------------------------

/*!
 * \brief ContourManager::ContourManager
 * \param _binThresh Binarization thresh for contour detection.
 * \param _cvRetrievalMode OpenCV contour retrieval mode. See OpenCV documentation on findContours for more details.
 * \param _cvApproxMethod OpenCV contour approximation method. See OpenCV documentation on findContours for more details.
 * \param _hierarchyEnabled Defines if hierarchy must be found when contour list is built.
 */
ContourManager::ContourManager(Thresh _binThresh, int _cvRetrievalMode, int _cvApproxMethod):
    m_binThresh(_binThresh),
    m_cvRetrievalMode(_cvRetrievalMode),
    m_cvApproxMethod(_cvApproxMethod)
{
}//ContourManager Thresh int int bool

//-------------------------

/*!
 * \brief ContourManager::setBinThresh Set the binarization thresh for contour detection.
 * \param _binThresh
 */
void ContourManager::setBinThresh(Thresh _binThresh)
{
    m_binThresh = _binThresh;
}//setBinThresh

//-------------------------

/*!
 * \brief setRetrievalMode Defines OpenCV contour retrieval mode. See OpenCV documentation on findContours for more details.
 * \param _cvRetrievalMode
 */
void ContourManager::setRetrievalMode(int _cvRetrievalMode)
{
    m_cvRetrievalMode = _cvRetrievalMode;
}//setRetrievalMode

//-------------------------

/*!
 * \brief setApproxMethod Defines OpenCV contour approximation method. See OpenCV documentation on findContours for more details.
 * \param _cvApproxMethod
 */
void ContourManager::setApproxMethod(int _cvApproxMethod)
{
    m_cvApproxMethod = _cvApproxMethod;
}//setApproxMethod

//-------------------------

void ContourManager::contours(ContourVector & _contours)
{
    _contours = m_tempContours;
}//contours

//-------------------------

void ContourManager::contours(ContourVector & _contours, Hierarchy & _hierarchy)
{
    contours(_contours);
    _hierarchy = m_tempHierarchy;
}//contours

//-------------------------

/*!
 * \brief boundingRects : populate _rects with the ones found after a call to process.
 * \param _rects Output rect list.
 */
void ContourManager::boundingRects(RectVector & _rects)
{
    _rects = m_tempBoundings;
}//boundingRects

//-------------------------

/*!
 * \brief massCenters Use this function to retrieve the mass centers after a call to process.
 * \param _massCenters Output mass centers.
 */
void ContourManager::massCenters(PointVector & _massCenters)
{
    _massCenters = m_tempMassCenters;
}//massCenters

//-------------------------

void ContourManager::process(Mat & _mat, ComputeFlags _flags)
{
    setImage(_mat);

    if (_flags & CFHierarchy)
        buildContours(m_tempContours,m_tempHierarchy);
    else
        buildContours(m_tempContours);

    if (_flags & CFBoundings)
        buildBoundings(m_tempContours,m_tempBoundings);

    if (_flags & CFMassCenters)
        buildMassCenters(m_tempContours,m_tempMassCenters);
}//process Mat

//-------------------------

/*!
 * \brief process Process the image contained in _mat according to _flags.
 * \param _mat Input image.
 * \param _flags Computing parameters.
 */
void ContourManager::process(const Mat & _mat, ComputeFlags _flags)
{
    process(_mat.clone(),_flags);
}//process const Mat

//-------------------------

/*!
 * \brief ContourManager::setImage Defines the image. _mat may be modified.
 * \warning Don't modify _mat while you want this class to work on it.
 * \param _mat
 */
void ContourManager::setImage(Mat & _mat)
{
    m_originalMat = _mat;

    m_tempContours.clear();
    m_tempBoundings.clear();
    m_tempMassCenters.clear();
    m_tempHierarchy.clear();
}//setImage Mat &

//-------------------------

/*!
 * \brief ContourManager::setImage Defines the image.
 * \param _mat
 */
/*void ContourManager::setImage(const Mat & _mat)
{
    setImage(_mat.clone());
}//setImage const Mat &

//-------------------------
*/
void ContourManager::buildContours(ContourVector & _contours)
{
    prepareContourSearch(m_originalMat,m_binThresh,m_grayMat,m_contourMat);

    cv::findContours(m_contourMat, _contours,
                     m_cvRetrievalMode,
                     m_cvApproxMethod,
                     Point(0, 0));
}//buildContours

//-------------------------

void ContourManager::buildContours(ContourVector & _contours, Hierarchy & _hierarchy)
{
    prepareContourSearch(m_originalMat,m_binThresh,m_grayMat,m_contourMat);

    cv::findContours(m_contourMat, _contours, _hierarchy,
                     m_cvRetrievalMode,
                     m_cvApproxMethod,
                     Point(0, 0));
}//buildContours

//-------------------------

/*!
 * \brief buildMassCenters Use this function to retrieve the bounding rects of _contours.
 * \param _contours Input contours.
 * \param _massCenters Output bounding rects. Its size must be at least equal to _contours size. No control is made.
 */
void ContourManager::buildBoundings(const ContourVector & _contours, RectVector & _boundings)
{
    ContourVector::size_type contourSize = _contours.size();
    _boundings.resize(contourSize);

#if SD_APPROX_CURVES
    //Approximate contour curves
    std::vector<ContourCurve> contourCurves(contourSize);
#endif//SD_APPROX_CURVES

    //Retrieving bounding rects
    for(ContourVector::size_type i = 0; i < contourSize; ++i)
    {
#if SD_APPROX_CURVES
        cv::approxPolyDP(cv::Mat(_contours[i]), contourCurves[i], 3, true);
        _boundings[i] = cv::boundingRect(cv::Mat(contourCurves[i]));
#else
        _boundings[i] = cv::boundingRect(cv::Mat(_contours[i]));
#endif//SD_APPROX_CURVES
    }//for(ContourVector::size_type i = 0; i < contourSize; ++i)
}//buildBoundings

//-------------------------

/*!
 * \brief buildMassCenters Use this function to retrieve the mass centers of _contours.
 * \param _contours Input contours.
 * \param _massCenters Output mass centers.
 */
void ContourManager::buildMassCenters(const ContourVector & _contours, PointVector & _massCenters)
{
    ContourVector::size_type contourSize = _contours.size();
    _massCenters.resize(contourSize);

    for (ContourVector::size_type i = 0; i < contourSize; ++i)
    {
        massCenter(_contours[i],_massCenters[i]);
    }//for(ContourVector::size_type i = 0; i < contourSize; ++i)
}//buildMassCenters

//-------------------------

}//namespace SubDetection
