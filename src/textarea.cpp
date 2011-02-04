////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/textarea.h"
#include "../include/MASkinG/util.h"
#include <fstream>

using namespace std;
using namespace MAS;


void TextArea::CalculateRange() {
// determine max length
   vector<string>::iterator i;
   int maxlen = 0;
   
   int state = Disabled() ? 2 : (Selected() ? 1 : (HasFocus() ? 3 : 0));
   Font f = GetFont(state);
   
   for (i = lines.begin(); i != lines.end(); i++) {
      int len = f.TextLength (i->c_str());
      if (len > maxlen) maxlen = len;
   }  

   SetScrollSize (maxlen, f.TextHeight()*lines.size());
}


TextArea::TextArea() {
}


void TextArea::MsgInitSkin() {   
   ScrollBox::MsgInitSkin();

   for (int i=0; i<4; i++) {
      SetFontColor(skin->fcol[Skin::INFO_TEXT][i], skin->scol[Skin::INFO_TEXT][i], i);
      SetFont(skin->fnt[Skin::INFO_TEXT][i], i);
   }
}


void TextArea::DrawClientArea(Bitmap &dest, int xofst, int yofst) {
   int state = Disabled() ? 2 : (Selected() ? 1 : (HasFocus() ? 3 : 0));
   
   // get the font and font colors
   Font f = GetFont(state);
   Color fg = GetFontColor(state);
   Color bg = GetShadowColor(state);
   int rowheight = f.TextHeight();
   
   if (lines.size() > 0) {
      vector<string>::iterator i;
      int y;
      for (
         i = lines.begin() + (GetYOffset() / rowheight), y = -(GetYOffset() % rowheight); 
         i != lines.end() && y <= dest.h(); 
         ++i, y += rowheight)
      {   
         f.Textout(dest, i->c_str(), -xofst, y, fg, bg, 0);         
      }      
   }   
}


void TextArea::LoadLinesFromFile(char *filename) {
   ifstream f;
   f.open(filename, ios::in);
   
   lines.clear();
   while (f) {
      string s;
      getline(f, s);
      lines.push_back(s); 
   }
   
   f.close();

   CalculateRange();
}


void TextArea::SetText(const char *text) {
   lines.clear();
   AddText(text);
}


void TextArea::AddText(const char *text) {
   char *tok;
   char *copy;
   char *last;
   
   copy = new char[1+strlen(text)];
   strcpy(copy, text);
   
   tok = tokenize(copy, "\n", &last);
   while (tok) {
      string s = tok;
      lines.push_back(s);

      tok = tokenize(NULL, "\n", &last);
   }

   CalculateRange();
   
   delete [] copy;
}
