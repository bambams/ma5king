// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_SEPARATOR_H
#define      MASKING_SEPARATOR_H

#include "widget.h"
#include "dialog.h"
#include "maskingdll.h"

namespace MAS {
/**
   The separator widget.
   This is a very simple widget whose sole purpose is to visually separate
   widgets and subdialogs. A separator can be either horizontal or vertical.
*/
class MASKING_DLL_DECLSPEC Separator : public Widget {
   protected:
      /**
         The orientation of the separator. 0 is vertical, 1 is horizotnal.
      */
      int orientation;

   protected:
      // Message functions
      /**
         Draws either a horizontal or a vertical separator.
      */
      void Draw(Bitmap &canvas);
      /**
         Initializes the separator with the default separator bitmap.
      */
      void MsgInitSkin();
      /**
         Automatically calculates the size of the separator depending on orientation.
      */
      void UpdateSize();

   public:
      Separator();
      /**
         Sets the separator's orientation. Pass 0 for a vertical separator or 1 for a horizontal one
      */
      virtual void SetOrientation(int o);
      /**
         Returns the separator's orientation.
      */
      virtual int GetOrientation();
      /**
         A shortcut for Widget::Setup() and SetOrientation().
      */
      void Setup(int x, int y, int w, int h, int key, int flags, int o);
};
}

#endif         //MASKING_SEPARATOR_H
