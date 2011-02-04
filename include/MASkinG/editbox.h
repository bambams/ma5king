// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_EDITBOX_H
#define      MASKING_EDITBOX_H

#include "label.h"
#include "maskingdll.h"

#ifdef DrawText
#undef DrawText
#endif


namespace MAS {
/**
 An EditBox is like a label except that the text can be edited. It is rendered with a sunken panel
background into which the text is printed. If there is too much text to fit into the panel it can be
scrolled. EditBox also allows parts of the text to be selected.
*/
class MASKING_DLL_DECLSPEC EditBox : public Label {
   protected:
      /**
      The size of the text buffer and efectively the maximum number of characters the edit box is able to hold.
      */
      int maxLength;
      /**
      The position of the text cursor.
      */
      int cursorpos;
      /**
      How far the edit box has scrolled into the string.
      */
      int offsetpos;
      /**
      These variables help handling the selection of text.
      */
      int selStart, selEnd, firstSel, oldSel;
   protected:
      /**
      A helper function for setting the text. Allocates memory if necessary and copies the text over.
      */
      virtual void Setup(const char *text, int len);
      /**
      Draws as much of the text as it fits on the canvas from the given offset and returns the index of the
      last character that was drawn.
      */
      virtual int DrawText(int offset, Bitmap& canvas);
      /**
      Finds the index of the character where the mouse cursor is.
      */
      virtual int FindCursorPos();
      /**
      Scrolls the text after a key press if necessary.
      */
      virtual void UpdateOffset();

   protected:
      // Message functions
      /**
      Draws the widget onto the canvas.
      */
      void Draw(Bitmap &canvas);
      /**
      Initializes the widget with the default skin bitmap, fonts and colours.
      */
      void MsgInitSkin();
      /**
      These two functions handle keyboard input. MsgChar() handles cursor navigation and text management (
      selecting, deleting...) while MsgUChar() handles text input. The cursor may be freely moved with the
      arrow keys and HOME and END. The text can be selected with the same keys if the SHIFT key is pressed
      simultaneously. The text can be deleted with both the DELETE and BACKSPACE keys. When ENTER is pressed
      the EditBox sends a MSG_ACTIVATE message to its parent dialog.
      */
      bool MsgChar(int c);
      /**
      See MsgChar()
      */
      bool MsgUChar(int c);
      /**
      This function handles selecting the text and moving the text cursor with the mouse.
      */
      void MsgLPress();
      /**
      This function handles selecting the text and moving the text cursor with the mouse.
      */
      void MsgLRelease();
      /**
      This function handles selecting the text and moving the text cursor with the mouse.
      */
      void MsgMousemove(const Point &d);
      /**
      Selects the entire text.
      */
      void MsgDClick();
      /**
      When the edit box gets the mouse it changes the mouse cursor to the text selection cursor.
      */
      void MsgGotmouse();
      /**
      See MsgGotMouse()
      */
      void MsgLostmouse();
      /**
      Returns true.
      */
      bool MsgWantfocus();

   public:
      EditBox();

      /**
      A shortcut for Widget::Setup() and SetText().
      */
      virtual void Setup(int x, int y, int w, int h, int key, int flags, const char *text, int len = -1);
      /**
      Puts text into the edit box. The second parameter is the maximum number of characters you want the edit
      box to be able to hold. If you pass -1 (or don't pass anything) the maximum length stays the same as it
      was before calling this function. Initially the maximum length of the text is set to 256. If you pass 0
      the maximum length also stays unchanged but if it hasn't been setup yet it is set to the length of the
      passed string (or 256 if the string is empty or NULL).
      */
      virtual void SetText(const char *, int = -1);
      /**
      These two select and deselect the entire text as if it was double clicked.
      */
      virtual void Select();
      /**
      See Select()
      */
      virtual void Deselect();
      /**
      Sets the cursor at the given offset and scrolls the text if necessary.
      */
      virtual void ScrollTo(int offset);
      /**
      Returns the maximum number of characters the editbox can hold.
      */
      int GetMaxLength();
};
}

#endif      //MASKING_EDITBOX_H
