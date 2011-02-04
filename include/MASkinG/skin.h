// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_SKIN_H
#define      MASKING_SKIN_H

#include <allegro5/allegro.h>
#include "bitmap.h"
#include "font.h"
#include "cursor.h"
#include "sample.h"
#include "error.h"
#include "color.h"
#include <vector>
#include "maskingdll.h"

#ifdef LoadCursor
#undef LoadCursor
#endif

#ifdef LoadBitmap
#undef LoadBitmap
#endif


namespace MAS {
/*
   The size of the array holding extra info for some widgets.
*/
#define nInfoItems 13

/**
   The skin class.
   Used to store all the data that descriebes a skin: bitmaps, fonts, cursors, colors, etc.
*/
class MASKING_DLL_DECLSPEC Skin {
   public:
      /** */static const int nBitmaps;
      /** */static const int nCursors;
      /** */static const int nFonts;
      /**
         Number of skin data elements.
         These define the number of bitmaps, cursors, fonts and samples in a
         skin datafile:
<pre>
      nBitmaps = 46;
      nCursors = 12;
      nFonts = 8;
      nSamples = 7;
      nInfoItems = 13;
</pre>
      */
      static const int nSamples;

      /** */static const int nBitmapsEx;
      /** */static const int nCursorsEx;
      /** */static const int nFontsEx;
      /**
         Sizes of data arrays.
         These are the sizes of the bitmap, cursor, font and sample vectors.
         The skin itself doesn't use the entire vectors, so all the space
         above nBitmaps, nCursors, nFonts and nSamples in their respective
         arrays can be used by the user. The array sizes are:
<pre>
      nBitmapsEx = 1024;
      nCursorsEx = 64;
      nFontsEx = 64;
      nSamplesEx = 1024;
</pre>
      */
      static const int nSamplesEx;

      Error lastError;

      /**
         Bitmap indices.
         These are the indices of the skin bitmaps in the datafile and the
         skins bitmap array. The possible values are as follows:
<pre>
      BOX
      BOX_SHADOW
      BUTTON
      CHECKBOX
      CLEAR_BACK
      COMBO_BUTTON
      HSLIDER_BACK
      HSLIDER_GRIP
      ICONEXIT
      ICONMAX
      ICONMIN
      ICONRESTORE
      LIST
      MENU_BACK
      MENU_BUTTON
      MENU_ITEM
      PANEL_GROOVE
      PANEL_RAISED
      PANEL_RIDGE
      PANEL_SUNKEN
      PROGRESSH
      PROGRESSV
      RADIO
      SCROLL_DOWN
      SCROLL_HBACK
      SCROLL_HGRIP
      SCROLL_HGRIPOVERLAY
      SCROLL_LEFT
      SCROLL_RIGHT
      SCROLL_UP
      SCROLL_VBACK
      SCROLL_VGRIP
      SCROLL_VGRIPOVERLAY
      SEPARATORH
      SEPARATORV
      TAB_BUTTON
      TAB_WINDOW
      VSLIDER_BACK
      VSLIDER_GRIP
      WINBOTTOM
      WINDOW
      WINGRIP
      WINLEFT
      WINRIGHT
      WINTEXT
      WINTOP
      XTRA_PALETTE
</pre>
         The last entry is the palette that is to be used for 8 bit mode.
      */
      enum SkinBitmapIndex {
         BOX,
         BOX_SHADOW,
         BUTTON,
         CHECKBOX,
         CLEAR_BACK,
         COMBO_BUTTON,
         HSLIDER_BACK,
         HSLIDER_GRIP,
         ICONEXIT,
         ICONMAX,
         ICONMIN,
         ICONRESTORE,
         LIST,
         MENU_BACK,
         MENU_BUTTON,
         MENU_ITEM,
         PANEL_GROOVE,
         PANEL_RAISED,
         PANEL_RIDGE,
         PANEL_SUNKEN,
         PROGRESSH,
         PROGRESSV,
         RADIO,
         SCROLL_DOWN,
         SCROLL_HBACK,
         SCROLL_HGRIP,
         SCROLL_HGRIPOVERLAY,
         SCROLL_LEFT,
         SCROLL_RIGHT,
         SCROLL_UP,
         SCROLL_VBACK,
         SCROLL_VGRIP,
         SCROLL_VGRIPOVERLAY,
         SEPARATORH,
         SEPARATORV,
         TAB_BUTTON,
         TAB_WINDOW,
         VSLIDER_BACK,
         VSLIDER_GRIP,
         WINBOTTOM,
         WINDOW,
         WINGRIP,
         WINLEFT,
         WINRIGHT,
         WINTEXT,
         WINTOP,
         XTRA_PALETTE
      };

      /**
         Cursor indices.
         These are the indices of the cursor bitmaps in the cursor datafile
         and the cursor array. Possible values are as follows:
<pre>
      MOUSE_ALTERNATIVE
      MOUSE_CROSSHAIR
      MOUSE_HOURGLASS
      MOUSE_MOVE
      MOUSE_NORMAL
      MOUSE_SELECT
      MOUSE_SIZE_DIAGONAL1
      MOUSE_SIZE_DIAGONAL2
      MOUSE_SIZE_HORIZONTAL
      MOUSE_SIZE_VERTICAL
      MOUSE_TEXTSELECT
      MOUSE_UNAVAILABLE
</pre>
      */
      enum SkinCursorIndex {
         MOUSE_ALTERNATIVE,
         MOUSE_CROSSHAIR,
         MOUSE_HOURGLASS,
         MOUSE_MOVE,
         MOUSE_NORMAL,
         MOUSE_SELECT,
         MOUSE_SIZE_DIAGONAL1,
         MOUSE_SIZE_DIAGONAL2,
         MOUSE_SIZE_HORIZONTAL,
         MOUSE_SIZE_VERTICAL,
         MOUSE_TEXTSELECT,
         MOUSE_UNAVAILABLE,
         MOUSE_XY
      };

      /**
         Sample indices.
         These are the indices of the sound samples in the sample datafile
         and the sample array. Possible values are as follows:
<pre>
      SAMPLE_ACTIVATE
      SAMPLE_CLOSE
      SAMPLE_GOTFOCUS
      SAMPLE_KEY
      SAMPLE_LOSTFOCUS
      SAMPLE_OPEN
      SAMPLE_SCROLL
</pre>
      */
      enum SkinSampleIndex {
         SAMPLE_ACTIVATE,
         SAMPLE_CLOSE,
         SAMPLE_GOTFOCUS,
         SAMPLE_KEY,
         SAMPLE_LOSTFOCUS,
         SAMPLE_OPEN,
         SAMPLE_SCROLL
      };

      /**
         Font and color array indices.
         These are the indices into the font and font color arrays within the
         skin for each widget. Possible values are as follows:
<pre>
      INFO_BOX
      INFO_BUTTON
      INFO_CHECKBOX
      INFO_RADIO
      INFO_LIST
      INFO_WINDOW
      INFO_MENU
      INFO_TEXT
      INFO_EDITBOX
      INFO_BAR
      INFO_HYPER
      INFO_TAB
      INFO_TOOLTIP
</pre>
      */
      enum SkinFontInfo {
         INFO_BOX,
         INFO_BUTTON,
         INFO_CHECKBOX,
         INFO_RADIO,
         INFO_LIST,
         INFO_WINDOW,
         INFO_MENU,
         INFO_TEXT,
         INFO_EDITBOX,
         INFO_BAR,
         INFO_HYPER,
         INFO_TAB,
         INFO_TOOLTIP
      };

      /**
         Object states.
         These are the possible states each dialog object can be in. Every
         object can have a different font and font color for each of these
         states. Most of the skin bitmaps are divided into four subbitmaps,
         one for each state. The possible states are as follows:
<pre>
      NORMAL              - the normal idle state
      SELECT              - the widget is selected
      DISABLE             - the widget is disabled
      FOCUS               - the widget has input focus
</pre>
      */
      enum SkinWidgetState {
         NORMAL,
         SELECT,
         DISABLE,
         FOCUS
      };

   protected:
      /**
         The array of bitmaps that are used to skin the dialog objects.
      */
      std::vector<Bitmap *> bmpList;

      /**
         The array of cursors used by the skin.
      */
      std::vector<Cursor *> curList;

      /**
         The array of fonts that can be used in the skin.
      */
      std::vector<Font *> fntList;

      /**
         The array of samples that are used by the skin.
      */
      std::vector<Sample *> smpList;

      /**
         Path to the file the skin was loaded from.
         The skin saves the path to the skin file that was loaded in this
         string. This allows the skin to be reloaded later if necessary.
      */
      char *skinFilePath;


      /**
         Attempts to load all the skin bitmaps from a regular Allegro datafile.
         If some bitmaps are missing old ones stay untouched.
      */
      virtual Error LoadData(ALLEGRO_PATH *dir, ALLEGRO_CONFIG *config);

      /**
         Attempts to load the cursors in the same way as LoadData().
      */
      virtual Error LoadCursors(ALLEGRO_PATH *dir, ALLEGRO_CONFIG *config);

      /**
          Attempts to load the samples in the same way as LoadData().
      */
      virtual Error LoadSamples(ALLEGRO_PATH *dir);

      /**
         Generate default skin bitmaps for all the dialog objects. These are
         extremely ugly and shouldn't be viewed by anyone :) This function only
         fails if it runs out of memory (unlikely).
      */
      virtual void GenerateDefaultBitmaps();

      /**
         Generates the default bitmap in slot i of the bitmap array.
      */
      virtual void GenerateDefaultBitmap(int i);

      /**
         Destroys all samples and generates default empty ones.
      */
      virtual void GenerateDefaultSamples();

      /**
         Destroys all existing cursors and sets them to the default Allegro arrow.
      */
      virtual void GenerateDefaultCursors();

      /**
         Sets all the fonts in the font array to the default Allegro font.
      */
      virtual void GenerateDefaultFonts();

      /**
         Resets the default skin colors to the default values (a grayish green theme).
      */
      virtual void ResetColors();

      /**
         Resets all the extra object info to default values.
      */
      virtual void ResetControls();

   public:
      /**
         Default constructor.
         Load the skin from a file. If the filename is NULL or reads "default",
         then the default skin is generated instead of trying to load from a file.
         The path must be an absolute one (as opposed to being relative)
      */
      Skin(const char *fileName);

      virtual ~Skin();

      // Interface

      /**
         Attempts to load a skin configuration file.
         If the file exists it tries to read every possible bit of skin information.
         If some data is missing it applies default values. This means that the old
         skin is lost even if the file you're trying to load isn't a valid skin file.
         Load() will also try to load the bitmaps, fonts, samples and cursors.

         The file path you specify must be an absolute path. If you pass a relative
         path, the function may fail.
      */
      virtual Error Load(const char *file);

      /**
         Attempts to reload the skin from the file that was last loaded.
      */
      virtual void Reload();

      /**
         Unloads the skin.
         Frees all memory allocated for bitmaps, cursors, fonts and samples and unloads all
         loaded datafiles. This is called by the destructor so you normally don't have to
         care about it.
      */
      virtual void Unload();

      /**
         Resets all skin data to default values.
         This includes bitmaps, fonts, cursors, colors, etc.

         Note: all user bitmaps, cursors and fonts will be lost too.
      */
      virtual void Reset();

      /**
      */
      Error GetError();

      /**
         Returns the bitmap from slot i of the bitmap array.
      */
      Bitmap& GetBitmap(int i) const;

      /**
         Returns the cursor from slot i of the cursor array.
      */
      Cursor& GetCursor(int i) const;

      /**
         Returns the font from slot i of the font array.
      */
      Font& GetFont(int i) const;

      /**
         Returns the sample from slot i of the sample array.
      */
      Sample& GetSample(int i) const;

      /**
         Attempts to load a bitmap from a file on disk into slot i of the bitmap array.
         This is usefull if you don't want to load your bitmaps from a regular Allegro
         datafile or if you want to load additional bitmaps for some of your own widgets.
         You can use as many slots in the bitmap array as defined here. The skin itself
         uses nBitmaps slots so all the slots above that are at your disposal.

         The bitmap has to be either in BMP, PCX or TGA format. Even if the new bitmap
         can't be loaded the old one may be destroyed.
      */
      Error LoadBitmap(const char *file, int i);

      /**
         The same as LoadBitmap() only this function loads mouse pointers. Pointers are
         stored on disk as bitmaps in either BMP, PCX or TGA format. You must use the
         Cursor::SetFocus() function to set your newly loaded cursor's focus point. The
         optional parameters are the number of frames and the interval between frames in
         miliseconds. They are relevant only if you're loading an animated cursor.
      */
      Error LoadCursor(const char *file, int i, int n = 1, int interval = 20);

      /**
         Attempts to load a font into the font array. The last parameter is the size
         (height) of the font that you are loading. Filename can be either relative or
         absolute path to the font. If the font can't be found MASkinG will look into your
         windows/fonts directory if it can find it. See Allegro Font documentation for a
         list of supported font formats.
      */
      Error LoadFont(const char *file, int i, int size = 10);

      /**
         Loads a sample in WAV or VOC format into the sample array.
      */
      Error LoadSample(const char *file, int i);

      /**
         Uploads the passed Bitmap into the bitmap array. The bitmap is loaded into slot i
         of the array by either making a link to it (a pointer) or by copying the actual
         data. If you make a copy you don't have to deal with destroying the data when it
         is no longer needed as the skin will do it for you but if you just set a pointer
         (the default) you have to destroy the bitmap yourself when you're done with your
         program.
      */
      void SetBitmap(Bitmap& b, int i, bool copy = false);

      /**
         Like SetBitmap() only this one uploads a mouse cursor.
      */
      void SetCursor(Cursor& c, int i);

      /**
         Uploads the passed font into the font array. It destroys the old font if it is
         different form the Allegro font. This function is a little different from SetBitmap()
         and SetCursor() as it can't copy the font, it just sets a pointer to the font data.
         This means you must make sure the font you pass to this function remains valid for
         the duration of the skin (i.e. you mustn't destroy it or close the datafile if it
         was loaded from one).
      */
      void SetFont(Font& f, int i);

      /**
         Like SetBitmap() only this one is for sound samples.
      */
      void SetSample(Sample& s, int i, bool copy = false);

      /**
         Attempts to play the sample in slot i of the sample array.
         If any instances of the same sample are already playing they are stopped.
      */
      void PlaySample(int i);

      /**
         Reloads the skin and colorizes all the bitmaps to the specified color.
         Alpha determines by how much the hue will shift towards the destination color.
         It's range is [0,255]. 0 will keep the bitmaps unchanged, 255 will colorize
         the bitmaps completely, any other value between 0 and 255 will let the original
         colours show through a bit.
      */
      void Colorize(const Color &col, int alpha);

      /**
         default color for boxes, menus, etc.
      */
      Color c_face;
      /**
         default text color
      */
      Color c_font;
      /**
         the lighter shade of the colors used for 3D effects
      */
      Color c_shad1;
      /**
         the darker shade of the shadow colors
      */
      Color c_shad2;
      /**
         the font color for disabled objects
      */
      Color c_disable;
      /**
         the face color for selected objects
      */
      Color c_select;
      /**
         the default color for buttons, etc.
      */
      Color c_deselect;
      /**
         the face color for objects that have the input focus
      */
      Color c_focus;
      /**
         the color for the sunken panel (for lists, edit boxes, etc.)
      */
      Color c_sunken;
      /**
         the default desktop color

         These colors are primarily used when no skin has been loaded from disk
         or some bitmaps were missing. They may be used by MASkinG widgets as well
         so every skin file has to specify them. If it doesn't then defaults are used.
      */
      Color c_back;

      /**
         indices into the array of fonts
      */
      int fnt[nInfoItems][4];
      /**
         the primary text colors
      */
      Color fcol[nInfoItems][4];
      /**
         the secondary (shadow) text colors

         These three arrays hold the font, color and shadow information for each widget
         for each state. Actually only info for objects that require it is stored. The
         possible states of objects are: normal, selected, disabled and focused.
      */
      Color scol[nInfoItems][4];

      /**
         How input focus follows the mouse.
         Possible values are:
<pre>
      0 - objects will get focus when the mouse is on top of them and loose it
          as soon as they loose the mouse
      1 - objects get focus when they get the mouse but they can hold it until
          another object takes it away
      2 - focus will not follow the mouse, i.e. you have to click on an
          object for it to get focus (like in Windows)
</pre>
         Default is 1.
      */
      int focus;

      /**
         Should widgets draw dotted rectangles?
         If set to true, objects that have the input focus should draw a dotted rectangle
         around themselves.
      */
      bool drawDots;

      // extra widget info

      /**
         the default style of the wallpaper widget
      */
      int wallpaperStyle;
      /**
         the displacement of the button text when selected (in pixels)
      */
      int buttonDisplacement;
      /**
         the type of animation used for buttons, checkboxes, etc.
      */
      int buttonAnimationType;
      /**
         the length of animation in miliseconds
      */
      int buttonAnimationLength;
      /**
         x offset of the group box title
      */
      int boxX;
      /**
         y offset of the group box title
      */
      int boxY;
      /**
         alignment of the box title text (left, right, centre)
      */
      int boxAlign;
      /**
         the background color of the group box title text (-1 is transparent)
      */
      Color boxBack;
      /**
         the position of the title text
      */
      Point winTextPos;
      /**
         the position of the exit icon
      */
      Point winExitPos;
      /**
         the position of the maximize icon
      */
      Point winMaxPos;
      /**
         the position of the minimize icon
      */
      Point winMinPos;
      /**
         alignment of the title text
      */
      int winTextAlign;
      /**
         true if the title text has a background false otherwise
      */
      int winTextBack;
      /**
         the type of animation used to popup a window
      */
      int winAnimationType;
      /**
         the length of animation in miliseconds
      */
      int winAnimationLength;
      /**
         the type of animation used for the list
      */
      int comboAnimationType;
      /**
         the length of list animation in miliseconds
      */
      int comboAnimationLength;
      /**
         the height of one menu entry
      */
      int menuHeight;
      /**
         the menu auto-open delay in miliseconds
      */
      int menuDelay;
      /**
         the type of menu animation
      */
      int menuAnimationType;
      /**
         the length of menu animation in miliseconds
      */
      int menuAnimationLength;
      /**
         the background colour for tooltips (pale yellow by default)
      */
      Color tooltipBack;
      /**
         the border colour for tooltips (black by default)
      */
      Color tooltipBorder;
      /**
         the tooltip animation type
      */
      int tooltipAnimationType;
      /**
         the tooltip animation length
      */
      int tooltipAnimationLength;
};

/**
   A pointer to the global default skin object.
   This skin is used by all the default dialog objects if no other skin is
   specified. It stores all the skin bitmaps, fonts, cursors and other data.
   Before you use a MASkinG dialog you have to initialize MASkinG by calling
   InstallMASkinG(). This function among other things either sets theSkin to
   hold the default skin data or loads it from a skin file. After you're done
   with the skin you have to delete it but don't delete theSkin directly -
   call ExitMASkinG() instead.
*/
extern MASKING_DLL_DECLSPEC Skin *theSkin;
}

#endif
