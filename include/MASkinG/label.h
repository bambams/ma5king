// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_LABEL_H
#define      MASKING_LABEL_H

#include "dialog.h"
#include "maskingdll.h"

namespace MAS {
/**
Label is a very simple widget for creating both static and dynamic textual labels.
It can display multi-line text with automatic wordwrapping. It treats newline
characters as hard line breaks.

The label can automatically resize itself to the length and height of the text and
can render the text with transparent background which is perfect for static text
because it minimizes the chances of redraw but if you want a label that changes
dynamically (for example to display the value of a slider or the current time) this
doesn't work to well because when the text changes and the label is redrawn
the old text can show through. In that case you should clear the D_AUTOSIZE flag,
set the text's size to the maximum length of the text you expect to display and set
the text background with the SetTextMode() function.

note: the autosize option doesn't work well with non left-aligned labels.
*/
class MASKING_DLL_DECLSPEC Label : public Widget {
   protected:
      char *text;
      int alignment;
        int valignment;
        bool wordwrap;

   protected:
      // Message functions
      /**
      Sets the text background and font colours to the default skin values.
      */
      void MsgInitSkin();
      /**
       If the D_AUTOSIZE flag is set this function resizes the label
      to the length and height of the text.
      */
      virtual void UpdateSize();
      /**
      Renders the text to the canvas. If the text mode colour is different than -1
      it first fills the canvas with this colour then it renders the text.
      If the size of the widget is different from the size of the actual text,
      the text is horizontally aligned according to the alignment
      settings and is centered vertically.
      */
      void Draw(Bitmap &canvas);

   public:
      /**
      default constructor
      */
      Label();
      /**
      destructor
      */
      ~Label();

      /**
      Shortcut for Widget::Setup, SetText and SetAlignment
      */
       void Setup(int x, int y, int w, int h, int key, int flags, const char *text, int a=0);

      // Modify the object text on the fly
      /**
       Sets the text string the label is supposed to print out.
      */
      virtual void SetText(const char *t);

      /**
       The label can render numbers as well as normal text. Pass an integer or a double and the
       label will render them as text. It will even make sure that just enough decimal
       places are rendered for doubles.
      */
      virtual void SetNumber(int value);

      /**
        See Setnumber (int)
      */
      virtual void SetNumber(double value);

      /**
      Returns the label's text.
      */
      virtual char *GetText();

      /**
       This function attempts to convert as much of the text as possible to an integer.
       If the text doesn't represent a number, 0 is returned.
      */
      virtual int GetInt();

      /**
       This function attempts to convert as much of the text as possible to a double.
       If the text doesn't represent a number, 0 is returned.
      */
      virtual double GetDouble();

      // Set alignment method
      /**
       Sets the alignment method the label should use to render text. Valid values
       for a are 0 for left alignment, 1 for right alignment and 2 for centered text.
       Note that this has meaning only if the D_AUTOSIZE flag is turned off.
       By default it isn't which means the label will automatically resize itself to
       the length and height of the text.
      */
      virtual void SetAlignment(int a);
      /**
      returns the current alignment
      */
      virtual int GetAlignment();
      /**
      Shortcut for SetAlignment (0);
      */
      virtual void AlignLeft();
      /**
      Shortcut for SetAlignment (1);
      */
      virtual void AlignRight();
      /**
      Shortcut for SetAlignment (2);
      */
      virtual void AlignCentre();

      /**
      Sets the vertical alignment. Possible values are 0 for top alignment, 1 for bottom and
      2 for vertical centre.
      */
      virtual void SetVAlignment (int value);
      /**
      returns the current vertical alignment setting.
      */
        virtual int GetVAlignment ();

      /**
      returns the current word-wrapping setting
      */
      virtual bool GetWordWrap ();
      /**
      turns word-wrapping on or off.
      */
        virtual void SetWordWrap (bool value);
      /**
      Calculates and returns the height required to print the entire text with the current
      font and at the current width.
      */
        virtual int GetPreferredHeight();

};
}

#endif      //MASKING_LABEL_H
