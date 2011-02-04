// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
//                     Modifications by Vladimir Kokovic.                    //
// ************************************************************************** //

#ifndef      MASKING_MENU_H
#define      MASKING_MENU_H

#include "dialog.h"
#include "panel.h"
#include "maskingdll.h"

namespace MAS {
class Menu;

/**
   Internal helper widget for making menu items.

   MenuItem implements a single item of a complex menu. Every item may have
   either a child menu which pops up when the item is selected or after a
   certain time delay or a message ID which is sent to the parent menu when
   the item is selected.

   @see Menu
*/
class MASKING_DLL_DECLSPEC MenuItem : public Widget {
   protected:
      /**
         The item's text. See above for a description of the text formatting.
      */
      char *text;
      /**
         A pointer to the menu item's child menu if any.
      */
      Menu *child;
      /**
         The ID of the message the item is to send to its parent menu when it
         is selected. This should normally be above the normal GUI MSG_SOMETHING
         constants, for example MSG_SUSER+1, MSG_SUSER+2, etc.
      */
      int callbackID;
      /**
         True if the item is part of a top bar menu.
      */
      bool topBar;
      /**
         A frame counter that help implement a short delay before a submenu
         automatically pops up when the mouse is on top of an item.
      */
      int delayTimer;

      bool allocated;

   protected:
      /**
         Initilizes the menu item with the default bitmaps, fonts and font colours as defined by the skin.
      */
      void MsgInitSkin();
      /**
         Returns true.
      */
      bool MsgWantfocus();
      /**
         Start the timer counter that counts frames and controls when a submenu automatically pops up.
      */
      void MsgGotfocus();
      /**
         Stops the timer counter that counts frames and controls when a submenu automatically pops up.
      */
      void MsgLostfocus();
      /**
         Handles mouse clicks with the left mouse button.
      */
      void MsgLPress();
      /**
         Handles mouse clicks with the left mouse button.

         When a menu item is clicked it either pops up its submenu or sends
         its callback message ID to the parent menu.
      */
      void MsgLRelease();
      /**
         Increments the frame counter and after a certain time has passed it pops up the child menu.
      */
      void MsgTick();
      /**
         Handles keyboard input.
      */
      void MsgKey();
      bool MsgChar(int c);
      /**
         Draws the menu item.
      */
      void Draw(Bitmap &canvas);
      /**
         Resizes the item according to the length of the text.
      */
      void UpdateSize();
      /**
         Returns false to prevent the menu items from  being tabbed to.
      */
      bool MsgWantTab();

   public:
      /**
         The constructor for making a menu item with a child menu.
      */
      MenuItem(const char *text, Menu *child);
      /**
         The constructor for making a menu item with a message ID.

         A menuitem may have a submenu or a mesage ID (or none of those
         but that's quite pointless). The text of a menu item is made
         of two parts separated with a horizontal tab character ('\t').
         The first part is printed on the left part of the item while the
         other part is printed on the right. Usually the left part contains
         the name of a command or selection and the right part contains a
         description of the matching keyboard shortcut. Also the left part
         may contain a character that is prefixed with '&'. This character
         will then be underlined and will act as a quick shortcut for
         navigating the menu. For example the text <i>"&New\tCtrl-N"</i>
         will produce a menu item that will read <i>"<u>N</u>ew          Ctrl-N"</i>.
         If text is NULL, the menu item will be a separator.
      */
      MenuItem(const char *text = NULL, int id = 0);
      virtual ~MenuItem();

      /**
         Returns the item's text.
      */
      virtual char *GetText();
      /**
         Sets the item's text.
      */
      virtual void SetText(const char *t);

       /**
         Returns callbackID of the menu item
      */
      virtual int GetCallbackID(){ return callbackID; }

      /**
         Returns pointer to child menu if any
      */
      virtual Menu* GetChildMenu(){ return child; }

      friend class MASKING_DLL_DECLSPEC Menu;
};

/**
   The Menu widget class.

   Implements the entire menu system. A menu can be a top bar menu or a popup menu.
   Every menu item can have either a submenu that pops up when the item is selected
   or an message ID which is sent to the parent dialog when the item is selected.

   Note: menu navigation with keyboard hasn't yet been implemented!

   @see MenuItem
*/
class MASKING_DLL_DECLSPEC Menu : public Dialog {
   protected:
      /**
         The menu's background panel.
      */
      Panel back;
      /**
         True if the menu is a top bar menu.
      */
      bool topBar;
      /**
         The menu's parent menu if any.
      */
      MenuItem *parentMenu;
      /**
         The currently open child menu if any.
      */
      Menu *openChildMenu;

      static std::vector<MenuItem *> itemsHistory;
      static std::vector<Menu *> menusHistory;

   protected:
      /**
         Initilizes the menu with the default bitmaps, fonts and font colours as defined by the skin.
      */
      void MsgInitSkin();
      /**
         Hnadles menu shortcuts (Alt + key) even when the menu doesn't have input focus.
      */
      bool MsgXChar(int x);
      /**
         Hnadles submenu shortcuts.
      */
      bool MsgChar(int c);
      /**
         Closes a popup menu after it has lost the mouse.
      */
      void MsgLostmouse();
      void MsgLostfocus();
      bool MsgWantfocus();
      bool MsgWantTab();

      /**
         Calculates the size of the menu and applies it. The height of a single menu item
         is defined by the skin while the width of the menu is the width of the widest menu item.
      */
      void UpdateSize();
      /**
         Checks if the mouse is on top of the menu or a parent menu (recursively).
      */
      bool MouseInMenu();
      /**
         Recursively checks if the mouse is on top of any of the menu's child menus.
      */
      bool MouseInChildMenu();
      /**
         Pops up the item's child menu.
      */
      void OpenChildMenu(MenuItem *item);
      /**
         Closes the currently open child menu.
      */
      void CloseChildMenu();
      /**
         Returns a pointer to the root menu of the menu structure.
      */
      Menu *GetRootMenu();
      /**
         Looks for a shortcut in the menu item list. If it finds an item with the
         given shortcut it returns the shortcuts ASCII code otherwise it returns 0.
      */
      int MenuAltKey(int k);
      /**
         Checks whether the string s contains shortcut c. Shortcut is the
         character prefixed with '&' if any.
      */
      bool MenuKeyShortcut(int c, const char *s);
      /**
         Makes sure a menu can't be moved.
      */
      ActionType GetAction();
      /**
         Moves the input focus around the menu in the given direction.
      */
      void MoveFocus(int dir);

   public:
      Menu();
      virtual ~Menu();
      /**
         Handles events such as keyboard input, menu item clicks, etc.
      */
      void HandleEvent(Widget& obj, int msg, intptr_t arg1=0, intptr_t arg2=0);

      /**
         Adds an existing menu item to the bottom of the menu.
      */
      virtual void Add(MenuItem *item);
      /**
         Creates a new menu item with a submenu and adds it to the bottom of the menu.
      */
      virtual void Add(const char *text, Menu &child);
      /**
         Creates a new menu item with a message ID and adds it to the bottom of the menu.
      */
      virtual void Add(const char *text = NULL, int id = 0);
      /**
         Removes item i from the menu item list. If the menu created the menu item it
         will delete it as well but if you created it yourself, you have take care of
         destroying it.
      */
      virtual void Remove(int i);
      /**
         Removes the item with the given callback ID.
      */
      virtual void RemoveByID(int ID);
      /**
         Returns a pointer to menu item i from the menu item list.
      */
      virtual MenuItem *GetItem(int i);
      /**
         Finds and returns the index of the menu item with the given callback ID.
         If the item with the requested ID isn't find this function returns -1.
      */
      virtual int GetItemByID(int ID);

      /**
         Enables item i.
      */
      virtual void Enable   (int i);
      /**
         Disables item i. Disabled items are greyed out and can't be selected.
      */
      virtual void Disable (int i);
      /**
         Checks item i. Checked items usually have a little checkmark drawn on
         their right side (that depends on the skin).
      */
      virtual void Check   (int i);
      /**
         Unchecks item i.
      */
      virtual void Uncheck (int i);
      /**
         Checks whether item i is disabled or not.
      */
      virtual bool Disabled(int i);
      /**
         Checks whether item i is checked or not.
      */
      virtual bool Checked (int i);

      /**
      */
      virtual void Popup(Dialog *parentDialog, MenuItem *parentMenu, int x, int y);
      /**
         Pops up a menu at the given position. You have to pass the menu's parent
         dialog (i.e. the dialog from where or on top of which the menu was popped
         up) and the menu's parent menu if any (usually NULL). Note that Popup() doesn't
         block the program but instead just adds the menu to the parent dialog and
         then execution continues in that dialog's main loop.
      */
      virtual void Popup(Dialog *parentDialog, MenuItem *parentMenu, const Point &p);
      /**
         Closes a menu that was popped up with Popup(). You shouldn't normally have to
         call this as a menu closes itself when it loses focus.
      */
      virtual void Close();

      void GetHistory( std::vector<MenuItem *> * & iHistory, std::vector<Menu *> * & mHistory );

      friend class MASKING_DLL_DECLSPEC MenuItem;
};
}

#endif      //MASKING_MENU_H
