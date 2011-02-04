// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_ANIMATOR_H
#define      MASKING_ANIMATOR_H

#include <allegro5/allegro.h>
#include "bitmap.h"
#include "maskingdll.h"

namespace MAS {
/**
   Provides basic animation functionality for the GUI. There are currently 26
   different available animation not all of which are really that impressive
   or usefull. Animatiions provided by this class are mostly used as
   transition effects for certain widget, for example for buttons changing
   from one state to another and so on.
*/
class MASKING_DLL_DECLSPEC Animator {
   public:
      /**
         Animation types.
         These are all currently available animations. The names are quite
         self explanatory in most cases I think.
<pre>
            NONE
            BLEND
            SLIDE_DOWN
            SLIDE_UP
            SLIDE_LEFT
            SLIDE_RIGHT
            ROLL_DOWN
            ROLL_UP
            ROLL_LEFT
            ROLL_RIGHT
            BLINDS_DOWN
            BLINDS_UP
            BLINDS_LEFT
            BLINDS_RIGHT
            OPEN_VERTICAL
            OPEN_HORIZONTAL
            OPEN_FROM_CENTRE
            ROLL_OPEN_VERTICAL
            ROLL_OPEN_HORIZONTAL
            ROLL_OPEN_FROM_CENTRE
            CLOSE_VERTICAL
            CLOSE_HORIZONTAL
            CLOSE_TO_CENTRE
            ROLL_CLOSE_VERTICAL
            ROLL_CLOSE_HORIZONTAL
            ROLL_CLOSE_TO_CENTRE
            ROLL_CLOSE_TO_CENTRE2
</pre>
      */
      enum AnimationType {
            NONE,                  // 0
            BLEND,                  // 1
            SLIDE_DOWN,               // 2
            SLIDE_UP,               // 3
            SLIDE_LEFT,               // 4
            SLIDE_RIGHT,            // 5
            ROLL_DOWN,               // 6
            ROLL_UP,               // 7
            ROLL_LEFT,               // 8
            ROLL_RIGHT,               // 9
            BLINDS_DOWN,            // 10
            BLINDS_UP,               // 11
            BLINDS_LEFT,            // 12
            BLINDS_RIGHT,            // 13
            OPEN_VERTICAL,            // 14
            OPEN_HORIZONTAL,         // 15
            OPEN_FROM_CENTRE,         // 16
            ROLL_OPEN_VERTICAL,         // 17
            ROLL_OPEN_HORIZONTAL,      // 18
            ROLL_OPEN_FROM_CENTRE,      // 19
            CLOSE_VERTICAL,            // 20
            CLOSE_HORIZONTAL,         // 21
            CLOSE_TO_CENTRE,         // 22
            ROLL_CLOSE_VERTICAL,      // 23
            ROLL_CLOSE_HORIZONTAL,      // 24
            ROLL_CLOSE_TO_CENTRE,      // 25
            ROLL_CLOSE_TO_CENTRE2      // 26
      };

   protected:
      /**
         The buffer that is actually being animated. The user should draw
         onto this buffer, then call the Draw() function to draw to the canvas.
      */
      Bitmap buffer;

      /**
         The background of the animated buffer. Most animations need to have
         this bitmap along with the animated buffer so they can interpolate
         each frame between them.
      */
      Bitmap back;

      /**
         Total number of frames in the animation.
      */
      int frames;

      /**
         The current frame of animation.
      */
      int progress;

   public:
      Animator();
      virtual ~Animator();

      /**
         Returns true if an animation is in progress.
      */
      bool Animating();

      /**
         Updates animation variables. Returns true if the animation is still
         in progress and false if it has finished.
      */
      bool Update();

      /**
         Resets the animation variables and destroys the background.
      */
      void Reset();

      /**
         Sets up a new animation. The parameters passed are the bitmap that is to
         be animated and the length of the animation in miliseconds.
      */
      void Create(Bitmap &bmp, int length);

      /**
         Returns the buffer onto which you can draw whatever is to be animated.
      */
      Bitmap& GetBuffer();

      /**
         Draws one frame of animation. Parameters are the canvas onto which the
         animation should be drawn and the type of animation that is to be used.
         See above for a list of available animation types.

         Here's a small example (blending from red to lightgray):
<pre>
      // clear the backbuffer with lightgray
      al_set_target_bitmap(al_get_backbuffer());
      al_clear(Color::lightgray);

      // make a canvas for a nice little "window"
      Bitmap canvas;
      canvas.CreateSubBitmap(al_get_backbuffer(), 100, 20, 120, 60);

      // now make an animator
      Animator anim;
      anim.Create(canvas, 500);             // first create an animated buffer
      anim.GetBuffer().Clear(Color::red);   // draw to the animated buffer

      // now do the main loop; this should normally be controled by a timer
      do {
         // update the animator
         anim.Update();

         // draw the next frame of animation on the canvas
         anim.Draw(canvas, Animator::BLEND);

         // wait for a while
         rest(20);
      } while (anim.Animating());
</pre>
      */
      virtual void Draw(Bitmap &canvas, int type);
};
}

#endif         //MASKING_ANIMATOR_H
