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

#ifndef SUBDETECTION_PARAMETERMANAGER_H
#define SUBDETECTION_PARAMETERMANAGER_H

#include <QObject>

#include "parameters.h"

#define SUBDETECTION_NOTIFY_PARAM_CHANGE 1

namespace SubDetection
{

/// Used to store a Parameters structure, save/load its values in/from a file.
class SUBDETECTIONSHARED_EXPORT ParameterManager : public QObject
{
    Q_OBJECT

public:
    ParameterManager();
    ParameterManager(const Parameters & _params);

    Parameters & parameters() {return m_params;}///< Returns a reference to the Parameters structure.
    const Parameters & constParameters() const {return m_params;}///< Returns a const reference to the Parameters structure.

public slots:
    void setParameters(const Parameters & _params);

    //HSV Min
    void setHsvMinHue(int _hue);
    void setHsvMinSat(int _sat);
    void setHsvMinVal(int _val);

    //HSV Max
    void setHsvMaxHue(int _hue);
    void setHsvMaxSat(int _sat);
    void setHsvMaxVal(int _val);

    //Detection zone
    void setZoneX(int _x);
    void setZoneY(int _y);
    void setZoneWidth(int _width);
    void setZoneHeight(int _height);

    //Char max size
    void setCharMaxWidth(int _width);
    void setCharMaxHeight(int _height);

    //Edge detection thresh
    void setThresh(int _thresh);

    //Tolerance
    void setXTolerance(int _tolerance);
    void setYTolerance(int _tolerance);

    //Match ratio
    void setMatchRatio(double _ratio);

    //Settings
    bool loadSettings(const QString & _filename);
    void saveSettings(const QString & _filename);

#if SUBDETECTION_NOTIFY_PARAM_CHANGE
signals:
    void parameterChanged(const SubDetection::Parameters & _params) const;
#endif//SUBDETECTION_NOTIFY_PARAM_CHANGE
protected:
#if  SUBDETECTION_NOTIFY_PARAM_CHANGE
    void changeNotification() const;

    bool m_notify;
#else
    void changeNotification() const {}
#endif//SUBDETECTION_NOTIFY_PARAM_CHANGE

    Parameters m_params;
private:
    Q_DISABLE_COPY(ParameterManager)
};//ParameterManager

}//namespace SubDetection

#endif // SUBDETECTION_PARAMETERMANAGER_H
