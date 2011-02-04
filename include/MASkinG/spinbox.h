// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_SPINBOX_H
#define      MASKING_SPINBOX_H

#include "compound.h"
#include "editbox.h"
#include "button.h"
#include "scrollobj.h"
#include "maskingdll.h"

namespace MAS {
/*
   Internal helper button for the SpinBox.

   This is basically just a regular button except that it draws itself a
   little differently. Used for the spinner buttons that are used for
   scrolling the SpinBox up and down.

   @see SpinBox
*/
class MASKING_DLL_DECLSPEC SpinButton : public Button {
   protected:
      void Draw(Bitmap &canvas);
      bool MsgWantTab();
};

/**
   A compound widget made of an EditBox and two spinner Buttons.
   Functionally a SpinBox is very similar to a Slider except that it presents
   the data differently. While a slider's position can be adjusted by dragging
   it's button, a SpinBox's position can be changed by pressing the two buttons
   or by directly entering the value into the text edit area. A SpinBox can
   operate in two modes: integer mode and double mode. In integer mode
   the number value it holds can only be an integer while in double mode it
   can only be a double.
*/
class MASKING_DLL_DECLSPEC SpinBox : public CompoundWidget, public ScrollObject {
   public:
      /**
         The underlaying edit box.
      */
      EditBox editBox;
      /**
         The underlaying up arrow button.
      */
      SpinButton bUp;
      /**
         The underlaying down arrow button.
      */
      SpinButton bDown;

   protected:
      /**
         The current mode of operation. True if the spinbox is in integer
         mode, false otherwise.
      */
      bool integerMode;

   protected:
      /**
         Initializes the SpinBox and it's widgets with the default bitmaps,
         fonts and colours as defined by the skin.
      */
      void MsgInitSkin();
      /**
         Places and resizes the editbox and the buttons according to the geometry
         of the SpinBox.
      */
      void UpdateSize();
      void MsgGotfocus();
      void MsgLostfocus();

   public:
      SpinBox();

      /**
         Shortcut for Widget::Setup() and ScrollObject::Setup(). Double version.
      */
      void Setup(int x, int y, int w, int h, int key, int flags, double min, double max, double pos, double inc);
      /**
         Shortcut for Widget::Setup() and ScrollObject::Setup(). Integer version.
      */
      void Setup(int x, int y, int w, int h, int key, int flags, int min, int max, int pos, int inc);
      /**
         Handles events such as spinner button clicks so that the box is updated whenever a button is clicked.
      */
      void HandleEvent(Widget& obj, int msg, intptr_t arg1=0, intptr_t arg2=0);
      /**
         Sets the position (the value) of the SpinBox's contents.
      */
      void SetPosition(double pos);
      /**
         Increments the box's value.
      */
      void Increment();
      /**
         Increments the box's value.
      */
      void Decrement();
};
}

#endif      //MASKING_SPINBOX_H
