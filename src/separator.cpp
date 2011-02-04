////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/separator.h"

MAS::Separator::Separator() : Widget(), orientation(0) {
}


////////////////////////////////////////////////////////////////////////////////
// Sets the orientation and updates bitmap indices
//0 - vertical
//   1 - horizontal
void MAS::Separator::SetOrientation(int o) {
   orientation = o;
   SetBitmap(o == 1 ? Skin::SEPARATORH : Skin::SEPARATORV);
   UpdateSize();
}


int MAS::Separator::GetOrientation() {
   return orientation;
}


void MAS::Separator::Setup(int x, int y, int w, int h, int key, int flags, int o) {
   Widget::Setup(x, y, w, h, key, flags);
   SetOrientation(o);
}


void MAS::Separator::MsgInitSkin() {
   Widget::MsgInitSkin();

   if (orientation == 1) {
      if (GetBitmapIndex() == -1) SetBitmap(Skin::SEPARATORH);
   }
   else {
      if (GetBitmapIndex() == -1) SetBitmap(Skin::SEPARATORV);
   }
   
   UpdateSize();
}


void MAS::Separator::UpdateSize() {
   Bitmap bmp = GetBitmap();
   if (!bmp) return;
      
   if (orientation == 1) {
      h(bmp.h());
   }
   else {
      w(bmp.w());
   }
}


void MAS::Separator::Draw(Bitmap &canvas) {
   Bitmap bmp = GetBitmap();
   if (orientation == 1) {
      bmp.HorizontalTile(canvas, 0, 0, 0, w(), 1);
   }
   else {
      bmp.VerticalTile(canvas, 0, 0, 0, h(), 1);
   }
}
