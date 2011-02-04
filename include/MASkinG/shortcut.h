// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_SHORTCUT_H
#define      MASKING_SHORTCUT_H

#include "dialog.h"
#include "maskingdll.h"

namespace MAS {
/**
   The shortcut widget.
   This is an invisible widget that helps implement keyboard shortcuts. It
   supports more or less all keys and combinations of Ctrl, Shift and Alt keys.

   Note: Shortcut is deprecated. Use the Accelerator class instead.

   @see Accelerator
*/
class MASKING_DLL_DECLSPEC Shortcut : public Widget {
   protected:
      /**
         The key the widget is supposed to respond to.
      */
      int key;
      /**
         The combination of key shifts the widget is supposed to respond to.
      */
      int shifts;

   protected:
      /**
         Handles keypresses. If it detects a combination of keys defined with
         the Setup() function was pressed it sends MSG_ACTIVATE to the parent dialog.
      */
      bool MsgXChar(int xchar);

   public:
      Shortcut();

      /**
         Sets the combination of keys the widget should respond to. Key is an
         Allegro KEY_* constant and shifts is a combination of key shifts flags
         such as KB_CTRL_FLAG, KB_ALT_FLAG, KB_SHIFT_FLAG, etc. Whenever the
         widget detects its combination of keys was pressed it send a
         MSG_ACTIVATE message to the parent dialog.
      */
      void Setup(int key, int shifts = 0);
};
}

#endif      //MASKING_SHORTCUT_H
