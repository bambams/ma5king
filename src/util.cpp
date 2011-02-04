////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/util.h"
#include <stdio.h>
#include <string.h>

char *MAS::tokenize(char *str, const char *delim, char **saveptr) {
    char *token;
    
    if(str) {
        *saveptr = str;
    }
    
    token = *saveptr;

    if(!token) {
        return NULL;
    }

    token += strspn(token, delim);
    *saveptr = strpbrk(token, delim);
    
    if(*saveptr) {
        *(*saveptr)++ = '\0';
    }

    return *token ? token : NULL; 
}
