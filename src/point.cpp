////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/point.h"
#include "../include/MASkinG/rect.h"


MAS::Point::Point(int x, int y)
:posx(x),
   posy(y)
{
}


bool MAS::Point::operator==(const Point &p) const {
   return ((x() == p.x()) && (y() == p.y()));
}


bool MAS::Point::operator!=(const Point &p) const {
   return ((x() != p.x()) || (y() != p.y()));
}


bool MAS::Point::operator<(const Rect &r) const {
   return (x() > r.x() && x() < r.x2() && y() > r.y() && y() < r.y2());
}


bool MAS::Point::operator<=(const Rect &r) const {
   return (x() >= r.x() && x() <= r.x2() && y() >= r.y() && y() <= r.y2());
}
