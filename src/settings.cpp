////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/settings.h"
#include "../include/MASkinG/global.h"


char  MAS::Settings::skinPath[2048];
bool  MAS::Settings::useVideoMemory       = true;
bool  MAS::Settings::fullDrag             = true;
bool  MAS::Settings::fullResize           = true;
int   MAS::Settings::logicFrameRate       = -1; //must be overriden by the user
bool  MAS::Settings::guiSound             = true;
int   MAS::Settings::soundVolume          = 100;
bool  MAS::Settings::showMouse            = true;
bool  MAS::Settings::mouseShadow          = true;
int   MAS::Settings::doubleClickDelay     = 200;
bool  MAS::Settings::useKeyboard          = true;
bool  MAS::Settings::useJoystick          = false;
int   MAS::Settings::tooltipDelay         = 200;
int   MAS::Settings::tooltipAutohideDelay = 5000;
int   MAS::Settings::scrollingSpeed       = 3;


int MAS::Settings::get_int(const char *key, int def, ALLEGRO_CONFIG *config) {
   const char *val = al_get_config_value(config, "MASkinG", key);
   if (!val) {
      return def;
   }

   return strtol(val, NULL, 10);
}


void MAS::Settings::Load(const char *file) {
   ALLEGRO_CONFIG *config = NULL;

   if (!file)
      return;

   config = al_load_config_file(file);
   if (!config)
      return;

#define GET_CONFIG_VAL(val_name) val_name = get_int(#val_name, val_name, config)
#define GET_CONFIG_BOOL(val_name) val_name = (get_int(#val_name, val_name, config) != 0)
   
   strcpy(skinPath, al_get_config_value(config, "MASkinG", "skinPath"));
   GET_CONFIG_BOOL(useVideoMemory);
   GET_CONFIG_BOOL(fullDrag);
   GET_CONFIG_BOOL(fullResize);
   GET_CONFIG_BOOL(guiSound);
   GET_CONFIG_VAL(soundVolume);
   GET_CONFIG_BOOL(showMouse);
   GET_CONFIG_BOOL(mouseShadow);
   GET_CONFIG_VAL(doubleClickDelay);
   GET_CONFIG_BOOL(useKeyboard);
   GET_CONFIG_BOOL(useJoystick);
   GET_CONFIG_VAL(tooltipDelay);
   GET_CONFIG_VAL(tooltipAutohideDelay);
   GET_CONFIG_VAL(scrollingSpeed);

#undef GET_CONFIG_VAL
#undef GET_CONFIG_BOOL

   al_destroy_config(config);
}
