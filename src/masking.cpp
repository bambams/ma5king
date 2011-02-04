////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include <allegro5/allegro.h>
#include "../include/MASkinG.h"


/* TODO
   We currently require this file to be there. This is becuase there is no
   default font in A5.
*/
#define DEFAULT_FONT "skins/default_font.tga"
MAS::Font* DefaultFont = NULL;


////////////////////////////////////////////////////////////////////////////////
// Installs the MASkinG library by setting up Allegro and Allegro Font,
// reseting the skin and optionally loading a user skin from disk
MAS::Error MAS::InstallMASkinG(const char *install_path) {
   DefaultFont = new Font(DEFAULT_FONT);

   MAS::Color::InitBasicColors();

   MAS::Settings::Load(install_path);

   theSkin = new MAS::Skin(MAS::Settings::skinPath);
   if (!theSkin) {
      return MAS::Error(MAS::Error::MEMORY);
   }
   else if (theSkin->GetError()) {
      return theSkin->GetError();
   }

   return Error::NONE;
}


void MAS::ExitMASkinG() {
   if (theSkin) {
      delete theSkin;
      theSkin = NULL;
   }

   delete DefaultFont;
}


void MAS::SetLogicRate(int rate) {
   MAS::Settings::logicFrameRate = rate;
}

