// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_GROUPBOX_H
#define      MASKING_GROUPBOX_H

#include "panel.h"
#include "label.h"
#include "dialog.h"
#include "maskingdll.h"

namespace MAS {
/**
   Similar to a Panel except that it can also have a title.
*/
class MASKING_DLL_DECLSPEC GroupBox : public Dialog {
   public:
      /**
         The underlaying panel widget.
      */
      Panel box;
      /**
         The underlaying title label.
      */
      Label title;

   protected:
      /**
         Initialized the groupbox with the default bitmap, fonts and colours as defined by the skin.
      */
      void MsgInitSkin();
      /**
         Updates the size of the box and the title.
         It resizes the panel to fit into the box and positions the title
         text according to the title alignment setting. The title can be
         aligned left or right or can be centered and can be placed at an
         arbitrary offset from either the left or right edge of the box or
         from the centre.
      */
      void UpdateSize();

   public:
      GroupBox();

      /**
         Sets the title text of the groupbox.
      */
      void SetTitle(const char *title);
      /**
         Returns the title of the groupbox.
      */
      const char *GetTitle();
      /**
         Sets the bitmap that is to be used for the groupbox's underlying panel.
      */
      void SetBitmap(int index);
};
}

#endif      //MASKING_GROUPBOX_H
