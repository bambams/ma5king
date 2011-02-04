////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
//                     Modifications by Vladimir Kokovic.                     //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/scroller.h"

#define MAX(x,y)     (((x) > (y)) ? (x) : (y))
#define MIN(x,y)     (((x) < (y)) ? (x) : (y))


MAS::Scroller::Scroller() : MAS::CompoundWidget(), visibleItems(1), orientation(0), hInc( -1 ), vInc( -1 ) {
Add(back);
   Add(bUp);
   Add(bGrip);
   Add(bDown);
}


void MAS::Scroller::MsgInitSkin() {
   Dialog::MsgInitSkin();

   bUp.SetSample(-1, Skin::SAMPLE_ACTIVATE);
   bDown.SetSample(-1, Skin::SAMPLE_ACTIVATE);
   SetSample(Skin::SAMPLE_SCROLL, Skin::SAMPLE_SCROLL);
   bUp.SetFlag(D_SPINNER | D_AUTOSIZE);
   bDown.SetFlag(D_SPINNER | D_AUTOSIZE);
   SetOrientation(orientation);
   CalculateGripperPosition();
}


void MAS::Scroller::Setup(int x, int y, int w, int h, int key, int flags, int totalItems, int visibleItems, int pos, int o) {
   Dialog::Shape(x, y, w, h);
   ScrollObject::Setup(0, totalItems, pos, 1);
   SetOrientation(o);
   SetRange(totalItems, visibleItems);
}


void MAS::Scroller::SetOrientation(int o) {
   orientation = o;
   back.orientation = o;
   if (orientation == 1) {
      bUp.SetBitmap(Skin::SCROLL_LEFT);
      bDown.SetBitmap(Skin::SCROLL_RIGHT);
      bGrip.SetBitmap(Skin::SCROLL_HGRIP);
      back.SetBitmap(Skin::SCROLL_HBACK);
      bGrip.SetOverlay(Skin::SCROLL_HGRIPOVERLAY);
   }
   else {
      bUp.SetBitmap(Skin::SCROLL_UP);
      bDown.SetBitmap(Skin::SCROLL_DOWN);
      bGrip.SetBitmap(Skin::SCROLL_VGRIP);
      back.SetBitmap(Skin::SCROLL_VBACK);
      bGrip.SetOverlay(Skin::SCROLL_VGRIPOVERLAY);
   }
   UpdateSize();
}


int MAS::Scroller::GetOrientation() {
   return orientation;
}


void MAS::Scroller::OriginalHandleEvent(Widget& obj, int msg, intptr_t arg1, intptr_t arg2) {
   if (msg == MSG_ACTIVATE) {
      if (obj == bUp) {
         double oldpos = pos;
         Decrement();
         if (pos != oldpos) {
            PlaySample(Skin::SAMPLE_SCROLL);
            GetParent()->HandleEvent(*this, MSG_SCROLL, (intptr_t)pos);
         }
      }
      else if (obj == bDown) {
         double oldpos = pos;
         Increment();
         if (pos != oldpos) {
            PlaySample(Skin::SAMPLE_SCROLL);
            GetParent()->HandleEvent(*this, MSG_SCROLL, (intptr_t)pos);
         }
      }
      else if (obj == back) {
         if (!bGrip.HasMouse()) {
            int oldpos = (int)pos;
            double newpos = pos;
            double d = 0.0;

            Point mp = GetMouse()->pos;
            if (orientation == 1) {
               if (mp.x() < bGrip.x()) {
                  d = -(max - min)/100;
                  d = MIN(-1, d);
               }
               else if (mp.x() > bGrip.x2()) {
                  d = (max - min)/100;
                  d = MAX(1, d);
               }
            }
            else {
               if (mp.y() < bGrip.y()) {
                  d = -(max - min)/100;
                  d = MIN(-1, d);
               }
               else if (mp.y() > bGrip.y2()) {
                  d = (max - min)/100;
                  d = MAX(1, d);
               }
            }

            newpos += d;
            newpos = (newpos < min) ? min : ((newpos > max) ? max : newpos);
            if ((int)newpos != oldpos) {
               SetPosition((int)newpos);
               PlaySample(Skin::SAMPLE_SCROLL);
               GetParent()->HandleEvent(*this, MSG_SCROLL, (intptr_t)newpos);
            }
         }
      }
   }
   else if (msg == MSG_LPRESS && obj == bGrip) {
      gOffset.x(arg1);
      gOffset.y(arg2);
   }
   else if (msg == MSG_SCROLL && obj == bGrip) {
      int oldpos = (int)pos;
      double newpos = pos;

      int d = 1;
      /* TODO
      if (key_shifts & KB_SHIFT_FLAG) {
         d = visibleItems;
      }
      */

      switch (arg1) {
         case ALLEGRO_KEY_LEFT:   if (orientation == 1) newpos -= d;   break;
         case ALLEGRO_KEY_RIGHT:   if (orientation == 1) newpos += d;   break;
         case ALLEGRO_KEY_UP:   if (orientation == 0) newpos -= d;   break;
         case ALLEGRO_KEY_DOWN:   if (orientation == 0) newpos += d;   break;
         case ALLEGRO_KEY_PGUP:   newpos -= visibleItems;            break;
         case ALLEGRO_KEY_PGDN:   newpos += visibleItems;            break;
         case ALLEGRO_KEY_HOME:   newpos = min;                  break;
         case ALLEGRO_KEY_END:   newpos = max;                  break;
         case -1: {
            Point mp = GetMousePos();
            if (orientation == 1) {
               newpos = GetRange()*(mp.x() - gOffset.x() - bGrip.w()/2 - bUp.w())/(back.w() - bGrip.w());
            }
            else {
               newpos = GetRange()*(mp.y() - gOffset.y() - bGrip.h()/2 - bUp.h())/(back.h() - bGrip.h());
            }
         }
      };

      newpos = (newpos < min) ? min : ((newpos > max) ? max : newpos);
      if ((int)newpos != oldpos) {
         SetPosition((int)newpos);
         PlaySample(Skin::SAMPLE_SCROLL);
         GetParent()->HandleEvent(*this, MSG_SCROLL, (intptr_t)newpos);
      }
   }
   else if (msg == MSG_WHEEL) {
      double oldpos = pos;
      SetPosition((int)(pos + (orientation==1 ? arg1 : -arg1)));
      if (pos != oldpos) {
         PlaySample(Skin::SAMPLE_SCROLL);
         GetParent()->HandleEvent(*this, MSG_SCROLL, (intptr_t)pos);
      }
   }

   Dialog::HandleEvent(obj, msg, arg1, arg2);
}


 void MAS::Scroller::HandleEvent( MAS::Widget & obj, int msg, intptr_t arg1, intptr_t arg2 ) {
   if ( (hInc == -1) && (vInc == -1) ) {
     OriginalHandleEvent( obj, msg, arg1, arg2 );
     return;
   }
   if ( msg == MSG_ACTIVATE ) {
     if ( obj == bUp ) {
       arg1 = (orientation == 1) ? ALLEGRO_KEY_LEFT : ALLEGRO_KEY_UP;  goto vkScroll;
     }
     else if ( obj == bDown ) {
       arg1 = (orientation == 1) ? ALLEGRO_KEY_RIGHT : ALLEGRO_KEY_DOWN;  goto vkScroll;
     }
     else if ( obj == back ) {
       if ( !bGrip.HasMouse() ) {
         Point mp = GetMouse()->pos;
         if ( orientation == 1 ) {
           if ( mp.x() < bGrip.x() ) {
             arg1 = ALLEGRO_KEY_LEFT;  goto vkScroll;
           }
           else if ( mp.x() > bGrip.x2() ) {
             arg1 = ALLEGRO_KEY_RIGHT;  goto vkScroll;
           }
         }
         else {
           if ( mp.y() < bGrip.y() ) {
             arg1 = ALLEGRO_KEY_PGUP;  goto vkScroll;
           }
           else if ( mp.y() > bGrip.y2() ) {
             arg1 = ALLEGRO_KEY_PGDN;  goto vkScroll;
           }
         }
       }
     }
   }
   else if ( msg == MSG_LPRESS && obj == bGrip ) {
     gOffset.x( arg1 );
     gOffset.y( arg2 );
   }
   else if ( msg == MSG_SCROLL ) {
 vkScroll:
     int oldpos = (int)pos;
     double newpos = pos;
     switch ( arg1 ) {
       case ALLEGRO_KEY_LEFT:  if (orientation == 1) newpos -= hInc;  break;
       case ALLEGRO_KEY_RIGHT: if (orientation == 1) newpos += hInc;  break;
       case ALLEGRO_KEY_UP:  if (orientation == 0) newpos -= vInc;  break;
       case ALLEGRO_KEY_DOWN:  if (orientation == 0) newpos += vInc;  break;
       case ALLEGRO_KEY_PGUP:  newpos -= visibleItems;       break;
       case ALLEGRO_KEY_PGDN:  newpos += visibleItems;       break;
       case ALLEGRO_KEY_HOME:  newpos = min;           break;
       case ALLEGRO_KEY_END: newpos = max;           break;
       case -1: {
         Point mp = GetMousePos();
         if ( orientation == 1 ) {
           newpos = GetRange() * (mp.x() - gOffset.x() - bGrip.w() / 2 - bUp.w()) / (back.w() - bGrip.w());
         }
         else {
           newpos = GetRange() * (mp.y() - gOffset.y() - bGrip.h() / 2 - bUp.h() ) / (back.h() - bGrip.h());
         }
       }
     };
     newpos = (newpos < min) ? min : ((newpos > max) ? max : newpos);
     if ( (int)newpos != oldpos ) {
       SetPosition( (int)newpos );
       PlaySample( Skin::SAMPLE_SCROLL );
       GetParent()->HandleEvent( *this, MSG_SCROLL, (intptr_t)newpos );
     }
   }
   else if ( msg == MSG_WHEEL ) {
     double oldpos = pos;
     SetPosition( (int)(pos + (orientation == 1 ? arg1 * hInc : -arg1 * vInc * 3)) );
     if ( pos != oldpos ) {
       PlaySample( Skin::SAMPLE_SCROLL );
       GetParent()->HandleEvent( *this, MSG_SCROLL, (intptr_t)pos );
     }
   }
   Dialog::HandleEvent( obj, msg, arg1, arg2 );
 }


 /*
 void MAS::Scroller::MsgWheel(int z) {
   double oldpos = pos;
   SetPosition( (int)(pos + (orientation == 1 ? z * hInc : -z * vInc * 3)) );
   if ( pos != oldpos ) {
     PlaySample( Skin::SAMPLE_SCROLL );
     GetParent()->HandleEvent( *this, MSG_SCROLL, (intptr_t)pos );
   }
 }
 */




void MAS::Scroller::SetRange(int totalItems, int visibleItems) {
   if (visibleItems > totalItems)
        {
            ScrollObject::SetRange(0,0);
        }
        else
        {
            ScrollObject::SetRange(0, totalItems-visibleItems);
        }
        double gripSize;
        if (totalItems == 0 || (visibleItems > totalItems))
            gripSize = 1.0;
        else
            gripSize = (double)visibleItems/(double)totalItems;
   Bitmap up = bUp.GetBitmap();
   if (!up) return;
   Bitmap down = bDown.GetBitmap();
   if (!down) return;
   if (orientation == 1) {
      int ww = (int)((w() - up.w() - down.w())*gripSize);
      if (ww < 10) ww = 10;
      bGrip.w(ww);
   }
   else {
      int hh = (int)((h() - up.h()/4 - down.h()/4)*gripSize);
      if (hh < 10) hh = 10;
      bGrip.h(hh);
   }
   this->visibleItems = visibleItems;
   CalculateGripperPosition();
}


int MAS::Scroller::GetVisibleItems() {
   return visibleItems;
}


void MAS::Scroller::SetPosition(int pos) {
   ScrollObject::SetPosition(pos);
   CalculateGripperPosition();
}


void MAS::Scroller::Increment() {
   ScrollObject::Increment();
   CalculateGripperPosition();
}


void MAS::Scroller::Decrement() {
   ScrollObject::Decrement();
   CalculateGripperPosition();
}


void MAS::Scroller::CalculateGripperPosition() {
   Bitmap up = bUp.GetBitmap();
   if (!up) return;
   Bitmap down = bDown.GetBitmap();
   if (!down) return;
   if (orientation == 1) {
      double range = GetRange();
      if (range <= 0)
         bGrip.x(up.w());
      else
         bGrip.x(up.w() + (int)(pos*((w() - up.w() - down.w()) - bGrip.w())/range));
   }
   else {
      double range = GetRange();
      if (range <= 0)
         bGrip.y(up.h()/4);
      else
         bGrip.y(up.h()/4 + (int)(pos*((h() - up.h()/4 - down.h()/4) - bGrip.h())/range));
   }
}


void MAS::Scroller::UpdateSize() {
   Bitmap bmp = back.GetBitmap();
   if (!bmp) return;

   if (orientation == 1) {
      h(bmp.h()/4);
      back.Shape(bDown.GetBitmap().w(), 0, w() - bDown.GetBitmap().w() - bUp.GetBitmap().w(), h());
      bUp.Shape(0, 0, bUp.GetBitmap().w(), h());
      bGrip.y(0);
      bGrip.h(h());
      bDown.Shape(w() - bDown.GetBitmap().w(), 0, bDown.GetBitmap().w(), h());
   }
   else {
      w(bmp.w());
      back.Shape(0, bDown.GetBitmap().h()/4, w(), h() - bDown.GetBitmap().h()/4 - bUp.GetBitmap().h()/4);
      bUp.Shape(0, 0, w(), bUp.GetBitmap().h()/4);
      bGrip.x(0);
      bGrip.w(w());
      bDown.Shape(0, h() - bDown.GetBitmap().h()/4, w(), bDown.GetBitmap().h()/4);
   }

   Dialog::UpdateSize();
}


void MAS::ScrollBack::Draw(Bitmap &canvas) {
   int state = Disabled() ? 2 : (Selected() ? 1 : (HasFocus() ? 3 : 0));
   GetBitmap().TiledBlit(canvas, 0, state, 0, 0, w(), h(), 1, 4);
}


void MAS::ScrollBack::MsgTick() {
   Widget::MsgTick();
   if (TestFlag(D_PRESSED)) {
      Mouse *m = parent->GetMouse();
      if (m->pos <= *this) {
         parent->HandleEvent(*this, MSG_ACTIVATE);
      }
   }
}


void MAS::ScrollBack::MsgLPress() {
   Widget::MsgLPress();
   SetFlag(D_PRESSED);
}


void MAS::ScrollBack::MsgLRelease() {
   Widget::MsgLRelease();
   ClearFlag(D_PRESSED);
}


void MAS::ScrollBack::MsgLostmouse() {
   Widget::MsgLostmouse();
   ClearFlag(D_PRESSED);
}


void MAS::ScrollButton::Draw(Bitmap &canvas) {
   Bitmap bmp = GetBitmap();
   int state = Disabled() ? 2 : (Selected() ? 1 : (HasFocus() ? 3 : 0));
   bmp.TiledBlit(canvas, 0, state, 0, 0, w(), h(), 1, 4);
   Bitmap ovr = skin->GetBitmap(overlay);
   if ((w() >= ovr.w()) && (h() >= ovr.h()/4)) {
      ovr.Blit(canvas, 0, state*ovr.h()/4, (w() - ovr.w())/2, (h() - ovr.h()/4)/2, ovr.w(), ovr.h()/4);
   }
}


void MAS::ScrollButton::SetOverlay(int i) {
   overlay = i;
}


void MAS::ScrollButton::MsgMousemove(const Point &d) {
   if (Selected()) {
      GetParent()->HandleEvent(*this, MSG_SCROLL, -1);
   }

   Button::MsgMousemove(d);
}


bool MAS::ScrollButton::MsgChar(int c) {
   switch (c>>8) {
      case ALLEGRO_KEY_LEFT:
         GetParent()->HandleEvent(*this, MSG_SCROLL, ALLEGRO_KEY_LEFT);
         break;

      case ALLEGRO_KEY_RIGHT:
         GetParent()->HandleEvent(*this, MSG_SCROLL, ALLEGRO_KEY_RIGHT);
         break;

      case ALLEGRO_KEY_UP:
         GetParent()->HandleEvent(*this, MSG_SCROLL, ALLEGRO_KEY_UP);
         break;

      case ALLEGRO_KEY_DOWN:
         GetParent()->HandleEvent(*this, MSG_SCROLL, ALLEGRO_KEY_DOWN);
         break;

      case ALLEGRO_KEY_PGUP:
         GetParent()->HandleEvent(*this, MSG_SCROLL, ALLEGRO_KEY_PGUP);
         break;

      case ALLEGRO_KEY_PGDN:
         GetParent()->HandleEvent(*this, MSG_SCROLL, ALLEGRO_KEY_PGDN);
         break;

      case ALLEGRO_KEY_HOME:
         GetParent()->HandleEvent(*this, MSG_SCROLL, ALLEGRO_KEY_HOME);
         break;

      case ALLEGRO_KEY_END:
         GetParent()->HandleEvent(*this, MSG_SCROLL, ALLEGRO_KEY_END);
         break;

      default:
         return Button::MsgChar(c);
   };

   Widget::MsgChar(c);
   return true;
}


void MAS::ScrollButton::MsgLPress() {
   Button::MsgLPress();
   Point mp = GetMousePos();
   parent->HandleEvent(*this, MSG_LPRESS, mp.x() - w()/2, mp.y() - h()/2);
}


bool MAS::ScrollButton::MsgWantTab() {
   return false;
}


bool MAS::ArrowButton::MsgWantTab() {
   return false;
}


bool MAS::Scroller::MsgWantTab() {
   return false;
}
