// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_WIDGET_H
#define      MASKING_WIDGET_H

#include "maskingdll.h"
#include "global.h"
#include "defines.h"
#include "rect.h"
#include "skin.h"
#include "animator.h"
#include <list>

#ifdef SendMessage
   #undef SendMessage
#endif


namespace MAS {
/**
   A simple structure which combines widget's font and font color settings.
*/
struct MASKING_DLL_DECLSPEC FontInfo {
   /**
      background of the text
   */
   Color textMode;
   /**
      4 primary font colors for 4 states (normal, selected, disabled, focused)
   */
   Color fntCol[4];
   /**
      4 secondary (shadow) font colors
   */
   Color shdCol[4];
   /**
      4 fonts (indices into the skin font array)
   */
   int fnt[4];
};

class MASKING_DLL_DECLSPEC Dialog;

/**
   The widget baseclass.
   This class is quite useless as it is, but all MASkinG widgets need to inherit
   from it. It contains many variables and methods that control the basic
   apprearance and functionality of the widgets such as position, size, flags, etc.
*/
class MASKING_DLL_DECLSPEC Widget : public Rect {
   protected:
      /**
         Flags that define the objects state. Available flags are all flags defined
         by Allegro and the D_SOMETHING flags. Click \URL[here]{"index.html"} and
         scroll down to "Macros" to see the full list.
      */
      int flags;

      /**
         The keyboard shortcut.
         This shortcut can be used to activate the widget. This may be a character such
         as 'a' to respond to a simple keypress, or a number 1-26 to respond to a
         control key a-z.
      */
      int key;

      /**
         The ID of the main callback message for this widget.
      */
      int callbackID;

      /**
         Information about fonts and font colours the widget should use. This is obviously
         relevant only for widgets that contain text.
      */
      FontInfo fontInfo;

      /**
         Index of the bitmap in the skin's bitmap array the widget will use. If a widget
         uses more than one bitmap, it obviously needs to keep track of all the indices
         by itself.
      */
      int bitmap;

      /**
         Sample indices in the skin's sample array for samples that are to be played by
         the widget at different events.
      */
      int sample[7];

      /**
         A pointer to the widget's parent Dialog. The parent of the top level dialog is
         the dialog itself (this pointer).
      */
      Dialog *parent;

      /**
         A pointer to the skin the widget is to use to skin itself. This usually
         points to the global skin object (\URL[theSkin]{"theSkin.html"}), but in
         theory every widget can have its own skin.
      */
      Skin *skin;

      /**
         The widget's relative position. This is relative to the parent (as opposed
         to being the absolute position on the screen).
      */
      Point relativePos;

      Rect normalizedRect;

      /**
         The animator object that is used to animate the widget.
      */
      Animator *animator;

      /**
         Animation length of the default animation in miliseconds.
      */
      int animLength;

      /**
         Animation type of the default animation. The type can be one of
         \URL[these values]{"animator.html#AnimationType"}.
      */
      int animType;

      /**
         The text displayed in tooltip help bubbles when the mouse hovers over the
         widget. If NULL, no help bubble is displayed.
      */
      char *tooltipText;

   protected:
      /**
         Returns the widget's own canvas onto which the widget may draw itself.
         The canvas is most often the same size as the widget but you shouldn't
         rely on that (in some cases the widget's canvas may be clipped). The
         canvas is actually a subbitmap of the dialog's canvas so the widget
         should draw itself at position (0,0) which is different than in previous
         versions of MASkinG where every widget had access to its dialog's canvas
         and had to draw itself at position (x,y). Normally you shouldn't need to
         call this function because the dialog manager does it for you in the
         MsgDraw() message handler.
      */
      ALLEGRO_BITMAP *GetCanvas();

      /**
         Releases the widget's canvas that was obtained by the GetCanvas() function.
         It is necessary for every call to GetCanvas() to have a matching call to
         ReleaseCanvas() otherwise you get a memory leak.
      */
      void ReleaseCanvas(ALLEGRO_BITMAP *canvas);

      /**
         Changes the global mouse cursor. Actually it calls the SetCursor() function
         in the \URL[Mouse]{"mouse.html"} class. The parameter i is the index of the
         cursors in the skin's cursor array as defined \URL[here]{"skin.html#SkinCursorIndex"}.
      */
      virtual void SetCursor(int i);

      /**
         Updates the widget's absolute position.   This means tha widget is moved to
         it's absolute position on the screen if it's a part of a subdialog and not
         of the main dialog. You shouldn't have to call this function yourself, the
         dialog manager does it when the widget is added to the dialog.
      */
      void UpdatePosition();

      Rect *CanAdd(const Rect& r);

   protected:
      /**
         Called when the dialog is started. This allows the widget to initialize itself
         if necessary before being used and drawn. MsgStart() is also called when the
         widget is dynamically added to a dialog while the dialog is already running.
      */
      virtual void MsgStart();
      /**
         Called when the dialog is closed. Allows the widget to clean up after itself
         before being destroyed. This function is also called when the widget is
         removed from a dialog while the dialog is still running.
      */
      virtual void MsgEnd();
      /**
         Called to tell the widget to draw itself. Normally you shouldn't overload this
         because the default implementation takes care of thing like getting and
         releasing the canvas, updating the screen and doing animation. In most cases
         you should implement the drawing code in the Draw() function.
      */
      virtual void MsgDraw();
      /**
         Called whenever the widget needs to be redrawn. The canvas parameter is the
         canvas bitmap on which the widget should draw itself. You should normally
         implement this function instead of MsgDraw() unless you're doing something
         really clever. The default implementation of MsgDraw() takes care of acquiring
         and releasing the canvas and doing the animation. You should implement MsgDraw()
         only if you don't need this for some reason or if you want to do it differently
         otherwise you should just implement Draw().
      */
      virtual void Draw(Bitmap &canvas);
      /**
         Called whenever a mouse button was clicked while the mouse cursor was over
         the widget. This doesn't differentiate between mouse buttons so if you need
         to handle clicks with different buttons and if you want better control over
         mouse presses and releases, implement the appropriate lower level message
         handlers instead.
      */
      virtual void MsgClick();
      /**
         Called whenever the widget was doubleclicked.
      */
      virtual void MsgDClick();
      /**
         Called if the widget's shortcut key was pressed.
      */
      virtual void MsgKey();
      /**
         Called whenever a keyboard button was pressed while the widget had input focus.
         The character passed into this function is in the "extended" format with the
         ASCII code in the low byte and the scancode in the high byte. This function
         should return true if the widget handled the key, otherwise it should return
         false to indicate that it isn't interested.
      */
      virtual bool MsgChar(int c);
      /**
         The Unicode version of MsgChar(). This is called if MsgChar() returned false,
         passing the Unicode value of the pressed key as the parameter. Should return
         true if the widget handled the key, false otherwise.
      */
      virtual bool MsgUChar(int c);
      /**
         Sent to all widgets in a dialog when a keypress occurs but the widget with
         input focus   doesn't handle it and the key doesn't match any of the dialog's widgets
         keyboard shortcut. This allows you to handle special keypresses even when
         you don't have input focus.
      */
      virtual bool MsgXChar(int c);
      /**
         Called to ask the widget if it accepts input focus.
      */
      virtual bool MsgWantfocus();
      /**
         Called to inform the widget that it received input focus.
      */
      virtual void MsgGotfocus();
      /**
         Called to inform the widget that it lost input focus.
      */
      virtual void MsgLostfocus();
      /**
         Called to ask the widget if cares about the mouse being over it and if
         the mouse is actually over it. This allows you to implement non-rectangular
         widgets. This function is called whenever the mouse cursor enters the
         widget's bounding rectangle but the widget itself should take care of
         figuring out whether the mouse is actually on top of the widget or not.
      */
      virtual bool MsgWantmouse();
      /**
         Called to inform the widget that the mouse just came on top of it.
      */
      virtual void MsgGotmouse();
      /**
         Called to inform the widget that the mouse just left it.
      */
      virtual void MsgLostmouse();
      /**
         Called when a radio button object is activate so that other radio
         buttons in the same group can deselect themselves.
      */
      virtual void MsgRadio(int g);
      /**
         Called when the mouse wheel moves. Parameter z is the number of "clicks".
      */
      virtual void MsgWheel(int z);
      /**
         Called when the left mouse button was pressed.
      */
      virtual void MsgLPress();
      /**
         Called when the middle mouse button was pressed.
      */
      virtual void MsgMPress();
      /**
         Called when the right mouse button was pressed.
      */
      virtual void MsgRPress();
      /**
         Called when the left mouse button was released.
      */
      virtual void MsgLRelease();
      /**
         Called when the middle mouse button was released.
      */
      virtual void MsgMRelease();
      /**
         Called when the right mouse button was released.
      */
      virtual void MsgRRelease();
      /**
         Called whenever a timer has ticked. The passed parameter is
         the the timer event that caused the message to be sent.
      */
      virtual void MsgTimer(ALLEGRO_TIMER_EVENT *timer);
      /**
         Called exactly once per frame.
      */
      virtual void MsgTick();
      /**
         Called to inform the widget that it was moved.
      */
      virtual void MsgMove();
      /**
         Called to inform the widget that it was resized.
      */
      virtual void MsgResize();
      /**
         Called to inform the widget that it was both moved and resized.
      */
      virtual void MsgShape();
      /**
         Called when the mouse has moved. When the mouse is on top of a
         widget or the widget has the D_PRESSED flag set this function is
         called whenever the mouse has moved passing the difference between
         the current and the previous mouse positions (delta).
      */
      virtual void MsgMousemove(const Point& d);
      /**
         Called right after the widget's skin has changed. Most widgets
         read their skin information (like fonts, font colours, bitmaps,
         samples, etc.) from the skin in this function.
      */
      virtual void MsgInitSkin();
      /**
         Called to ask if the dialog can really be closed.
         This is obviously relevant only for dialogs and windows. It is
         passed whenever a request to close the dialog was made (either
         by pressing escape, selecting a widget that has the D_EXIT
         flag set or by explicitly calling the Close() function). This
         functions should return true if the dialog may be closed
         otherwise it should return false. Most often you should popup
         a dialog asking the user if they really want to close the
         window or something similar in this function.
      */
      virtual bool MsgClose();
      /**
         Called to ask if the widget can be tabbed to. This function should
         return true if the widget can be tabbed to with the TAB key or with
         the arrow keys, otherwise it should return false. By default the
         value MsgWantfocus() returns is returned.

         The above messages are called every time a message needs to be passed to
         the widget. Every derived widget class may implement any or all of these
         functions to define it's appearance and behaviour. Most of the messages
         are directly related to similar messages in the default Allegro GUI but
         some are completely new and unique to MASkinG.
      */
      virtual bool MsgWantTab();

   public:
      Widget();
      virtual ~Widget();

      // get object state
      virtual int x()   const;
      virtual int y()   const;
      virtual int x2()const;
      virtual int y2()const;
      virtual int w()   const;
      virtual int h()   const;
      /**
         Returns the position of the widget relative to it's parent dialog.
      */
      virtual Point RelativePos() const;
      /**
         Returns the widget's keyboard shortcut.
      */
      virtual int Key()      const;
      /**
         Returns the flags defining the widget's state.
      */
      virtual int Flags()      const;
      /**
         Tests a flag or a group of them.
      */
      virtual bool TestFlag(int i)   const;
      /**
         Tests if the widget has input focus.
      */
      virtual bool HasFocus();
      /**
         Tests if the widget currently has the mouse over it. Note that only one
         widget can have a mouse at any time so when widgets are stacked on top
         of each other, only the one on top can have the mouse.
      */
      virtual bool HasMouse();
      /**
         Tests if the widget is hidden or not.
      */
      virtual bool Hidden()   const;
      /**
         Checks if activating the widget will close the parent dialog.
      */
      virtual bool WillExit() const;
      /**
         Checks if the widget is selected. Note that being selected may have
         different meaning for different widgets.
      */
      virtual bool Selected() const;
      /**
         Checks if the widget is disabled and grayed out.
      */
      virtual bool Disabled() const;
      /**
         Checks if the widget contains child widgets.
      */
      virtual bool HasChildren() const;
      /**
         Checks if the widget is a window that can potentially be moved and resized.
      */
      virtual bool IsWindow() const;
      /**
         Returns a pointer to the widget's parent dialog.
      */
      virtual Dialog *GetParent() const;
      /**
         Returns true is this widget is a child of the passed widget.
      */
      virtual bool IsChildOf(Widget *w) const;

      /** */virtual void x(int i, bool normalized=false);
      /** */virtual void y(int i, bool normalized=false);
      /** */virtual void w(int i, bool normalized=false);
      /**
         These set the widget's x and y coordinates within the dialog and
         the widgets width and height.
         If <i>normalized</i> is true, then the value should be a normalized
         number in the range between 0 and 100 (i.e. percentage of the parent
         dialog size), otherwise it should be the actual number of pixels.
      */
      virtual void h(int i, bool normalized=false);

      /**
         Sets the keyboard shortcut.
      */
      virtual void Key(int i);
      /**
         Sets a flag or a group of them.
      */
      virtual void SetFlag(int i);
      /**
         Sets the flags to the given value.
      */
      virtual void SetFlags(int flags);
      /**
         Clears a flag.
      */
      virtual void ClearFlag(int i);
      /**
         Enables the widget.
      */
      virtual void Enable();
      /**
         Disables the widget and grays it out.
      */
      virtual void Disable();
      /**
         Hides the widget.
      */
      virtual void Hide();
      /**
         Unhides the widget.
      */
      virtual void Unhide();
      /**
         Flags the widget with the D_EXIT flag to indicate that activating it
         will close the parent dialog.
      */
      virtual void MakeExit();
      /**
         Clears the D_EXIT flag.
      */
      virtual void DontExit();
      /**
      */
      virtual void Place(int nx, int ny, bool normalized=false);
      /**
         Places the widget at the desired position.
      */
      virtual void Place(const Point& p, bool normalized=false);
      /**
      */
      virtual void Resize(int nw, int nh, bool normalized=false);
      /**
         Resize the widget to the desired size.
      */
      virtual void Resize(const Size& s, bool normalized=false);
      /**
      */
      virtual void Shape(int nx, int ny, int nw, int nh, bool normalized=false);
      /**
      */
      virtual void Shape(const Point& p, const Size& s, bool normalized=false);
      /**
         Sets the widgets size and position to the desired values.
      */
      virtual void Shape(const Rect &r, bool normalized=false);
      /**
         Selects the widget.
      */
      virtual void Select();
      /**
         Deselects the widget.
      */
      virtual void Deselect();
      /**
         Sets the widget's parent dialog. Probably shouldn't be used!
      */
      virtual void SetParent(Dialog *f);

      /**
         Changes the animator the widget is to use for animating. By default
         the widgets make an instance of the base Animator class but you can
         easily add your own animations by deriving an animator class from it.
         This function deletes the previous animator if any. Also the animator
         is deleted when the widget expires so you don't have to worry about
         destroying it yourself.
      */
      virtual void SetAnimator(Animator *a);
      /**
         Sets the default animation length and type for the widget. Most often
         this is read from the skin description file and set in the
         MsgInitSkin() function.
      */
      virtual void SetAnimationProperties(int length, int type);
      /**
      */
      virtual void Animate();
      /**
         Starts animating the widget. Basically these functions just set a flag
         which means the widget is being animated and then the dialog manager
         makes sure the animation is actually performed.
      */
      virtual void Animate(int type);
      /**
         Stops the animation. You shouldn't normally have to call this function
         as the animation will acutomatically stop when it reaches the end.
      */
      virtual void StopAnimating();
      /**
         Stops animating and resets the animator so it can start a new animation from scratch.
      */
      virtual void ResetAnimation();

      /**
         Sets the text tooltip help bubbles display when the mouse hovers over the widget.
         Set text to NULL to disable tooltips.
      */
      virtual void SetTooltipText(const char *text);
      /**
         Returns the widget's tooltip text.
         If the text is NULL, the parent dialog's tooltip text is returned instead!
      */
      virtual const char *GetTooltipText();
      /**
         Sets The ID of the main callback message for this widget.
         If this value is set to a positive number or 0 (default is -1), an event with this
         ID will be created and sent to the parent dialog via HandleEvent() instead of the
         primary message of the widget.
         For example when a button is clicked, it sends MSG_ACTIVATE to the parent dialog,
         but if this ID is set, it will be sent instead. This makes it easy to map several
         widgets to the same message (so they will perform the same action). For example
         you might have a File/Open menu item, a Ctrl+O keyboard shortcut and an icon all
         do the same thing (popup a file open dialog).
         So to sum it up, this ID overrides MSG_ACTIVATE in widgets that send it when
         they are activated.
      */
      void SetCallbackID(int ID);
      /**
         Returns the widget's callback ID. Returns -1 if no ID is set.
      */
      int GetCallbackID();

   public:
      /**
         Sets up the most important widget properties
         (position, size, keyboard shortcut and flags) in one function call.
      */
      virtual void Setup(int x, int y, int w, int h, int key=0, int flags=0);
      /**
         Sets the skin the widget is supposed to use for drawing itself.
         Usually all widgets use the global skin \URL[theSkin]{"theSkin.html"}
         but every widget may have it's own skin.
      */
      virtual void SetSkin(Skin *skin);
      /**
         Returns a pointer to the widget's skin.
      */
      virtual Skin *GetSkin();
      /**
         Tests if two references to widgets are one and the same.
      */
      bool operator==(const Widget& obj) const;
      /**
         Tests if two references to widgets are not the same.
      */
      bool operator!=(const Widget& obj) const;
      /**
         Sends the widget a message with one optional parameter. Becuse all
         the message functions are protected, widgets can only communicate
         with each other by sending messages and events. Available messages
         are all the messages defined by Allegro and the messages defined
         \URL[here]{"index.html"} (scroll down to "Macros" to see a full list).
      */
      int SendMessage(int msg, intptr_t c=0);
      /**
         Returns the current mouse position relative to the widget's top left corner.
      */
      virtual Point GetMousePos() const;

      // font specific operations
      /** */void SetTextMode(Color m);
      /** */void SetFontColor(Color col, Color shd, int type);
      /** */void SetFont(int f, int type);
      /** */Font& GetFont(int type);
      /** */Color& GetTextMode();
      /** */Color& GetFontColor(int type);
      /**
         Functions for setting and getting the colours of the text background, the
         text and the text shadow and the font indices in the skins font array.
         These properties can be inidvidually set for all four possible
         \URL[states]{skin.html#SkinWidgetState}. Obviously this is relevant only
         for wigets that actually contain some sort of text.
      */
      Color& GetShadowColor(int type);
      int GetFontIndex(int type);

      // bitmap specific operations
      /**
         Selects the bitmap the widget is supposed to use to skin itself.
         The parameter i is the index of the bitmap in the skin's bitmap array.
      */
      void SetBitmap(int i);
      /**
         Returnes the widget's bitmap index in the skin bitmap array.
      */
      int GetBitmapIndex() const;
      /**
         Retreives the selected bitmap from the widget's skin.
      */
      Bitmap& GetBitmap() const;

      // sample specific operations
      /**
         Selects the sample the widget is supposed to play for each event.
         The parameter i is the index of the sample in the skin's sample array.
         Event is one of \URL[these events]{skin.html#SkinSampleIndex}.
      */
      void SetSample(int i, int event);
      /**
         Returnes the widget's sample index in the skin sample array for the given event.
      */
      int GetSampleIndex(int event) const;
      /**
         Retreives the selected sample for the given event from the widget's skin.
      */
      Sample& GetSample(int event) const;
      /**
         Plays the sample for the given event.
      */
      void PlaySample(int event) const;

      friend class MASKING_DLL_DECLSPEC Dialog;
};
}

#endif      //MASKING_WIDGET_H
