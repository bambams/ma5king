#include "../include/MASkinG/splitter.h"

#define MAX(x,y)     (((x) > (y)) ? (x) : (y))
#define MIN(x,y)     (((x) < (y)) ? (x) : (y))
#define CLAMP(x,y,z) MAX((x), MIN((y), (z)))

MAS::Splitter::Splitter()
:Dialog(),
   bufferZone(4),
   offsetX(0xffff),
   offsetY(0xffff),
   horizontalMove(false),
   verticalMove(false)
{
   attached[0] = NULL;
   attached[1] = NULL;
   attached[2] = NULL;
   attached[3] = NULL;
   ClearFlag(D_TOPLEVEL);
}


void MAS::Splitter::UpdateSize() {
   Dialog::UpdateSize();
   
   if (offsetX == 0xffff) {
      offsetX = 0;
      offsetY = 0;
   }
   
   int wx, wy, ww, wh;
   
   if (attached[0]) {
      wx = 0;
      wy = 0;
      ww = (attached[2]) ? (w() - bufferZone)/2 + offsetX : w();
      wh = (attached[1] || attached[3]) ? (h() - bufferZone)/2 + offsetY : h();
      attached[0]->Shape(wx, wy, ww, wh);
   }

   if (attached[1]) {
      wx = 0;
      wy = (attached[0] || attached[2]) ? (h() + bufferZone)/2 + offsetY : 0;
      ww = (attached[3]) ? (w() - bufferZone)/2 + offsetX : w();
      wh = (attached[0] || attached[2]) ? (h() - bufferZone)/2 - offsetY : h();
      attached[1]->Shape(wx, wy, ww, wh);
   }

   if (attached[2]) {
      wx = (attached[0]) ? (w() + bufferZone)/2 + offsetX : 0;
      wy = 0;
      ww = (attached[0]) ? (w() - bufferZone)/2 - offsetX : w();
      wh = (attached[1] || attached[3]) ? (h() - bufferZone)/2 + offsetY : h();
      attached[2]->Shape(wx, wy, ww, wh);
   }

   if (attached[3]) {
      wx = (attached[1]) ? (w() + bufferZone)/2 + offsetX : 0;
      wy = (attached[0] || attached[2]) ? (h() + bufferZone)/2 + offsetY : 0;
      ww = (attached[1]) ? (w() - bufferZone)/2 - offsetX : w();
      wh = (attached[0] || attached[2]) ? (h() - bufferZone)/2 - offsetY : h();
      attached[3]->Shape(wx, wy, ww, wh);
   }   
}


void MAS::Splitter::MsgLostmouse() {
   Dialog::MsgLostmouse();
   if (!TestFlag(D_PRESSED)) {
      SetCursor(Skin::MOUSE_NORMAL);
   }
}


void MAS::Splitter::MsgLPress() {
   Dialog::MsgLPress();
   SetFlag(D_PRESSED);
}


void MAS::Splitter::MsgLRelease() {
   Dialog::MsgLRelease();
   ClearFlag(D_PRESSED);
   
   MsgMousemove(Point(0,0));
   if (!(mouse->pos <= *this)) {
      SetCursor(Skin::MOUSE_NORMAL);
   }
}


void MAS::Splitter::MsgMousemove(const Point &d) {
   Dialog::MsgMousemove(d);
   
   if (TestFlag(D_PRESSED)) {
      int x = offsetX;
      int y = offsetY;
      
      if (horizontalMove) {
         x += d.x();
      }
      
      if (verticalMove) {
         y += d.y();
      }
      
      if (horizontalMove || verticalMove) {
         SetOffset(x, y);
      }
   }
   else {
      Point mp = GetMousePos();
      
      if (mp <= *this - topLeft()) {
         horizontalMove = (mp.x() >= (w() - bufferZone)/2 + offsetX && mp.x() <= (w() + bufferZone)/2 + offsetX);
         verticalMove   = (mp.y() >= (h() - bufferZone)/2 + offsetY && mp.y() <= (h() + bufferZone)/2 + offsetY);
         
         for (int i=0; i<4; i++) {
            if (attached[i] && attached[i]->HasMouse()) {
               horizontalMove = verticalMove = false;
               break;
            }
         }
   
         if (horizontalMove && verticalMove) {
            SetCursor(Skin::MOUSE_MOVE);
         }
         else if (horizontalMove) {
            SetCursor(Skin::MOUSE_SIZE_HORIZONTAL);
         }
         else if (verticalMove) {
            SetCursor(Skin::MOUSE_SIZE_VERTICAL);
         }
         else {
            SetCursor(Skin::MOUSE_NORMAL);
         }
      }
      else {
         SetCursor(Skin::MOUSE_NORMAL);
      }
   }
}


void MAS::Splitter::Attach(Widget *w, int section) {
   if (attached[section]) {
      Detach(attached[section]);
   }
   attached[section] = w;
   Add(*attached[section]);
   w->ClearFlag(D_TOPLEVEL);
   UpdateSize();
}


void MAS::Splitter::Detach(Widget *w) {
   for (int section = 0; section < 4; section++) {
      if (attached[section] == w) {
         Remove(*attached[section]);
         attached[section] = NULL;
         UpdateSize();
         break;
      }
   }
}


void MAS::Splitter::SetBufferZone(int pixels) {
   bufferZone = MAX(pixels, 1);
   UpdateSize();
}


void MAS::Splitter::SetOffset(int x, int y) {
   offsetX = CLAMP(-(w()/2 - bufferZone), x, (w()/2 - bufferZone));
   offsetY = CLAMP(-(h()/2 - bufferZone), y, (h()/2 - bufferZone));
   UpdateSize();
}
