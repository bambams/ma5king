////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/scrollobj.h"


MAS::ScrollObject::ScrollObject()
:min(0.0),
   max(100.0),
   pos(0.0),
   inc(1.0)
{
}


MAS::ScrollObject::~ScrollObject() {
}


void MAS::ScrollObject::SetMin(double min) {
   this->min = min;
}


void MAS::ScrollObject::SetMax(double max) {
   this->max = max;
}


void MAS::ScrollObject::SetRange(double min, double max) {
   SetMin(min);
   SetMax(max);
}


void MAS::ScrollObject::SetPosition(double pos) {
   this->pos = (pos < min) ? min : ((pos > max) ? max : pos);
}


void MAS::ScrollObject::SetIncrement(double inc) {
   this->inc = inc;
}


void MAS::ScrollObject::Setup(double min, double max, double pos, double inc) {
   SetRange(min, max);
   SetPosition(pos);
   SetIncrement(inc);
}


void MAS::ScrollObject::Increment() {
   pos += inc;
   pos = (pos > max) ? max : pos;
}


void MAS::ScrollObject::Decrement() {
   pos -= inc;
   pos = (pos < min) ? min : pos;
}


double MAS::ScrollObject::GetMin() {
   return min;
}


double MAS::ScrollObject::GetMax() {
   return max;
}


double MAS::ScrollObject::GetRange() {
   return max - min;
}


double MAS::ScrollObject::GetPosition() {
   return pos;
}


double MAS::ScrollObject::GetIncrement() {
   return inc;
}
