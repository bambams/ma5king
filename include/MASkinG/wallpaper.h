// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_WALLPAPER_H
#define      MASKING_WALLPAPER_H

#include "image.h"
#include "maskingdll.h"

namespace MAS {
/**
Similar to ClearScreen just that Wallpaper doesn't use a solid colour
to clear the canvas but a wallpaper bitmap which can be drawn in one of several styles.
Note that Wallpaper can be quite a lot slower than ClearScreen.
*/
class MASKING_DLL_DECLSPEC Wallpaper : public Image {
   public:
      /**
      These are the available wallpaper styles.
      The wallpaper bitmap can be blitted in the centre of
      the dialog's canvas or in its top left corner, it can be stretched over
      the canvas or it can be tiled in one of two ways. The first uses a simple tiling
      function while the other uses a special GUI tile function that only tile the
      central area of the bitmap and uses the edges to create a border around the tiled area.
      In styles that don't cover the entire screen the screen is first cleared to
      the background colour defined by the skin.
      The available wallpaper styles are:
<pre>
      CENTERED     - the wallpaper bitmap is centered on the screen
      TOPLEFT      - the wallpaper is drawn once in the top left corner of the screen
      STRETCHED    - the wallpaper is stretched to fit onto the screen
      TILED        - the wallpaper is tiled across the screen (default)
      TILED2       - the wallpaper is tiled using a special GUI tile function
</pre>
       */
       enum WallpaperStyle {
         UNDEFINED = -1,
         CENTERED,
         TOPLEFT,
         STRETCHED,
         TILED,
         TILED2
      };

   protected:
      /**
      The actual wallpaper style.
      */
      WallpaperStyle style;

      // Message functions

      /**
      Updates the size of the wallpaper when the dialog starts.
      */
      void MsgStart();
      /**
      Sets the default skin bitmap, style and background colour.
      */
      void MsgInitSkin();
      /**
      Updates the size of the widget to match the size of the parent dialog.
      */
      void UpdateSize();
      /**
      Draws the wallpaper on the canvas with the selected style.
      */
      void Draw(Bitmap &canvas);

   public:
      /**
      default constructor
      */
      Wallpaper();

      /**
      Sets the wallpaper style to the selected style.
      */
      virtual void SetStyle(WallpaperStyle s);

      /**
      return the current wallpaper style
      */
      virtual WallpaperStyle GetStyle();

      /**
       These Setup(...) functions are basically just shortcuts for Image::Setup() and SetStyle().
      */
      void Setup(int x, int y, int w, int h, int key, int flags, int i, WallpaperStyle s);

      /**
       See above
      */
      void Setup(int x, int y, int w, int h, int key, int flags, const char *file, WallpaperStyle s);

      /**
       See above
      */
      void Setup(int x, int y, int w, int h, int key, int flags, const Bitmap& bmp, WallpaperStyle s);

};
}

#endif      //MASKING_WALLPAPER_H
