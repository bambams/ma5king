// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_LISTBOX_EX_H
#define      MASKING_LISTBOX_EX_H

#include "maskingdll.h"
#include "scrollbox.h"

#include <vector>
#include <string>
#include <algorithm>

namespace MAS {
/**
   The ListBoxEx widget.
   A ListBoxEx is a widget for displaying lists of items. It provides many modes
   of operation. By default it behaves like a plain single column list box like
   the one found in the default Allegro GUI. But it can also accept an arbitrary
   number of column that can be interactively resized. It can behave as a normal
   single select list where only one item at a time is selected, or as a multi
   select list where any number of items can be selected. Column headers can be
   forcefully turned off if required. Clicking a column header automatically
   sorts the list lexicographically according to the selected column. Both the
   item and the header classes can be easily extended to support custom look,
   feel and behaviour.

   ListBoxEx sends MSG_SCROLL to the parent dialog whenever the selection
   changes with the index of the selected item as the first argument to the
   HandleEvent() function. When an item is double clicked (or single clicked
   if the single click option is turned on), or the enter or space keys are
   pressed, MSG_ACTIVATE is sent with the index of the item that was activated
   as the first argument. MSG_ACTIVATE can be overridden with a custom
   message with the Wiget::SetCallback() function.
*/
class MASKING_DLL_DECLSPEC ListBoxEx : public ScrollBox {
   public:
      /**
         Represents one column item. ListBox can contain any number of columns.
      */
      class MASKING_DLL_DECLSPEC Column {
         public:

         /**
            header text
         */
         std::string title;

         /**
            width in pixels; if negative, appropriate  width is calculated automatically
         */
         int width;

         /**
            title alignment: 0 (left), 1 (right) or 2 (centre)
         */
         int alignment;

         /**
            Used when sorting the list. Each column has an order value, 1 for ascending,
            -1 for descending. Every time a column header is clicked the list is sorted
            in that column's order and the order is flipped so that the next time reverse
            sorting is used.
         */
         int sortOrder;
      };

      /**
         The listbox header widget. Draws all column headers and allows individual
         headers to be clicked. You can derive your own headers from it to implement
         custom drawing or behaviour.
      */
      class MASKING_DLL_DECLSPEC Header : public Button {
         public:
            /**
               pointer to a vector of columns; the actual list is owned by the listbox
            */
            std::vector<Column> *columns;

         protected:
            /**
               index of the column the mouse is currently hovering over
            */
            int mouseColumn;

            /**
               index of the column that is being resized; -1 if no column is being resized
            */
            int resizingColumn;

            /**
               the distance between the mouse x position and the border of the column at
               the time when resizing is initiated
            */
            int resizeOffset;

         protected:
            /**
               draws the header
            */
            void Draw(Bitmap &canvas);

            /**
               updates mouseColumn on mouse move
            */
            void MsgMousemove(const Point &d);

            /**
               handles resizing the columns with the mouse; called by MsgMousemove()
            */
            void OnResize();

            /**
               handles switching focus between header columns with the mouse; called by MsgMousemove()
            */
            void OnMousemove();

            void MsgLRelease();
            void MsgLostmouse();

            /**
               Draws one background for one column header. Overload this to implement custom drawing
               of the header background. The default implementation draws the column header backgrounds
               using the bitmap normally used to skin buttons.

               @param dst destination bitmap
               @param item pointer to the item being drawn
               @param state status of the item (normal, selected, disabled or focused)
               @param dstRect rectangle into which the header should be rendered
            */
            virtual void DrawItemBackground(Bitmap &dst, Column *item, int state, const Rect &dstRect);

            /**
               Draws the contents of one column header (i.e. the column title). Overload to implement
               custom drawing of the header contents (i.e. the header titles).

               @see DrawItemBackground
            */
            virtual void DrawItemContents(Bitmap &dst, Column *item, int state, const Rect &dstRect);

            /**
               Returns the preferred height of the header. The listbox may actually ignore this but
               will try to use it if possible. Overload this in a derived header class if you want
               a differently sized header.
            */
            virtual int PreferredHeight();

         public:
            Header();
            ~Header();

            friend class ListBoxEx;
      };

      /**
         Represent one listbox item (one row of values)
      */
      class MASKING_DLL_DECLSPEC Item {
         protected:
            /**
               Vector of item values (strings), one for each column.
            */
            std::vector<std::string> cells;

            /**
               A pointer to the item's parent listbox. The item uses this pointer
               to access bitmaps, fonts and colours in the skin that is used to
               skin the listbox.
            */
            ListBoxEx *parent;

            /**
               True when the item is selected, false otherwise.
            */
            bool selected;

         public:
            /**
               Index of the column that is to be used for sorting. The listbox sets this
               value as necessary when sorting the list.
            */
            int sortColumn;

            /**
               The sorting order. Positive for ascending order, negative for descending.
            */
            int sortOrder;

            /**
               The less than operator. Used for sorting lists. Overload this for custom
               sorting. The default implementation sorts lexicographically. The operator
               should use sortColumn and sortOrder to calculate the result. It should
               return true if this item should come before the item in the right side of
               the comparison expression, false if it should come after. If items are
               equal, the order is arbitrary.
            */
            virtual bool operator<(const Item &rhs) const;

            static bool cmp_sort(const Item *i1, const Item *i2) {
               return (*i1 < *i2);
            }

            long userData; //don't change long to int - 64 bits processors

         protected:

            /**
               Draws background for the entire row. State is different than in most other
               widgets. Each item can be in one of 8 states (as opposed to just 4):
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
               and 6 are possible and are equivalent to the familiar states NORMAL,
SELECTED,
               FOCUSED and DISABLED.

               @param dst the destination bitmap
               @param state the state of the item as described above
               @param dstRect the clipping rectangle of the item; all drawing should be done inside this rectangle

               Overload this to implement custom drawing of the list item backgrounds.
            */
            virtual void DrawBackground(Bitmap &dst, int state, const Rect &dstRect);

            /**
               Draws contents (i.e. the text) of the specified column of this item. All
               parameters are the same as for DrawBackground() with the addition of i
               which is the index of the column to be drawn. Overload this to implement
               custom drawing of list box items.

               @see DrawBackground
            */
            virtual void DrawItem(Bitmap &canvas, int i, int state, const Rect &dstRect);

            /**
               Draws gridlines for this item and column i. Default implementation draws the
               gridlines by drawing one vertical dotted line on the rightmost side of the item
               and one horizontal dotted line at the bottom of the item. Overload to draw some
               fancy custom gridlines.

               @see DrawItem
            */
            virtual void DrawGrid(Bitmap &canvas, int i, int state, const Rect &dstRect);

         public:
            /**
               Inserts a new column (subitem) with an empty string at the specified index. If index is a
               negative number, new column is added to the en of the list. The listbox makes
               sure each item in the list always has the same number of subitmes as there are columns
               in the listbox.
            */
            virtual void InsertColumn(int index=-1);

            /**
               Deletes the specified column.
            */
            virtual void DeleteColumn(int index);

            /**
               Returns the number of subitems in this item.
            */
            virtual int GetColumnCount();

            /**
               Returns the rendering height of the item. The height of the skin bitmap used for
               rendering the list items is the default height. Overload this if you want items
               of different size.
            */
            virtual int h();

            /**
               Checks whether the item is selected or not.
            */
            virtual bool IsSelected();

            /**
               Selects the item.
            */
            virtual void Select();

            /**
               Deselects the item.
            */
            virtual void Deselect();

            /**
               Toggles item selection status.
            */
            virtual void Toggle();

         public:
            Item();

            /** Copy constructor */
            Item(const Item &rhs, intptr_t userData=0);

            /** Assignment operator */
            Item& operator=(const Item &rhs);

            virtual ~Item();

            /**
               Sets the value of the specified subitem.
            */
            virtual void SetText(const char *str, int index=0);

            /**
               Retreives the value of the specified subitem.
            */
            virtual const char *GetText(int index=0) const;

            friend class ListBoxEx;
      };

      /**
         Invisible helper widget the listbox uses for getting input (keyboard, mouse). It is setup
         so that it covers all the space not covered by the headers and the scrollers (i.e. the
         client are minus the headers). Basically it just takes input focus when offered and
         forwards all the messages to the parent listbox. A bit of a hack but require because of
         an oversight in the design of the Dialog class.
      */
      class MASKING_DLL_DECLSPEC InputHandler : public Widget {
         protected:
            /**
               makes sure the widget accepts the mouse when offered
            */
            bool MsgWantmouse() { return true; }

            /**
               makes sure the widget accepts input focus when offered
            */
            bool MsgWantfocus() { return true; }

            /**
               intercepts some of the keys that are usually used for switching focus (e.g. arrow
               keys) and forwards the message to the parent listbox for further processing
            */
            bool MsgChar(int c);

            void MsgGotfocus();
            void MsgLostfocus();

            /*
               NOTE: all other input message handlers are forwarded to the widget's parent
               automatically by the Widget class so there's no need to reimplement them
            */
      };

   protected:
      /**
         the array of column headers
      */
      std::vector<Column> columns;

      /**
         Pointer to a header widget used for renering column headers. Users may override this
         with their own custom header widget (for example one that draws some fancy icons or
         something) which is the reason why a pointer is stored.
      */
      Header *header;

      /**
         The listbox's own default header widget. Used by default in case the user doesn't
         supply their own header object (i.e. almost always).
      */
      Header ownHeader;

      /**
         The array of listbox items. Pointers are used because the users may derive their own
         special item classes from the default one to implement some custome behaviour.
      */
      std::vector<Item *> items;

      /**
         The invisible input helper widget.
      */
      InputHandler inputHandler;

      /**
         The index of the item the mouse is currently hovering over. This is also used when
         browsing the listbox with the keyboard. In single select mode the focused item is
         always immediately selected.
      */
      int focusedItem;

      /**
         The currently selected item. Only relevant in single select mode.
      */
      int selectedItem;

      /**
         True if header should be shown, false otherwise. This is determined automatically
         from the list contents, but can be overridden by the user. Lists with one column
         and no column title automatically hide the header, in all other cases the header
         is automatically turned on.
      */
      bool showHeader;

      /**
         True if the list is in multiselect mode, false otherwise. When in multiselect
         mode any item can be selected or deselected independantly of all other items.
         In single select mode exactly one items is always selected (except when the
         list is empty of course). Default is false.
      */
      bool multiSelect;

      /**
         Defines how many clicks are necessary to activate an item. If true, one click
         activates, otherwise a double click is required. The first is useful for drop
         down lists while the other is usually better for normal lists. The default is
         false.
      */
      bool singleClick;

      /**
         Temporary variable used for iterating through the selected items of the list
         with GetFirstSelectedIndex() and GetNextSelectedIndex().
      */
      int iterationIndex;

      /**
         True if clicking on a header column should sort the list, false otherwise.
         Default is false.
      */
      bool autoSort;

      /**
         True if column resizing is enabled, false otherwise. Default is true.
      */
      bool columnResizing;

      /**
         True if gridlines should be drawn between cells of the listbox. Default is false.
      */
      bool gridLines;

   protected:
      void MsgInitSkin();
      void MsgGotfocus();

      /**
         Called after every change in listbox geometry to update the size and position of
         the headers widget and the fake input handler.
      */
      void UpdateSize();

      /**
         Draws the contents of the listbox (i.e. the list items) with the current scroll offset.
      */
      void DrawClientArea(Bitmap &dest, int xofst, int yofst);

      /**
         Called on every geometry change as well as every time the contents of the listbox
         have been modified (i.e. items or columns added or removed) to update the scrollers.
      */
      virtual void UpdateScrollers();

      /*
         The following functions are triggered by the events passed on by input handler.
      */

      /**
         Called on every mouse move. Handles changing focus item on mouse moves.
      */
      virtual void OnMousemove();

      /**
         Called on every key press while the list has input. Handles switching focus
         and/or selection between items and activates items on enter or space.
         Overload this to implement some custom behaviour, like for example a listbox
         that allows editing of the items or something similar.
      */
      virtual void OnChar(int c);

      /**
         Called whenever the left mouse button is clicked on the list. Handles changing
         item selection status with the mouse.
      */
      virtual void OnLPress();

      /**
         Called on every mouse wheel scroll event while the list has input focus. Handles
         scrolling the list with the mouse wheel.
      */
      virtual void OnWheel(int z);

      /**
         Called when an item is double clicked. Handles item activation on double clicks
         in case single click mode is not enabled.
      */
      virtual void OnDClick();

   public:
      ListBoxEx();
      ~ListBoxEx();

      /**
         Sets a custom header object. If you derive a custom header class from Header, create
         a new instance and pass the pointer to it to this function. The listbox then owns
         this object so you must not attempt to delete it yourself, it will be done automatically
         for you.

         @see Header
      */
      virtual void SetHeader(Header *header);

      /**
         Returns a pointer to the header object. Useful for manipulating custom headers.
         You probably shouldn't call any methods the Header inherits from the button class
         otherwise logic and appearance of the listbox might get messed up. Typically you
         will only need to call this function to opbtain a pointer to a custom header
         object, so you can for example feed it some additional data.

         @see Header
      */
      virtual Header *GetHeader();

      /**
         Adds a new column to the listbox.

         @param titleText the column title
         @param index the position where you want to add the new column; if negative the column will be added to the end of the column list
         @param align alignment of the column title: 0 is left, 1 is right and 2 is centre alignment
         @param colWidth width of the column; if -1 the most appropriate width will be calculated automatically

         @return Index of the position where the column was actually added or -1 on failure (e.g. invalid index).
      */
      virtual int InsertColumn(const char *titleText, int index=-1, int align=0, int colWidth=-1);

      /**
         Deletes the specified column.
      */
      virtual void DeleteColumn(int index);

      /**
         Returns the number of columns in the listbox.
      */
      virtual int GetColumnCount();

      /**
         Sets the title for the specified column.
      */
      virtual void SetColumnTitle(int index, const char *title);

      /**
         Retreives the title of the specified column.
      */
      virtual const char *GetColumnTitle(int index);

      /**
         Sets the width of the specified column. Width may be -1 to allow the listbox
         to determine the actual width of the item automatically.
      */
      virtual void SetColumnWidth(int index, int width);

      /**
         Retreives the width of the specified column. Returns -1 if the index is invalid.
      */
      virtual int GetColumnWidth(int index);

      /**
         Adds a new item to the list. The item is added at the specified index or at
         the end of the list if index is a negative number. After adding an item, the
         listbox owns it, so you must not attempt to delete it.

         Note: If a column hadn't been added to the listbox yet, one will be created automatically.

         @return The index of item where it was actually added to the list or -1 on failure.

         @see Item
      */
      virtual int InsertItem(Item *item, int index=-1);

      /**
         Adds an item with the specified text in the first column.
      */
      virtual int InsertItem(const char *item, int index=-1);

      /**
         Removes the specified item from the list.
      */
      virtual void DeleteItem(int index);

      /**
         Clears the list of all items.
      */
      virtual void DeleteAllItems();

      /**
         Shortcut for setting the text at the specified row and column. If the specified
         column doesn't exist yet, enough columns are added to the list so that this
         function may succeed.

         @param row index of the item you want to change
         @param column index of the column you want to change
         @param str the new item text
      */
      virtual void SetItem(int row, int column, const char *str);

      /**
         Retreives the text at the specified row and column. Returns NULL if the
         given coordinates are invalid.
      */
      virtual const char* GetItem(int row, int col);

      /**
         Retreives the item at the specified row. Returns NULL if the row is invalid.
      */
      virtual Item *GetItem(int row);

      /**
         Returns number of items (rows) in the list.
      */
      virtual int GetItemCount();

      /**
         Scrolls the listbox if necessary so that the item with the specified index
         becomes visible. If force is true, the function scrolls regardless of the
         previous status, else it scrolls only if necessary. Returns true if any
         scrolling was actually done, otherwise returns false.
      */
      virtual bool ScrollToItem(int index, bool force=false);


      /**
         Overrides the automatically determined header visibility status. Pass false
         to turn the heaer off on multicolumn lists.
      */
      virtual void SetHeaderVisibility(bool on);

      /**
         Returns user's header visibility setting.
      */
      virtual bool GetHeaderVisibility();

      /**
         Enables or disables multiselect mode. If multiselect mode is turned off while
         more than one item is selected, all items are deselected except for the first
         one that was selected. If none were selected, the first one is selected after
         calling this function.
      */
      virtual void EnableMultiSelect(bool on=true);

      /**
         Returns the value of the multiSelect variable.
      */
      virtual bool IsMultiSelectEnabled();

      /**
         Counts the number of selected items.
      */
      virtual int GetSelectedCount();

      /**
         Returns true if the specified item is selected, false otherwise. If index
         is invalid, returns false.
      */
      virtual bool IsSelected(int index);

      /**
         Selects the item at the specified index. In single select mode the previously
         selected item is deselected.
      */
      virtual void Select(int index);

      /**
         Deselects the item at the specified index if it is not deselected already.
         In single select mode, this function does nothing.
      */
      virtual void Deselect(int index);

      /**
         Returns a pointer to the selected item. In multiselect mode the first
         selected item is returned, or NULL if no items are selected.
      */
      virtual Item *GetSelectedItem();

      /**
         Returns the index of the selected item. This function is equivalent to
         GetFirstSelectedIndex().
      */
      virtual int GetSelectedIndex();

      /**
         Returns the index of the item (row) the mouse cursor is on top of. Returns -1
         if the mouse is not top of a list item.
      */
      virtual int GetMouseRowIndex();

      /**
         Returns the index of the item (column) the mouse cursor is on top of. Returns -1
         if the mouse is not top of a list item.
      */
      virtual int GetMouseColumnIndex();

      /**
         Toggles the state of the specified item.
         In multiselect mode selects the item at the specified index if it was
         previously not selected or deselects it if it was. Has no meaning in
         single select mode.
      */
      virtual void Toggle(int index);

      /**
         Deselects all previously selected items. Only relevant in multiselect mode.
      */
      virtual void DeselectAllItems();

      /**
         Returns the index of the first selected item or -1 if no items are selected.
      */
      virtual int GetFirstSelectedIndex();

      /**
         Returns the next selected item or -1 if no more items are selected. This
         function is used to iterate through the selected items of the list. To
         initialize the iteration first call GetFirstSelectedIndex() then this
         function until it returns -1.
      */
      virtual int GetNextSelectedIndex();

      /**
         Sets the value of the singleClick variable.
      */
      virtual void EnableSingleClick(bool on=true);

      /**
         Gets the value of the singleClick variable.
      */
      virtual bool IsSingleClickEnabled();

      /**
         Sorts the list.
         The list is sorted in either ascending or descending order according to
         the specified column and order.

         @param column The index of the column used for sorting.
         @param order Order of sorting. Positive values (typically 1) select ascending order, negative values (typically -1) select descending order.

         Sorting requires being able to compare items. For that the less than
         operator of the Item class is used. The default implementation sorts the
         items lexicographically. To implement custom sorting (e.g. by numerical
         values, dates, etc) simply overload    the less than operator in your
         custom item class.

         @see Item
      */
      virtual void Sort(int column=0, int order=1);

      /**
         Enables or disables automatic list sorting. When automatic sorting is on,
         the list is automatically sorted when a column header is clicked.
         Explicitely calling Sort() still sorts the list even when autosorting is
         disabled. By default autosorting is disabled.
      */
      virtual void EnableAutosort(bool on=true);

      /**
         Returns the status of autosorting.
      */
      bool IsAutosortEnabled();

      /**
         Enables or disables column resizing
      */
      void EnableColumnResizing(bool on=true);

      /**
         Checks whether column resizing is enabled or not.
      */
      bool IsColumnResizingEnabled();

      /**
         Turns gridlines between cells of the list on or off.
      */
      void EnableGridLines(bool on=true);

      /**
         Checks whether gridlines are turned on or off.
      */
      bool AreGridLinesEnabled();

      /**
         Event handler. Basicalyl jsut maps messages from the input handler to the OnSomething() functions.
      */
      void HandleEvent(Widget &obj, int msg, intptr_t arg1=0, intptr_t arg2=0);

      friend class ListBoxEx::Item;
};
}

#endif      //MASKING_LISTBOX_EX_H
