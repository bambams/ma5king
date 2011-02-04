// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_BUTTON_H
#define      MASKING_BUTTON_H

#include "label.h"
#include "maskingdll.h"

namespace MAS {

/**
Button is a simple widget that implements the functionality of several types
of buttons. When a button is clicked or the button's shortcut is pressed or
space or enter is pressed while the button has input focus, it sends a
MSG_ACTIVATE message to its parent dialog. If the D_TOGGLE flag is set the button
can be toggled on and off otherwise it is a normal push button. If the D_SPINNER
flag is set the button sends MSG_ACTIVATE to the parent dialog at regular intervals
while it is being pressed down otherwise it sends MSG_ACTIVATE only when it is
released. The button can contain text or it can only have a button bitmap.
In that case it automatically resizes itself to the size of the bitmap
if the D_AUTOSIZE flag is set.
*/
class MASKING_DLL_DECLSPEC Button : public Label {
   protected:
      /**
       A couple of counters that help make sure the button is properly animated when MSG_KEY is
        received and that MSG_ACTIVATE messages are sent at regular intervals if D_SPINNER is set.
      */
      int counter;
      /**
      See counter
      */
      int spinCounter;

   protected:
      // Message functions
      /**
       Draws the button on the canvas. The button can draw either a button bitmap or text or both.
      */
      void Draw(Bitmap &canvas);

      /**
      Initializes the button whe the dialog starts.
      */
      void MsgStart();

      /**
      Initializes the button with the default bitmap, fonts and font colours from the skin.
      */
      void MsgInitSkin();

      /**
      Every time a button gets or loses input focus it plays an appropriate sound and animates itself.
      */
      void MsgGotfocus();

      /**
      See MsgGotfocus()
      */
      void MsgLostfocus();

      /**
       When a button receives a MSG_KEY it simulates a mouse click.
      */
      void MsgKey();

      /**
       When a button receives MSG_LPRESS it just sets the D_PRESSED flag.
       When the mouse button is later released the button sends a MSG_ACTIVATE to
       its parent dialog if the mouse release was made while the mouse was
       still on top of the button or just clears the D_PRESSED flag to
       abort detecting a button click if the mouse button was released outside of the button.
      */
      void MsgLPress();

      /**
       See MsgLPress();
      */
      void MsgLRelease();

      /**
       returns true
      */
      bool MsgWantfocus();

      /**
       Handles animating the button after receiving a
       MSG_KEY message (it toggles the button back a few hundred miliseconds after
       MsgKey() was called) and makes sure MSG_ACTIVATE messages are sent at
       regular intervals to the parent dialog if D_SPINNER is set.
      */
      void MsgTick();

      /**
        Toggles the button's state between normal and selected states and animates it.
      */
      void Toggle();

      /**
       If D_AUTOSIZE flag is set this function resizes the button
       to the size of the button bitmap. This makes it possible to create icon buttons.
      */
      void UpdateSize();

   public:
      /**
      default constructor
      */
      Button();
      /**
      Like Label::Setup() with the exception that there is no text alignment parameter.
      On a button the text is always centered.
      */
      void Setup(int x, int y, int w, int h, int key, int flags, const char *text);
      /** */void Setup(int x, int y, int w, int h, int key, int flags, const char *file, int index, const char *text=NULL);
      /** */void Setup(int x, int y, int w, int h, int key, int flags, Bitmap &bmp, int index, const char *text=NULL);
      /**
         Extra overloads for easily changing the default button bitmap.
         To change the bitmap the button will use, you first need to upload it to the skin
         and then pass its index in the skin bitmap array to Button::SetBitmap(). These
         functions are shortcuts for doing that in only one step. Pass the index of the
         bitmap you want to assign to the button and either the bitmap you want to upload
         to the skin or the path to the file you want to upload.
      */
      void Setup(int x, int y, int w, int h, int key, int flags, int index, const char *text=NULL);
};
}

#endif      //MASKING_BUTTON_H
