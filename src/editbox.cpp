////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/editbox.h"
#include "../include/MASkinG/clipboard.h"
#include <stdio.h>

#define MIN(x,y)     (((x) < (y)) ? (x) : (y))
#define MAX(x,y)     (((x) > (y)) ? (x) : (y))
#define CLAMP(x,y,z) MAX((x), MIN((y), (z)))

#ifdef DrawText
#undef DrawText
#endif

static void uinsert(char *s, int index, int c)
{
   if (index < 0) {
      index = strlen(s) - index;
   }

   memmove(s + index + 1, s + index, strlen(s) - index + 1);
   s[index] = c;
}

static void uremove(char *s, int index)
{
   if (index < 0) {
      index = strlen(s) - index;
   }

   memmove(s + index, s + index + 1, strlen(s) - index);
}


MAS::EditBox::EditBox()
:Label(),
   maxLength(0),
   cursorpos(0),
   offsetpos(0),
   selStart(0),
   selEnd(0),
   firstSel(0),
   oldSel(0)
{
   ClearFlag(D_AUTOSIZE);
}


void MAS::EditBox::Setup(int x, int y, int w, int h, int key, int flags, const char *text, int len) {
   Widget::Setup(x, y, w, h, key, flags);
   if (len >= 0) {
      Setup(text, len);
   }
   else {
      Setup(text, 256);
   }
}


void MAS::EditBox::MsgInitSkin() {
   if (GetBitmapIndex() == -1) SetBitmap(Skin::PANEL_SUNKEN);
   if (GetSampleIndex(Skin::SAMPLE_GOTFOCUS) == -1) SetSample(Skin::SAMPLE_GOTFOCUS, Skin::SAMPLE_GOTFOCUS);
   if (GetSampleIndex(Skin::SAMPLE_LOSTFOCUS) == -1) SetSample(Skin::SAMPLE_LOSTFOCUS, Skin::SAMPLE_LOSTFOCUS);
   if (GetSampleIndex(Skin::SAMPLE_ACTIVATE) == -1) SetSample(Skin::SAMPLE_ACTIVATE, Skin::SAMPLE_ACTIVATE);
   if (GetSampleIndex(Skin::SAMPLE_KEY) == -1) SetSample(Skin::SAMPLE_KEY, Skin::SAMPLE_KEY);
   ClearFlag(D_AUTOSIZE);
   for (int i=0; i<4; i++) {
      if (GetFontColor(i) == Color::transparent) SetFontColor(skin->fcol[Skin::INFO_EDITBOX][i], skin->scol[Skin::INFO_EDITBOX][i], i);
      if (GetFontIndex(i) == -1) SetFont(skin->fnt[Skin::INFO_EDITBOX][i], i);
   }
   UpdateSize();
   Widget::MsgInitSkin();
}


int MAS::EditBox::FindCursorPos() {
   int p;
   int x1 = 4;
   char s[12];
   sprintf(s, ".");
   int l = strlen(text);

   //int mx = GetMousePos().x() + skin->GetCursor(Skin::MOUSE_TEXTSELECT).GetFocus().x();
   int mx = GetMousePos().x() + GetParent()->GetMouse()->GetCursor()->GetFocus().x();
   Font f = GetFont(HasFocus() ? Skin::FOCUS : Skin::NORMAL);

   for (p = offsetpos; p < l; p++) {
      s[0] = text[p];
      x1 += f.TextLength(s);
      if (x1 > mx) {
         break;
      }
   }

   return p;
}


void MAS::EditBox::MsgLPress() {
   int oldCursorpos = cursorpos;

   Label::Select();
   selStart = FindCursorPos();
   selEnd = selStart;
   cursorpos = selStart;
   firstSel = selStart;
   if (cursorpos != oldCursorpos) {
      UpdateOffset();
   }
   oldSel = 0;
   SetFlag(D_PRESSED);

   Widget::MsgLPress();
}


void MAS::EditBox::MsgMousemove(const Point &d) {
   if (Flags() & D_PRESSED) {
      selEnd = cursorpos = FindCursorPos();
      if (selEnd <= selStart) {
         selStart = selEnd;
         selEnd = firstSel;
      }
      if (selEnd - selStart != oldSel) {
         oldSel = selEnd - selStart;
      }
      UpdateOffset();
   }

   Widget::MsgMousemove(d);
}


void MAS::EditBox::MsgLRelease() {
   if (Flags() & D_PRESSED) {
      cursorpos = FindCursorPos();
      if (selStart == selEnd) {
         Label::Deselect();
      }
      ClearFlag(D_PRESSED);
      UpdateOffset();
   }

   Widget::MsgLRelease();
}


void MAS::EditBox::MsgDClick() {
   selStart = 0;
   selEnd = strlen(text);
   Label::Select();
   Widget::MsgDClick();
}


bool MAS::EditBox::MsgChar(int c) {
   bool ret = true;
   bool has_changed = false;
   int l = strlen(text);
   int ch = c>>8;
   int mod = c>>16;

   switch (ch) {
      case ALLEGRO_KEY_LEFT: {
         if (cursorpos > 0) {
            cursorpos--;
            if (mod & ALLEGRO_KEYMOD_SHIFT) {
               if (!Selected()) {
                  Label::Select();
                  selStart = cursorpos+1;
                  selEnd = selStart;
               }
               if (selStart > cursorpos) {
                  selStart = cursorpos;
               }
               else {
                  selEnd = cursorpos;
               }
            }
            else {
               Label::Deselect();
            }
         }
         else if (!(mod & ALLEGRO_KEYMOD_SHIFT)) {
            Label::Deselect();
         }
      }
      break;

      case ALLEGRO_KEY_RIGHT: {
         if (cursorpos < l) {
            cursorpos++;
            if (mod & ALLEGRO_KEYMOD_SHIFT) {
               if (!Selected()) {
                  Label::Select();
                  selStart = cursorpos-1;
                  selEnd = selStart;
               }
               if (selEnd < cursorpos) {
                  selEnd = cursorpos;
               }
               else {
                  selStart = cursorpos;
               }
            }
            else {
               Label::Deselect();
            }
         }
         else if (!(mod & ALLEGRO_KEYMOD_SHIFT)) {
            Label::Deselect();
         }
      }
      break;

      case ALLEGRO_KEY_HOME: {
         if (mod & ALLEGRO_KEYMOD_SHIFT) {
            if (!Selected()) {
               selStart = 0;
               selEnd = cursorpos;
               Label::Select();
            }
            else {
               selEnd = selStart;
               selStart = 0;
               Label::Select();
            }
            cursorpos = 0;
         }
         else {
            Label::Deselect();
            cursorpos = 0;
         }
      }
      break;

      case ALLEGRO_KEY_END: {
         if (mod & ALLEGRO_KEYMOD_SHIFT) {
            if (!Selected()) {
               selStart = cursorpos;
               selEnd = l;
               Label::Select();
            }
            else {
               selStart = selEnd;
               selEnd = l;
               Label::Select();
            }
            cursorpos = l;
         }
         else {
            Label::Deselect();
            cursorpos = l;
         }
      }
      break;

      case ALLEGRO_KEY_DELETE: {
         if (!TestFlag(D_READONLY)) {
            if (Selected()) {
               int i=0;
               for (int p=selEnd; text[p]; p++) {
                  text[selStart+i] = text[p];
                  i++;
               }
               text[selStart+i] = '\0';
               cursorpos = selStart;
               Label::Deselect();
               PlaySample(Skin::SAMPLE_KEY);
            }
            else if (cursorpos < l) {
               uremove(text, cursorpos);

               PlaySample(Skin::SAMPLE_KEY);
               has_changed = true;
            }
         }
         else {
            ret = false;
         }
      }
      break;

      case ALLEGRO_KEY_BACKSPACE: {
         if (!TestFlag(D_READONLY)) {
            if (Selected()) {
               int i=0;
               for (int p=selEnd; text[p]; p++) {
                  text[selStart+i] = text[p];
                  i++;
               }
               text[selStart+i] = '\0';
               cursorpos = selStart;
               Label::Deselect();
               PlaySample(Skin::SAMPLE_KEY);
            }
            else if (cursorpos > 0)      {
               --cursorpos;
               uremove(text, cursorpos);
               if (cursorpos <= offsetpos) offsetpos = 0;

               PlaySample(Skin::SAMPLE_KEY);
               has_changed = true;
            }
         }
         else {
            ret = false;
         }
      }
      break;

      case ALLEGRO_KEY_ENTER: {
         Label::Deselect();
         PlaySample(Skin::SAMPLE_ACTIVATE);
         GetParent()->HandleEvent(*this, callbackID >= 0 ? callbackID : MSG_ACTIVATE);
      }
      break;

      case ALLEGRO_KEY_X: {
         if ((mod & ALLEGRO_KEYMOD_SHIFT) && Selected()) {
            // copy selected text to clipboard
            char *tmp = new char[selEnd - selStart + 2];
            strncpy(tmp, text+selStart, selEnd - selStart + 1);
            Clipboard cb;
            cb.SetText(tmp);
            delete [] tmp;

            // delete selected text
            if (!TestFlag(D_READONLY)) {
               int i=0;
               for (int p=selEnd; text[p]; p++) {
                  text[selStart+i] = text[p];
                  i++;
               }
               text[selStart+i] = '\0';
               cursorpos = selStart;
               Label::Deselect();
            }
         }
         else {
            ret = false;
         }
      }
      break;

      case ALLEGRO_KEY_C: {
         if ((mod & ALLEGRO_KEYMOD_SHIFT) && Selected()) {
            // copy selected text to clipboard
            char *tmp = new char[selEnd - selStart + 2];
            strncpy(tmp, text+selStart, selEnd - selStart + 1);
            Clipboard cb;
            cb.SetText(tmp);
            delete [] tmp;
         }
         else {
            ret = false;
         }
      }
      break;

      case ALLEGRO_KEY_V: {
         // get text from clipboard
         Clipboard cb;
         if ((mod & ALLEGRO_KEYMOD_SHIFT) && cb.GetText() && !TestFlag(D_READONLY)) {
            // delete selected text
            if (Selected()) {
               int i=0;
               for (int p=selEnd; text[p]; p++) {
                  text[selStart+i] = text[p];
                  i++;
               }
               text[selStart+i] = '\0';
               cursorpos = selStart;
               Label::Deselect();
            }

            // copy text from clipboard
            for (size_t i=0; i<cb.bufSize-1; i++) {
               if (cursorpos < maxLength) {
                  uinsert(text, cursorpos, cb.buffer[i]);
                  cursorpos++;
               }

               UpdateOffset();
            }
         }
         else {
            ret = false;
         }
      }
      break;

      default: {
         ret = false;
      }
      break;

   };

   UpdateOffset();

   Widget::MsgChar(c);
   return ret;
}


bool MAS::EditBox::MsgUChar(int c) {
   bool ret = false;
   int l = strlen(text);
   if (c >= ' ' && !TestFlag(D_READONLY)) {
      if (Selected()) {
         int i=0;
         for (int p=selEnd; text[p]; p++) {
            text[selStart+i] = text[p];
            i++;
         }
         text[selStart+i] = '\0';
         cursorpos = selStart;
         Label::Deselect();
      }
      if (l < maxLength) {
         uinsert(text, cursorpos, c);
         cursorpos++;
         PlaySample(Skin::SAMPLE_KEY);
      }

      UpdateOffset();
      ret = true;
   }

   Widget::MsgUChar(c);
   return ret;
}


void MAS::EditBox::MsgGotmouse() {
   if (!TestFlag(D_READONLY)) {
      SetCursor(Skin::MOUSE_TEXTSELECT);
   }
   Widget::MsgGotmouse();
}


void MAS::EditBox::MsgLostmouse() {
   if (!TestFlag(D_READONLY)) {
      SetCursor(Skin::MOUSE_NORMAL);
   }
   Widget::MsgLostmouse();
}


void MAS::EditBox::Setup(const char *tex, int len) {
   int texLen = 0;
   if (tex && strcmp(tex, "") != 0) texLen = strlen(tex);

   int l = len;
   if (l < texLen) l = texLen;
   if (l <= 0) l = 256;

   // Make room and copy the text
   if (text) delete [] text;
   text = new char[6*(l+1)];

   if (tex != NULL) strcpy(text, tex);
   else strcpy(text, "");

   maxLength = l;
   //usetat(text, uoffset(text, -1), '\0');

   cursorpos = strlen(text);
   offsetpos = 0;
}


void MAS::EditBox::SetText(const char *tex, int len) {
   const char *tmp_txt = "";
   if (tex != NULL) tmp_txt = tex;

   if (len == -1) {
      len = maxLength == 0 ? 256 : maxLength;
   }
   if (len != -1 && len != maxLength) {
      Setup(tmp_txt, len);
   }
   else {
      // Copy the stuff over
      strncpy(text, tmp_txt, maxLength);
      cursorpos = strlen(text);
   }

   offsetpos = 0;
}


void MAS::EditBox::UpdateOffset() {
   int lastchar;
   Bitmap null;
   while (true) {
      // Try with the current offset, but don't really draw
      lastchar = DrawText(offsetpos, null);

      // need to decrease offset?
      if (offsetpos > cursorpos) {
         offsetpos--;
         if (offsetpos < 0) {
            offsetpos = 0;
            break;
         }
      }
      // need to increase offset?
      else if (cursorpos > lastchar+1) {
         offsetpos++;
      }
      // or is it just right?
      else {
         break;
      }
   }
}


void MAS::EditBox::Draw(Bitmap &canvas) {
   // Draw the text on the canvas
   GetBitmap().TiledBlit(canvas, 0, 0, 0, 0, w(), h(), 1, 1);
   DrawText(offsetpos, canvas);

   // Possibly draw the dotted rectangle
   if (skin->drawDots && HasFocus()) {
      canvas.DrawDottedRect(2, 2, w()-3, h()-3, skin->c_font);
   }
}


// Helper to draw the editable text
// Returned value is last character number drawn
int MAS::EditBox::DrawText(int offsetpos, Bitmap &canvas) {
   // get the button state
   int state = Disabled() ? 2 : (HasFocus() ? 3 : 0);

   // get the button colors and font
   Color fg = GetFontColor(state);
   Color bg = GetShadowColor(state);
   Font f = GetFont(state);
   Color fgs = GetFontColor(Skin::SELECT);
   Color bgs = GetShadowColor(Skin::SELECT);

   int   x1 = 4;
   int   y1 = (h()>>1) - (f.TextHeight()>>1) + 1;
   int   xx = w()-10;
   char s[12];
   sprintf(s, ".");
   int   i = offsetpos;
   int   len;
   int cursorx = -1;
   int ii;

   // Assume that we will go to the end
   int there_is_more = 0;

   // Possibly remember where the cursor should go
   if (i == cursorpos) cursorx = x1;

   while (text[i] != 0) {
      // Find the length of the character
      s[0] = text[i];
      len = f.TextLength(s);

      // Draw the character, possibly with cursor
      if (canvas) {
         if (Selected() && i>=selStart && i<selEnd) {
            if (HasFocus()) {
               f.GUITextout(canvas, s, x1, y1, fgs, bgs, skin->c_select, 0);
            }
            else {
               f.GUITextout(canvas, s, x1, y1, fg, bg, skin->c_deselect, 0);
            }
         }
         else {
            f.GUITextout(canvas, s, x1, y1, fg, bg, Color::transparent, 0);
         }
      }

      x1 += len;

      // Stop drawing if we are out of bounds
      if (x1 > xx) {
         // Remember that there is more to draw
         there_is_more = 1;
         break;
      }
      i++;

      // Possibly remember where the cursor should go
      if (i == cursorpos) cursorx = x1;
   }
   // If we ran off the end before finding the cursor, then its at the end
   if ((there_is_more) && (cursorx < 0)) cursorx = x1;

   // Draw the cursor if we have the focus
   if ((cursorx >= 0) && HasFocus() && canvas) {
      for (ii=0; ii<f.TextHeight(); ii++) {
         canvas.Putpixel(cursorx-1, y1+ii, fg);
      }
   }
   return i;
}


void MAS::EditBox::Select() {
   MsgDClick();
}


void MAS::EditBox::Deselect() {
   selStart = selEnd = firstSel = oldSel = 0;
   offsetpos = cursorpos = 0;
   Label::Deselect();
}


void MAS::EditBox::ScrollTo(int offset) {
   if (!text) return;
   Deselect();
   offset = CLAMP(0, offset, (int)strlen(text));
   cursorpos = offset;
   UpdateOffset();
}


bool MAS::EditBox::MsgWantfocus() {
   return true;
}


int MAS::EditBox::GetMaxLength() {
   return maxLength;
}
