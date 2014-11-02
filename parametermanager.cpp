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

#include "parametermanager.h"

#include <QSettings>
#include <QString>
#include <QFileInfo>

namespace SubDetection
{

namespace
{
const QString SETTING_GROUP_HSV = "hsv";
const QString SETTING_GROUP_MIN = "min";
const QString SETTING_GROUP_MAX = "max";
const QString SETTING_GROUP_ZONE = "zone";
const QString SETTING_GROUP_CHAR_MAX_SIZE = "char_max_size";
const QString SETTING_GROUP_EDGE_DETECTION = "edge_detection";
const QString SETTING_GROUP_TOLERANCE = "tolerance";
const QString SETTING_GROUP_SIMILARITY_DETECTION = "similarity_dectection";

const QString SETTING_HUE = "h";
const QString SETTING_SAT = "s";
const QString SETTING_VAL = "v";
const QString SETTING_X = "x";
const QString SETTING_Y = "y";
const QString SETTING_WIDTH = "width";
const QString SETTING_HEIGHT = "height";
const QString SETTING_THRESH = "thresh";
const QString SETTING_MATCH_RATIO = "match_ratio";

}//namespace

ParameterManager::ParameterManager():
    m_notify(true)
{
}//ParameterManager

//-------------------------

ParameterManager::ParameterManager(const Parameters & _params):
    m_notify(true),
    m_params(_params)
{
}//ParameterManager

//-------------------------

void ParameterManager::setParameters(const Parameters & _params)
{
    m_params = _params;

    changeNotification();
}//setParameters

//-------------------------

void ParameterManager::setHsvMinHue(int _hue)
{
    m_params.hsvMin.setHue(_hue);

    changeNotification();
}//setHsvMinHue

//-------------------------

void ParameterManager::setHsvMinSat(int _sat)
{
    m_params.hsvMin.setSaturation(_sat);

    changeNotification();
}//setHsvMinSat

//-------------------------

void ParameterManager::setHsvMinVal(int _val)
{
    m_params.hsvMin.setValue(_val);

    changeNotification();
}//setHsvMinVal

//-------------------------

void ParameterManager::setHsvMaxHue(int _hue)
{
    m_params.hsvMax.setHue(_hue);

    changeNotification();
}//setHsvMaxHue

//-------------------------

void ParameterManager::setHsvMaxSat(int _sat)
{
    m_params.hsvMax.setSaturation(_sat);

    changeNotification();
}//setHsvMaxSat

//-------------------------

void ParameterManager::setHsvMaxVal(int _val)
{
    m_params.hsvMax.setValue(_val);

    changeNotification();
}//setHsvMaxVal

//-------------------------

void ParameterManager::setZoneX(int _x)
{
    m_params.zone.x = _x;

    changeNotification();
}//setZoneX

//-------------------------

void ParameterManager::setZoneY(int _y)
{
    m_params.zone.y = _y;

    changeNotification();
}//setZoneY

//-------------------------

void ParameterManager::setZoneWidth(int _width)
{
    m_params.zone.width = _width;

    changeNotification();
}//setZoneWidth

//-------------------------

void ParameterManager::setZoneHeight(int _height)
{
    m_params.zone.height = _height;

    changeNotification();
}//setZoneHeight

//-------------------------

void ParameterManager::setCharMaxWidth(int _width)
{
    m_params.charMaxSize.width = _width;

    changeNotification();
}//setCharMaxWidth

//-------------------------

void ParameterManager::setCharMaxHeight(int _height)
{
    m_params.charMaxSize.height = _height;

    changeNotification();
}//setCharMaxHeight

//-------------------------

void ParameterManager::setThresh(int _thresh)
{
    m_params.thresh = _thresh;

    changeNotification();
}//setThresh

//-------------------------

void ParameterManager::setXTolerance(int _tolerance)
{
    m_params.xTolerance = _tolerance;

    changeNotification();
}//setXTolerance

//-------------------------

void ParameterManager::setYTolerance(int _tolerance)
{
    m_params.yTolerance = _tolerance;

    changeNotification();
}//setYTolerance

//-------------------------

void ParameterManager::setMatchRatio(double _ratio)
{
    m_params.matchRatio = _ratio;

    changeNotification();
}//setMatchRatio

//-------------------------

bool ParameterManager::loadSettings(const QString & _filename)
{
    QFileInfo fileInfo(_filename);

    if (!fileInfo.exists()) return false;

    QSettings settings(_filename,QSettings::IniFormat);

    m_notify = false;

    settings.beginGroup(SETTING_GROUP_HSV);
    {
      settings.beginGroup(SETTING_GROUP_MIN);
      {
          setHsvMinHue(settings.value(SETTING_HUE,0).toInt());
          setHsvMinSat(settings.value(SETTING_SAT,0).toInt());
          setHsvMinVal(settings.value(SETTING_VAL,0).toInt());
      }settings.endGroup();//min

      settings.beginGroup(SETTING_GROUP_MAX);
      {
          setHsvMaxHue(settings.value(SETTING_HUE,0).toInt());
          setHsvMaxSat(settings.value(SETTING_SAT,0).toInt());
          setHsvMaxVal(settings.value(SETTING_VAL,0).toInt());
      }settings.endGroup();//max
    }settings.endGroup();//HSV

    settings.beginGroup(SETTING_GROUP_ZONE);
    {
        setZoneX(settings.value(SETTING_X,0).toInt());
        setZoneY(settings.value(SETTING_Y,0).toInt());
        setZoneWidth(settings.value(SETTING_WIDTH,0).toInt());
        setZoneHeight(settings.value(SETTING_HEIGHT,0).toInt());
    }settings.endGroup();//Zone

    settings.beginGroup(SETTING_GROUP_CHAR_MAX_SIZE);
    {
        setCharMaxWidth(settings.value(SETTING_WIDTH,0).toInt());
        setCharMaxHeight(settings.value(SETTING_HEIGHT,0).toInt());
    }settings.endGroup();//Char max size

    settings.beginGroup(SETTING_GROUP_EDGE_DETECTION);
    {
        setThresh(settings.value(SETTING_THRESH,0).toInt());
    }settings.endGroup();//Edge detection

    settings.beginGroup(SETTING_GROUP_TOLERANCE);
    {
        setXTolerance(settings.value(SETTING_X,0).toInt());
        setYTolerance(settings.value(SETTING_Y,0).toInt());
    }settings.endGroup();//Tolerance

    settings.beginGroup(SETTING_GROUP_SIMILARITY_DETECTION);
    {
        setMatchRatio(settings.value(SETTING_MATCH_RATIO,0.05).toDouble());
    }settings.endGroup();//Similarity detection

    m_notify = true;
    changeNotification();

    return true;
}//loadSettings

//-------------------------

void ParameterManager::saveSettings(const QString & _filename)
{
    QSettings settings(_filename,QSettings::IniFormat);

    settings.beginGroup(SETTING_GROUP_HSV);
    {
      settings.beginGroup(SETTING_GROUP_MIN);
      {
          settings.setValue(SETTING_HUE,m_params.hsvMin.hue());
          settings.setValue(SETTING_SAT,m_params.hsvMin.saturation());
          settings.setValue(SETTING_VAL,m_params.hsvMin.value());
      }settings.endGroup();//min

      settings.beginGroup(SETTING_GROUP_MAX);
      {
          settings.setValue(SETTING_HUE,m_params.hsvMax.hue());
          settings.setValue(SETTING_SAT,m_params.hsvMax.saturation());
          settings.setValue(SETTING_VAL,m_params.hsvMax.value());
      }settings.endGroup();//max
    }settings.endGroup();//HSV

    settings.beginGroup(SETTING_GROUP_ZONE);
    {
        settings.setValue(SETTING_X,m_params.zone.x);
        settings.setValue(SETTING_Y,m_params.zone.y);
        settings.setValue(SETTING_WIDTH,m_params.zone.width);
        settings.setValue(SETTING_HEIGHT,m_params.zone.height);
    }settings.endGroup();//Zone

    settings.beginGroup(SETTING_GROUP_CHAR_MAX_SIZE);
    {
        settings.setValue(SETTING_WIDTH,m_params.charMaxSize.width);
        settings.setValue(SETTING_HEIGHT,m_params.charMaxSize.height);
    }settings.endGroup();//Char max size

    settings.beginGroup(SETTING_GROUP_EDGE_DETECTION);
    {
        settings.setValue(SETTING_THRESH,m_params.thresh);
    }settings.endGroup();//Edge detection

    settings.beginGroup(SETTING_GROUP_TOLERANCE);
    {
        settings.setValue(SETTING_X,m_params.xTolerance);
        settings.setValue(SETTING_Y,m_params.yTolerance);
    }settings.endGroup();//Tolerance

    settings.beginGroup(SETTING_GROUP_SIMILARITY_DETECTION);
    {
        settings.setValue(SETTING_MATCH_RATIO,m_params.matchRatio);
    }settings.endGroup();//Similarity detection

}//saveSettings

//-------------------------
#if SUBDETECTION_NOTIFY_PARAM_CHANGE
void ParameterManager::changeNotification() const
{
    if (m_notify) emit parameterChanged(m_params);
}//changeNotification
#endif//SUBDETECTION_NOTIFY_PARAM_CHANGE
//-------------------------

}//namespace SubDetection
