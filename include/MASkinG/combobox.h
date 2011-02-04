// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_COMBOBOX_H
#define      MASKING_COMBOBOX_H

#include "editbox.h"
#include "button.h"
#include "listbox.h"
#include "maskingdll.h"

namespace MAS {
/*
   Internal helper button for the ComboBox.

   Basically the same as a normal button except that it can also have and
   overlay bitmap (to draw the little arrow).

   @see ComboBox
*/
class MASKING_DLL_DECLSPEC ComboButton : public Button {
   protected:
      /**
         Index of the overlay bitmap in the skin's bitmap array.
      */
      int overlay;
      /**
         Draws the button with a overlay.
      */
      void Draw(Bitmap &canvas);

   public:
      /**
         Sets the index of the overlay bitmap.
      */
      void SetOverlay(int ovr);
};

/*
   Internal helper editbox for the ComboBox.

   Basically the same as a normal EditBox except that it intercepts KEY_UP and KEY_DOWN
   events and passes them on to the ComboBox to change the selection and it activates
   on mouse click.

   @see ComboBox
*/
class MASKING_DLL_DECLSPEC ComboEditBox : public EditBox {
   protected:
      bool MsgChar(int c);
      void MsgLPress();
};

/**
   The ComboBox widget.
   A ComboBox is a compound widget made out of an edit box, a button and
   a listbox. By default the listbox is hidden but if the button is pressed
   the listbox is shown. When a listbox item is double clicked the combobox
   sends a MSG_ACTIVATE message to the parent dialog with the index of the
   selected item as the first argument.
*/
class MASKING_DLL_DECLSPEC ComboBox : public CompoundWidget {
   public:
      /**
         The underlaying edit box widget.
      */
      ComboEditBox editBox;
      /**
         The underlaying button with the little arrow for showing the list.
      */
      ComboButton button;
      /**
         The underlaying listbox object.
      */
      ListBox list;

   protected:
      /**
         Initilizes the combo box with the default bitmaps, fonts and font colours as defined by the skin.
      */
      void MsgInitSkin();

      /**
         Resizes and repositions all the child widgets when the combobox itself is resized.
      */
      void UpdateSize();
      /**
         Helper function for hiding the list in the combobox.
      */
      void HideList();
      /**
         Helper function for showing the list in the combobox.
      */
      void ShowList();
      void MsgGotfocus();
      void MsgLostfocus();

   public:
      ComboBox();

      /**
         Handles events such as button clicks and list box clicks.
      */
      void HandleEvent(Widget& obj, int msg, intptr_t arg1=0, intptr_t arg2=0);
      /**
         Shortcut for Widget::Setup() and list.Select(i).
      */
      void Setup(int x, int y, int w, int h, int key, int flags, int i);
};
}

#endif      //MASKING_COMBOBOX_H
