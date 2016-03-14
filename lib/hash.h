#ifndef SUBDETECTION_HASH_H
#define SUBDETECTION_HASH_H

#include <QtGlobal>

class Point;

namespace cv
{

//Needed for internal use
uint qHash(const Point & _point);

}//cv

#endif // SUBDETECTION_HASH_H
