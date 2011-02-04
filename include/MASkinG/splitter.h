// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_SPLITTER_H
#define      MASKING_SPLITTER_H

#include "dialog.h"
#include "maskingdll.h"

namespace MAS {
/**
   An invisible splitter object.
   Splitters can be used to separate and resize up to 4 widgets (or dialogs).
   The dialogs attached to a splitter share the area of the splitter and the
   splitter allows the user to resize them at will.
*/
class MASKING_DLL_DECLSPEC Splitter : public Dialog {
   protected:
      /**
         Pointers to the 4 attached widgets/dialogs.
      */
      Widget *attached[4];

      /**
         The width of the buffer zone between dialogs. Should be positive.
         The default is 4 pixels.
      */
      int bufferZone;

      /**
         Horizontal offset from the centre.
      */
      int offsetX;

      /**
         Vertical offset from the centre.
      */
      int offsetY;

      bool horizontalMove;
      bool verticalMove;

      /**
         Updates the size and position of all the widgets attached to the splitter.
         This function places and resizes all the widgets so that they take up
         as much space of the spitter window as possible without overlapping.
      */
      void UpdateSize();

      void MsgLostmouse();
      void MsgLPress();
      void MsgLRelease();
      void MsgMousemove(const Point &d);

   public:
      Splitter();

      /**
         Attaches an exisiting widget (or dialog) to the splitter.
         Pass the section of the splitter where you want to put the
         widget. Possible sections are:
<pre>
      0 - top left
      1 - bottom left
      2 - top right
      3 - bottom right
</pre>
         If a widget already occupies the desired section, the existing
         widget will be removed.
      */
      void Attach(Widget *w, int section);

      /**
         Removes the passed widget from the splitter.
      */
      void Detach(Widget *w);

      /**
         Sets the size of the buffer zone between widgets in pixels.
         The default value is 4.
      */
      void SetBufferZone(int pixels);

      /**
         Sets the offset of the centre point of the splitter from its centre.
         The default is (0,0). Negative values move the splitter left and up,
         positive move it right and down.<br>
         Note: you can only set the offset after you've set the size of the
         splitter.
      */
      void SetOffset(int x, int y);
};
}

#endif      //MASKING_SPLITTER_H
