// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_GLOBAL_H
#define      MASKING_GLOBAL_H

#include <allegro5/allegro.h>
#include "error.h"
#include "maskingdll.h"

namespace MAS {
   class Settings;
   class Error;
   class Dialog;

   /**
      Initializes MASkinG. It reads all the settings from an Allegro
      config file the path to which you can to provide. This path should be
      relative to the location of the executable. You should call this
      function once somewhere near the beginning of your program. If you pass
      NULL or don't pass anything no config file will be read
      and the currently valid settings will be used. This allows you to setup
      your program without using the config files before actually installing
      MASkinG. The function returns an Error you should check after calling
      InstallMASkinG().
   */
   Error MASKING_DLL_DECLSPEC InstallMASkinG(const char *settings = NULL);

   /**
      Cleans up after the GUI. Call this function just before the end of
      your program (or when you're done with MASkinG and don't want to use it
      anymore).
   */
   void MASKING_DLL_DECLSPEC ExitMASkinG();

   /**
      Specify the rate at which you will call Dialog::DoTick(). It is your
      responsability to set the rate and try your best to call DoTick() in
      regular intervals, using your favorite timing method.
      YOU MUST CALL THIS BEFORE InstallMASkinG()!
   */
   void SetLogicRate(int rate);
}

#endif         //MASKING_GLOBAL_H
