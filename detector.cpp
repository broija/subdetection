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

#include "detector.h"

#include <QMap>
#include <QHash>
#include <QImage>
#include <QStringList>

#include <opencv2/imgproc/imgproc.hpp>

#include "deepdebug.h"

#define DETECT_EDGE_CANNY 0
#define DETECT_EDGE_BLUR 0
#define APPROX_CURVES 0

#define TEST_DRAW 1
#define TEST_DRAW_RESULT 1

#define TEST_CENTERED 0

#if TEST_DRAW
#include <opencv2/highgui/highgui.hpp>
#endif//TEST_DRAW

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
    Point::value_type UNDEFINED_COORD = -1;

    typedef QMap<Point::value_type,quint32> CoordCountMap;
    typedef QHash<Point,Rect> BoundingRectHash;
//------
#ifdef DEEP_DEBUG
    inline void printHsv(const char * _name, const Hsv & _hsv)
#else
    inline void printHsv(const char * , const Hsv & )
#endif//DEEP_DEBUG
    {
        deepDebug("HSV[%s]: %d %d %d",_name,_hsv.hue(),_hsv.saturation(),_hsv.value());
    }//printHsv
//------
#ifdef DEEP_DEBUG
    inline void printParameters(const char * _name, const Parameters & _params)
#else
    inline void printParameters(const char * , const Parameters & _params)
#endif//DEEP_DEBUG
    {
        deepDebug("Detection parameters[%s]",_name);
        printHsv("min",_params.hsvMin);
        printHsv("max",_params.hsvMax);
    }//printParameters

}//namespace

Detector::Detector():
    m_pParams(new Parameters()),
    m_drawBoundings(false),
    m_ocr(".", "eng")
{
    forget();
}//Detector

//-------------------------

Detector::Detector(Parameters *_pParams):
    m_drawBoundings(false),
    m_ocr(".", "eng")
{
    setParameters(_pParams);
}//Detector Parameters *

//-------------------------

Detector::Detector(const Parameters & _params):
    m_pParams(new Parameters()),
    m_drawBoundings(false),
    m_ocr(".", "eng")
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
Detector::Detector(Parameters * _pParams, const QString & _tessdataParentPath, const QString & _lang):
    m_drawBoundings(false),
    m_ocr(_tessdataParentPath,_lang)
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
    m_ocr(_tessdataParentPath,_lang)
{
}//Detector const QString &, const QString &

//-------------------------

void Detector::setParameters(Parameters *_pParams)
{
    if (_pParams)
        m_pParams.reset(_pParams);
    else
        m_pParams.reset(new Parameters);

    forget();
}//setParameters

//-------------------------

void Detector::setParameters(const Parameters & _params)
{
    printParameters("passed",_params);
    printParameters("old",*m_pParams);

    *m_pParams = _params;

    printParameters("new",*m_pParams);

    forget();
}//setParameters

//-------------------------

/*!
 * \brief Detector::forget : if "detect" was precedently called, "forget" allows to forget previous detection, in case
 *        text has not changed. When your Detector Parameters points directly to an outside structure,
 *        thus the object is not notified for parameter changes, you can call "forget" to act as if the image that
 *        is analyzed were the first one.
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
 * \param _subtitles detected text. If text has not changed, or Detector thinks so, _subtitles won't be modified.
 */
void Detector::detect(const Mat & _image, QStringList & _subtitles)
{
    deepDebug("rows:%d, cols:%d",_image.rows,_image.cols);

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
        return;
    }//if (!m_pParams->zone.width...

    Q_ASSERT_X(_image.rows && _image.cols,"Detector::detect","Invalid row or col number for input Mat.");

//    m_centered = _centered;
    m_matOriginal = _image;

//----HSV masking
    cv::cvtColor(m_matOriginal,m_matHSV,cv::COLOR_BGR2HSV);//HSV conversion

    //Change desired colors into white
    cv::inRange(m_matHSV, m_pParams->hsvMin.toScalar(), m_pParams->hsvMax.toScalar(), m_matThresh);

    m_matTextZone = m_matThresh(m_pParams->zone);

    //Detecting if text has changed
    if (!m_forget && compareImages(m_matOldTextZone,m_matTextZone))
    {
        deepDebug("Text has not changed!");
        return;
    }//if (compareImages(m_matOldTextZone,m_matTextZone))

    if (m_forget) m_forget = false;

    m_matOldTextZone = m_matTextZone.clone();

    //Get only desired colors from the original image
    m_matMasked = Mat::zeros(m_matOriginal.size(),m_matOriginal.type());
    cv::bitwise_and(m_matOriginal,m_matOriginal,m_matMasked,m_matThresh);

    //Grayscale before edge detection
    cv::cvtColor(m_matMasked,m_matGray,CV_BGR2GRAY);
#if DETECT_EDGE_BLUR
    cv::blur(m_matGray,m_matGray,Size(3,3));
#endif//DETECT_EDGE_BLUR

//----Detecting edges
#if DETECT_EDGE_CANNY
    cv::Canny(imgGray, imgOutput, edge_thresh, edge_thresh * ratio, kernel_size);
#else
    cv::threshold(m_matGray, m_matContours, m_pParams->thresh, 255, cv::THRESH_BINARY);
#endif

    std::vector<Contour> contours;
    Hierarchy hierarchy;

//----Extracting contours
    cv::findContours(m_matContours, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    m_boundingRects.clear();
    getTextBoundingRects(contours,m_boundingRects);

    m_ocr.setImage(m_matThresh);

    for (RectVector::size_type i = 0; i < m_boundingRects.size(); ++i)
    {
        m_ocr.setRectangle(m_boundingRects[i]);
        _subtitles.push_back(m_ocr.getUtf8Text());
    }//for (RectVector::size_type i = 0; i < m_boundingRects.size(); ++i)
}//detect

//-------------------------

/*!
 * \brief getTextBoundingRects. Compute bounding rects of the given contours. Each rect contains a maximum of aligned contours.
 * \param _contours
 * \param _rects The computed rects.
 */
void Detector::getTextBoundingRects(const ContourVector & _contours, RectVector & _rects)
{
    ContourVector::size_type contourSize = _contours.size();

    //Moments
    std::vector<cv::Moments> moments(contourSize);
    for (ContourVector::size_type i = 0; i < contourSize; ++i)
    {
        moments[i] = cv::moments(_contours[i], false);
    }//for(ContourVector::size_type i = 0; i < contourSize; ++i)

    //Mass centers
    std::vector<MassCenter> massCenters(contourSize);
    for (ContourVector::size_type i = 0; i < contourSize; ++i)
    {
        massCenters[i] = MassCenter(lround(moments[i].m10/moments[i].m00),
                                    lround(moments[i].m01/moments[i].m00));
    }//for (ContourVector::size_type i = 0; i < contourSize; ++i)

    //Approximate contour curves
    std::vector<ContourCurve> contourCurves(contourSize);
    std::vector<cv::Rect> boundingRects(contourSize);

    //Filtering mass centers:
    for(ContourVector::size_type i = 0; i < contourSize; ++i)
    {
#if APPROX_CURVES
        cv::approxPolyDP(cv::Mat(_contours[i]), contourCurves[i], 3, true);
        boundingRects[i] = cv::boundingRect(cv::Mat(contourCurves[i]));
#else
        boundingRects[i] = cv::boundingRect(cv::Mat(_contours[i]));
#endif//APPROX_CURVES
    }//for(ContourVector::size_type i = 0; i < contourSize; ++i)

#if TEST_DRAW
    //Drawing
    //Drawing vars
    cv::Mat drawing;
    cv::Scalar color;

    Size originalSize(m_matOriginal.cols,m_matOriginal.rows);

    bool draw = (originalSize.width && originalSize.height);

    if (draw) drawing = cv::Mat::zeros(originalSize, CV_8UC3);
#endif//TEST_DRAW

    std::vector<MassCenter> validMassCenters;
    BoundingRectHash massCentersBoundingRects;

    cv::Size roiSize = m_pParams->zone.size();
    if (roiSize.width && roiSize.height)
    {
#if TEST_DRAW
        cv::rectangle(drawing,m_pParams->zone.tl(),m_pParams->zone.br(),cv::Scalar(255,0,0),2,8,0);
#endif//TEST_DRAW
        for (ContourVector::size_type i = 0; i< contourSize; ++i)
        {
            //Object is in region of interest
            if (m_pParams->zone.contains(massCenters[i]))
            {
#if TEST_DRAW
                if (draw) color = cv::Scalar(0,0,255);
#endif//TEST_DRAW
                cv::Size rectSize = boundingRects[i].size();

                //Object not too big?
                if (rectSize.width <= m_pParams->charMaxSize.width && rectSize.height <= m_pParams->charMaxSize.height)
                {
#if TEST_DRAW
                    if (draw) cv::rectangle(drawing, boundingRects[i].tl(), boundingRects[i].br(), color, 2, 8, 0);//contour bounding rect
                    if (draw) cv::circle(drawing, massCenters[i], 4, color, -1, 8, 0);//mass center
#endif//TEST_DRAW
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

                deepDebug("Point list:");
                //Building a count map for each different Y coordinate
                for (std::vector<MassCenter>::iterator it = validMassCenters.begin(); it != validMassCenters.end(); ++it)
                {
                    Point::value_type y = (*it).y;

                    deepDebug("%d:%d",(*it).x,(*it).y);

                    int alignmentCount = alignmentCountMap.value(y,0) +1;
                    alignmentCountMap.insert(y,alignmentCount);
                }//for (std::vector<MassCenter>::iterator it = validMassCenters.begin(); it != validMassCenters.end(); ++it)

                deepDebug("Map:");
                for (CoordCountMap::const_iterator i = alignmentCountMap.begin(); i != alignmentCountMap.end(); ++i)
                {
                    deepDebug("[%d,%d]",i.key(),*i);
                }//for (CoordCountMap::const_iterator i = alignmentCountMap.begin(); i != alignmentCountMap.end(); ++i)

                int maxCount = 0;

                CoordCountMap::const_iterator firstItem;
                CoordCountMap::const_iterator lastItem;

                bool stop;

                deepDebug("Searching best alignment...");
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

                    deepDebug("Count (i): %d",count);
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
                            deepDebug("Count (j): %d",count);
                            if (count > maxCount)
                            {
                                maxCount = count;

                                firstItem = i;
                                lastItem = j;
                            }//if (count > maxCount)
                        }//if ((j.key() - refY) > m_pParams->yTolerance)...else
                    }//for (CoordCountMap::const_iterator j = i+1; j != alignmentCountMap.end() && !stop; ++j)
                }//for (CoordCountMap::const_iterator i = alignmentCountMap.begin(); i != alignmentCountMap.end(); ++i)

                deepDebug("First item [%d,%d]",firstItem.key(),*firstItem);
                deepDebug("Last item [%d,%d]",lastItem.key(),*lastItem);

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

                    deepDebug("Building result:");
                    //Building result
                    for (PointVector::iterator it = validMassCenters.begin(); it != validMassCenters.end() && !stop;)
                    {
                        deepDebug("%d:%d",(*it).x,(*it).y);

                        validPoint = false;
                        currentX = (*it).x;//Saving X coordinate for spacing check

                        //Checking if point is in alignment range
                        if ((*it).y >= firstItem.key() && (*it).y <= lastItem.key())
                        {
                            deepDebug("Point is in range {%d,%d}",firstItem.key(),lastItem.key());

                            //First point of this group?
                            if (lastX == UNDEFINED_COORD)
                            {
                                deepDebug("First");
                                validPoint = true;
                            }//if (lastX == UNDEFINED_COORD)
                            else
                            {
                                deepDebug("Current[%d] Last[%d]",currentX,lastX);
                                //Is spacing correct?
                                if ((currentX - lastX) <= m_pParams->xTolerance)
                                {
                                    deepDebug("Correct spacing");
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
#if TEST_CENTERED
                    if (validRect && m_centered)
                    {
                        double centeringRatio = static_cast<double>(m_pParams->zone.br().x - maxX) / static_cast<double>(minX - m_pParams->zone.tl().x);
                        deepDebug("Centering ratio: %lf",centeringRatio);
                        validRect = (centeringRatio > 0.8 && centeringRatio < 1.2);//20%

                        if (!validRect) deepDebug("Text is not centered.");
                    }//if (validRect && m_centered)
#endif
                    if (!validRect)
                    {
                        deepDebug("Invalid rect: count[%d] w[%d] h[%d]",maxCount,rectWidth,rectHeight);
                    }//if (!validRect)
                    else
                    {
                        deepDebug("Valid rect: count[%d] w[%d] h[%d]",maxCount,rectWidth,rectHeight);
                        _rects.push_back(Rect(minX,minY, rectWidth, rectHeight));
                    }//if (!validRect)...else
                }//if (maxCount == 1)...else
             } while (!validMassCenters.empty());

            //Sorting rects from top to bottom of the image.
            if (!_rects.empty()) std::sort(_rects.begin(),_rects.end(),&rectCompareAlongYAxis);
        }//if (!validMassCenters.empty())
#if TEST_DRAW_RESULT
        for (RectVector::size_type i = 0; i < _rects.size(); ++i)
        {
            cv::rectangle(drawing,_rects[i],cv::Scalar(0,255,0),1,4);
        }//for (RectVector::size_type i = 0; i < _rects.size(); ++i)

        cv::namedWindow("Contours", CV_WINDOW_NORMAL);
        cv::imshow("Contours", drawing);
#endif//TEST_DRAW_RESULT
    }//if (roiSize.width && roiSize.height)
}//getTextBoundingRects

//------------------------------

/*!
 * \brief Detector::compareImages
 * \param _first
 * \param _second
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

        deepDebug("Distance: %lf, Match value: %lf",distance,matchValue);

        result = (matchValue < m_pParams->matchRatio);
    }//if (_first.rows != _second.rows...else

    return result;
}//compareImages

//------------------------------

}//namespace SubDetection

uint qHash(const SubDetection::Point & _point)
{
    return qHash(_point.x + _point.y);
}//qHash
