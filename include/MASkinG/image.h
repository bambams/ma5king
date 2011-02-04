// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_IMAGE_H
#define      MASKING_IMAGE_H

#include "widget.h"
#include "dialog.h"
#include "maskingdll.h"

namespace MAS {

/**
Image is a class for putting static bitmaps on the screen.
It is capable of loading its bitmap from a file on disk or from the skin and it can draw masked
bitmaps with transparent areas that can optionally be filled with a user selected colour.
In previous version of MASkinG this widget was called MASBitmap but the name had to be changed
for obvious reasons.
*/
class MASKING_DLL_DECLSPEC Image : public Widget {
   protected:
      /**
      Tracks whether the widget has allocated its own memmory for the bitmap or if it just made a pointer.
      */
      bool allocated;

      /**
      The actual bitmap.
      */
      Bitmap bmp;

      /**
      The background colour.
      */
      Color bg;

      /**
      Optionally clears the canvas to the background colour and draws the bitmap onto the canvas.
      */
      virtual void Draw(Bitmap &canvas);

      /**
      Adjusts the size of the widget to match the size of the bitmap.
      */
      virtual void UpdateSize();

   public:
      /**
      default constructor
      */
      Image();

       /**
      default destructor.
      */
      ~Image();

      /**
      Sets the images bitmap that is to be drawn. The bitmap can either be loaded from a file (the format must be BMP, PCX, TGA or LBM), copied from an existing bitmap or loaded from the skin bitmap array. Note that the size of the widget is automatically adjusted to the size of the bitmap if the D_AUTOSIZE flag is set which it is unless you explicitly turn it off.
      */
      virtual void SetBitmap(const char *file);
      /**
      See SetBitmap (const char *file)
      */
      virtual void SetBitmap(const Bitmap& bmp);
      /**
      See SetBitmap (const char *file)
      */
      virtual void SetBitmap(int i);
      /**
      Returns the widget's bitmap.
      */
      Bitmap &GetBitmap();
      /**
      The color with which the canvas is filled prior to drawing the bitmap. This is obviously relevant only when masked drawing is turned on or the bitmap is smaller than the widget. Even if masked is on it is not necessary to set a background colour, you should however do that if you plan to dynamically change the widget's bitmap during the execution of the program. The default colour is -1 (i.e. no colour).
      */
      virtual void SetBackgroundColor(const Color& bg);
      /**
      A shortcut function for Widget::Setup(), SetBitmap(), SetMasked() and SetBackgroundColor().
      */
      virtual void Setup(int x, int y, int w, int h, int key, int flags, int i, bool masked = false, const Color& = Color::transparent);
      /**
      See above
      */
      virtual void Setup(int x, int y, int w, int h, int key, int flags, const char *file, bool masked = false, const Color& = Color::transparent);
      /**
      See above
      */
      virtual void Setup(int x, int y, int w, int h, int key, int flags, const Bitmap& bmp, bool masked = false, const Color& = Color::transparent);
};
}

#endif      //MASKING_IMAGE_H
