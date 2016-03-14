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

#include <QSharedPointer>

#include "subdetection_global.h"
#include "types.h"
#include "parameters.h"
#include "opticalcharrecognizer.h"
#include "contourmanager.h"

class QImage;

namespace SubDetection
{

class Blob;

/*!
 * \brief The Detector class. Subtitle detection class based on Tesseract-OCR.
 */
class SUBDETECTIONSHARED_EXPORT Detector
{
public:
    typedef QSharedPointer<Blob> BlobPtr;

    enum ReturnCode
    {
        RC_OK,///< Success.
        RC_AMBIGUOUS,///< Result is ambiguous and cannot be returned.
        RC_BAD_PARAM,///< At least one of the input parameters was invalid. If it's an input image, RC_INVALID_INPUT_IMAGE will be returned instead.
        RC_NO_RESULT,///< No result.
        RC_INVALID_INPUT_IMAGE,///< Input image is invalid. Typically with zero width or height.
        RC_NO_CHANGE,///< Result has not changed since last call.
        RC_INCONSISTENT///< Result found seems inconsistent.
    };//Result

    /// Blob selection behavior when there is a choice
    enum BlobSelectionBehavior
    {
        BSB_NONE,///< No decision made
        BSB_OUTTER,///< Always choose outter blob
        BSB_INNER///< Always choose inner blob (default)
    };//BlobSelectionBehavior

    Detector();
    Detector(const QSharedPointer<Parameters> & _pParams);
    Detector(const Parameters & _params);
    Detector(const QSharedPointer<Parameters> &  _pParams, const QString & _tessdataParentPath, const QString & _lang);
    Detector(const QString & _tessdataParentPath, const QString & _lang);
    ~Detector();

    void setParameters(const QSharedPointer<Parameters> &  _pParams);
    void setParameters(const Parameters & _params);

    void setBlobSelectionBehavior(BlobSelectionBehavior _behavior);
    void enableBoundingsDrawing(bool _enabled);

    void forget();

    ReturnCode detect(const Mat & _image, QStringList & _subtitles);

    ReturnCode getPointedBlob(const Mat & _image, const Point & _point, BlobPtr & _pBlob);

//    ReturnCode getSelectionParameters(const Rect & _roi, Parameters & _params);

    /// After a call to "detect", returns the HSV representation of the original Mat.
    const Mat & hsvMat() const {return m_hsvMat;}
    /// After a call to "detect", returns the thresholded representation of the original Mat regarding HSV parameters.
    const Mat & thresholdedMat() const {return m_threshMat;}
    /// After a call to "detect", returns only the desired colors in the original Mat regarding HSV parameters.
    const Mat & maskedMat() const {return m_maskedMat;}
    /// After a call to "detect", returns the thresholded text zone of the orignal Mat regarding HSV parameters.
    const Mat & textZoneMat() const {return m_textZoneMat;}
    /// After a call to "detect", returns an image with contours of the orignal Mat regarding HSV parameters.
    const Mat & contoursMat() const {return m_contourMat;}
    /// After a call to "detect", returns an image containing text zone and blob boundings.
    const Mat & boundingsMat() const {return m_boundingsMat;}

protected:
    void createParameters();

    ReturnCode checkImage(const Mat & _image) const;
    bool compareImages(const Mat & _first, const Mat & _second) const;

    void getTextBoundingRects(const ContourVector & _contours, RectVector & _rects);

    void blobContourSetup(ContourManager::Attributes & _cmAttributes);

    ReturnCode setBlobMat(const Mat & _image);
    ReturnCode getPointedBlob(const ContourManager::Attributes & _cmAttributes, const Point & _point, BlobPtr & _pBlob);

    QSharedPointer<Parameters> m_pParams;

    Mat m_originalMat;
    Mat m_hsvMat;
    Mat m_threshMat;
    Mat m_textZoneMat;
    Mat m_oldTextZoneMat;
    Mat m_maskedMat;
    Mat m_grayMat;
    Mat m_contourMat;
    Mat m_boundingsMat;

    Mat m_blobMat;

    RectVector m_boundingRects;

    bool m_drawBoundings;

    OpticalCharRecognizer m_ocr;

    ContourManager m_contourManager;

    bool m_centered;
    bool m_forget;

    BlobSelectionBehavior m_bsbehavior;
};//SubDetector

}//namespace SubDetection

#endif // SUBDETECTION_DETECTOR_H
