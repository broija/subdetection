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

#ifndef SUBDETECTION_DETECTOR_H
#define SUBDETECTION_DETECTOR_H

#include "memory"

#include "subdetection_global.h"
#include "types.h"
#include "parameters.h"
#include "opticalcharrecognizer.h"

class QImage;

namespace SubDetection
{

/*!
 * \brief The Detector class. Subtitle detection class based on Tesseract-OCR.
 */
class SUBDETECTIONSHARED_EXPORT Detector
{
public:
    Detector();
    Detector(Parameters * _pParams);
    Detector(const Parameters & _params);
    Detector(Parameters * _pParams, const QString & _tessdataParentPath, const QString & _lang);

    void setParameters(Parameters * _pParams);///< Detector parameters will point directly to passed parameters.
    void setParameters(const Parameters & _params);

    void forget();

    void detect(const Mat & _image, QStringList & _subtitles);

    /// After a call to "detect", returns the HSV representation of the original Mat.
    const Mat & hsvMat() const {return m_matHSV;}
    /// After a call to "detect", returns the thresholded representation of the original Mat regarding HSV parameters.
    const Mat & thresholdedMat() const {return m_matThresh;}
    /// After a call to "detect", returns only the desired colors in the original Mat regarding HSV parameters.
    const Mat & maskedMat() const {return m_matMasked;}
    /// After a call to "detect", returns the thresholded text zone of the orignal Mat regarding HSV parameters.
    const Mat & textZoneMat() const {return m_matTextZone;}
    /// After a call to "detect", returns the contours of the orignal Mat regarding HSV parameters.
    const Mat & contoursMat() const {return m_matContours;}

protected:
    void getTextBoundingRects(const ContourVector & _contours, RectVector & _rects);
    bool compareImages(const Mat & _first, const Mat & _second) const;///< Check whether images are similar.

    std::auto_ptr<Parameters> m_pParams;

    Mat m_matOriginal;
    Mat m_matHSV;
    Mat m_matThresh;
    Mat m_matTextZone;
    Mat m_matOldTextZone;
    Mat m_matMasked;
    Mat m_matGray;
    Mat m_matContours;

    RectVector m_boundingRects;

    bool m_drawBoundings;

    OpticalCharRecognizer m_ocr;

    bool m_centered;
    bool m_forget;
};//SubDetector

}//namespace SubDetection

//Needed for internat use
uint qHash(const SubDetection::Point & _point);

#endif // SUBDETECTION_DETECTOR_H
