////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/label.h"
#include <math.h>
#include <stdio.h>


MAS::Label::Label()
:Widget(),
   text(NULL),
   alignment(0),
   valignment(0),
   wordwrap(false)

{
   SetFlag(D_AUTOSIZE);
}


MAS::Label::~Label() {
   if (text != NULL) {
      delete [] text;
      text = NULL;
   }
}


void MAS::Label::Setup(int x, int y, int w, int h, int key, int flags, const char *title, int a) {
   SetText(title);
   switch (a) {
      case 0:   AlignLeft();   break;
      case 1:   AlignRight();   break;
      case 2:   AlignCentre();   break;
   }
   Widget::Setup(x, y, w, h, key, flags);
}


char *MAS::Label::GetText() {
   return text;
}


int MAS::Label::GetInt() {
   return (int)atof(text);
}


double MAS::Label:: GetDouble() {
   return atof(text);
}


void MAS::Label::SetAlignment(int a) {
   alignment = a;
   UpdateSize();
}

void MAS::Label::AlignLeft() {
   SetAlignment(0);
}


void MAS::Label::AlignRight() {
   SetAlignment(1);
}


void MAS::Label::AlignCentre() {
   SetAlignment(2);
}


void MAS::Label::SetWordWrap (bool value)
{
    if (wordwrap != value)
    {
        wordwrap = value;
      UpdateSize();
    }
}


int MAS::Label::GetPreferredHeight()
{
    int result = 0;

    if (text)
    {
        int type = Disabled() ? Skin::DISABLE : Skin::NORMAL;
        Font f = GetFont(type);
        result = f.TextHeight() * f.CountLines (text, w());
    }
    return result;
}


void MAS::Label::Draw(Bitmap &canvas) {
   if (text) {
      int type = Disabled() ? Skin::DISABLE : Skin::NORMAL;
      Color fg = GetFontColor(type);
      Color bg = GetShadowColor(type);
      Font f = GetFont(type);

      Color textMode = GetTextMode();
      if (textMode) {
         canvas.Clear(textMode);
      }

      if (wordwrap) {
         f.BoxPrint(canvas, text, fg, bg, textMode, 0, 0, w(), h(), alignment, valignment);
      }
      else {
         int xx = 0;

         switch (alignment) {
            case 1:      xx = w();      break;
            case 2:      xx = w()/2;      break;
         };

         f.GUITextout(canvas, text, xx, (h() - f.TextHeight())/2, fg, bg, textMode, alignment);
      }
   }
}


void MAS::Label::MsgInitSkin() {
   for (int i=0; i<4; i++) {
      if (GetFontColor(i) == Color::transparent) SetFontColor(skin->fcol[Skin::INFO_TEXT][i], skin->scol[Skin::INFO_TEXT][i], i);
      if (GetFontIndex(i) == -1) SetFont(skin->fnt[Skin::INFO_TEXT][i], i);
   }
   UpdateSize();
   Widget::MsgInitSkin();
}


void MAS::Label::UpdateSize() {
   if (!text || !TestFlag(D_AUTOSIZE)) return;

   int type = Disabled() ? Skin::DISABLE : Skin::NORMAL;
   Font f = GetFont(type);
   if (wordwrap) {
      Resize(w(), GetPreferredHeight());
   }
   else {
      Resize(f.GUITextLength(text), f.TextHeight());
   }
}


void MAS::Label::SetText(const char *t) {
   if (text != NULL) {
      delete [] text;
      text = NULL;
   }

   if (t) {
      text = new char[1+strlen(t)];
      strcpy(text, t);
   }

   UpdateSize();
}


void MAS::Label::SetNumber(int value) {
   char buf[64];
   snprintf(buf, 64, "%d", value);
   SetText((const char *)buf);
}


void MAS::Label::SetNumber(double value) {
   char buf[256];
   snprintf(buf, 256, "%f", value);
   int l=strlen(buf)-1;
   for (int i=l; i>0; i--) {
      if (buf[i] != '0') {
         if (buf[i] != '.' && buf[i] != ',')
            buf[i+1] = 0;
         else if (i<l-1)
            buf[i+2] = 0;
         break;
      }
   }
   SetText((const char *)buf);
}


int MAS::Label::GetAlignment() {
   return alignment;
}


void MAS::Label::SetVAlignment (int value) {
   valignment = value;
}


int MAS::Label::GetVAlignment () {
   return valignment;
}


bool MAS::Label::GetWordWrap () {
   return wordwrap;
}
