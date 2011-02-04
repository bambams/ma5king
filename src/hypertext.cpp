////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/hypertext.h"


void MAS::HyperText::MsgInitSkin() {
for (int i=0; i<4; i++) {
   if (GetFontColor(i) == Color::transparent) SetFontColor(skin->fcol[Skin::INFO_HYPER][i], skin->scol[Skin::INFO_HYPER][i], i);
      if (GetFontIndex(i) == -1) SetFont(skin->fnt[Skin::INFO_HYPER][i], i);
   }
   if (GetTextMode() == Color::transparent) SetTextMode(skin->c_face);
   SetFlag(D_AUTOSIZE);
   UpdateSize();
   ClearFlag(D_TOGGLE | D_SPINNER);
   Button::MsgInitSkin();
}


void MAS::HyperText::Draw(Bitmap &canvas) {
   // get the button state
   int state = Disabled() ? 2 : ((Selected() || (Flags() & D_PRESSED)) ? 1 : (HasFocus() ? 3 : 0));
   
   // get the button colors and font
   Color fg = GetFontColor(state);
   Color bg = GetShadowColor(state);
   Font f = GetFont(state);
   Color textMode = GetTextMode();

   canvas.Clear(textMode);
   
   switch (state) {
      case 0:
      case 2:
         f.GUITextoutEx(canvas, text, 1, 1, fg, bg, textMode, 0);
         canvas.Hline(1, h()-3, w()-2, fg);
         break;
         
      case 1:
         f.GUITextoutEx(canvas, text, 2, 2, fg, bg, textMode, 0);
         canvas.DrawDottedRect(1, 1, w()-1, h()-1, skin->c_font);
         canvas.Hline(1, h()-2, w()-2, fg);
         break;
         
      case 3:
         f.GUITextoutEx(canvas, text, 1, 1, fg, bg, textMode, 0);
         canvas.DrawDottedRect(0, 0, w()-2, h()-2, skin->c_font);
         canvas.Hline(1, h()-3, w()-2, fg);
         break;
   };
}


void MAS::HyperText::MsgGotmouse() {
   SetCursor(Skin::MOUSE_SELECT);
   Button::MsgGotmouse();
}


void MAS::HyperText::MsgLostmouse() {
   SetCursor(Skin::MOUSE_NORMAL);
   Button::MsgGotmouse();
}


void MAS::HyperText::UpdateSize() {
   Label::UpdateSize();
   Resize(w()+3, h()+4);
}
