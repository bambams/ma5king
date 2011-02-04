////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/rect.h"


MAS::Rect::Rect()
:origin(0, 0),
   dim(0, 0)
{
}


MAS::Rect::Rect(int x, int y, int w, int h)
   :origin(x, y),
   dim(w, h)
{
}


MAS::Rect::Rect(const Point &p, const Size &s)
   :origin(p),
   dim(s)
{
}


MAS::Rect::~Rect() {
}


bool MAS::Rect::operator==(const Rect &r) const {
   return ((topLeft() == r.topLeft()) && (size() == r.size()));
}


bool MAS::Rect::operator>(const Rect &r) const {
   return (r.topLeft() < (*this) && r.bottomRight() < (*this));
}


bool MAS::Rect::operator>=(const Rect &r) const {
   return (r.topLeft() <= (*this) && r.bottomRight() <= (*this));
}


bool MAS::Rect::operator>(const Point &p) const {
   return (p < (*this));
}


bool MAS::Rect::operator>=(const Point &p) const {
   return (p <= (*this));
}


bool MAS::Rect::Intersects(const Rect &r) const {
   if (x2() <= r.x()) return false; else
   if (x() >= r.x2()) return false; else
   if (y2() <= r.y()) return false; else
   if (y() >= r.y2()) return false; else
   return true;
}
