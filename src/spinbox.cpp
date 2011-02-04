////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/spinbox.h"


MAS::SpinBox::SpinBox() : MAS::CompoundWidget(), integerMode(false) {
editBox.SetText(NULL, 16);
   Add(editBox);
   Add(bUp);
   Add(bDown);
}


void MAS::SpinBox::MsgInitSkin() {
   Dialog::MsgInitSkin();
   bUp.SetBitmap(Skin::SCROLL_UP);
   bDown.SetBitmap(Skin::SCROLL_DOWN);
   bUp.SetSample(-1, Skin::SAMPLE_ACTIVATE);
   bDown.SetSample(-1, Skin::SAMPLE_ACTIVATE);
   SetSample(Skin::SAMPLE_SCROLL, Skin::SAMPLE_SCROLL);
   bUp.SetFlag(D_SPINNER);
   bDown.SetFlag(D_SPINNER);
}


void MAS::SpinBox::UpdateSize() {
   Bitmap bmp = bUp.GetBitmap();
   if (!bmp) return;

   bUp.Shape(w() - bmp.w(), 0, bmp.w(), h()/2);
   bDown.Shape(w() - bmp.w(), h()/2, bmp.w(), h()/2);
   editBox.Resize(w() - bmp.w(), h());

   Dialog::UpdateSize();
}


void MAS::SpinBox::Setup(int x, int y, int w, int h, int key, int flags, double min, double max, double pos, double inc) {
   Dialog::Shape(x, y, w, h);
   ScrollObject::Setup(min, max, pos, inc);
   editBox.Setup(0, 0, w, h, key, flags, NULL, 10);
   editBox.SetNumber(pos);
   integerMode = false;
}


void MAS::SpinBox::Setup(int x, int y, int w, int h, int key, int flags, int min, int max, int pos, int inc) {
   Setup(x, y, w, h, key, flags, (double)min, (double)max, (double)pos, (double)inc);
   integerMode = true;
   editBox.SetNumber((int)pos);
}


void MAS::SpinBox::HandleEvent(Widget& obj, int msg, intptr_t arg1, intptr_t arg2) {
   if (msg == MSG_ACTIVATE) {
      if (obj == bUp) {
         double oldpos = pos;
         SetPosition(editBox.GetDouble());
         Increment();
         if (oldpos != pos) {
            PlaySample(Skin::SAMPLE_SCROLL);
            GetParent()->HandleEvent(*this, MSG_SCROLL, (intptr_t)GetIncrement());
         }
      }
      else if (obj == bDown) {
         double oldpos = pos;
         SetPosition(editBox.GetDouble());
         Decrement();
         if (oldpos != pos) {
            PlaySample(Skin::SAMPLE_SCROLL);
            GetParent()->HandleEvent(*this, MSG_SCROLL, -(intptr_t)GetIncrement());
         }
      }
      else {
         GetParent()->HandleEvent(*this, callbackID >= 0 ? callbackID : MSG_ACTIVATE);
      }
   }
   else if (msg == MSG_UCHAR && obj == editBox) {
      if (integerMode) {
         SetPosition(editBox.GetInt());
      }
      else {
         ScrollObject::SetPosition(editBox.GetDouble());
      }
   }
}


void MAS::SpinBox::SetPosition(double pos) {
   ScrollObject::SetPosition(pos);
   editBox.SetNumber(pos);
}


void MAS::SpinBox::Increment() {
   ScrollObject::Increment();
   if (integerMode) {
      editBox.SetNumber((int)GetPosition());
   }
   else {
      editBox.SetNumber(GetPosition());
   }
}


void MAS::SpinBox::Decrement() {
   ScrollObject::Decrement();
   if (integerMode) {
      editBox.SetNumber((int)GetPosition());
   }
   else {
      editBox.SetNumber(GetPosition());
   }
}


void MAS::SpinBox::MsgGotfocus() {
   Dialog::MsgGotfocus();
   GiveFocusTo(&editBox);
}


void MAS::SpinBox::MsgLostfocus() {
   Dialog::MsgLostfocus();
   TakeFocusFrom(focusObject);
}


void MAS::SpinButton::Draw(Bitmap &canvas) {
   Bitmap bmp = GetBitmap();
   int state = Disabled() ? 2 : (Selected() ? 1 : (HasFocus() ? 3 : 0));
   bmp.StretchBlit(canvas, 0, state*bmp.h()/4, bmp.w(), bmp.h()/4, 0, 0, w(), h());
}


bool MAS::SpinButton::MsgWantTab() {
   return false;
}
