// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_CURSOR_H
#define      MASKING_CURSOR_H

#include <allegro5/allegro.h>
#include <vector>
#include "bitmap.h"
#include "point.h"
#include "color.h"
#include "maskingdll.h"

namespace MAS {
class Mouse;
class Skin;

/**
   Represents an animated mouse cursor with a Windows XP style shadow.
*/
class MASKING_DLL_DECLSPEC Cursor {
   protected:
      /**
         A vector of mouse sprites.
         These are effectively frames of the animated sprite.
      */
      std::vector<Bitmap *> sprite;

      /**
         A vector of mouse shadows.
         There's a matching shadow for each frame of the sprite animation.
      */
      std::vector<Bitmap *> shadow;

      /**
         Effectively the intensity of the blur effect on the shadow sprites.
         It is also the number of pixels the shadow sprite must be larger than
         the main sprite at every edge, hence the name of the variable.
      */
      static int PADDING;

      /**
         The color of the cursor shadow.
         Typically this is some shade of gray. A darker shade of gray produces
         a thicker shadow. A different color might produce a glow effect rather
         then a shadow.
      */
      static Color shadowColor;

      /**
         The offset of the shadow relative to the main sprite.
         Typically the shadow is just a little to the right of the main sprite.
      */
      static Point shadowOffset;

      /**
         The sprites focus point.
      */
      Point focus;

      /** */int frameCount;
      /** */float frame;
      /**
         Animation specific variables.
         The number of frames, the current frame and the interval bewteen frames in miliseconds.
      */
      float interval;

      /**
         Blurs the passed bitmap.
         Radius is effectively the depth of the blur effect. Good values are between 1 and 4.
      */
      void Blur(Bitmap &bmp, int radius);

      /**
         Creates a shadow sprite for every main cursor sprite of the animated cursor.
         It does so by creating a new sprite the size of the main sprite and adds a
         border the size of the blur depth on each side. It then clears the shadow
         sprite to the shadow color and makes a black and white mask in the alpha
         channel with black being the transparent color. Then it blurs the alpha
         channel. When the cursor is rendered the shadow is drawn with draw_trans_sprite().
      */
      void MakeShadow();

      /**
         Reads cursor specific data from the skin config file.
      */
      static void ReadSkinData(ALLEGRO_CONFIG *config);

   public:
      Cursor();
      virtual ~Cursor();

      /**
         Returns the current mouse sprite.
      */
      Bitmap& Sprite() const;

      /**
         Returns the current mouse shadow.
      */
      Bitmap& Shadow() const;

      /**
         Creates a new mouse cursor with a shadow.
         This function creates the mouse cursor from the passed bitmap.
         The second parameter is the number of frames in the bitmap. If the number of
         frames is greater than 1, an animated cursor will be created by horizontally
         cutting   the bitmap into n equally sized frames.
      */
      Cursor& Create(const Bitmap &bmp, int n=1);

      /**
         Creates a new mouse cursor with a shadow.
         This function creates the mouse cursor from the default Allegro sprite
         or whatever sprite is currently loaded as the Allegro mouse sprite.
      */
      Cursor& Create();

      /**
         Destroys the cursor.
         After this function is called the cursor is invalid and shouldn't be
         used anymore.
      */
      void Destroy();

      /** */void SetFocus(const Point &p);
      /** */void SetFocus(int x, int y);
      /**
         Get and set the focus point of cursor.
         The focus point is the point where the mouse is actually positioned relative
         to the top left of the cursor sprite. For a regular mouse arrow this is
         point [0,0] but for many other cursor sprites this may be at a different location.
      */
      const Point& GetFocus();

      /**
         Effectively sets the speed of the mouse cursor animation.
         The interval is the time between frames of animation in miliseconds.
      */
      void SetAnimationInterval(int i);

      /**
         Updates the animation.
         This is called by the GUI core once every frame.
      */
      void Update();

      friend class MASKING_DLL_DECLSPEC Mouse;
      friend class MASKING_DLL_DECLSPEC Skin;
};
}

#endif      //MASKING_CURSOR_H
