// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef         MASKING_SETTINGS_H
#define      MASKING_SETTINGS_H

#include "global.h"
#include "maskingdll.h"

namespace MAS {
/**
   Stores all the settings and preferences for a MASkinG application.
   It has functions for reading the values from an Allegro config file
   If settings can't be read from the given file, sensible
   default values are provided. All the data and methods are static so this acts
   more as a namespace for various preferences that would otherwise be global.
*/
class MASKING_DLL_DECLSPEC Settings {
   private:
      static int get_int(const char* key, int def, ALLEGRO_CONFIG *config);

   public:
      ~Settings();

      /**
         Path to the skin file the GUI should use.
      */
      static char skinPath[2048];

      /**
         Should we use hardware acceleration with video bitmaps?
         Set to true if you want the GUI to use video memory for holding skin
         data and the screen buffers. This can improve performance significantly
         in some configurations.
      */
      static bool useVideoMemory;

      /**
         Show window contents while dragging?

         Note: not currently implemented.
      */
      static bool fullDrag;

      /**
         Show window contents while resizing?

         Note: not currently implemented.
      */
      static bool fullResize;

      /**
         The frame rate of the GUI logic.
         This is NOT read from the configuration file. This must be set
         by the user.
      */
      static int logicFrameRate;

      /**
         Enable/disable GUI sounds.
      */
      static bool guiSound;

      /**
         Volume of the rollover sounds, from 0 to 100.
      */
      static int soundVolume;

      /**
         Should the GUI use the mouse or not?
         This doesn't just hide the mouse cursor but disables mouse tracking comepletely.
         But the Allegro mouse module is still installed even if showMouse is false, this
         setting enables you to dynamically turn the mouse on and off during the execution
         of the program.
      */
      static bool showMouse;

      /**
         Should we render the mouse shadow or not?
         Generating mouse shadow can take quite a lot of time especially if you use large
         animated cursors (several seconds on slower systems) and drawing them is quite
         slow too, especially on video memory. Mouse shadow are automatically disabled
         in 8 bpp mode.
      */
      static bool mouseShadow;

      /**
         Double click delay in miliseconds.
      */
      static int doubleClickDelay;

      /**
         Should the GUI handle keyboard input?
         If turned off joystick input will be turned off too as it is simulated as
         keyboard input.
      */
      static bool useKeyboard;

      /**
         Should the GUI use the joystick for simulating keybord input?
      */
      static bool useJoystick;

      /**
         The tooltip delay in miliseconds.
         This is the time the mouse has to be completely still before a tooltip help bubble
         is shown. The default is 200.
      */
      static int tooltipDelay;

      /**
         The delay before a tooltip automatically hides itself.
      */
      static int tooltipAutohideDelay;

      /**
         The scrooling speed of the mouse wheel as number of items scrolled at a time.
      */
      static int scrollingSpeed;

   public:
      /**
         Attempts to load the settings from the given config file.
         If a value can't be read, the default is used.
      */
      static void Load(const char *fileName);
};
}

#endif         //MASKING_SETTINGS_H
