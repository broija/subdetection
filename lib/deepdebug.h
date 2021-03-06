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

#ifndef SUBDETECTION_DEEPDEBUG_H
#define SUBDETECTION_DEEPDEBUG_H

#if DEBUG_LEVEL >= 3
#define deepDebug3   qDebug
#define deepWarning3 qWarning
#else
#define deepDebug3(...)
#define deepWarning3(...)
#endif//DEBUG_LEVEL == 3

#if DEBUG_LEVEL >= 2
#define deepDebug2   qDebug
#define deepWarning2 qWarning
#else
#define deepDebug2(...)
#define deepWarning2(...)
#endif//DEBUG_LEVEL == 2

#if DEBUG_LEVEL >= 1
    #define deepDebug   qDebug
    #define deepWarning qWarning
#else
    #define deepDebug(...)
    #define deepWarning(...)
#endif//DEBUG_LEVEL == 1

#endif // DEEPDEBUG_H
