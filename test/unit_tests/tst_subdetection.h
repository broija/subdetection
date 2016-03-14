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
#ifndef TST_SUBDETECTIONSTATISTICAL_H
#define TST_SUBDETECTIONSTATISTICAL_H

#include <QtTest>

#include "hsv.h"
#include "hsvlist.h"

typedef SubDetection::Hsv Hsv;
typedef SubDetection::HsvList HsvList;

class SubDetectionTest : public QObject
{
    Q_OBJECT

public:
    SubDetectionTest();

private Q_SLOTS:
    void hsvDefaultConstructor();

    void hsvConstructor_data();
    void hsvConstructor();

    void averageHsv_data();
    void averageHsv();

    void averageHsvList_data();
    void averageHsvList();

    void medianHsvList_data();
    void medianHsvList();

//    void cleanupTestCase();
};//SubDetectionTest

Q_DECLARE_METATYPE(Hsv)
Q_DECLARE_METATYPE(HsvList)

#endif // TST_SUBDETECTIONSTATISTICAL_H
