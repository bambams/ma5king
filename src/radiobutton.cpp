////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/radiobutton.h"


MAS::RadioButton::RadioButton() : CheckBox(), group(0) {
}


void MAS::RadioButton::SetGroup(int g) {
group = g;
}


void MAS::RadioButton::Setup(int x, int y, int w, int h, int key, int flags, const char *t, int g) {
   Label::Setup(x, y, w, h, key, flags, t, 0);
   SetGroup(g);
}


void MAS::RadioButton::MsgInitSkin() {
   Button::MsgInitSkin();
   for (int i=0; i<4; i++) {
      if (GetFontColor(i) == Color::transparent) SetFontColor(skin->fcol[Skin::INFO_RADIO][i], skin->scol[Skin::INFO_RADIO][i], i);
      if (GetFontIndex(i) == -1) SetFont(skin->fnt[Skin::INFO_RADIO][i], i);
   }
   if (GetTextMode() == Color::transparent) SetTextMode(skin->c_face);
   if (GetBitmapIndex() == Skin::BUTTON) SetBitmap(Skin::RADIO);
   SetFlag(D_TOGGLE);
   ClearFlag(D_SPINNER | D_AUTOSIZE);
}


void MAS::RadioButton::MsgKey() {
   if (!Selected()) {
      CheckBox::MsgKey();
      GetParent()->DialogMessage(MSG_RADIO, group);
   }
   else {
      Widget::MsgKey();
   }
}


void MAS::RadioButton::MsgLPress() {
   if (!Selected()) {
      CheckBox::MsgLPress();
   }
   else {
      Widget::MsgLPress();
   }
}


void MAS::RadioButton::MsgLRelease() {
   bool pressed = TestFlag(D_PRESSED);
   bool oldSel = Selected();
   CheckBox::MsgLRelease();
   if (oldSel == Selected() && pressed) {
      GetParent()->DialogMessage(MSG_RADIO, group);
   }
}


void MAS::RadioButton::MsgRadio(int g) {
   if (g == group && !HasFocus() && Selected()) {
      Toggle();
   }
   
   Widget::MsgRadio(g);
}


int MAS::RadioButton::GetGroup() {
   return group;
}
