// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_LISTBOX_H
#define      MASKING_LISTBOX_H

#include "panel.h"
#include "scroller.h"
#include "compound.h"
#include <vector>
#include "maskingdll.h"

namespace MAS {
class ListView;
class ListBox;

/**
   The base class for making different kinds of items that can be put in a ListBox.
   It defines a kind of an interface for all derived list item classes to implement.
   ListItem itself doesn't implement anything else but drawing the item background
   but derived classes may draw text, bitmaps, etc.

   @see ListBox, ListView, ListBoxEx
*/
class MASKING_DLL_DECLSPEC ListItem {
   public:
      ListItem();
      virtual ~ListItem();

      /**
         Draws the list item background onto the canvas. By default skinBitmap is the
         list item bitmap. Note that a list item can have up to eight different states
         and not just the familiar four (normal, selected, disabled and focused). The
         list item's states are as follows:
<pre>
      0 - list does not have focus, item is not selected
      1 - list does not have focus, item is selected
      2 - list has focus, item is not the focus item, item is not selected
      3 - list has focus, item is the focus item, item is not selected
      4 - list has focus, item is the focus item, item is selected
      5 - list has focus, item is not the focus item, item is selected
      6 - list is disabled, item is not selected
      7 - list is disabled, item is selected
</pre>
         This only applies to multiselect lists. In normal mode only states 0, 1, 5
         and 6 are possible and are equivalent to the familiar states NORMAL, SELECTED,
         FOCUSED and DISABLED.

         Derived classes should normally first call this function to draw the background,
         and then draw themselves on top of it.
      */
      virtual void Draw(Bitmap &canvas, Bitmap &skinBitmap, int state);
      /**
         Returns the height of the list item. By default this function returns the height
         of the item bitmap but derived classes should return the actual height of the item.
      */
      virtual int h();
      /**
         Returns the items text if any. This implementation returns NULL.
      */
      virtual char *GetText() const;

      /**
         The item's parent \URL[ListView]{ListView.html}.
      */
      ListView *parent;
      /**
         True if the item is selected, false otherwise.
      */
      bool selected;
};

/**
   A simple list item class for making textual list items.
*/
class MASKING_DLL_DECLSPEC ListItemString : public ListItem {
   protected:
      /**
         The actual list item's text string.
      */
      char *text;

   public:
      ListItemString();
      /**
         Constructor for making a list item from a string.
      */
      ListItemString(const char *text);
      ~ListItemString();

      /**
         Draws the item as a backgorund list button and some text on top of it.
      */
      void Draw(Bitmap &canvas, Bitmap &skinBitmap, int state);
      /**
         Sets the item's text.
      */
      void SetText(const char *text);
      /**
         Returns the item's text.
      */
      char *GetText() const;
};

/**
   An internal helper widget for the ListBox.

   This is the core of a ListBox. It contains a list of list items, it provides an
   inteface for working with this list, it draws the background and the items
   and it handles mouse clicks, mouse moves and keyboard input.

   @see ListItem, ListBox
*/
class MASKING_DLL_DECLSPEC ListView : public Widget {
   protected:
      /**
         The actual list (vector actually) of list items.
      */
      std::vector<ListItem *> items;
      /**
         Stores how far the list has scrolled.
      */
      int offset;
      /**
         The index of the item with the focus (the item under the mouse).
      */
      int focus;
      /**
         True when the list is in multiselect mode. In this mode more than
         one item can be selected at once while in normal mode only one
         can be selected.
      */
      bool multiSelect;
      /**
         If true then the listbox responds to single clicks on the list as activation
      */
      bool singleClick;

   protected:
      /**
         Initilizes the list view with the default bitmaps, fonts and font colours as defined by the skin.
      */
      void MsgInitSkin();
      /**
         Draws the list view by drawing all the list items.
      */
      void Draw(Bitmap &canvas);
      /**
         A list view takes focus so this function returns true.
      */
      bool MsgWantfocus();
      /**
         Handles keyboard input. The list can be navigated with the cursor keys,
         home, end, page up and page down. In multiselect mode items can be
         selected and deselected with spacebar. If enter is pressed (or space
         in normal mode) the list sends MSG_ACTIVATE to the parent dialog.
      */
      bool MsgChar(int c);
      /**
         Handles mouse input. Whenever the left mouse button is pressed the item
         with the mouse is either selected (normal mode) or toggled (multiselect mode).
      */
      void MsgLPress();
      /**
         Issues a MsgDClick() in case the singleClick flag is set.
      */
      void MsgLRelease();
      /**
         Sends MSG_ACTIVATE to the parent dialog.
      */
      void MsgDClick();
      /**
         Makes sure the focus follows the mouse.
      */
      void MsgMousemove(const Point& d);

      /**
         A helper function for deselecting all items.
      */
      void DeselectAllItems();
      /**
         Returns the index of the item the mouse is on top of. If no item is found this function returns -1.
      */
      int FindMouseItem();

   public:
      ListView();
      ~ListView();

      /**
         Returns the number of items in the list.
      */
      int TotalItems();
      /**
         Returns the number of items the list can display on one screen.
      */
      int VisibleItems();
      /**
         Tests if item i is selected or not.
      */
      bool Selected(int i);
      /**
         Selects item i. If multiselect mode is off this will first deselect the previously selected item.
      */
      void Select(int i);
      /**
         Deselects item i.
      */
      void Deselect(int i);
      /**
         Returns the index of the firs selected item in the list, 0 if none are selected.
      */
      int Selection();
      /**
         Attempts to scroll the list to the first selected item.
      */
      void ScrollToSelection();
      /**
         Returns the index of the focused item. In normal mode focused and selected items are one and the same.
      */
      int Focus();
      /**
         Moves focus to item i. In normal mode this also selects item i.
      */
      void SetFocus(int i);
      /**
         Inserts a new item to the list at index i.
      */
      void InsertItem(ListItem *item, int i=0);
      /**
         Returns a pointer to the item with index i.
      */
      ListItem *GetItem(int i);
      /**
         Returns a pointer to the focused item. If no item has focus it returns NULL!
      */
      ListItem *GetFocusedItem();
      /**
         Returns a pointer to the first selected item in the list. In normal mode this
         is the same as the focused item. If no item is selected it returns NULL!
      */
      ListItem *GetSelectedItem();
      /**
         Deletes item i from the list.
      */
      void DeleteItem(int i);
      /**
         Clears the list.
      */
      void DeleteAllItems();
      /**
         Returns how far the list has scrolled.
      */
      int Offset();
      /**
         Sorts the list in ascending order according to the list item text regardless of case.
      */
      void Sort();
      /**
         Sorts the list by using the given comparison function. The function should
         compare two items and return true if the first item is smaller than the second one.
      */
      void Sort(bool (*cmp)(const ListItem *, const ListItem *));
      /**
         Returns whether the list is in single click mode or not
      */
      bool GetSingleClick();
      /**
         Sets the single click mode for the list on or off
      */
      void SetSingleClick(bool single=false);

      friend class MASKING_DLL_DECLSPEC ListBox;
};

/**
   The ListBox widget.
   A ListBox is a widget for displaying lists of items. If there are too many
   items to display they can be scrolled vertically. The ListBox can operate
   in two modes: in normal mode only one item can be selected at any time and
   in multiselect mode more than one item can be selected. Every time the
   selection (or focus) changes the ListBox sends MSG_SCROLL to the parent
   dialog with the index of the focus item as the first argument and when an
   item is clicked or space or enter is pressed it sends MSG_ACTIVATE with the
   index of the selected item as the first argument.

   Note: deprectated in favour of ListBoxEx.

   @see ListItem, ListItemString, ListView
*/
class MASKING_DLL_DECLSPEC ListBox : public CompoundWidget {
   protected:
      /**
         The underlaying panel widget.
      */
      Panel box;
      /**
         The underlaying vertical scroller used to scroll the main list view.
      */
      Scroller vScroller;
      /**
         The underlaying ListView object.
      */
      ListView *list;

   protected:
      /**
         Initializes the widget with the default bitmaps, fonts and colours.
      */
      void MsgInitSkin();
      /**
         Updates the size and position of the underlaying widgets when the
         listbox itself is resized.
      */
      void UpdateSize();
      void MsgGotfocus();
      void MsgLostfocus();

   public:
      ListBox();
      ListBox(ListView *l);
      ~ListBox();

      /**
         ListBox event handler. Handles events such as scrollbar scrolls
         and clicks in the list view.
      */
      void HandleEvent(Widget& obj, int msg, intptr_t arg1=0, intptr_t arg2=0);

      /**
         Selects either the multiselect or normal mode. In multiselect mode
         more than one item can be selected at once while in normal mode only
         one can be selected. Default is normal mode.
      */
      void SetMultiSelect(bool m);
      /**
         Returns the number of items in the list.
      */
      int Size();

      /** */bool Selected(int i);
      /** */void Select(int i);
      /** */void Deselect(int i);
      /** */int Selection();
      /** */void ScrollToSelection();
      /** */int Focus();
      /** */void SetFocus(int i);
      /** */void InsertItem(ListItem *item, int i=0);
      /** */ListItem *GetItem(int i);
      /** */ListItem *GetFocusedItem();
      /** */ListItem *GetSelectedItem();
      /** */void DeleteItem(int i);
      /** */void DeleteAllItems();
      /** */void Sort();
      /**   */bool GetSingleClick();
      /**   */void SetSingleClick(bool single=false);
      /**
         These a are basically just wrappers for functions with the same names in \URL[ListView]{"ListView.html"}.
      */
      void Sort(bool (*cmp)(const ListItem *, const ListItem *));

      friend class MASKING_DLL_DECLSPEC ComboBox;
      void Dump();
};
}

#endif      //MASKING_LISTBOX_H
