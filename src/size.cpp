////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/size.h"


MAS::Size::Size(int w, int h)
:wide(w),
   high(h)
{
}


bool MAS::Size::operator==(const Size &s) const {
   return ((w() == s.w()) && (h() == s.h()));
}


bool MAS::Size::operator!=(const Size &s) const {
   return ((w() != s.w()) || (h() != s.h()));
}
