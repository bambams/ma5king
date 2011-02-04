// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_COMPOUND_WIDGET_H
#define      MASKING_COMPOUND_WIDGET_H

#include "dialog.h"
#include "maskingdll.h"

namespace MAS {
/**
 An intermediate virtual class for making compound widgets.
 A compound widget is basically a dialog but some of its wunctions are slightly
 different from a normal user dialog. This class serves as an intermediate
 layer between a dialog and compound widgets (such as listboxes, combo boxes, etc)
 All compound widgets should be derived from this class instead of the dialog
 class directly.
*/
class MASKING_DLL_DECLSPEC CompoundWidget : public Dialog {
   public:
      CompoundWidget() : Dialog() {
         ClearFlag(D_TOPLEVEL);
      }

   protected:
      /**
         Supposed to fill a list with objects that can be tabbed to. In this
         class the implementation of this function is empty. Every widget
         should take care of forwarding input focus to the right subwidget
         when tabbed to by itself by overloading MsgGotfocus() and giving
         focus to the appropriate widget with GiveFocusTo().
      */
      void FillObjectList(std::vector<MAS_OBJ_LIST *> &objList, int (*cmp)(const MAS::Widget *, const MAS::Widget *), MAS::Widget *focusObject, int &c) {}

      /**
         Returns true to indicate a compound widget can take input focus.
         A derived compound widget should overload this function and return
         false in case it does not take input focus.
      */
      bool MsgWantTab() { return true; }
      bool MsgWantfocus() { return true; }
};
}

#endif      //MASKING_COMPOUND_WIDGET_H
