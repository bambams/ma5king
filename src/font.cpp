////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "../include/MASkinG/font.h"
#include "../include/MASkinG/settings.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


extern MAS::Font* DefaultFont;


MAS::Font::Font()
{
   *this = *DefaultFont;
}


MAS::Font::Font(ALLEGRO_FONT *f)
{
   Create(f);
}


MAS::Font::Font(const char *f, int size, int flags)
{
   allocated = false;
   font = NULL;
   Load(f, size, flags);
}


MAS::Font::Font(const Font &f) {
   font = f.font;
   allocated = false;
}


MAS::Font& MAS::Font::operator=(const Font &f) {
   font = f.font;
   allocated = false;
   return *this;
}


MAS::Font::~Font() {
   Destroy();
}


void MAS::Font::Destroy() {
   if (!allocated)
      return;
   if (!font)
      return;

   al_destroy_font(font);
   allocated = false;
   *this = *DefaultFont;
}


void MAS::Font::Textout(Bitmap &bmp, const char *s, const Point &p, const Color &c, const Color &bg, int align) const {
   Textout(bmp, s, p.x(), p.y(), c, bg, align);
}


void MAS::Font::Textout(Bitmap &bmp, const char *s, int x, int y,
                        const ALLEGRO_COLOR &color,
                        const ALLEGRO_COLOR &bg,
                        int align) const {
   ALLEGRO_STATE state;
   al_store_state(&state, ALLEGRO_STATE_BLENDER | ALLEGRO_STATE_TARGET_BITMAP);

   al_set_target_bitmap(bmp);

   if (Color(bg) != Color::transparent) {
      int tw = al_get_text_width(font, s);
      int th = al_get_font_line_height(font);
      al_draw_filled_rectangle(x, y, x + tw, y + th, bg);
   }

   al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

   switch (align) {
      case 0:
         al_draw_text(font, color, x, y, 0, s);
      break;

      case 1:
         al_draw_text(font, color, x, y, ALLEGRO_ALIGN_RIGHT, s);
      break;

      case 2:
         al_draw_text(font, color, x, y, ALLEGRO_ALIGN_CENTRE, s);
      break;
   }

   al_restore_state(&state);
}


void MAS::Font::TextPrintf(Bitmap &bmp, int x, int y,
                           const ALLEGRO_COLOR &color,
                           const ALLEGRO_COLOR &bg,
                           int align, const char *format, ...) const {
   char buf[512];

   va_list ap;
   va_start(ap, format);
   vsnprintf(buf, sizeof(buf), format, ap);
   va_end(ap);

   Textout(bmp, buf, x, y, color, bg, align);
}


void MAS::Font::TextPrintf(Bitmap &bmp, const Point &p, const Color &color, const Color &bg,
                           int align, const char *format, ...) const {
   char buf[512];

   va_list ap;
   va_start(ap, format);
   vsnprintf(buf, sizeof(buf), format, ap);
   va_end(ap);

   Textout(bmp, buf, p, color, bg, align);
}


int MAS::Font::TextHeight() const {
   return al_get_font_line_height(font);
}


int MAS::Font::TextLength(const char *str) const {
   return al_get_text_width(font, str);
}


bool MAS::Font::IsFixed() const {
   return TextLength(".") == TextLength("O");
}


MAS::Font& MAS::Font::Create(ALLEGRO_FONT *f) {
   Destroy();
   font = f;
   allocated = false;
   return *this;
}


MAS::Font& MAS::Font::Create() {
   *this = *DefaultFont;
   return *this;
}


MAS::Font& MAS::Font::operator=(ALLEGRO_FONT *f) {
   return Create(f);
}


MAS::Error MAS::Font::Load(const char *file, int size, int flags) {
   ALLEGRO_PATH *p;
   p = al_create_path(file);

   if (!al_is_path_present(p)) {
      al_destroy_path(p);

      char *path = getenv("WINDIR");
      if (!path) {
         path = getenv("windir");
      }
      if (!path) {
         return Error(Error::NO_FILE);
      }

      ALLEGRO_PATH *p2;
      p2 = al_create_path(path);
      al_append_path_component(p2, "/fonts/");
      al_set_path_filename(p2, file);
      if (!al_is_path_present(p2)) {
         al_destroy_path(p2);
         return Error(Error::NO_FILE);
      }
      p = p2;
   }

   // possibly destroy the old font
   Destroy();
   const char* filename = al_path_cstr(p, ALLEGRO_NATIVE_PATH_SEP);

   font = al_load_font(filename, size, flags);
   allocated = true;

   al_destroy_path(p);

   // if the font wasn't loaded set the default font
   if (!font) {
      if (DefaultFont && this != DefaultFont) {
         *this = *DefaultFont;
         return Error(Error::LOAD_FILE);
      }
      else {
         TRACE("Error loading default font!");
         assert(0);
      }
   }

   return Error(Error::NONE);
}


////////////////////////////////////////////////////////////////////////////////
// GUITextout:
//   Wrapper function for drawing text to the screen, which interprets the
//   & character as an underbar for displaying keyboard shortcuts. Returns
//   the width of the output string in pixels.
int MAS::Font::GUITextoutEx(Bitmap &bmp, const char *s, int x, int y,
                            const ALLEGRO_COLOR &col1,
                            const ALLEGRO_COLOR &col2,
                            const ALLEGRO_COLOR &bg,
                            int align) const
{
   char tmp[1024];
   int hline_pos = -1;
   int len = 0;
   int in_pos = 0;
   int out_pos = 0;
   int pix_len, c;

   while (((c = s[in_pos]) != '\0') && (out_pos < (int)(sizeof(tmp)-1))) {
      if (c == '&') {
         in_pos++;
         c = s[in_pos];
         if (c == '&') {
            tmp[out_pos] = '&';
            out_pos++;
            in_pos++;
            len++;
         }
         else
            hline_pos = len;
      }
      else {
         tmp[out_pos] = c;
         out_pos++;
         in_pos++;
         len++;
      }
   }

   tmp[out_pos] = '\0';
   pix_len = TextLength(tmp);

   if (align == 1) {
      x -= pix_len;
   }
   else if (align == 2) {
      x -= pix_len/2;
   }

   if (bmp) {
      if (Color(col2) != Color::transparent) {
         Textout(bmp, tmp, x+1, y+1, col2, bg, 0);
         Textout(bmp, tmp, x, y, col1, Color::transparent, 0);
      }
      else {
         Textout(bmp, tmp, x, y, col1, bg, 0);
      }

      if (hline_pos >= 0) {
         c = tmp[hline_pos];
         tmp[hline_pos] = '\0';
         hline_pos = TextLength(tmp)+1;
         tmp[0] = c;
         tmp[1] = '\0';
         c = TextLength(tmp)-1;
         bmp.Hline(x+hline_pos, y+TextHeight(), x+hline_pos+c-1, col1);
      }
   }

   return pix_len;
}


////////////////////////////////////////////////////////////////////////////////
// Returns the length of a string in pixels, ignoring '&' characters.
int MAS::Font::GUITextLength(const char *s) const {
   Bitmap null;
   return GUITextoutEx(null, s, 0, 0, Color::black, Color::black, Color::black, 0);
}

int MAS::Font::GUITextoutEx(Bitmap &bmp, const char *text, const Point &p, const Color &col1, const Color &col2, const Color &bg, int align) const {
   return GUITextoutEx(bmp, text, p.x(), p.y(), col1, col2, bg, align);
}


void MAS::Font::GUITextout(Bitmap &bmp, const char *text, int x, int y,
                           const ALLEGRO_COLOR &col1,
                           const ALLEGRO_COLOR &col2,
                           const ALLEGRO_COLOR &bg, int align) const
{
   if (Color(col2) != Color::transparent) {
      Textout(bmp, text, x+1, y+1, col2, bg, align);
      Textout(bmp, text, x, y, col1, Color::transparent, align);
   }
   else {
      Textout(bmp, text, x, y, col1, bg, align);
   }
}

void MAS::Font::GUITextout(Bitmap &bmp, const char *text, const Point &p, const Color &col1, const Color &col2, const Color &bg, int align) const {
   GUITextout(bmp, text, p.x(), p.y(), col1, col2, bg, align);
}


/**
  returns number of characters of the string text that fit on this line, while performing word-wrapping.
  Maxwidth is the width of this line.  Preferredwidth is the actual width of that number of characters,
  so you can calculate the remaining space by substracting preferredwidth from maxwidth and align the text as you wish.

  treats \n characters as hard (=forced) line breaks, so even if more characters fit on a line it always stops at a \n.
*/
int MAS::Font::GetWrapPos(const char *text, int maxwidth, int &preferredwidth) const
{
    int ww = 0; // word width in pixels
    int wl = 0; // number of chars in word
    int lw = 0; // line width, excluding current word
    int ll = 0; // line length

    const char *pos = text;
    char temp[2] = {0, 0}; // used to calculate length of a single character
    bool quit = false;

    while (!quit)
    {
        if (*pos == '\0')
        {
            if (lw + ww <= maxwidth || lw == 0)
            {
                lw += ww;
                ll += wl; // don't count 0 itself
            }
            quit = true;
        } else if (*pos == '\n') // newline
        {
            if (lw + ww <= maxwidth || lw == 0)
            {
                lw += ww;
                ll += wl + 1; // do count '\n' itself
            }
            quit = true;
        } else if (*pos <= 32) // some kind of space, not newline or \0
        {
            // end of word
            if (lw + ww <= maxwidth || lw == 0)
            {
                lw += ww;
                ll += wl;
                temp[0] = *pos;
                ww = GUITextLength (temp);
                wl = 1;
                pos++;
            }
            else
            {
                quit = true;
            }
        } else
        {
            temp[0] = *pos;
            ww += GUITextLength (temp);
            wl++;
            pos++;
        }
    }
    preferredwidth = lw;
    return ll;
}

/**
   simple wrapper for when you don't need the preferredwidth parameter
*/
int MAS::Font::GetWrapPos(const char *text, int maxwidth) const
{
    int temp;
    return GetWrapPos (text, maxwidth, temp);
}

/**
  returns number of whitespace at the start of text. Doesn't count \0 or \n as whitespace.
*/
int MAS::Font::SkipWhiteSpace (const char *text) const
{
    int result = 0;
    const char *pos = text;
    while ((*pos <= 32) && (*pos > 0) && (*pos != '\n')) { pos++; result++; }
    return result;
}

/**
  counts number of lines needed to print text in an area with width = maxwidth.
  empty string is 0 lines. Only whitespace is 1 line.

  usefull for calculating the height of a textbox
*/
int MAS::Font::CountLines (const char *text, int maxwidth) const
{
    const char *pos = text;
    int result = 0;
    while (*pos != 0)
    {
        result++; // count a line
        pos += SkipWhiteSpace (pos);
        if (*pos != 0)
        {
            pos += GetWrapPos (pos, maxwidth);
        }
    }
    return result;
}

/**
  prints text word-wrapped on a column with a specified width but unspecified height.
  note: hal = horizontal alignment
  performs horizontal alignment but not vertical.
*/
void MAS::Font::ColumnPrint (MAS::Bitmap &bmp, const char *text,
                             const ALLEGRO_COLOR &fg,
                             const ALLEGRO_COLOR &bg,
                             const ALLEGRO_COLOR &textMode,
                             int x, int y, int w, int hal) const
{
    char *pos_orig = strdup(text);
    char *pos = pos_orig;
    int xco, yco = y;
    switch (hal)
    {
        case 0:   xco = x;            break;
        case 1:   xco = x + w;        break;
        case 2:
        default:   xco = x + (w / 2); break;
    };

    while (*pos != 0)
    {
        pos += SkipWhiteSpace (pos);
        if (*pos != 0)
        {
            char temp;
            int n = GetWrapPos (pos, w);
            int n2 = n;
            if (pos[n-1] == '\n')
               n2 = n-1; // little hack to skip trailing newlines
            temp = pos[n2];
            pos[n2] = 0;
            GUITextout(bmp, pos, xco, yco, fg, bg, textMode, hal);
            pos[n2] = temp;
            pos += n;
            yco += TextHeight();
        }
    }
    free(pos_orig);
}

/**
   prints text word-wrapped in a box with specified width and height. Performs horizontal and vertical alignment.
*/
void MAS::Font::BoxPrint (MAS::Bitmap &bmp, const char *text, const ALLEGRO_COLOR &fg, const ALLEGRO_COLOR &bg, const ALLEGRO_COLOR &textMode, int x, int y, int w, int h, int hal, int val) const
{
    int minh = TextHeight() * CountLines (text, w);
    int yco;
    switch (val)
    {
        case 0: yco = 0;                break;
        case 1: yco = y + h - minh;     break;
        case 2:
        default: yco = y + (h - minh) / 2; break;
    }
    ColumnPrint (bmp, text, fg, bg, textMode, x, yco, w, hal);
}
