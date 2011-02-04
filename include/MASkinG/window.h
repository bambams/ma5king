// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_WINDOW_H
#define      MASKING_WINDOW_H

#include "dialog.h"
#include "panel.h"
#include "label.h"
#include "button.h"
#include "separator.h"
#include "maskingdll.h"

namespace MAS {
/*
   Internal helper widget for the Window class.

   Implements a single border element. There are five of them in a window
   (left, right, top and bottom borders and the caption area).

   @see Window
*/
class MASKING_DLL_DECLSPEC FrameBorder : public Widget {
   protected:
      /**
         Type of border. Can be 0 for left, 1 for right, 2 for top, 3 for
         bottom and 4 for caption bar.
      */
      int type;

      /**
         Initializes the border with the appropriate bitmap from the skin.
      */
      void MsgInitSkin();
      /**
         Draws the border.
      */
      void Draw(Bitmap &canvas);

   public:
      /**
         Sets the type of border.
      */
      void SetType(int type);
      /**
         Returns the section of the border the mouse is over. Section can be
         0 for beginning (left or top), 2 for the end part (right or bottom)
         or 1 for everything in between. This is used to determine in which
         direction a window surrounded by the border can be resized.
      */
      int GetSection();
};


/*
   Internal helper widget for the Window class.

   This is the default client area dialog. It's a simlpe dialog containing
   nothing but a single window panel.

   @see Window
*/
class MASKING_DLL_DECLSPEC WinClientArea : public Dialog {
   protected:
      /**
         The underlying panel widget.
      */
      Panel pan;

      /**
         Initializes the dialog and the panel with the default skin bitmap.
      */
      void MsgInitSkin();

   public:
      WinClientArea();

      /**
         Handles events by passing the on to the parent dialog (the window).
      */
      void HandleEvent(Widget &obj, int msg, intptr_t arg1=0, intptr_t arg2=0);
};

/*
   Internal helper widget for the Window class.

   It's a simple widget similar to an Image or a Separator. Used for drawing the
   window title background if any.

   @see Window
*/
class MASKING_DLL_DECLSPEC TextBack : public Widget {
   protected:
      /**
         Draws the widget.
      */
      void Draw(Bitmap &canvas);
};

/*
   Internal helper widget for the Window class.

   The same as a normal button except that it can't be tabbed to.

   @see Window
*/
class MASKING_DLL_DECLSPEC WinIcon : public Button {
   protected:
      bool MsgWantTab();
};

/**
   A movable and resizable dialog aka window.

   A Window implements a dialog that has a frame with a caption area, three icons
   for minimizing, maximizing and closing the window and a client area for holding
   the window's contents. A window can be modal (it takes exclusive input focus)
   or modeless (it coexists with other widgets in a dialog). A window can but
   doeasn't have to be movable and/or resizable. It can use it's own client area
   for holding its contents or you can give it any dialog to act as a client area.
*/
class MASKING_DLL_DECLSPEC Window : public Dialog {
   public:
      /**
         The window's title Label. This is public so you can access it freely
         without the need of a special access function.
      */
      Label title;

   protected:
      /**
         Left border.
      */
      FrameBorder bLeft;
      /**
         Right border.
      */
      FrameBorder bRight;
      /**
         Top border.
      */
      FrameBorder bTop;
      /**
         Bottom border.
      */
      FrameBorder bBottom;
      /**
         The caption area.
      */
      FrameBorder bGrip;
      /**
         Minimize icon.
      */
      WinIcon iconMin;
      /**
         Maximize icon.
      */
      WinIcon iconMax;
      /**
         Exit icon. The exit icon actually closes a modal window but if the
         window is modeless, it will just send MSG_REMOVEME to the parent dialog
         and the dialog needs to actually remove the window from itself.
      */
      WinIcon iconExit;
      /**
         The background for the window title. Note that only some skins use this.
      */
      TextBack textBack;
      /**
         A pointer to the window's client area.
      */
      Dialog *clientArea;
      /**
         The window's own client area that is used by default if the user
         doesn't provide one.
      */
      WinClientArea ownClientArea;
      /**
         The restore rectangle. This rectangle stores the original window size
         and position so the window can be restored after being minimized or maximized.
      */
      Rect restoreRect;
      enum { W_MINIMIZED=1, W_MAXIMIZED=2, W_AUTOROLLUP=4 };
      /**
         Some window specific flags that don't fit in the normal widget flags scheme.
<pre>
         W_MINIMIZED=1      - the window is minimized
         W_MAXIMIZED=2      - the window is maximized
         W_AUTOROLLUP=4     - the window automatically minimizes when it loses the mouse
</pre>
      */
      int extraFlags;

      /**
         Initilizes the window item with the default bitmaps, fonts and font colours as defined by the skin.
      */
      void MsgInitSkin();
      /**
         Sends a MSG_GOTFOCUS to all the widgets (except for the client area)
         and plays an appropriate sample to indicate the window got input focus.
      */
      void MsgGotfocus();
      /**
         Sends a MSG_LOSTFOCUS to all the widgets (except for the client area)
         and plays an appropriate sample to indicate the window lost input focus.
      */
      void MsgLostfocus();
      bool MsgWantfocus();
      /**
         Restores the window when it gets the mouse if the autorollup feature is enabled.
      */
      void MsgGotmouse();
      /**
         Minimizes the window when it loses the mouse if the autorollup feature is enabled.
      */
      void MsgLostmouse();

      /**
         Resizes and repositions some of the child widgets after the window has
         been resized or moved. You should overload this function if you want your
         widgets to follow the window's size and position properly.
      */
      void UpdateSize();
      /**
         Returns the ActionType accordin to the position of the mouse. This
         makes sure the window can be resized when the mouse is on top of a border
         and moved when it is on top of the caption area.
      */
      ActionType GetAction();
      /**
         A helper function for enabling and disabling the icons according to the window's properties.
      */
      void UpdateWindowProperties();

   public:
      Window();

      /**
         Handles events such as icon clicks.
      */
      void HandleEvent(Widget &obj, int msg, intptr_t arg1=0, intptr_t arg2=0);

      /**
         Adds a widget to the client area.
      */
      void Add(Widget &w);
      /**
         Removes a widget from the client area
      */
      void Remove(Widget &w);

      /**
         Sets the passed dialog as the window's client area. The window contains
         its own default client area dialog but if you want you can have any
         dialog act as a client area for a window.
      */
      virtual void SetClientArea(Dialog *clientArea);
      /**
         Returns a pointer to the window's client area dialog.
      */
      Dialog *GetClientArea();
      /**
         Minimizes the window or restores it if it is already minimized. Note
         that in MASkinG minimizing a window isn't exactly the same as minimizing
         a window in MS Windows. In MASkinG minimizing is about halfway between
         rolling up and normal minimizing.

         Note: this can change in the future.
      */
      virtual void Minimize();
      /**
         Maximizes the window or restores it if it is already maximized.
      */
      virtual void Maximize();
      /**
         Enables or disables the autorollup (or auto-minimize) feature. If
         enabled, the window will automatically minimize itself when it loses
         the mouse and restore itself when it gets it back.
      */
      virtual void Autorollup(bool);
      //Widget *Popup(Dialog *parent, int x = INT_MAX, int y = INT_MAX, Widget *focus = NULL);
};
}


#endif      //MASKING_WINDOW_H
