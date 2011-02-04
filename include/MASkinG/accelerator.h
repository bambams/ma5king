// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef         MASKING_ACCELERATOR_H
#define      MASKING_ACCELERATOR_H

#include "dialog.h"
#include "maskingdll.h"

namespace MAS {
/*
   A helper structure for representing a keyboard combination.
   A keayboard combination is a normal key with optional
   key modifiers (ctrl, alt, shift, etc.) that all have to be
   pressed simultaneously.
*/
class MASKING_DLL_DECLSPEC KeyCombo {
   private:
      int key;
      int shifts;
      int msg;

   public:
      KeyCombo();
      KeyCombo(int key, int shifts, int msg);

      friend class MASKING_DLL_DECLSPEC Accelerator;
};

/**
   The accelerator widget.
   This widget is similar to a Shortcut except it allows you to setup more
   than one keyboard shortcut. When it detects a predefined keyboard
   combination, it will send the message associated with this combination.
*/
class MASKING_DLL_DECLSPEC Accelerator : public Widget {
   protected:
      /**
         The vector of keyboard combination.
      */
      std::vector<KeyCombo> acc;

   protected:
      /**
         Handles keypresses. If it detects a combination of keys defined
         in the accelerator array, it sends the message associated with
         the keyboard combination in question to the parent dialog.
      */
      bool MsgXChar(int xchar);

   public:
      Accelerator();

      /**
         Adds a combination of keys the widget should respond to and the
         the message it should respond with. <b>Key</b> is an Allegro
         KEY_* constant (as in KEY_A, KEY_F1, KEY_LEFT, etc.) and <b>shifts</b>
         is a combination of key shifts flags such as KB_CTRL_FLAG,
         KB_ALT_FLAG, KB_SHIFT_FLAG, etc. Whenever the widget detects that
         this combination of keys was pressed, it send the message
         <b>msg</b> to the parent dialog.
      */
      void Add(int key, int shifts, int msg);
};
}

#endif      //MASKING_ACCELERATOR_H
