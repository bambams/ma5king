////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/accelerator.h"


MAS::KeyCombo::KeyCombo() : key(0), shifts(0), msg(0) {
}


MAS::KeyCombo::KeyCombo(int k, int s, int m) : key(k), shifts(s), msg(m) {
}


MAS::Accelerator::Accelerator() : MAS::Widget() {
}


void MAS::Accelerator::Add(int key, int shifts, int msg) {
   acc.push_back(KeyCombo(key, shifts, msg));
}


bool MAS::Accelerator::MsgXChar(int xchar) {
   for (std::vector<KeyCombo>::iterator i = acc.begin(); i != acc.end(); ++i) {
      if (((xchar >> 8) == i->key) && ((xchar >> 16) == i->shifts)) {
         GetParent()->HandleEvent(*this, i->msg);
         return true;
      }
   }
   
   return false;
}
