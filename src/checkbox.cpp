////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/checkbox.h"


void MAS::CheckBox::MsgInitSkin() {
Button::MsgInitSkin();
   for (int i=0; i<4; i++) {
      if (GetFontColor(i) == Color::transparent) SetFontColor(skin->fcol[Skin::INFO_CHECKBOX][i], skin->scol[Skin::INFO_CHECKBOX][i], i);
      if (GetFontIndex(i) == -1) SetFont(skin->fnt[Skin::INFO_CHECKBOX][i], i);
   }
   if (GetTextMode() == Color::transparent) SetTextMode(skin->c_face);
   if (GetBitmapIndex() == Skin::BUTTON) SetBitmap(Skin::CHECKBOX);
   SetFlag(D_TOGGLE);
   ClearFlag(D_SPINNER | D_AUTOSIZE);
}


void MAS::CheckBox::Draw(Bitmap &canvas) {
   // get the button state
   int state = Disabled() ? 2 : ((Flags() & D_PRESSED) ? 1 : (HasFocus() ? 3 : 0));
   
   // get the button colors, font and bitmap
   Color fg = GetFontColor(state);
   Color bg = GetShadowColor(state);
   Font f = GetFont(state);
   Color textMode = GetTextMode();
   Bitmap bmp = GetBitmap();
   int bw = bmp.w()/2;
   int bh = bmp.h()/4;

   // get bitmap offsets
   int ox = Selected() ? 1 : 0;
   int oy = state;

   // calculate the position and size of the box and the text
   int th = f.TextHeight();
   int txtlen   = f.GUITextLength(text);
   int box_x1   = 0;
   int box_y1   = th > bh ? (th - bh)/2 : 0;
   int box_x2   = bh;
   //int box_y2   = box_y1 + bh - 1;
   int text_x   = box_x2 + 4;
   int text_y   = bh > th ? (bh - th)/2 : 1;

   // draw the checkbox
   canvas.Clear(textMode);
   bmp.Blit(canvas, ox*bw, oy*bh, box_x1, box_y1, bw, bh);
   
   // print the text
   f.GUITextoutEx(canvas, text, text_x, text_y, fg, bg, textMode, 0);
   
   // draw the dotted rectangle
   if (skin->drawDots && HasFocus()) {
      canvas.DrawDottedRect(text_x - 2, text_y - 1, text_x + txtlen + 2, text_y + th, fg);
   }
}
