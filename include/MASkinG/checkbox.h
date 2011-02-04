// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_CHECKBOX_H
#define      MASKING_CHECKBOX_H

#include "button.h"
#include "maskingdll.h"

namespace MAS {
/**
 A check box is almost identical in functionality to a toggle button except that
 it draws itself differently. It has a box with some sort of a check mark in
 it on the left side and some text on the right side.
*/
class MASKING_DLL_DECLSPEC CheckBox : public Button {
   protected:
      // Message functions
      /**
       Draws the checkbox on the canvas. A small box with an optional check mark is drawn
        on the left and a single line of text is printed on the right side.
      */
      void Draw(Bitmap &canvas);
      /**
       Initializes the widget with the default checkbox bitmap,
      fonts and font colours from the skin and sets all the flags that define the
      behaviour of a checkbox.
      */
      void MsgInitSkin();
};
}

#endif      //MASKING_CHECKBOX_H
