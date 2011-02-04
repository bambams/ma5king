////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/window.h"

#define MAX(x,y)     (((x) > (y)) ? (x) : (y))

void MAS::FrameBorder::MsgInitSkin() {
SetType(type);
   Widget::MsgInitSkin();
}


void MAS::FrameBorder::SetType(int type) {
   this->type = type;
   switch (type) {
      case 0:
         SetBitmap(Skin::WINLEFT);
         break;

      case 1:
         SetBitmap(Skin::WINRIGHT);
         break;

      case 2:
         SetBitmap(Skin::WINTOP);
         break;

      case 3:
         SetBitmap(Skin::WINBOTTOM);
         break;

      case 4:
         SetBitmap(Skin::WINGRIP);
         break;
   };
}


int MAS::FrameBorder::GetSection() {
   int section = 0;
   Bitmap bmp = GetBitmap();
   Point mp = GetMousePos();

   switch (type) {
      case 0:
      case 1:
         if (mp.y() < (bmp.h() - bmp.ThickY())/2) {
            section = 0;
         }
         else if (mp.y() >= h() - (bmp.h() - bmp.ThickY())/2) {
            section = 2;
         }
         else {
            section = 1;
         }
         break;

      case 2:
      case 3:
         if (mp.x() < (bmp.w() - bmp.ThickX())/2) {
            section = 0;
         }
         else if (mp.x() >= w() - (bmp.w() - bmp.ThickX())/2) {
            section = 2;
         }
         else {
            section = 1;
         }
         break;

      default:
         section = 0;
         break;
   };

   return section;
}


void MAS::FrameBorder::Draw(Bitmap &canvas) {
   Bitmap bmp = GetBitmap();
   int off = HasFocus() ? 0 : 1;

   switch (type) {
      case 0:
      case 1:
         bmp.VerticalTile(canvas, off, 0, 0, h(), 2);
         break;

      case 2:
      case 3:
      case 4:
         bmp.HorizontalTile(canvas, off, 0, 0, w(), 2);
         break;
   }
}


void MAS::TextBack::Draw(Bitmap &canvas) {
   Bitmap bmp = GetBitmap();
   bmp.HorizontalTile(canvas, HasFocus() ? 0 : 1, 0, 0, w(), 2);
}


MAS::Window::Window() : Dialog(), extraFlags(0) {
   // setup the border frame
   bLeft.SetType(0);
   bRight.SetType(1);
   bTop.SetType(2);
   bBottom.SetType(3);
   bGrip.SetType(4);
   Dialog::Add(bLeft);
   Dialog::Add(bRight);
   Dialog::Add(bTop);
   Dialog::Add(bBottom);
   Dialog::Add(bGrip);

   // setup default title
   textBack.Hide();
   Dialog::Add(textBack);
   title.SetText("Window");
   Dialog::Add(title);

   // setup the icons
   iconMin.SetFlag(D_AUTOSIZE | D_DISABLED);
   iconMax.SetFlag(D_AUTOSIZE | D_DISABLED);
   iconExit.SetFlag(D_AUTOSIZE | D_EXIT | D_DISABLED);
   Dialog::Add(iconMin);
   Dialog::Add(iconMax);
   Dialog::Add(iconExit);

   // setup client area
   ownClientArea.Resize(0,0);
   ownClientArea.ClearFlag(D_TOPLEVEL);
   clientArea = &ownClientArea;
   Dialog::Add(*clientArea);

   // Set window properties
   SetFlag(D_WINDOW);
   SetFlag(D_MOVABLE);
   SetFlag(D_RESIZABLE);
   ClearFlag(D_MODAL);
}

//TODO: popups
#if 0
MAS::Widget *MAS::Window::Popup(MAS::Dialog *p, int xx, int yy, MAS::Widget *f) {
   SetAnimationProperties(skin->winAnimationLength, skin->winAnimationType);

   Animate();
   Widget *ret = Dialog::Popup(p, xx, yy, f);
   ResetAnimation();
   return ret;
}
#endif


void MAS::Window::UpdateWindowProperties() {
   if (HasFocus()) {
      iconMax.Enable();
      iconMin.Enable();
      iconExit.Enable();
      title.Enable();
   }
   else {
      iconMax.Disable();
      iconMin.Disable();
      iconExit.Disable();
      title.Disable();
   }

   if (Flags() & D_RESIZABLE) {
      if (!Hidden()) {
         iconMax.Unhide();
         iconMin.Unhide();
      }
   }
   else {
      iconMax.Hide();
      iconMin.Hide();
   }
}


void MAS::Window::MsgInitSkin() {
   Dialog::MsgInitSkin();
   // setup icons
   iconMin.SetBitmap(Skin::ICONMIN);
   iconMax.SetBitmap(Skin::ICONMAX);
   iconExit.SetBitmap(Skin::ICONEXIT);

   // setup title
   title.SetFontColor(skin->fcol[Skin::INFO_WINDOW][Skin::NORMAL], skin->scol[Skin::INFO_WINDOW][Skin::NORMAL], Skin::NORMAL);
   title.SetFont(skin->fnt[Skin::INFO_WINDOW][Skin::NORMAL], Skin::NORMAL);
   title.SetFontColor(skin->fcol[Skin::INFO_WINDOW][Skin::SELECT], skin->scol[Skin::INFO_WINDOW][Skin::SELECT], Skin::DISABLE);
   title.SetFont(skin->fnt[Skin::INFO_WINDOW][Skin::SELECT], Skin::DISABLE);
   title.SetTextMode(Color::transparent);
   title.SetAlignment(skin->winTextAlign);
   title.SetFlag(D_AUTOSIZE);
   if (skin->winTextBack) {
      textBack.SetBitmap(Skin::WINTEXT);
      textBack.Unhide();
   }
   else {
      textBack.Hide();
   }

   // The samples
   SetSample(Skin::SAMPLE_GOTFOCUS, Skin::SAMPLE_GOTFOCUS);
   SetSample(Skin::SAMPLE_LOSTFOCUS, Skin::SAMPLE_LOSTFOCUS);
   SetSample(Skin::SAMPLE_OPEN, Skin::SAMPLE_OPEN);
   SetSample(Skin::SAMPLE_CLOSE, Skin::SAMPLE_CLOSE);
   iconMax.SetSample(Skin::SAMPLE_OPEN, Skin::SAMPLE_ACTIVATE);
   iconMin.SetSample(Skin::SAMPLE_CLOSE, Skin::SAMPLE_ACTIVATE);

   SetAnimationProperties(skin->winAnimationLength, skin->winAnimationType);

   if (clientArea->w() && clientArea->h()) {
      w(clientArea->w() + bLeft.GetBitmap().w()/2 + bRight.GetBitmap().w()/2);
      h(clientArea->h() + bTop.GetBitmap().h()/2 + bBottom.GetBitmap().h()/2 + bGrip.GetBitmap().h()/2);
   }

   minSize.w(minSize.w() + bLeft.GetBitmap().w()/2 + bRight.GetBitmap().w()/2);
   minSize.h(minSize.h() + bTop.GetBitmap().h()/2 + bBottom.GetBitmap().h()/2 + bGrip.GetBitmap().h()/2);

   UpdateSize();
   UpdateWindowProperties();
}


void MAS::Window::UpdateSize() {
   Dialog::UpdateSize();

   // resize border frame
   Bitmap bmp = bLeft.GetBitmap();
   if (!bmp) return;
   bLeft.Shape(0, 0, bmp.w()/2, h());

   bmp = bRight.GetBitmap();
   bRight.Shape(w() - bmp.w()/2, 0, bmp.w()/2, h());

   bmp = bTop.GetBitmap();
   bTop.Shape(bLeft.w(), 0, w() - bLeft.w() - bRight.w(), bmp.h()/2);

   bmp = bBottom.GetBitmap();
   bBottom.Shape(bLeft.w(), h() - bmp.h()/2, w() - bLeft.w() - bRight.w(), bmp.h()/2);

   bmp = bGrip.GetBitmap();
   bGrip.Shape(bLeft.w(), bTop.h(), w() - bLeft.w() - bRight.w(), bmp.h()/2);

   // place icons
   iconMin.Place(skin->winMinPos);
   iconMax.Place(skin->winMaxPos);
   iconExit.Place(skin->winExitPos);

   // place title
   switch (skin->winTextAlign) {
      case 0:
         title.Place(skin->winTextPos);
         break;

      case 1:
         title.Place(w() - title.w() - skin->winTextPos.x(), skin->winTextPos.y());
         break;

      case 2:
         title.Place((w() - title.w())/2, skin->winTextPos.y());
         break;
   };

   if (!textBack.Hidden()) {
      bmp = textBack.GetBitmap();
      textBack.Shape(MAX(0, title.x() - x() - (bmp.w() - bmp.ThickX())/2), 0, title.w() + bmp.w() - bmp.ThickX(), bmp.h()/2);
   }

   minSize.w(MAX(textBack.w() + 32, minSize.w()));
   minSize.h(MAX(bTop.h() + bGrip.h() + bBottom.h(), minSize.h()));

   // setup the client area
   clientArea->Shape(bLeft.w(), bTop.h() + bGrip.h(), w() - bLeft.w() - bRight.w(), h() - bTop.h() - bGrip.h() - bBottom.h());
}


MAS::Dialog::ActionType MAS::Window::GetAction() {
   ActionType action = NONE;

   if (iconMax.HasMouse() || iconMin.HasMouse() || iconExit.HasMouse() || clientArea->HasMouse()) {
      return action;
   }

   int section;
   Point mp = GetMousePos() + topLeft();
   int mpSection = -1;
   if (mp <= bLeft)   mpSection = 0;      else
   if (mp <= bRight)   mpSection = 1;      else
   if (mp <= bTop)      mpSection = 2;      else
   if (mp <= bBottom)   mpSection = 3;      else
   if (mp <= bGrip)   mpSection = 4;

   if (TestFlag(D_MOVABLE) && !(extraFlags & W_MAXIMIZED) && mpSection == 4) {
      action = MOVE;
   }
   else if (TestFlag(D_RESIZABLE) && !(extraFlags & W_MAXIMIZED) && !(extraFlags & W_MINIMIZED)) {
      switch (mpSection) {
         case 0: {
            section = bLeft.GetSection();
            switch (section) {
               case 0:      action = RESIZE_UP_LEFT;      break;
               case 1:      action = RESIZE_LEFT;         break;
               case 2:      action = RESIZE_DOWN_LEFT;      break;
            };
         }
         break;

         case 1: {
            section = bRight.GetSection();
            switch (section) {
               case 0:      action = RESIZE_UP_RIGHT;      break;
               case 1:      action = RESIZE_RIGHT;         break;
               case 2:      action = RESIZE_DOWN_RIGHT;      break;
            };
         }
         break;

         case 2: {
            section = bTop.GetSection();
            switch (section) {
               case 0:      action = RESIZE_UP_LEFT;      break;
               case 1:      action = RESIZE_UP;            break;
               case 2:      action = RESIZE_UP_RIGHT;      break;
            };
         }
         break;

         case 3: {
            section = bBottom.GetSection();
            switch (section) {
               case 0:      action = RESIZE_DOWN_LEFT;      break;
               case 1:      action = RESIZE_DOWN;         break;
               case 2:      action = RESIZE_DOWN_RIGHT;      break;
            };
         }
         break;
      };
   }

   return action;
}


void MAS::Window::MsgGotfocus() {
   bLeft.SendMessage(MSG_GOTFOCUS);
   bRight.SendMessage(MSG_GOTFOCUS);
   bTop.SendMessage(MSG_GOTFOCUS);
   bBottom.SendMessage(MSG_GOTFOCUS);
   bGrip.SendMessage(MSG_GOTFOCUS);
   textBack.SendMessage(MSG_GOTFOCUS);
   UpdateWindowProperties();
   Dialog::MsgGotfocus();
}


void MAS::Window::MsgLostfocus() {
   Dialog::MsgLostfocus();
   bLeft.SendMessage(MSG_LOSTFOCUS);
   bRight.SendMessage(MSG_LOSTFOCUS);
   bTop.SendMessage(MSG_LOSTFOCUS);
   bBottom.SendMessage(MSG_LOSTFOCUS);
   bGrip.SendMessage(MSG_LOSTFOCUS);
   textBack.SendMessage(MSG_LOSTFOCUS);
   UpdateWindowProperties();
   MoveFocus(focusObject, NULL);
   iconExit.Disable();
   iconMin.Disable();
   iconMax.Disable();
   title.Disable();
}


bool MAS::Window::MsgWantfocus() {
   return true;
}


void MAS::Window::MsgGotmouse() {
   Dialog::MsgGotmouse();

   if ((extraFlags & W_AUTOROLLUP) && (extraFlags & W_MINIMIZED)) {
      Minimize();
      UpdateWindowProperties();
   }
}


void MAS::Window::MsgLostmouse() {
   Dialog::MsgLostmouse();

   SetCursor(Skin::MOUSE_NORMAL);

   if ((extraFlags & W_AUTOROLLUP) && !(extraFlags & W_MINIMIZED)) {
      Minimize();
      UpdateWindowProperties();
   }

   MoveMouse(mouseObject, NULL);
}


void MAS::Window::Add(Widget &w) {
   //Dialog::Remove(*clientArea);
   clientArea->Add(w);
   Dialog::Add(*clientArea);
}


void MAS::Window::Remove(Widget &w) {
   clientArea->Remove(w);
   Dialog::Remove(w);
}


void MAS::Window::SetClientArea(Dialog *clientArea) {
   clientArea->ClearFlag(D_TOPLEVEL);
   Dialog::Remove(*(this->clientArea));
   this->clientArea = clientArea;
   Resize(clientArea->w() + bLeft.w() + bRight.w(), clientArea->h() + bTop.h() + bGrip.h() + bBottom.h());
   Dialog::Add(*clientArea);
}


void MAS::Window::Minimize() {
   if (extraFlags & W_MAXIMIZED) {
      return;
   }

   if (!(extraFlags & W_MINIMIZED)) {
      restoreRect = (*this);

      extraFlags |= W_MINIMIZED;
      Dialog::Remove(*clientArea);
      Rect oldRect = *this;
      //Resize(120, bTop.h() + bGrip.h() + bBottom.h());
      Resize(w(), bTop.h() + bGrip.h() + bBottom.h());
      iconMax.SetSample(Skin::SAMPLE_ACTIVATE, Skin::SAMPLE_ACTIVATE);
      iconMin.SetSample(Skin::SAMPLE_OPEN, Skin::SAMPLE_ACTIVATE);
   }
   else {
      extraFlags &= ~W_MINIMIZED;
      Resize(restoreRect.w(), restoreRect.h());
      Dialog::Add(*clientArea);
      iconMax.SetSample(Skin::SAMPLE_OPEN, Skin::SAMPLE_ACTIVATE);
      iconMin.SetSample(Skin::SAMPLE_CLOSE, Skin::SAMPLE_ACTIVATE);
   }

   if (!HasMouse()) {
      if (Flags() & D_RESIZABLE) {
         iconMin.Disable();
         iconMax.Disable();
      }
      iconExit.Disable();
   }

   MsgShape();
}


void MAS::Window::Maximize() {
   if (extraFlags & W_MINIMIZED) {
      return;
   }

   if (!(extraFlags & W_MAXIMIZED)) {
      restoreRect = (*this);

      extraFlags |= W_MAXIMIZED;
      iconMax.SetBitmap(Skin::ICONRESTORE);
      Place(GetParent()->topLeft());
      Resize(GetParent()->size());
      iconMax.SetSample(Skin::SAMPLE_CLOSE, Skin::SAMPLE_ACTIVATE);
      iconMin.SetSample(Skin::SAMPLE_ACTIVATE, Skin::SAMPLE_ACTIVATE);
   }
   else {
      extraFlags &= ~W_MAXIMIZED;
      iconMax.SetBitmap(Skin::ICONMAX);
      Shape(restoreRect);
      iconMax.SetSample(Skin::SAMPLE_OPEN, Skin::SAMPLE_ACTIVATE);
      iconMin.SetSample(Skin::SAMPLE_ACTIVATE, Skin::SAMPLE_ACTIVATE);
   }

   MsgShape();
}


void MAS::Window::Autorollup(bool a) {
   if (a) {
      extraFlags |= W_AUTOROLLUP;
      if (!HasFocus() && !(extraFlags & W_MINIMIZED)) {
         Minimize();
      }
   }
   else {
      extraFlags &= ~W_AUTOROLLUP;
      if ((extraFlags & W_MINIMIZED)) {
         Minimize();
      }
   }
}


void MAS::Window::HandleEvent(MAS::Widget &obj, int msg, intptr_t arg1, intptr_t arg2) {
   Dialog::HandleEvent(obj, msg, arg1, arg2);

   if (msg == MSG_ACTIVATE) {
      if (obj == iconMin) {
         Minimize();
      }
      else if (obj == iconMax) {
         Maximize();
      }
   }
   else if (msg == MSG_DCLICK) {
      if (obj == bGrip || obj == textBack || obj == title) {
         if (TestFlag(D_RESIZABLE)) {
            if (extraFlags & W_MAXIMIZED) {
               PlaySample(Skin::SAMPLE_CLOSE);
            }
            else {
               PlaySample(Skin::SAMPLE_OPEN);
            }
            Maximize();
         }
      }
   }
   else if (msg == MSG_CLOSE && obj == *clientArea) {
      Close();
   }
}


void MAS::WinClientArea::MsgInitSkin() {
   Dialog::MsgInitSkin();
   pan.SetBitmap(Skin::WINDOW);
}


MAS::WinClientArea::WinClientArea() : Dialog() {
   pan.Shape(0, 0, 100, 100, true);
   Add(pan);
}


void MAS::WinClientArea::HandleEvent(Widget &obj, int msg, intptr_t arg1, intptr_t arg2) {
   parent->HandleEvent(obj, msg, arg1, arg2);
}


bool MAS::WinIcon::MsgWantTab() {
   return false;
}


MAS::Dialog *MAS::Window::GetClientArea() {
   return clientArea;
}
