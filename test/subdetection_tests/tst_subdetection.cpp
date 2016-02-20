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

#include "tst_subdetection.h"

#include <QString>

#include "hsv.h"
#include "statistical_tools.h"

//using namespace SubDetectionTest;
namespace
{
typedef SubDetection::Hsv::Hue Hue;
typedef SubDetection::Hsv::Saturation Saturation;
typedef SubDetection::Hsv::Value Value;
typedef SubDetection::HsvList HsvList;
}//

SubDetectionTest::SubDetectionTest()
{
}//SubDetectionTest

//-------------------------

void SubDetectionTest::hsvDefaultConstructor()
{
    Hsv result;

    QCOMPARE(result.hue(),0);
    QCOMPARE(result.saturation(),0);
    QCOMPARE(result.value(),0);
}//hsvDefaultConstructor

//-------------------------

void SubDetectionTest::hsvConstructor_data()
{
    QTest::addColumn<Hue>("hue");
    QTest::addColumn<Hue>("exp_hue");
    QTest::addColumn<Saturation>("sat");
    QTest::addColumn<Saturation>("exp_sat");
    QTest::addColumn<Value>("val");
    QTest::addColumn<Value>("exp_val");

    QTest::newRow("set 1")
            << Hue(0)
            << Hue(0)
            << Saturation(0)
            << Saturation(0)
            << Value(0)
            << Value(0);

    QTest::newRow("set 2")
            << Hue(-1)
            << Hue(0)
            << Saturation(-1)
            << Saturation(0)
            << Value(-1)
            << Value(0);

    QTest::newRow("set 3")
            << Hue(500)
            << Hue(179)
            << Saturation(500)
            << Saturation(255)
            << Value(500)
            << Value(255);
}//hsvConstructor_data

//-------------------------

void SubDetectionTest::hsvConstructor()
{
    QFETCH(Hue,hue);
    QFETCH(Hue,exp_hue);
    QFETCH(Saturation,sat);
    QFETCH(Saturation,exp_sat);
    QFETCH(Value,val);
    QFETCH(Value,exp_val);

    Hsv result(hue,sat,val);

    QCOMPARE(result.hue(),exp_hue);
    QCOMPARE(result.saturation(),exp_sat);
    QCOMPARE(result.value(),exp_val);
}//hsvConstructor

//-------------------------

void SubDetectionTest::averageHsv_data()
{
    QTest::addColumn<Hue>("hue1");
    QTest::addColumn<Hue>("hue2");

    QTest::addColumn<Saturation>("sat1");
    QTest::addColumn<Saturation>("sat2");

    QTest::addColumn<Value>("val1");
    QTest::addColumn<Value>("val2");

    QTest::addColumn<Hsv>("expected");

    QTest::newRow("set 1")
            << Hue(15)
            << Hue(10)
            << Saturation(30)
            << Saturation(50)
            << Value(100)
            << Value(200)
            << Hsv(12,40,150);

    QTest::newRow("set 2")
            << Hue(-5)
            << Hue(10)
            << Saturation(500)
            << Saturation(50)
            << Value(100)
            << Value(-4)
            << Hsv(5,152,50);

    QTest::newRow("set 3")
            << Hue(800)
            << Hue(-5)
            << Saturation(-5)
            << Saturation(-500)
            << Value(452)
            << Value(-4)
            << Hsv(89,0,127);
}//averageHsv_data

//-------------------------

void SubDetectionTest::averageHsv()
{
    QFETCH(Hue,hue1);
    QFETCH(Saturation,sat1);
    QFETCH(Value,val1);

    QFETCH(Hue,hue2);
    QFETCH(Saturation,sat2);
    QFETCH(Value,val2);

    QFETCH(Hsv,expected);

    Hsv hsv1(hue1,sat1,val1);
    Hsv hsv2(hue2,sat2,val2);

    Hsv result = average(hsv1,hsv2);

    QCOMPARE(result,expected);
}//averageHsv

//-------------------------

void SubDetectionTest::averageHsvList_data()
{
    QTest::addColumn<HsvList>("list");
    QTest::addColumn<Hsv>("expected");

    HsvList list;
    list << Hsv(1,0,0)
         << Hsv(0,1,0)
         << Hsv(0,0,1);

    QTest::newRow("set 1")
            << list
            << Hsv(0,0,0);
//++++++++++++++++++++++++
    list.clear();
    list << Hsv(1,0,4)
         << Hsv(2,1,0)
         << Hsv(0,3,1);

    QTest::newRow("set 2")
            << list
            << Hsv(1,1,1);
//++++++++++++++++++++++++
    list.clear();
    list << Hsv(1,-1,4)
         << Hsv(2,1,-5)
         << Hsv(-1,3,1);

    QTest::newRow("set 3")
            << list
            << Hsv(1,1,1);
//++++++++++++++++++++++++
    list.clear();
    list << Hsv(-1,0,-4)
         << Hsv(-2,-1,-5)
         << Hsv(0,-3,1);

    QTest::newRow("set 4")
            << list
            << Hsv(0,0,0);
//++++++++++++++++++++++++
    list.clear();
    list << Hsv(200,255,255)
         << Hsv(200,255,255)
         << Hsv(200,255,255);

    QTest::newRow("set 5")
            << list
            << Hsv(179,255,255);
//++++++++++++++++++++++++
}//averageHsvList_data

//-------------------------

void SubDetectionTest::averageHsvList()
{
    QFETCH(HsvList,list);
    QFETCH(Hsv,expected);

    Hsv result = SubDetection::average(list);
    QCOMPARE(result,expected);
}//averageHsvList

//-------------------------

void SubDetectionTest::medianHsvList_data()
{
    QTest::addColumn<HsvList>("list");
    QTest::addColumn<Hsv>("expected");

    HsvList list;

    QTest::newRow("set 0")
            << list
            << Hsv();
//++++++++++++++++++++++++
    list.clear();
    list << Hsv(1,0,0)
         << Hsv(0,1,0)
         << Hsv(0,0,1);

    QTest::newRow("set 1")
            << list
            << Hsv(0,1,0);
//++++++++++++++++++++++++
    list.clear();
    list << Hsv(1,0,4)
         << Hsv(2,1,0)
         << Hsv(0,3,1);

    QTest::newRow("set 2")
            << list
            << Hsv(1,0,4);
//++++++++++++++++++++++++
    list.clear();
    list << Hsv(1,-1,4)
         << Hsv(2,1,-5)
         << Hsv(-1,3,1);

    QTest::newRow("set 3")
            << list
            << Hsv(1,0,4);
//++++++++++++++++++++++++
    list.clear();
    list << Hsv(-1,0,-4)
         << Hsv(-2,-1,-5)
         << Hsv(0,-3,1);

    QTest::newRow("set 4")
            << list
            << Hsv(0,0,0);
//++++++++++++++++++++++++
    list.clear();
    list << Hsv(200,255,255)
         << Hsv(200,255,255)
         << Hsv(200,255,255);

    QTest::newRow("set 5")
            << list
            << Hsv(179,255,255);
//++++++++++++++++++++++++
    list.clear();
    list << Hsv(200,255,255)
         << Hsv(200,255,255)
         << Hsv(200,255,255)
         << Hsv(200,255,255);

    QTest::newRow("set 6")
            << list
            << Hsv(179,255,255);
//++++++++++++++++++++++++
    list.clear();
    list << Hsv(-1,0,-4)
         << Hsv(-2,-1,-5)
         << Hsv(0,-3,1)
         << Hsv(-2,4,-5);

    QTest::newRow("set 7")
            << list
            << Hsv(0,0,0);
//++++++++++++++++++++++++
    list.clear();
    list << Hsv(1,0,4)
         << Hsv(2,1,5)
         << Hsv(0,3,1)
         << Hsv(2,4,5);

    QTest::newRow("set 8")
            << list
            << Hsv(1,0,4);
//++++++++++++++++++++++++
    list.clear();
    list << Hsv(1,0,4)
         << Hsv(2,1,5)
         << Hsv(0,3,1)
         << Hsv(2,4,5)
         << Hsv(0,3,1);

    QTest::newRow("set 9")
            << list
            << Hsv(1,0,4);
//++++++++++++++++++++++++
    list.clear();
    list << Hsv(1,0,4)
         << Hsv(2,1,5)
         << Hsv(0,3,1)
         << Hsv(2,4,5)
         << Hsv(0,3,1)
         << Hsv(8,0,7);

    QTest::newRow("set 10")
            << list
            << Hsv(1,0,4);
//++++++++++++++++++++++++
    list.clear();
    list << Hsv(5,0,4)
         << Hsv(2,1,5)
         << Hsv(0,3,1)
         << Hsv(2,4,5)
         << Hsv(0,3,1)
         << Hsv(8,0,7);

    QTest::newRow("set 11")
            << list
            << Hsv(2,2,5);
//++++++++++++++++++++++++
    list.clear();
    list << Hsv(5,0,4);

    QTest::newRow("set 12")
            << list
            << Hsv(5,0,4);
//++++++++++++++++++++++++
}//medianHsvList_data

//-------------------------

void SubDetectionTest::medianHsvList()
{
    QFETCH(HsvList,list);
    QFETCH(Hsv,expected);

    Hsv result = SubDetection::median(list);

//    qDebug("RESULT %s",qPrintable(result.toString()));
    QCOMPARE(result,expected);
}//medianHsvList

//-------------------------
/*
void SubDetectionTest::cleanupTestCase()
{
}
*/
//#include "tst_subdetectionstatistical.moc"
//#include "debug/moc_tst_subdetectionstatistical.cpp"

QTEST_APPLESS_MAIN(SubDetectionTest)
