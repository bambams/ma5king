// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_MESSAGEBOX_H
#define      MASKING_MESSAGEBOX_H

#include "window.h"
#include "maskingdll.h"

namespace MAS {
/**
   Standard message box dialog.
   A MessageBox is a simple dialog that can hold up to three lines of text
   and up to three buttons that all close it. It is primarily used for
   informing the user that a certain event has happened and for asking the
   user simple questions that can be answered with yes and no or something
   similar.

   Note: the name MessageBox clashes with a define in the Win32 API.
   Micro$soft was stupid enough to call their message box function
   <i>MessageBoxA</i> and then they defined <i>MessageBox</i> as
   <i>"#define MessageBox MessageBoxA"</i> (most probably for backwards
   compatibility or something). If for some reason you need to use the Win32
   API alongside MASkinG you have to undefine the Win32 MessageBox right
   after including the windows header:
<pre>
      #include &lt;windows.h&gt;
      #undef MessageBox
</pre>
*/
class MASKING_DLL_DECLSPEC MessageBox : public Window {
   public:
      /** */Button button1;
      /** */Button button2;
      /** */Button button3;
      /** */Label line1;
      /** */Label line2;
      /**
         The three buttons and labels. Note that all of these are public
         so you can access and change them as you wish.
      */
      Label line3;

   protected:
      /**
         Calculates the sizes and positions of all the labels and buttons
         according to the lengths of all the strings.
      */
      void CalculateGeometry();

   public:
      /**
         The MessageBox constructor. You can setup your message box in the
         constructor but you can change it at any time you want. The first
         parameter is the box's title, the next three are the three lines
         of text the box is to display and the last three are the labels
         of the three buttons. If you pass NULL instead of a valid string
         the relevant label or button will not be shown but a message box
         must have at least one label and at least on button.
      */
      MessageBox(   const char* = "Message",
               const char* = "This is a message.",
               const char* = NULL,
               const char* = NULL,
               const char* = "OK",
               const char* = NULL,
               const char* = NULL);

      /**
      */
      virtual int Popup(Dialog *p, int x = INT_MAX, int y = INT_MAX);
      /**
         Pops up the mesage box at the given position. Returns the 1 based
         index of the button that was used to close the dialog. If the exit
         icon was used or the box was escaped 0 is returned.

         Example:
<pre>
      bool MainDialog::MsgClose() {
        if (HaveUnsavedData()) {
           MessageBox msg("Unsaved data", "Do you want to save your data before exiting?", NULL, NULL, "Yes", "No", "Cancel");
           switch (msg.Popup(this)) {
              case 1:
                 Save();

              case 2:
                 return true;

              default:
                 return false;
           };
        }
        else {
           MessageBox msg("Really?", "Do you really want to exit?", NULL, NULL, "OK", "Cancel");
           if (msg.Popup(this) == 1) {
              return true;
           }
           else {
              return false;
           }
        }
      }
</pre>
      */
      virtual int Popup(Dialog *p, const MAS::Point &pos);
};
}

#endif         //MASKING_MESSAGEBOX_H
