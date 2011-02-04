// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_SLIDER_H
#define      MASKING_SLIDER_H

#include "progress.h"
#include "maskingdll.h"

namespace MAS {
/**
   The slider widget.
   This is a simple slider that can be moved with both the mouse and the
   keyboard. Can be horizontal or vertical. Every time the position of the
   slider changes it sends a MSG_SCROLL message to the parent dialog passing
   the new position as the first argument.
*/
class MASKING_DLL_DECLSPEC Slider : public Progress {
   protected:
      /**
         Two indices pointing to the slider bitmaps in the skin's bitmap arrays.
      */
      int bitmap[2];

      /**
         The offset of the point on the gripper where the user grabbed it to drag from the centre.
      */
      int offset;

   protected:
      // Message functions
      /**
         Draws either a horizontal or a vertical slider.
      */
      void Draw(Bitmap &canvas);
      /** */void MsgLPress();
      /** */void MsgLRelease();
      /** */void MsgMousemove(const Point &d);
      /**
         These handle moving the slider button with the mouse.
      */
      void MsgWheel(int z);
      /**
         Handles keyboard input. It enables moving the slider button
         with the keyboard. The slider can be moved by eithe left
         and right or up and down arrows (depending on orientation)
         and HOME and END keys.
      */
      bool MsgChar(int c);
      /**
         Returns true to indicate the slider takes input focus.
      */
      bool MsgWantfocus();
      /**
         Initializes the slider with the default slider bitmaps.
      */
      void MsgInitSkin();

      /**
         Helper function for drawing a horizontal slider.
      */
      void DrawHorizontalSlider(Bitmap &canvas);
      /**
         Helper function for drawing a vertical slider.
      */
      void DrawVerticalSlider(Bitmap &canvas);
      /**
         A helper function for switching the state of the slider between normal and selected.
      */
      void Toggle();

      void MsgResize();

   public:
      /**
         Sets the orientation of the slider. Pass 0 for vertical and 1 for horizontal.
      */
      void SetOrientation(int o);
      /**
         Sets the sliders skin bitmaps.
         Unlike most other widgets the slider uses two bitmaps instead of just one.
         It uses one for the background and another for the draggable button. The
         slider is automatically resized to the width or height of the background
         bitmap depending on orientation. The button bitmap is usually of the same
         width (or height) as the background bitmap although it may be smaller but
         it mustn't be bigger.
      */
      void SetBitmap(int background, int button);
      /**
         A shortcut for Widget::Setup(), ScrollObject::SetRange(), ScrollObject::SetPosition() and SetOrientation().
      */
      void Setup(int x, int y, int w, int h, int key, int flags, int m, int M, int p, int o);
};
}

#endif      //MASKING_SLIDER_H
