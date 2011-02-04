// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_CLEARSCREEN_H
#define      MASKING_CLEARSCREEN_H

#include "clearscreen.h"
#include "dialog.h"
#include "maskingdll.h"

namespace MAS {

/**
   ClearScreen is a very simple widget whose sole purpose is to clear the canvas to a certain colour.
*/
class MASKING_DLL_DECLSPEC ClearScreen : public Widget {
   protected:
      /**
      The background colour.
      */
      Color col;

   protected:
      /**
      Resizes the widget to the parent's size when the dialog starts.
      */
      void MsgStart();
      /**
      Sets the default clear colour which is the background colour defined by the skin.
      */
      void MsgInitSkin();
      /**
      Clears the canvas with the selected colour.
      */
      void Draw(Bitmap &canvas);

   public:
      ClearScreen();

      /**
      Sets the colour that is to be used to clear the canvas.
      */
      void SetColor(const Color& col);

      /**
      Gets the colour that is to be used to clear the canvas.
      */
      Color &GetColor ();
};
}

#endif      //MASKING_CLEARSCREEN_H
