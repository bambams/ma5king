////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/error.h"
#include <allegro5/allegro.h>


const char *errorDescription[] = {
   "No error.\n",
   "Error: ran out of memory!!!\n",
   "Error: ran out of video memory!\n",
   "Error: skin description file not found!\n",
   "Error: skin data file not found!\n",
   "Error: file not found!\n",
   "Error: couldn't load file!\n",
   "Error: array index out of bounds!\n",
   "An unknown error occured!\n"
};


MAS::Error::Error(Type t)
   :type(t)
{
}


MAS::Error::~Error() {
}


const char *MAS::Error::GetDescription() {
   return errorDescription[type];
}


MAS::Error::operator bool() const {
   return (type != NONE);
}


bool MAS::Error::operator==(const Type  &t) {
   return (type == t);
}


bool MAS::Error::operator!=(const Type  &t) {
   return (type != t);
}


void MAS::Error::Report() {
   TRACE("MA5king: %s\n", GetDescription());
   abort();
}
