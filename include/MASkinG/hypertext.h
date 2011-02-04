// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_HYPERTEXT_H
#define      MASKING_HYPERTEXT_H

#include "button.h"
#include "maskingdll.h"

namespace MAS {
/**
 A HyperText is almost the same as a Button without a bitmap except that it looks more
 like an internet link.
*/
class MASKING_DLL_DECLSPEC HyperText : public Button {
   protected:
      // Message functions
      /**
      Draws the hypertext on the canvas. The link is drawn as underlined
      text with a dotted rectangle around it.
      */
      void Draw(Bitmap& canvas);

      /**
      When the hypertext gets the mouse it changes the cursor to an internet link pointer
      hand cursor thingy and when it loses the mouse it changes the cursor back to normal.
      */
      void MsgGotmouse();

      /**
      See MsgGotmouse()
      */
      void MsgLostmouse();

      /**
      Initializes the widget with the default hypertext fonts and font colours from
      the skin and sets all the flags that define the behaviour of a hypertext widget.
      */
      void MsgInitSkin();

      /**
      This widget resizes itself to the size of the text regardless of the
      status of the D_AUTOSIZE flag.
      */
      void UpdateSize();
};
}

#endif      //MASKING_HYPERTEXT_H
