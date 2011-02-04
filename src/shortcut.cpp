////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/shortcut.h"


MAS::Shortcut::Shortcut() : MAS::Widget(), key(-1), shifts(0) {
}


void MAS::Shortcut::Setup(int key, int shifts) {
this->key = key;
   this->shifts = shifts;
}


bool MAS::Shortcut::MsgXChar(int xchar) {
   if (((xchar >> 8) == key) && ((xchar >> 16) == shifts)) {
      GetParent()->HandleEvent(*this, callbackID >= 0 ? callbackID : MSG_ACTIVATE);
      return true;
   }
   
   return false;
}
