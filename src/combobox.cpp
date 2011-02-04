////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/combobox.h"

#define MIN(x,y)     (((x) < (y)) ? (x) : (y))
#define MAX(x,y)     (((x) > (y)) ? (x) : (y))
#define CLAMP(x,y,z) MAX((x), MIN((y), (z)))


void MAS::ComboButton::SetOverlay(int ovr) {
overlay = ovr;
}


void MAS::ComboButton::Draw(Bitmap &canvas) {
   Button::Draw(canvas);
   Bitmap ovr = skin->GetBitmap(overlay);
   int state = Disabled() ? 2 : (Selected() ? 1 : (HasFocus() ? 3 : 0));
   ovr.Blit(canvas, 0, state*ovr.h()/4, (w() - ovr.w())/2, (h() - ovr.h()/4)/2, ovr.w(), ovr.h()/4);
}


bool MAS::ComboEditBox::MsgChar(int c) {
   int cc = c>>8;
   if (cc == ALLEGRO_KEY_UP || cc == ALLEGRO_KEY_DOWN) {
      Widget::MsgChar(c);
      return true;
   }
   else {
      return EditBox::MsgChar(c);
   }
}


void MAS::ComboEditBox::MsgLPress() {
   Widget::MsgLPress();
   GetParent()->HandleEvent(*this, MSG_ACTIVATE);
}


MAS::ComboBox::ComboBox() : MAS::CompoundWidget() {
   Add(editBox);
   Add(button);
   Add(list);
   list.Hide();
   list.h(80);
   list.SetSingleClick(true);
   editBox.SetFlag(D_READONLY);
}


void MAS::ComboBox::MsgInitSkin() {
   Dialog::MsgInitSkin();
   
   button.SetOverlay(Skin::COMBO_BUTTON);
   list.SetAnimationProperties(skin->comboAnimationLength, skin->comboAnimationType);
   button.SetSample(Skin::SAMPLE_OPEN, Skin::SAMPLE_ACTIVATE);
}


void MAS::ComboBox::UpdateSize() {
   Bitmap bmp = skin->GetBitmap(Skin::COMBO_BUTTON);
   if (!bmp) return;
   if (list.Hidden()) {
      button.Shape(w() - bmp.w(), 0, bmp.w(), h());
      editBox.Resize(w() - bmp.w(), h());
   }
   else {
      button.Shape(w() - bmp.w(), 0, bmp.w(), h() - list.h());
      editBox.Resize(w() - bmp.w(), h() - list.h());
   }
   if (!list.GetSelectedItem()) {
      editBox.SetText(NULL);
   }
   else {
      editBox.SetText(list.GetSelectedItem()->GetText());
   }
   list.Place(0, editBox.h());
   list.w(w());
   Dialog::UpdateSize();
}


void MAS::ComboBox::Setup(int x, int y, int w, int h, int key, int flags, int i) {
   Dialog::Setup(x, y, w, h, key, flags);
   if (i<0) i = 0; else if (i>=list.Size()) i=list.Size()-1;
   list.Select(i);
   MAS::ListItem *li = list.GetItem(i);
   if (li) {
      editBox.Setup(0, 0, w, h, key, flags, li->GetText());
   }
   else {
      editBox.Setup(0, 0, w, h, key, flags, NULL);
   }
   list.w(w);
}


void MAS::ComboBox::HandleEvent(Widget& obj, int msg, intptr_t arg1, intptr_t arg2) {
   Dialog::HandleEvent(obj, msg, arg1, arg2);
   
   if (msg == MSG_ACTIVATE) {
      if (obj == button || obj == editBox) {
         if (list.Hidden()) {
            ShowList();
            RootWindow()->MoveFocusTo(list.list);
            //GetParent()->MoveFocusTo(list.list);
            //MoveFocusTo(list.list);
         }
         else {
            HideList();
         }
      }

      else if (obj == list) {
         editBox.SetText(list.GetSelectedItem()->GetText());
         HideList();
         RootWindow()->MoveFocusTo(&button);
         //GetParent()->MoveFocusTo(&button);
         //GiveFocusTo(&button);
         parent->HandleEvent(*this, callbackID >= 0 ? callbackID : MSG_ACTIVATE, arg1);
      }
   }
   else if (msg == MSG_LOSTMOUSE) {
      Point mp = GetMousePos() + GetOffset();
      if (!(mp <= list) && !(mp <= *this)) {
         HideList();
      }
   }
   else if (msg == MSG_CHAR) {
      if (obj == editBox) {
         int curSel = list.Selection();
         
         switch (arg1>>8) {
            case ALLEGRO_KEY_UP:   --curSel;      break;
            case ALLEGRO_KEY_DOWN:   ++curSel;      break;
            default:                  return;
         };
         
         curSel = CLAMP(0, curSel, list.Size());
         if (curSel != list.Selection()) {
            list.Select(curSel);
            editBox.SetText(list.GetSelectedItem()->GetText());
            parent->HandleEvent(*this, callbackID >= 0 ? callbackID : MSG_ACTIVATE, curSel);
         }
      }
   }
}


void MAS::ComboBox::HideList() {
   button.SetSample(Skin::SAMPLE_OPEN, Skin::SAMPLE_ACTIVATE);
   list.ResetAnimation();
   list.Hide();
   h(editBox.h());
}


void MAS::ComboBox::ShowList() {
   button.SetSample(Skin::SAMPLE_ACTIVATE, Skin::SAMPLE_ACTIVATE);
   h(editBox.h() + list.h());
   list.Animate();
   list.Unhide();
   parent->BringToTop(*this);
}



void MAS::ComboBox::MsgGotfocus() {
   Dialog::MsgGotfocus();
   GiveFocusTo(&button);
}


void MAS::ComboBox::MsgLostfocus() {
   Dialog::MsgLostfocus();
   TakeFocusFrom(focusObject);
}
