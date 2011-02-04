// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_FONT_H
#define      MASKING_FONT_H

#include "bitmap.h"
#include "point.h"
#include "color.h"
#include "error.h"
#include "maskingdll.h"

struct ALLEGRO_FONT;

namespace MAS {
/**
   Class for handling fonts.
   This class used to be a wrapper for the two kinds of allegro font but this
   is not necessary any more with A5 because font addon offers an unufied API
   for both bitmaped and TrueType fonts. There are also other text output
   functions specifically meant for GUI purposes.
*/
class MASKING_DLL_DECLSPEC Font {
   protected:
      /**
         The wrapped font.
      */
      ALLEGRO_FONT *font;

      /**
         Did we actually allocate memory or did we just make a pointer?
         This variable is set to true if the object allocated memory for
         the font itself. If it did then it will be freed when the object
         is destroyed or a new font is loaded, otherwise the "owner" of the
         underlying data is responsible for freeing it.
      */
      bool allocated;

   public:
      /**
         Default constructor.
         Creates a new font and initializes it with the default Allegro 8x8 font
         or whatever is curently loaded in <i>font</i>.
      */
      Font();

      /**
         Constructor for making a font with an existing ALLEGRO_FONT.
         Doesn't copy the data, only makes a pointer!
      */
      Font(ALLEGRO_FONT *f);

      /**
         Constructor for making a font by loading it from a file.
         See below for a list of supported formats.
      */
      Font(const char *f, int size = 8, int flags = 0);

      /**
         Copy constructor.
      */
      Font(const Font& f);

      virtual ~Font();

      /** */Font& Create();
      /**
         Create a font from either the default font or an existing FONT or ALFONT_FONT.
         If you create a Font from and existing font, the font data doesn't get copied
         as you might expect but only a pointer is made!
      */
      Font& Create(ALLEGRO_FONT *f);

      /** */Font& operator=(ALLEGRO_FONT *f);
      /**
         Assignment operators.
         These create a font from an existing font by making a pointer (not an actual copy!)
      */
      Font& operator=(const Font &f);

      /**
         Attempts to load a new font from a disk file.
         If unsuccessull it sets itself to the default Allegro 8x8 font. The file can be
         in one of the following formats:
<pre>
      - bitmap font (either BMP, PCX or TGA)
      - any of the formats supported by TTF addon (True Type)
</pre>
      */
      Error Load(const char *f, int size = 8, int flags = 0);

      /**
         Destroys the font and sets it to the default Allegro 8x8 font.
      */
      void Destroy();

      /** */void Textout(Bitmap &bmp, const char *s, const Point &p, const Color &c, const Color &bg, int align) const;
      /** */void TextPrintf(Bitmap &bmp, const Point &p, const Color &c, const Color &bg, int align, const char *format, ...) const;
      /** */void Textout(Bitmap &bmp, const char *s, int x, int y, const ALLEGRO_COLOR &c, const ALLEGRO_COLOR &bg, int align) const;
      /**
         Wrappers for regular Allegro text output functions.
         They take the same argument list execpt for the font (obviously) and
         the align parameter. Align may be 0 for left alignment 1 for right
         alignment and 2 for centered text.
      */
      void TextPrintf(Bitmap &bmp, int x, int y, const ALLEGRO_COLOR &c, const ALLEGRO_COLOR &bg, int align, const char *format, ...) const;

      /** */void GUITextout(Bitmap &bmp, const char *text, const Point &p, const Color &col1, const Color &col2, const Color &bg, int align) const;
      /** */void GUITextout(Bitmap &bmp, const char *text, int x, int y, const ALLEGRO_COLOR &col1, const ALLEGRO_COLOR &col2, const ALLEGRO_COLOR &bg, int align) const;
      /** */int GUITextoutEx(Bitmap &bmp, const char *text, const Point &p, const Color &col1, const Color &col2, const Color &bg, int align) const;
      /**
         Special GUI text output.
         Same as the normal text output functions except that the second color
         is used as a text shadow. The functions appended with Ex also interpret
         the '&' character as an underscore and render the next character with a
         horizontal line below it and return the length in pixels of actually
         renderd text.
      */
      int GUITextoutEx(Bitmap &bmp, const char *text, int x, int y, const ALLEGRO_COLOR &col1, const ALLEGRO_COLOR &col2, const ALLEGRO_COLOR &bg, int align) const;

      /**
         Returns the length of the text as printed with GUITextoutEx().
      */
      int GUITextLength(const char *text) const;

      /**
         Prints text word-wrapped on a column with a specified width but unspecified height.
         note: hal = horizontal alignment. Performs horizontal alignment but not vertical.
      */
      void ColumnPrint(Bitmap &bmp, const char *text, const ALLEGRO_COLOR &fg, const ALLEGRO_COLOR &bg, const ALLEGRO_COLOR &textMode, int x, int y, int w, int hal) const;

      /**
         Prints text word-wrapped in a box with specified width and height.
         Performs horizontal and vertical alignment.
      */
      void BoxPrint(Bitmap &bmp, const char *text, const ALLEGRO_COLOR &fg, const ALLEGRO_COLOR &bg, const ALLEGRO_COLOR &textMode, int x, int y, int w, int h, int hal, int val) const;

      /**
         Returns the height of the font.
      */
      int TextHeight() const;

      /**
         Returns the length of the string in pixels.
      */
      int TextLength(const char *str) const;

      /**
         Returns true if the font supports several fixed sizes.
         Only a few font formats support this.
      */
      bool IsFixed() const;

      /**
         Number of characters that fit on a line.
         Returns number of characters of the string text that fit on this line,
         while performing word-wrapping. Maxwidth is the width of this line.
         Preferredwidth is the actual width of that number of characters, so
         you can calculate the remaining space by substracting preferredwidth
         from maxwidth and align the text as you wish. Treats \n characters as
         hard (=forced) line breaks, so even if more characters fit on a line
         it always stops at a \n.
      */
      int GetWrapPos(const char *text, int maxwidth, int &preferredwidth) const;

      /**
         Simple wrapper for when you don't need the preferredwidth parameter.
      */
      int GetWrapPos(const char *text, int maxwidth) const;

      /**
         Returns number of whitespace at the start of text.
         Doesn't count \0 or \n as whitespace.
      */
      int SkipWhiteSpace(const char *text) const;

      /**
         Counts number of lines needed to print text in an area with width = maxwidth.
         Empty string is 0 lines. Only whitespace is 1 line. Usefull for calculating
         the height of a textbox.
      */
      int CountLines(const char *text, int maxwidth) const;
};
}

#endif      //MASKING_FONT
