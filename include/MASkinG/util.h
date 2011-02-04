// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_UTIL_H
#define      MASKING_UTIL_H

namespace MAS {
    /**
     Implementation of strtok_r, because strtok_r isn't standard.
     */
    extern char *tokenize(char *str, const char *delim, char **saveptr);
}

#endif      //MASKING_UTIL_H
