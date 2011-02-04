// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
//           ScrollBox written by Martijn "amarillion" van Iersel             //
//                    Modifications by Vladimir Kokovic.                   //
// ************************************************************************** //

#ifndef SCROLLBOX_H
#define SCROLLBOX_H

#include "compound.h"
#include "scroller.h"
#include "clearscreen.h"
#include "panel.h"
#include "maskingdll.h"

namespace MAS {
class ScrollBox;

// helper widget
// needed because the scroll area needs to be drawn after all the other scrollbox widgets are drawn.
// Dialog::draw() method called before all other widgets, so would be drawn over.
class MASKING_DLL_DECLSPEC ScrollBoxHelper : public Widget {
private:
      ScrollBox *parent;

   public:
      ScrollBoxHelper();
      virtual void SetScrollBox(ScrollBox *p);
      virtual void Draw(Bitmap &canvas);
};

/**
  This is a widget that can display something that is larger than the area of the widget on the screen.
  It provides scrollbars to be able to scroll through the content of this widget.
  everything that has optional (h and/or v) scrollbars should be derived from this.
  The first thing you should do is override the DrawClientArea function to provide a way to
  draw the content of your derived scrollbox.
*/
class MASKING_DLL_DECLSPEC ScrollBox : public CompoundWidget {
   protected:
      Scroller hScroller, vScroller;
      ClearScreen clrscr;
      PanelSunken panel;
      ScrollBoxHelper helper;

      int scrollWidth, scrollHeight; // size of the scroll area. If larger than the client area,
                // scrollbars are displayed.

      int scrollX, scrollY; // offset of the scroll area
      int hopt, vopt; // scrollbars on, off or automatic.
      int bufb, bufr, bufl, buft; // size of the panel edges
      bool hashscroller, hasvscroller; // helpers for determining client dimensions

      int GetHScrollerSize();
      int GetVScrollerSize();

   protected:
/**
        override this to provide a way to draw the content of your derived scrollbox.
*/
      virtual void DrawClientArea (Bitmap &dest, int xofst, int yofst);
      void ResetScrollers ();
      void CheckPositionBounds();
      virtual void UpdateSize();
      virtual void HandleEvent(Widget &obj, int msg, intptr_t arg1=0, intptr_t arg2=0);

      // dimensions of the client canvas
      int GetClientBottom();
      int GetClientRight();
      int GetClientTop();
      int GetClientLeft();
      int GetClientHeight();
      int GetClientWidth();

   public:
/**
   Possible Scrollbar states.

<pre>
        SCROLLER_OFF
        SCROLLER_ON
        SCROLLER_AUTO
</pre>
*/
      enum { SCROLLER_OFF, SCROLLER_ON, SCROLLER_AUTO };
/**
  Default constructor
*/
      ScrollBox();
      bool IsInClientArea(MAS::Point &p);
      virtual void HelperDraw(Bitmap &canvas);
      virtual void MsgInitSkin();
      virtual void MsgScroll();
/**
 get the horizontal scroll position
*/
      int GetXOffset();
/**
  get the vertical scroll position
*/
      int GetYOffset();
/**
  Set the size of the scroll area. If this is more than the widget area and the scrollbar option is set to automatic,
  scrollbars will appear automatically.
*/
      void SetScrollSize (int width, int height);
      Size GetScrollSize ();
      void ScrollToArea (int x, int y, int w, int h);
/**
  set the horizontal & vertical scroll position.

  note: perhaps this shoud be renamed to ScrollTo (x, y) or SetOffset (x, y)
*/
      void SetPos (int x, int y);
/**
    scroll to the center of the scroll area
*/
      void Center ();
/**
    set the horizontal and vertical scrollbars on, off or automatic. Use the enum values ScrollBox::SCROLLER_ON,
    ScrollBox::SCROLLER_OFF or ScrollBox::SCROLLER_AUTO. In the last case scrollbars are on only if they are needed, i.e.
    when the scroll area is larger than the widget area.
*/
      void SetScrollOption (int newhopt, int newvopt);
/**
    returns ScrollBox::SCROLLER_OFF, ScrollBox::SCROLLER_AUTO or ScrollBox::SCROLLER_ON depending on
    the state of the Horizontal Scrollbar.
*/
      int GetHScrollOption();
/**
    returns ScrollBox::SCROLLER_OFF, ScrollBox::SCROLLER_AUTO or ScrollBox::SCROLLER_ON depending on
    the state of the Vertical Scrollbar.
*/
      int GetVScrollOption();

      /**
         Sets the increment value for horizontal scroller. Used when scrolling with the
         keyboard (arrow keys) or by clicking on the scroller background. Inc must be a
         positive value. Default is 1.
      */
      void SetHorizontalIncrement( int inc ) { hScroller.SetHorizontalIncrement( inc ); }

      /**
         Sets the increment value for vertical scroller. Used when scrolling with the
         keyboard (arrow keys) or by clicking on the scroller background. Inc must be a
         positive value. Default is 1.
      */
      void SetVerticalIncrement( int inc ) { vScroller.SetVerticalIncrement( inc ); }

      /**
         Gets the increment value for horizontal scroller.
      */
      int GetHorizontalIncrement( void ) { return hScroller.GetHorizontalIncrement(); }

      /**
         Gets the increment value for vertical scroller.
      */
      int GetVerticalIncrement( void ) { return vScroller.GetVerticalIncrement(); }

      /**
         Scrolls to the specified position.
      */
      void ScrollTo(int x, int y);

      /**
         Scrolls to the specified position (horizontally).
      */
      void ScrollToX(int x);

      /**
         Scrolls to the specified position (horizontally).
      */
      void ScrollToY(int y);

};
}

#endif   //SCROLLBOX_H
