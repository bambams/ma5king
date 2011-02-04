// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_PROGRESS_H
#define      MASKING_PROGRESS_H

#include "separator.h"
#include "scrollobj.h"
#include "maskingdll.h"

namespace MAS {
/**
   The progress bar widget.
   Displays either a horizontal or a vertical progress bar although it
   works best as a horizontal one.
*/
class MASKING_DLL_DECLSPEC Progress : public Separator, public ScrollObject {
   protected:
      // Message functions
      /**
         Draws either a horizontal or a vertical progress bar.
      */
      void Draw(Bitmap &canvas);
      /**
         Initializes the progress bar with the default progress bar bitmap.
      */
      void MsgInitSkin();
      /**
         Automatically calculates the size of the progress bar depending on orientation.
      */
      void UpdateSize();

   public:
      /**
         Set the orientation of the progress bar. Pass 0 for vertical and 1 for horizontal.
      */
      void SetOrientation(int o);
      /**
         Sets the position of the progress bar. You should use this only to set the
         initial position if it is different than 0 and use Increment() to advance the bar.
      */
      void SetPosition(int pos);
      /**
         A shortcut for Widget::Setup(), SetOrientation() and SetPosition().
      */
      void Setup(int x, int y, int w, int h, int key, int flags, int p, int o);
      /**
         Advances the progress bar. You should call this function every time the amount
         of progress increases.
      */
      void Increment();
};
}

#endif      //MASKING_PROGRESS_H
