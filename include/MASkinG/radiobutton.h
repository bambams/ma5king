// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_RADIOBUTTON_H
#define      MASKING_RADIOBUTTON_H

#include "checkbox.h"
#include "maskingdll.h"

namespace MAS {
/**
A radio button is almost identical to a checkbox. The only two differences are that usually a radio
button uses a sligtly different bitmap (although both widgets use the same format so the bitmaps can be
exchanged between a RadioButton and a CheckBox) and radio buttons are organized in groups. At any given
time only one radio button in the group may be selected. Whenever a radio button is toggled it sends a
MSG_RADIO message to all the widgets in the dialog so they can uncheck themselves. Also a radio button
cannot deselect iteslf. Only another button from the same group can do this.
*/
class MASKING_DLL_DECLSPEC RadioButton : public CheckBox {
   protected:
      /**
      The ID of the group the radio button belongs to.
      */
      int group;

   protected:
      // Message functions
      /**
      Like Button::MsgKey() except that this one doesn't toggle the button but only select
       it if it isn't already selected and send MSG_RADIO to all the widgets in the dialog.
      */
      void MsgKey();
      /**
      Like Button::MsgLPress() except that this one doesn't toggle the button but only select
       it if it isn't already selected and send MSG_RADIO to all the widgets in the dialog.
      */
      void MsgLPress();
      /**
      Like Button::MsgLRelease() except that this one doesn't toggle the button but only select
       it if it isn't already selected and send MSG_RADIO to all the widgets in the dialog.
      */
      void MsgLRelease();
      /**
      If the button is in the same group as the widget that sent this
      message (group g), the button deselects itself.
      */
      void MsgRadio(int g);
      /**
      Initializes the widget with the default radio button bitmap, fonts and font colours from
      the skin and sets all the flags that define the behaviour of a radio button.
      */
      void MsgInitSkin();

   public:
      /**
      default constructor
      */
      RadioButton();

      /**
      Assings the radio button to the group g.
      There may be any number of buttons in a group and there may be any number of groups in a
      dialog but they all have to have unique IDs.
      */
      void SetGroup(int g);
      /**
      returns the group id of this button.
      */
      int GetGroup ();
      /**
      Shortcut for Button::Setup() and SetGroup().
      */
      void Setup(int x, int y, int w, int h, int key, int flags, const char *t, int g);

};
}

#endif      //MASKING_RADIOBUTTON_H
