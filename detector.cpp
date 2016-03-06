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

#define SD_MASKED_TYPES 0

#define SD_TEST_DRAW 1
#define SD_TEST_DRAW_RESULT 1
#define SD_TEST_DRAW_BOUNDINGS_SEPARATE 0

#define SD_TEST_CENTERED 0

#include <QMap>
#include <QHash>
#include <QImage>
#include <QStringList>

#include <opencv2/imgproc/imgproc.hpp>
#if SD_TEST_DRAW
#include <opencv2/highgui/highgui.hpp>
#endif//SD_TEST_DRAW

#include <algorithm>

#include "deepdebug.h"
#include "blob.h"
#include "hsvblob.h"

#include "hash.h"
#include "types.h"

#include "detector.h"

namespace SubDetection
{

namespace
{
    inline void abs(Point::value_type & _value)
    {
        if (_value < 0) _value *= -1;
    }//abs
//------
    bool pointCompareAlongXAxis(const Point & _first, const Point & _second)
    {
        return ((_first.x < _second.x) || ((_first.x == _second.x) && (_first.y < _second.y)));
    }//bool pointCompareAlongXAxis(const Point & _first, const Point & _second)
//------
    bool rectCompareAlongYAxis(const Rect & _first, const Rect & _second)
    {
        return (_first.y < _second.y);
    }//rectCompareAlongYAxis
//------
    inline int min(int _first, int _second)
    {
        return std::min(_first,_second);
    }//min
//------
    inline int max(int _first, int _second)
    {
        return std::max(_first,_second);
    }//min
//------
    Point::value_type UNDEFINED_COORD = -1;

    typedef QMap<Point::value_type,quint32> CoordCountMap;
    typedef QHash<Point,Rect> BoundingRectHash;
    typedef QList<ContourVector::size_type> ContourIndexList;
//------

#if SD_TEST_DRAW
    cv::RNG COLOR_RNG(12345);

    inline cv::Scalar randomColor()
    {
        return Scalar(COLOR_RNG.uniform(0, 255), COLOR_RNG.uniform(0,255), COLOR_RNG.uniform(0,255));
    }//randomColor
#endif

//------

#if SD_TEST_DRAW_CONTOUR_SEPARATE
    const char * WINDOW_NAME_SELECTED_CONTOUR = "Selected contour";
#endif
#if SD_TEST_DRAW_BOUNDINGS_SEPARATE
    const char * WINDOW_NAME_BOUNDINGS = "Boundings";
#endif//SD_TEST_DRAW_BOUNDINGS_SEPARATE

    const char * DEFAULT_TESSERACT_PARENT_PATH = ".";
    const char * DEFAULT_LANGUAGE = "eng";

    Detector::BlobSelectionBehavior DEFAULT_BSBEHAVIOR = Detector::BSB_INNER;
//        Detector::BlobSelectionBehavior DEFAULT_BSBEHAVIOR = Detector::BSB_OUTTER;
}//namespace

Detector::Detector():
    m_pParams(0),
    m_drawBoundings(false),
    m_ocr(DEFAULT_TESSERACT_PARENT_PATH, DEFAULT_LANGUAGE),
    m_bsbehavior(DEFAULT_BSBEHAVIOR)
{
    createParameters();
    forget();
}//Detector

//-------------------------

Detector::Detector(const QSharedPointer<Parameters> & _pParams):
    m_drawBoundings(false),
    m_ocr(DEFAULT_TESSERACT_PARENT_PATH, DEFAULT_LANGUAGE),
    m_bsbehavior(DEFAULT_BSBEHAVIOR)
{
    setParameters(_pParams);
}//Detector Parameters *

//-------------------------

Detector::Detector(const Parameters & _params):
    m_drawBoundings(false),
    m_ocr(DEFAULT_TESSERACT_PARENT_PATH, DEFAULT_LANGUAGE),
    m_bsbehavior(DEFAULT_BSBEHAVIOR)
{
    setParameters(_params);
}//Detector const Parameters &

//-------------------------

/*!
 * \brief Detector::Detector
 * \param _pParams : a pointer to a Parameters structure.
 * \param _tessdataParentPath : tesseract "tessdata" parent directory.
 * \param _lang : language. Be sure that corresponding language files are available in tessdata directory.
 */
Detector::Detector(const QSharedPointer<Parameters> & _pParams, const QString & _tessdataParentPath, const QString & _lang):
    m_drawBoundings(false),
    m_ocr(_tessdataParentPath,_lang),
    m_bsbehavior(DEFAULT_BSBEHAVIOR)
{
    setParameters(_pParams);
}//Detector Parameters *, const QString &, const QString &

//-------------------------

/*!
 * \brief Detector::Detector
 * \param _tessdataParentPath : tesseract "tessdata" parent directory.
 * \param _lang : language. Be sure that corresponding language files are available in tessdata directory.
 */
Detector::Detector(const QString & _tessdataParentPath, const QString & _lang):
    m_drawBoundings(false),
    m_ocr(_tessdataParentPath,_lang),
    m_bsbehavior(DEFAULT_BSBEHAVIOR)
{
}//Detector const QString &, const QString &

//-------------------------

Detector::~Detector()
{
    cv::destroyAllWindows();
}//~Detector

//-------------------------

/*! Detector parameters will point directly to passed parameters.*/
void Detector::setParameters(const QSharedPointer<Parameters> & _pParams)
{
    m_pParams = _pParams;

    forget();
}//setParameters

//-------------------------

/*! Copies parameters.*/
void Detector::setParameters(const Parameters & _params)
{
    createParameters();

    *m_pParams = _params;

    forget();
}//setParameters

//-------------------------

/*! Sets how Detector should decide when a blob is selected.*/
void Detector::setBlobSelectionBehavior(BlobSelectionBehavior _behavior)
{
    m_bsbehavior = _behavior;
}//setBlobSelectionBehavior

//-------------------------

/*!
 * \brief Detector::enableBoundingsDrawing Enable or disable text boundings drawing.
 * \param _enabled true: enable, false: disable.
 */
void Detector::enableBoundingsDrawing(bool _enabled)
{
    m_drawBoundings = _enabled;
}//enableBoundingDrawings

//-------------------------

/*!
 * \brief Detector::forget : if "detect" was precedently called, "forget" allows to forget previous detection, in case
 *        text has not changed. When your Detector Parameters points directly to an outside structure,
 *        thus the object is not notified for parameter changes, you can call "forget" to act as if the image that
 *        is analyzed was the first one.
 */
void Detector::forget()
{
    m_forget = true;
}//forget

//-------------------------

/*!
 * \brief Detector::detect : tries to find subtitles in the _image according to previously set Parameters.
 *        If Detector finds out that text has not changed, subtitles won't be searched. Call "forget"
 *        to bypass this behaviour.
 * \param _image
 * \param _subtitles detected text. If text has not changed, or at least if Detector thinks so, _subtitles won't be modified.
 */
Detector::ReturnCode Detector::detect(const Mat & _image, QStringList & _subtitles)
{
    deepDebug2("rows:%d, cols:%d",_image.rows,_image.cols);

    if (!m_pParams->zone.width
     || !m_pParams->zone.height
     || (m_pParams->zone.x + m_pParams->zone.width > _image.cols)
     || (m_pParams->zone.y + m_pParams->zone.height > _image.rows))
    {
        deepDebug("Detector::detect : Invalid text zone. x:%d y:%d w:%d h:%d",
                  m_pParams->zone.x,
                  m_pParams->zone.y,
                  m_pParams->zone.width,
                  m_pParams->zone.height);

        return RC_BAD_PARAM;
    }//if (!m_pParams->zone.width...

    if (!_image.rows || !_image.cols)
    {
        deepDebug("Detector::detect : Invalid row or col number for input Mat.");

        return RC_INVALID_INPUT_IMAGE;
    }//if (!_image.rows || !_image.cols)

//    m_centered = _centered;
    m_originalMat = _image;

//----HSV masking
    cv::cvtColor(m_originalMat,m_hsvMat,cv::COLOR_BGR2HSV);//HSV conversion

    //Changing desired colors into white
    cv::inRange(m_hsvMat, m_pParams->hsvMin.toScalar(), m_pParams->hsvMax.toScalar(), m_threshMat);

    m_textZoneMat = m_threshMat(m_pParams->zone);

    //Detecting if text has changed
    if (!m_forget && compareImages(m_oldTextZoneMat,m_textZoneMat))
    {
        deepDebug("Text has not changed!");
        return RC_NO_CHANGE;
    }//if (compareImages(m_oldTextZoneMat,m_textZoneMat))

    if (m_forget) m_forget = false;

    m_oldTextZoneMat = m_textZoneMat.clone();

    //Get only desired colors from the original image
    m_maskedMat = Mat::zeros(m_originalMat.size(),m_originalMat.type());
    cv::bitwise_and(m_originalMat,m_originalMat,m_maskedMat,m_threshMat);

#if SD_MASKED_TYPES
    //Grayscale before edge detection
    switch (m_maskedMat.type())
    {
    case CV_8UC4:
        deepDebug("Masked type: CV_8UC4");
        break;
    case CV_8UC1:
        deepDebug("Masked type: CV_8UC1");
        break;
    default:
        deepDebug("Masked type: other");
        break;
    }//switch (m_maskedMat.type())
#endif//SD_MASKED_TYPES

    ContourVector contours;

    m_contourManager.setBinThresh(m_pParams->thresh);
    m_contourManager.setRetrievalMode(CV_RETR_EXTERNAL);
    m_contourManager.setApproxMethod(CV_CHAIN_APPROX_SIMPLE);

    m_contourManager.process(m_maskedMat,ContourManager::SFBoundings | ContourManager::SFMassCenters);

    m_contourManager.contours(contours);

    m_boundingRects.clear();
    getTextBoundingRects(contours,m_boundingRects);

    m_ocr.setImage(m_threshMat);

    for (RectVector::size_type i = 0; i < m_boundingRects.size(); ++i)
    {
        m_ocr.setRectangle(m_boundingRects[i]);
        _subtitles.push_back(m_ocr.getUtf8Text());
    }//for (RectVector::size_type i = 0; i < m_boundingRects.size(); ++i)

    return RC_OK;
}//detect

//-------------------------

/*!
 * \brief Detector::getPointedBlob : populates _pBlob attributes according to the blob at _point.
 * \param _image Input image.
 * \param _point Pointed pixel.
 * \param _pBlob Pointer to Blob to be filled.
 * \return ReturnCode RC_OK : valid result,
 *                    RC_INVALID_INPUT_IMAGE,
 *                    RC_BAD_PARAM : parameters are not set : thresh is needed,
 *                    RC_AMBIGUOUS : two or more possible blobs,
 *                    RC_INCONSISTENT : pointed blob is inconsistent.
 */
Detector::ReturnCode Detector::getPointedBlob(const Mat & _image, const Point & _point, BlobPtr & _pBlob)
{
    deepDebug("Detector::getPointedBlob : original image %dx%d",_image.cols,_image.rows);

    ReturnCode result = setBlobMat(_image);

    if (result != RC_OK) return result;

    //Retrieving contours
    ContourManager::Attributes cmAttributes;
    blobContourSetup(cmAttributes);

    return getPointedBlob(cmAttributes,_point,_pBlob);
}//getPointedBlob

//-------------------------

/*!
 * \brief Detector::createParameters Create new parameters, releasing previous ones.
 */
void Detector::createParameters()
{
    m_pParams = QSharedPointer<Parameters>(new Parameters);
}//createParameters

//-------------------------

/*!
 * \brief Detector::checkImage : check whether images are similar.
 * \param _image
 * \return RC_OK : image is correct. RC_INVALID_INPUT_IMAGE : image is not valid.
 */
Detector::ReturnCode Detector::checkImage(const Mat & _image) const
{
    if (!_image.rows || !_image.cols)
    {
        deepDebug("Detector::checkImage : Invalid row or col number for input Mat.");

        return RC_INVALID_INPUT_IMAGE;
    }//if (!_image.rows || !_image.cols)

    return RC_OK;
}//checkImage

//------------------------------

/*!
 * \brief Detector::compareImages : check whether images are similar.
 * \param _first first image.
 * \param _second second image.
 * \return true if images are considered very similar.
 */
bool Detector::compareImages(const Mat & _first, const Mat & _second) const
{
    bool result = false;

    if (_first.rows != _second.rows
     || _first.cols != _second.cols
     || _first.type() != _second.type())
    {
        deepDebug("Detector::compare : images are different.");
    }//if (_first.rows != _second.rows...)
    else
    {
        double distance = cv::norm(_first,_second,CV_L2);

        double matchValue = distance / static_cast<double>(_second.cols * _second.rows);

        deepDebug2("Distance: %lf, Match value: %lf",distance,matchValue);

        result = (matchValue < m_pParams->matchRatio);
    }//if (_first.rows != _second.rows...else

    return result;
}//compareImages

//------------------------------

/*!
 * \brief getTextBoundingRects Compute bounding rects of the given contours. Each rect contains a maximum of aligned contours.
 * \param _contours
 * \param _rects The computed rects.
 */
void Detector::getTextBoundingRects(const ContourVector &_contours, RectVector & _rects)
{
    ContourVector::size_type contourSize = _contours.size();

    PointVector massCenters;
    m_contourManager.massCenters(massCenters);

    RectVector boundingRects;
    m_contourManager.boundingRects(boundingRects);

#if SD_TEST_DRAW
    //Drawing
    //Drawing vars
    cv::Scalar color;

    Size originalSize(m_originalMat.cols,m_originalMat.rows);

    bool draw = (m_drawBoundings && originalSize.width && originalSize.height);
#if 1
    if (draw)
        m_boundingsMat = m_originalMat.clone();
    else
        m_boundingsMat = m_originalMat;
#else
    if (draw) m_boundingsMat = cv::Mat::zeros(originalSize, CV_8UC3);
#endif
#endif//SD_TEST_DRAW

    PointVector validMassCenters;
    BoundingRectHash massCentersBoundingRects;

    cv::Size roiSize = m_pParams->zone.size();
    if (roiSize.width && roiSize.height)
    {
#if SD_TEST_DRAW
        if (draw) cv::rectangle(m_boundingsMat,m_pParams->zone.tl(),m_pParams->zone.br(),cv::Scalar(255,0,0),2,8,0);
#endif//SD_TEST_DRAW
        //Filtering mass centers.
        for (ContourVector::size_type i = 0; i< contourSize; ++i)
        {
            //Object is in region of interest
            if (m_pParams->zone.contains(massCenters[i]))
            {
#if SD_TEST_DRAW
                if (draw) color = cv::Scalar(0,0,255);
#endif//SD_TEST_DRAW
                cv::Size rectSize = boundingRects[i].size();

                //Object not too big?
                if (rectSize.width <= m_pParams->charMaxSize.width && rectSize.height <= m_pParams->charMaxSize.height)
                {
#if SD_TEST_DRAW
                    if (draw)
                    {
                        cv::rectangle(m_boundingsMat, boundingRects[i].tl(), boundingRects[i].br(), color, 2, 8, 0);//contour bounding rect
                        cv::circle(m_boundingsMat, massCenters[i], 4, color, -1, 8, 0);//mass center
                    }//if (draw)
#endif//SD_TEST_DRAW
                    validMassCenters.push_back(massCenters[i]);

                    //Associating mass center and his bounding rect
                    massCentersBoundingRects.insert(massCenters[i],boundingRects[i]);

                }//if (rectSize.width <= m_pParams->charMaxSize.width && rectSize.height <= m_pParams->charMaxSize.height)
            }//if (m_pParams->zone.contains(massCenters[i]))
        }//for (ContourVector::size_type i = 0; i< contourSize; ++i)

        if (!validMassCenters.empty())
        {
            //Sorting by X
            std::sort(validMassCenters.begin(),validMassCenters.end(),&pointCompareAlongXAxis);

            //While there are points to deal with
            do
            {
                CoordCountMap alignmentCountMap;

                deepDebug3("Point list:");
                //Building a count map for each different Y coordinate
                for (PointVector::iterator it = validMassCenters.begin(); it != validMassCenters.end(); ++it)
                {
                    Point::value_type y = (*it).y;

                    deepDebug3("%d:%d",(*it).x,(*it).y);

                    int alignmentCount = alignmentCountMap.value(y,0) +1;
                    alignmentCountMap.insert(y,alignmentCount);
                }//for (PointVector::iterator it = validMassCenters.begin(); it != validMassCenters.end(); ++it)

                deepDebug3("Map:");
                for (CoordCountMap::const_iterator i = alignmentCountMap.begin(); i != alignmentCountMap.end(); ++i)
                {
                    deepDebug3("[%d,%d]",i.key(),*i);
                }//for (CoordCountMap::const_iterator i = alignmentCountMap.begin(); i != alignmentCountMap.end(); ++i)

                int maxCount = 0;

                CoordCountMap::const_iterator firstItem;
                CoordCountMap::const_iterator lastItem;

                bool stop;

                deepDebug3("Searching best alignment...");
                //Searching the biggest group of aligned points
                for (CoordCountMap::const_iterator i = alignmentCountMap.begin(); i != alignmentCountMap.end(); ++i)
                {
                    stop = false;

                    Point::value_type refY = i.key();
                    int count = *i;

                    if (count > maxCount)
                    {
                        maxCount = count;

                        firstItem = i;
                        lastItem = i;
                    }//if (count > maxCount)

                    deepDebug3("Count (i): %d",count);
                    for (CoordCountMap::const_iterator j = i+1; j != alignmentCountMap.end() && !stop; ++j)
                    {
                        //If points with this Y coordinate are not in tolerance zone
                        if ((j.key() - refY) > m_pParams->yTolerance)
                        {
                            stop = true;
                        }//if ((j.key() - refY) > m_pParams->yTolerance)
                        else
                        {
                            count += *j;
                            deepDebug3("Count (j): %d",count);
                            if (count > maxCount)
                            {
                                maxCount = count;

                                firstItem = i;
                                lastItem = j;
                            }//if (count > maxCount)
                        }//if ((j.key() - refY) > m_pParams->yTolerance)...else
                    }//for (CoordCountMap::const_iterator j = i+1; j != alignmentCountMap.end() && !stop; ++j)
                }//for (CoordCountMap::const_iterator i = alignmentCountMap.begin(); i != alignmentCountMap.end(); ++i)

                deepDebug3("First item [%d,%d]",firstItem.key(),*firstItem);
                deepDebug3("Last item [%d,%d]",lastItem.key(),*lastItem);

                //Only one point?
                if (maxCount == 1)
                {
                    //No need to keep it, removing first (and only one)
                    for (PointVector::iterator it = validMassCenters.begin(); it != validMassCenters.end(); ++it)
                    {
                        if ((*it).y == firstItem.key())
                        {
                            validMassCenters.erase(it);
                            break;
                        }//if ((*it).y == firstItem.key())
                    }//for (PointVector::iterator it = validMassCenters.begin(); it != validMassCenters.end(); ++it)
                }//if (maxCount == 1)
                else
                {
                    Point::value_type lastX = UNDEFINED_COORD;
                    Point::value_type currentX;
                    bool validPoint;
                    stop = false;

                    Rect::value_type minX = std::numeric_limits<Rect::value_type>::max();
                    Rect::value_type minY = std::numeric_limits<Rect::value_type>::max();
                    Rect::value_type maxX = 0;
                    Rect::value_type maxY = 0;

                    deepDebug3("Building result:");
                    //Building result
                    for (PointVector::iterator it = validMassCenters.begin(); it != validMassCenters.end() && !stop;)
                    {
                        deepDebug3("%d:%d",(*it).x,(*it).y);

                        validPoint = false;
                        currentX = (*it).x;//Saving X coordinate for spacing check

                        //Checking if point is in alignment range
                        if ((*it).y >= firstItem.key() && (*it).y <= lastItem.key())
                        {
                            deepDebug3("Point is in range {%d,%d}",firstItem.key(),lastItem.key());

                            //First point of this group?
                            if (lastX == UNDEFINED_COORD)
                            {
                                deepDebug3("First");
                                validPoint = true;
                            }//if (lastX == UNDEFINED_COORD)
                            else
                            {
                                deepDebug3("Current[%d] Last[%d]",currentX,lastX);
                                //Is spacing correct?
                                if ((currentX - lastX) <= m_pParams->xTolerance)
                                {
                                    deepDebug3("Correct spacing");
                                    validPoint = true;
                                }//if ((currentX - lastX) <= m_pParams->xTolerance)
                                else
                                {
                                    //If this point is too far from the last one, the next ones will be too
                                    stop = true;
                                }//if ((currentX - lastX) <= m_pParams->xTolerance)...else
                            }//if (lastX == UNDEFINED_COORD)...else
                        }//if ((*it).y >= firstItem.key() && (*it).y <= lastItem.key())

                        //Point is OK
                        if (validPoint)
                        {
                            Rect tmpRect(massCentersBoundingRects.value((*it)));
                            //Saving extrema
                            if (tmpRect.tl().x < minX) minX = tmpRect.tl().x;
                            if (tmpRect.tl().y < minY) minY = tmpRect.tl().y;
                            if (tmpRect.br().x > maxX) maxX = tmpRect.br().x;
                            if (tmpRect.br().y > maxY) maxY = tmpRect.br().y;

                            //Saving X coordinate to check spacing
                            lastX = currentX;

                            //Removing it from the list
                            it = validMassCenters.erase(it);
                        }//if (validPoint)
                        else
                            ++it;
                    }//for (PointVector::iterator it = validMassCenters.begin(); it != validMassCenters.end() && !stop;)

                    Rect::value_type rectWidth = (maxX - minX);
                    Rect::value_type rectHeight = (maxY - minY);

                    bool validRect = (maxCount > 2//If rect contains enough characters.
                                   && rectWidth > m_pParams->xTolerance *3//and rect is big enough
                                   && rectHeight >= (m_pParams->yTolerance / 10));
#if SD_TEST_CENTERED
                    if (validRect && m_centered)
                    {
                        double centeringRatio = static_cast<double>(m_pParams->zone.br().x - maxX) / static_cast<double>(minX - m_pParams->zone.tl().x);
                        deepDebug2("Centering ratio: %lf",centeringRatio);
                        validRect = (centeringRatio > 0.8 && centeringRatio < 1.2);//20%

                        if (!validRect) deepDebug2("Text is not centered.");
                    }//if (validRect && m_centered)
#endif
                    if (!validRect)
                    {
                        deepDebug2("Invalid rect: count[%d] w[%d] h[%d]",maxCount,rectWidth,rectHeight);
                    }//if (!validRect)
                    else
                    {
                        deepDebug2("Valid rect: count[%d] w[%d] h[%d]",maxCount,rectWidth,rectHeight);
                        _rects.push_back(Rect(minX,minY, rectWidth, rectHeight));
                    }//if (!validRect)...else
                }//if (maxCount == 1)...else
             } while (!validMassCenters.empty());

            //Sorting rects from top to bottom of the image.
            if (!_rects.empty()) std::sort(_rects.begin(),_rects.end(),&rectCompareAlongYAxis);
        }//if (!validMassCenters.empty())
#if SD_TEST_DRAW_RESULT
        if (draw)
        {
            for (RectVector::size_type i = 0; i < _rects.size(); ++i)
            {
                cv::rectangle(m_boundingsMat,_rects[i],cv::Scalar(0,255,0),1,4);
            }//for (RectVector::size_type i = 0; i < _rects.size(); ++i)
        }//if (draw)
#if SD_TEST_DRAW_BOUNDINGS_SEPARATE
        cv::namedWindow(WINDOW_NAME_BOUNDINGS, CV_WINDOW_NORMAL);
        cv::imshow(WINDOW_NAME_BOUNDINGS, m_boundingsMat);
#endif//SD_TEST_DRAW_BOUNDINGS_SEPARATE
#endif//SD_TEST_DRAW_RESULT
    }//if (roiSize.width && roiSize.height)
}//getTextBoundingRects

//------------------------------

/*!
 * \brief Detector::blobContourSetup Fill _cmAttributes with Blob detection parameters.
 * \param _cmAttributes Output contour attributes.
 */
void Detector::blobContourSetup(ContourManager::Attributes & _cmAttributes)
{
    m_contourManager.setBinThresh(m_pParams->thresh);
    m_contourManager.setRetrievalMode(CV_RETR_CCOMP);
    m_contourManager.setApproxMethod(CV_CHAIN_APPROX_NONE);

    m_contourManager.process(m_blobMat,ContourManager::SFBoundings | ContourManager::SFHierarchy);

    m_contourManager.attributes(_cmAttributes);
}//blobContourSetup

//-------------------------

Detector::ReturnCode Detector::setBlobMat(const Mat & _image)
{
    ReturnCode result = checkImage(_image);

    if (result == RC_OK)
    {
       m_blobMat = _image.clone();
    }//if (result == RC_OK)

    return result;
}//setBlobMat

//-------------------------

/*!
 * \brief Detector::getPointedBlob : populate _pBlob attributes according to the blob at _point.
 * \warning Be sure that setBlobMat has been called before.
 * \param _cmAttributes Contours and bounding rects computed previously
 * \param _point Pixel pointed.
 * \param _pBlob Pointer to Blob to be filled.
 */
Detector::ReturnCode Detector::getPointedBlob(const ContourManager::Attributes & _cmAttributes, const Point & _point, BlobPtr & _pBlob)
{
    //Parameters must be set before
    if (!m_pParams)
    {
        deepDebug("Detector::getPointedBlob : parameters must be set before calling this.");
        return RC_BAD_PARAM;
    }//if (!m_pParams)

    deepDebug("POINT[%d,%d]",_point.x,_point.y);

    deepDebug("%d contour(s) retrieved.",_cmAttributes.contours.size());

    ContourVector::size_type contourSize = _cmAttributes.contours.size();
    ContourIndexList validIndexes;

    deepDebug("Retrieving contours which may contain the point...");

    deepDebug("Bounding rect(s) containing it:");

    //First pass: retrieving contours which may contain _point.
    for (ContourVector::size_type i = 0; i < contourSize; ++i)
    {
        if (_cmAttributes.boundings[i].contains(_point))
        {
            deepDebug("  %d: X[%d] Y[%d] W[%d] H[%d]",
                      i,
                      _cmAttributes.boundings[i].tl().x,
                      _cmAttributes.boundings[i].tl().y,
                      _cmAttributes.boundings[i].width,
                      _cmAttributes.boundings[i].height);
            validIndexes.append(i);
        }//if (boundingRects[i].contains(_point))
    }//for (ContourVector::size_type i = 0; i < contourSize; ++i)

    deepDebug("Retrieving contours which certainly contain the point...");

    deepDebug("Contour(s):");

    //Second pass : retrieving contours which certainly contain _point
    for (ContourIndexList::size_type i = 0; i < validIndexes.size();)
    {
        //_point is inside or on an edge?
        if (cv::pointPolygonTest(_cmAttributes.contours[validIndexes.at(i)],_point,false) >= 0.)
        {
            deepDebug("  %d contains it!",validIndexes.at(i));
            ++i;
        }//if (cv::pointPolygonTest(_cmAttributes.contours[validIndexes.at(i)],_point,false) >= 0.)
        else
        {
            validIndexes.removeAt(i);
        }//if (cv::pointPolygonTest(_cmAttributes.contours[validIndexes.at(i)],_point,false) >= 0.)...else
    }//for (ContourIndexList::size_type i = 0; i < validIndexes.size();)

    //If no contour left, exit.
    if (validIndexes.empty()) return RC_NO_RESULT;

    int validIndex;

    //If more than one contour
    if (validIndexes.size() > 1)
    {
        int childCount = -1;
        int bestChildCount = -1;

        int (*pBlobSelectFunc)(int,int) = 0;

        switch (m_bsbehavior)
        {
        case BSB_INNER:
            {
                pBlobSelectFunc = min;
                break;
            }//BSB_INNER
        case BSB_OUTTER:
            {
                pBlobSelectFunc = max;
                break;
            }//BSB_OUTTER
        default:
            {
                return RC_AMBIGUOUS;
            }//default
        }//switch (m_bsbehavior)

        /*! Searching best candidate according chosen behavior.*/
        bestChildCount = ContourManager::childCount(_cmAttributes.hierarchy,validIndexes.at(0));
        ContourIndexList::size_type chosenBlob = 0;

        deepDebug("Child count %d",bestChildCount);

        /* Choosing best blob according to selected behavior.*/
        for (ContourIndexList::size_type i = 1; i < validIndexes.size(); ++i)
        {
            childCount = ContourManager::childCount(_cmAttributes.hierarchy,validIndexes.at(i));

            deepDebug("Child count %d",childCount);

            bestChildCount = pBlobSelectFunc(childCount,bestChildCount);

            if (bestChildCount == childCount) chosenBlob = i;
        }//for (ContourIndexList::size_type i = 1; i < validIndexes.size(); ++i)

        deepDebug("Best child count %d",bestChildCount);

        validIndex = validIndexes.at(chosenBlob);

        deepDebug("Chosen contour %d",validIndex);
    }//if (validIndexes.size() > 1)
    else
    {
        //Only one contour
        validIndex = validIndexes.at(0);
    }//if (validIndexes.size() > 1)...else

    //Copying to avoid multiple indexations.
    Rect boundingRect = _cmAttributes.boundings[validIndex];
    Contour contour = _cmAttributes.contours[validIndex];

    deepDebug("BRECT TL [%d,%d] W %d H %d",boundingRect.tl().x,boundingRect.tl().y,boundingRect.width,boundingRect.height);

    //If blob is too big, it's not worth getting it since detector is made to detect subtitles.
    if (static_cast<float>(boundingRect.width) / static_cast<float>(m_blobMat.cols) > 0.20f
     || static_cast<float>(boundingRect.height) / static_cast<float>(m_blobMat.rows) > 0.20f)
    {
        return RC_INCONSISTENT;
    }//if (static_cast<float>(boundingRect.width) / static_cast<float>(m_blobMat.cols) > 0.20f...

    ContourVector children;
    ContourManager::children(_cmAttributes.contours,_cmAttributes.hierarchy,validIndex,children);

    _pBlob->update(m_blobMat,boundingRect,contour,children);

    return RC_OK;
}//getPointedBlob

//-------------------------

}//namespace SubDetection
