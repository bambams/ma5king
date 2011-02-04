// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_COLORSEL_H
#define      MASKING_COLORSEL_H

#include "window.h"
#include "slider.h"
#include "separator.h"
#include "panel.h"
#include "maskingdll.h"

namespace MAS {
/**
   The standard color selector dialog.
   ColorSelect is a simple dialog for selecting a color either by defining
   it with three RGB sliders or three HSV sliders. This dialog isn't exactly
   pretty so if you have a better one for the same purpose, contributions
   are always welcome :)
*/
class MASKING_DLL_DECLSPEC ColorSelect : public Window {
   protected:
      /*
         A simple widget that just clears a rectangle with a given color.
         Used for the colour preview.
      */
      class MASKING_DLL_DECLSPEC ColorPreview : public Widget {
         protected:
            void Draw(Bitmap &canvas);

         public:
            Color bg;
      };

      // the underlying widgets that make up the dialog
      Panel panel;
      Slider sR, sG, sB;
      Label tR, tG, tB;
      Label lR, lG, lB;
      Slider sH, sS, sV;
      Label tH, tS, tV;
      Label lH, lS, lV;
      Button bOK, bCancel;
      ColorPreview colRect;
      Separator sep;

      /**
         Initializes the colour preview and the slider labels with the proper values.
      */
      void MsgInitSkin();

   public:
      /**
         Color selector constructor. You can pass the dialog's title and initial colour.
      */
      ColorSelect(const char *title = "Select color", Color c = 0);

      /**
         Pops up a colour selector at the given position. Returns the selected colour or
         -1 if the dialog was canceled or escaped.
      */
      virtual Color Popup(Dialog *p, int x = INT_MAX, int y = INT_MAX);
      /**
         Handles events such as slider moves.
      */
      void HandleEvent(Widget &obj, int msg, intptr_t arg1=0, intptr_t arg2=0);
};
}

#endif         //MASKING_COLORSEL_H
