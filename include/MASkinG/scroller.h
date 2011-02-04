// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
//                     Modifications by Vladimir Kokovic.                     //
// ************************************************************************** //

#ifndef      MASKING_SCROLLER_H
#define      MASKING_SCROLLER_H

#include "compound.h"
#include "scrollobj.h"
#include "button.h"
#include "separator.h"
#include "maskingdll.h"

namespace MAS {
/*
   Internal helper object for the Scroller.

   Draws the Scroller's background (i.e. the area between the two scroll
   buttons and behind the draggable button).

   @see Scroller
*/
class MASKING_DLL_DECLSPEC ScrollBack : public Widget {
   protected:
      void Draw(Bitmap &canvas);
      void MsgTick();
      void MsgLPress();
      void MsgLRelease();
      void MsgLostmouse();
   public:
      int orientation;
};


class Scroller;

/*
   Internal helper button for the Scroller.

   Used for the Scroller's left, right, up and down arrows. The only
   difference between this and a normal button is that this widget
   can't be tabbed to.

   @see Scroller
*/
class MASKING_DLL_DECLSPEC ArrowButton : public Button {
   protected:
      bool MsgWantTab();
};

/*
   Internal helper button for the Scroller.

   Like a normal Button except that it can also be dragged with the mouse
   or moved with the keyboard. Used for the Scroller's draggable button.

   @see Scroller
*/
class MASKING_DLL_DECLSPEC ScrollButton : public Button {
   protected:
      int overlay, orientation;
      void MsgMousemove(const Point &p);
      bool MsgChar(int c);
      void Draw(Bitmap &canvas);
      bool MsgWantTab();
      void MsgLPress();

   public:
      void SetOverlay(int i);
      friend class MASKING_DLL_DECLSPEC Scroller;
};

/**
   The scroller widget. A Scroller is just like a Slider or a SpinBox except
   that it represents the data a bit differently. Like a SpinBox it has two
   buttons for scrolling up and down but it also has a third button that can
   be dragged like a slider's button. A Scroller can be either horizontal or
   vertical. Its width or height (depending on orientation) is set automatically
   according to the background bitmap.

   @see Scroller, SpinBox
*/
class MASKING_DLL_DECLSPEC Scroller : public CompoundWidget, public ScrollObject {
   protected:
      /**
         The underlaying background widget.
      */
      ScrollBack back;
      /**
         The underlaying up/right arrow button.
      */
      ArrowButton bUp;
      /**
         The underlaying down/left arrow button.
      */
      ArrowButton bDown;
      /**
         The underlaying gripper button.
      */
      ScrollButton bGrip;
      /**
         The number of items the scrolled object can display on one screen.
         The scroll button is resized to reflect the relation between the
         number of visible items and the number of all items.
      */
      int visibleItems;
      /**
         The orientation of the scroller. If 0 the scroller is vertical otherwise it is horizontal.
      */
      int orientation;

      /**
         The offset of the point on the gripper where the user grabbed it to drag from the centre.
      */
      Point gOffset;

      /**
         Vertical and horizontal increment values.
      */
      int hInc, vInc;

   protected:
      /**
         Initializes the Scroller and it's widgets with the default bitmaps,
         fonts and colours as defined by the skin.
      */
      void MsgInitSkin();
      /**
         Updates the position of the draggable scroller button dependin on current position and orientation.
      */
      void CalculateGripperPosition();
      /**
         Updates the size and position of all the widgets depending on the
         size of the scroller and its orientation. If the scroller is horizontal
         its height is set to the height of the background bitmap othwerwise its
         width is set to the width if the background bitmap.
      */
      void UpdateSize();
      /**
         Returns false to indicate the scroller itself can't be tabbed to.
         Instead the underlaying widgets can be tabbed to.
      */
      bool MsgWantTab();

   public:
      Scroller();

      /**
         Shortcut for Widget::Setup(), SetRange(), SetPosition() and SetOrientation().
      */
      void Setup(int x, int y, int w, int h, int key, int flags, int totalItems, int visibleItems, int pos, int o);
      /**
         Main event handler.
         Handles events such as button clicks and keypresses so that the scroller
         is updated whenever a button is clicked or dragged, etc.
      */
      void HandleEvent(Widget& obj, int msg, intptr_t arg1=0, intptr_t arg2=0);
      void OriginalHandleEvent(Widget& obj, int msg, intptr_t arg1=0, intptr_t arg2=0);
      /**
         Sets the Scroller's orientation. Pass 0 for a vertical Scroller, 1 for a horizontal one.
      */
      virtual void SetOrientation(int o);
      /**
         Returns the orientation of the scroller.
      */
      virtual int GetOrientation();
      /**
         Sets the range of values in which the scroller may be scrolled.
         Note that this function takes different parameters than
         ScrollObject::SetRange(min, max). TotalItems is the number of all
         items the scroller can scroll (min is implicitlly 0 and max equals
         totalItems) and visibleItems is the number of items the scrolled
         object is capable of displaying on one screen. This is needed
         because the little draggable scroll button is sized proportionally
         to the number of currently visible items.
      */
      void SetRange(int totalItems, int visibleItems);
      /**
         Returns the number of items the object attached to the scroller can display
         on one "screen".
      */
      int GetVisibleItems();
      /**
         Sets the position of the scroller.
      */
      void SetPosition(int pos);
      /**
         Moves the scroller position right or up.
      */
      void Increment();
      /**
         Moves the scroller position left or down.
      */
      void Decrement();

      void SetHorizontalIncrement( int inc ) { hInc = inc; }
      void SetVerticalIncrement( int inc ) { vInc = inc; }
      int GetHorizontalIncrement( void ) { return hInc; }
      int GetVerticalIncrement( void ) { return vInc; }
};
}

#endif      //MASKING_SCROLLER_H
