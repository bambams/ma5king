// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef   MASKING_DEFINES_H
#define   MASKING_DEFINES_H


/* bits for the flags field */
#define D_EXIT          1        /* object makes the dialog exit */
#define D_SELECTED      2        /* object is selected */
#define D_GOTFOCUS      4        /* object has the input focus */
#define D_GOTMOUSE      8        /* mouse is on top of object */
#define D_HIDDEN        16       /* object is not visible */
#define D_DISABLED      32       /* object is visible but inactive */
#if 0 /* D_DIRTY must not be used */
#define D_DIRTY         64       /* object needs to be redrawn */
#endif
#define D_INTERNAL      128      /* reserved for internal use */
#define D_USER          256      /* from here on is free for your own use */


/* return values for the dialog procedures */
#define D_O_K           0        /* normal exit status */
#define D_CLOSE         1        /* request to close the dialog */
#define D_REDRAW        2        /* request to redraw the dialog */
#define D_REDRAWME      4        /* request to redraw this object */
#define D_WANTFOCUS     8        /* this object wants the input focus */
#define D_USED_CHAR     16       /* object has used the keypress */
#define D_REDRAW_ALL    32       /* request to redraw all active dialogs */
#define D_DONTWANTMOUSE 64       /* this object does not want mouse focus */


/* messages for the dialog procedures */
#define MSG_START       1        /* start the dialog, initialise */
#define MSG_END         2        /* dialog is finished - cleanup */
#define MSG_DRAW        3        /* draw the object */
#define MSG_CLICK       4        /* mouse click on the object */
#define MSG_DCLICK      5        /* double click on the object */
#define MSG_KEY         6        /* keyboard shortcut */
#define MSG_CHAR        7        /* other keyboard input */
#define MSG_UCHAR       8        /* unicode keyboard input */
#define MSG_XCHAR       9        /* broadcast character to all objects */
#define MSG_WANTFOCUS   10       /* does object want the input focus? */
#define MSG_GOTFOCUS    11       /* got the input focus */
#define MSG_LOSTFOCUS   12       /* lost the input focus */
#define MSG_GOTMOUSE    13       /* mouse on top of object */
#define MSG_LOSTMOUSE   14       /* mouse moved away from object */
#if 0 /* MSG_IDLE must not be used */
#define MSG_IDLE        15       /* update any background stuff */
#endif
#define MSG_RADIO       16       /* clear radio buttons */
#define MSG_WHEEL       17       /* mouse wheel moved */
#define MSG_LPRESS      18       /* mouse left button pressed */
#define MSG_LRELEASE    19       /* mouse left button released */
#define MSG_MPRESS      20       /* mouse middle button pressed */
#define MSG_MRELEASE    21       /* mouse middle button released */
#define MSG_RPRESS      22       /* mouse right button pressed */
#define MSG_RRELEASE    23       /* mouse right button released */
#define MSG_WANTMOUSE   24       /* does object want the mouse? */
#define MSG_USER        25       /* from here on are free... */


/**
   sent whenever a timer has ticked
*/
#define      MSG_TIMER         MSG_USER
/**
   sent once per logic frame
*/
#define      MSG_TICK         MSG_USER+1
/**
   sent whenever a new skin is loaded (and whe the dialog starts)
*/
#define      MSG_INITSKIN      MSG_USER+2
/**
   sent when a widget has been activated (button pressed, checkbox toggled, a list entry double clicked, etc)
*/
#define      MSG_ACTIVATE      MSG_USER+3
/**
   sent by widgets to ask to be removed from the dialog
*/
#define      MSG_REMOVEME      MSG_USER+4
/**
   sent when the position of a scroller, slider, etc has changed
*/
#define      MSG_SCROLL         MSG_USER+5
/**
   sent when a widget or dialog was moved
*/
#define      MSG_MOVE         MSG_USER+6
/**
   sent when a widget or dialog was resized
*/
#define      MSG_RESIZE         MSG_USER+7
/**
   sent when a widget or dialog was moved and resized
*/
#define      MSG_SHAPE         MSG_USER+8
/**
   sent when the mouse cursor moves
*/
#define      MSG_MOUSEMOVE      MSG_USER+9
/**
   sent when a widget that has the D_EXIT flag set was selected
*/
#define      MSG_CLOSE         MSG_USER+10
#ifndef      MSG_WANTMOUSE
/**
   sent to query whether a widget wants the mouse or not
*/
#define      MSG_WANTMOUSE      MSG_USER+11
#endif
/**
   sent to query whether a widget can be tabbed to
*/
#define      MSG_WANTTAB         MSG_USER+12
/**
   this is the first user available message.
   
   MASkinG uses messages for communication between widgets and dialogs.
   For this it uses both the message constants it defines and those defined
   by Allegro itself. All messages above MSG_SUSER may be used by the user.
*/
#define      MSG_SUSER         MSG_USER+13


/**
   set when the widget or dialog was moved
*/
#define      D_MOVED            D_USER
/**
   set when the widget or dialog was resized
*/
#define      D_RESIZED         D_USER<<1
/**
   set when the widget's or dialog's skin was changed
*/
#define      D_CHANGEDSKIN      D_USER<<2
/**
   set when the widget starts animating
*/
#define      D_ANIMATING         D_USER<<3
/**
   set if the widget has child widgets (ie it is some type of a dialog)
*/
#define      D_HASCHILDREN      D_USER<<4
/**
   set if the widget is a window (ie it can be moved and resized)
*/
#define      D_WINDOW         D_USER<<5
/**
   set when the widget is being pressed down (similar to D_SELECTED)
*/
#define      D_PRESSED         D_USER<<6
/**
   set if the widget is resized automatically. For example a label might resize itself
   automatically according to the length of the text or a button might
   automatically resize itself according to the size of its bitmap.
*/
#define      D_AUTOSIZE         D_USER<<7
/**
   set if the widget can be toggled (a toggle button for example)
*/
#define      D_TOGGLE         D_USER<<8
/**
   set if the widget should generate MSG_ACTIVATE messages while being pressed
   down (for example a spinner button)
*/
#define      D_SPINNER         D_USER<<9
/**
   set if the widget is movable
*/
#define      D_MOVABLE         D_USER<<10
/**
   set if the widget is resizable
*/
#define      D_RESIZABLE         D_USER<<11
/**
   set if the widget (or window) is modal, that is if it captures all input
   until it's closed
*/
#define      D_MODAL            D_USER<<12
/**
   set this flag to make the widget read-only; currently this is only used
   by the EditBox widget
*/
#define      D_READONLY         D_USER<<13
/**
   Every top level dialog should have this flag set. Top level dialogs are all
   dialogs derived from Dialog, except for compound widgets.
*/
#define      D_TOPLEVEL         D_USER<<14
/**
   you can use any powers of 2 above this.
   
   MASkinG uses flags to define the state of each widget and dialog. For this
   it uses both the flags it defines and those defined by Allegro. All values
   that are powers of two and above D_SUSER can be used by the user.
*/
#define      D_SUSER            D_USER<<15

#endif      //MASKING_DEFINES_H
