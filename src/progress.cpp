////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/progress.h"

////////////////////////////////////////////////////////////////////////////////
// Sets the orientation and updates bitmap indices
//0 - vertical
//   1 - horizontal
void MAS::Progress::SetOrientation(int o) {
   orientation = o;
   SetBitmap(o == 1 ? Skin::PROGRESSH : Skin::PROGRESSV);
   UpdateSize();
}


void MAS::Progress::SetPosition(int pos) {
   ScrollObject::SetPosition(pos);
}


void MAS::Progress::Increment() {
   ScrollObject::Increment();
}


void MAS::Progress::Setup(int x, int y, int w, int h, int key, int flags, int p, int o) {
   Separator::Setup(x, y, w, h, key, flags, o);
   ScrollObject::Setup(0, 100, 0, 1);
}


void MAS::Progress::MsgInitSkin() {
   Widget::MsgInitSkin();

   if (orientation == 1) {
      if (GetBitmapIndex() == -1) SetBitmap(Skin::PROGRESSH);
   }
   else {
      if (GetBitmapIndex() == -1) SetBitmap(Skin::PROGRESSV);
   }
   
   UpdateSize();
}


void MAS::Progress::UpdateSize() {
   Bitmap bmp = GetBitmap();
   if (!bmp) return;
      
   if (orientation == 1) {
      h(bmp.h());
   }
   else {
      w(bmp.w());
   }
}


void MAS::Progress::Draw(Bitmap &canvas) {
   Bitmap bmp = GetBitmap();
   if (orientation == 1) {
      bmp.HorizontalTile(canvas, 0, 0, 0, w(), 2);
      int ww = (int)(w()*GetPosition()/(GetMax() - GetMin()));
      bmp.HorizontalTile(canvas, 1, 0, 0, ww, 2);
   }
   else {
      bmp.VerticalTile(canvas, 0, 0, 0, h(), 2);
      int hh = (int)(h()*GetPosition()/(GetMax() - GetMin()));
      bmp.VerticalTile(canvas, 1, 0, 0, hh, 2);
   }
}
