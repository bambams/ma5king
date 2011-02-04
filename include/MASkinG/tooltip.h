// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_TOOLTIP_H
#define      MASKING_TOOLTIP_H

#include "widget.h"
#include "color.h"
#include "maskingdll.h"

namespace MAS {
/**
   A helper widget for displaying popup help bubbles. A tooltip is displayed
   when the mouse hovers over a widget for a while. It can display any length
   of text, even in several lines and it can be easily extended to display bitmaps
   and other data. Every dialog contains a default tooltip object which is added
   to it implicitely but you can create your own easily.
*/
class MASKING_DLL_DECLSPEC Tooltip : public Widget {
   protected:
      /**
         Counters for counting time the mouse hasn't moved.
      */
      int counter;
      /**
         Counters for counting time before the tooltip should hide itself.
      */
      int hideCounter;
      /**
         The background colour for the help bubble.
      */
      Color backCol;
      /**
         The border colour for the help bubble.
      */
      Color borderCol;
      /**
         The last known absolute screen coordinates of the mouse cursor. Used for
         checking whether the mouse has moved or not.
      */
      Point lastMousePos;
      /**
         The text the tooltip prints in the help bubble. If NULL, the tooltip
         isn't drawn at all.
      */
      char *text;
      /**
         A pointer to the last widget in the parent dialog the mouse was on top of.
         Used for checking when the mouse focus moved.
      */
      Widget *lastWidget;

   protected:
      // Message functions
      /**
         Draws the help bubble and fills it with text. It tries to be as
         clever as possible to choose the best possible size and position
         for the bubble in order to make it look as nice as possible.
      */
      void Draw(Bitmap &canvas);
      /**
         Sets the default font, colours and other info as defined by the skin.
      */
      void MsgInitSkin();
      /**
         The main help bubble logic function. In MsgTick() the tooltip constantly
         checks if the parent's mouse object changed, if the mouse has not moved
         for a certain period of time and so on and acts accordingly.
      */
      void MsgTick();
      /**
         Returns false to make sure the tooltips don't confuse the dialog when
         dealing with moving the input focus.
      */
      bool MsgWantmouse();

      /**
         Helper function for turning the tooltip on.
      */
      virtual void ShowTooltip();
      /**
         Helper function for turning the tooltip off.
      */
      virtual void HideTooltip();
      /**
         Calculates the geometry (size and position) of the tooltip help bubble.
         It does so by calling CalculateSize() and CalculatePosition(). Overload
         this function in a derived tooltip class so you can draw tooltips with
         different size, position and content.
      */
      virtual void CalculateGeometry();
      /**
         Lower level function for calculating the tooltip help bubble position.
         The position is based on the position of the mouse and the size of the
         bubble so that it fits on screen and isn't obscured by the mouse cursor.
      */
      virtual void CalculatePosition();
      /**
         Lower level function for calculating the tooltip help bubble size. The
         size is calculated from the length of the tooltip text. If there's too
         much text to fit in one line, it is distributed in several lines.
      */
      virtual void CalculateSize();

   public:
      Tooltip();

      /**
         Sets the background color of the help bubble. The default is pale
         yellow. The actual color is read from the skin file.
      */
      virtual void SetBackColor(const Color &col);
      /**
         Sets the border color of the help bubble. The default is black.
         The actual color is read from the skin file.
      */
      virtual void SetBorderColor(const Color &col);
      /**
         Sets the text used to display the help message. Usually you shouldn't
         have to worry about calling this function because the default
         implementation will obtain the text from the appropriate widget by itself.
      */
      virtual void SetText(const char *text);
};
}

#endif      //MASKING_TOOLTIP_H
