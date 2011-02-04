// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef MASKING_DLL_DECLSPEC
  #ifdef MASKING_DLL
    #ifdef BUILD_DLL
      #define MASKING_DLL_DECLSPEC __declspec(dllexport)
    #else
      #define MASKING_DLL_DECLSPEC __declspec(dllimport)
    #endif
  #else
    #define MASKING_DLL_DECLSPEC
  #endif
#endif
