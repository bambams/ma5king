////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/groupbox.h"

#define MAX(x,y)     (((x) > (y)) ? (x) : (y))

MAS::GroupBox::GroupBox() : Dialog() {
Add(box);
   Add(title);
   ClearFlag(D_TOPLEVEL);
}


void MAS::GroupBox::MsgInitSkin() {
   Dialog::MsgInitSkin();

   box.SetBitmap(Skin::BOX);

   for (int i=0; i<4; i++) {
      title.SetFontColor(skin->fcol[Skin::INFO_BOX][i], skin->scol[Skin::INFO_BOX][i], i);
      title.SetFont(skin->fnt[Skin::INFO_BOX][i], i);
   }
   title.SetTextMode(skin->boxBack);
   title.SetAlignment(skin->boxAlign);
}


void MAS::GroupBox::SetTitle(const char *t) {
   title.SetText(t);
   UpdateSize();
}


void MAS::GroupBox::SetBitmap(int i) {
   Dialog::SetBitmap(i);
   box.SetBitmap(i);
}


const char *MAS::GroupBox::GetTitle() {
   return title.GetText();
}


void MAS::GroupBox::UpdateSize() {
   box.Resize(w(), h());

   int tx = 0;
   int ty = 0;
   
   switch (skin->boxAlign) {
      case 0:
         tx = skin->boxX;
         ty = skin->boxY;
      break;
      
      case 1:
         tx = w() - skin->boxX - title.w();
         ty = skin->boxY;
      break;
      
      case 2:
         tx = (w() - title.w())/2 + skin->boxX;
         ty = skin->boxY;
      break;
   };
   
   tx = MAX(tx, 0);
   ty = MAX(ty, 0);
   
   title.Place(tx, ty);

   Dialog::UpdateSize();
}
