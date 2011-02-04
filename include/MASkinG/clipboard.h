// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
//                   Clipboard written by Vladimir Kokovic.                   //
// ************************************************************************** //


#ifndef MASKING_CLIPBOARD_H
#define MASKING_CLIPBOARD_H

#include "maskingdll.h"
#include <allegro5/allegro.h>


namespace MAS {
/**
Wrapper for the platform specific clipboard.
   Currently only allows copying text to and from the clipboard and is
   implemented only on Windows and Linux. On other platforms the class
   works as an internal clipboard only. To use the clipboard, create a
   Clipboard object and call either GetText() or SetText(). After calling
   GetText(), the retrieved text is stored in the buffer member variable
   and its length is stored in bufSize.
*/
class MASKING_DLL_DECLSPEC Clipboard {
   protected:
#ifdef ALLEGRO_UNIX
      /**
         Pointer to the X display (for accessing the X clipboard)
      */
      void *display;
#endif

   public:
      /**
         Pointer to the text buffer copied from the clipboard.
         Invalid when GetText() returns false.
      */
      char *buffer;

      /**
         Length of the buffer. Invalid when GetText() returns false.
      */
      unsigned long bufSize;

   public:
      Clipboard();
      ~Clipboard();

#if (defined ALLEGRO_WINDOWS)
      /**
         Retrieves text from the clipboard.

         @param hWnd window handle to be used by clipboard
         <br>
         @return TRUE - Text was successfully copied from clipboard
         @return FALSE - No text on the clipboard
      */
      bool GetText( HWND hWnd = NULL );

      /**
         Retrieves length of text on the clipboard

         @param hWnd window handle to be used by clipboard
         <br>
         @return TRUE       - Text length was successfully returned.
         @return FALSE      - No text on the clipboard
      */
      bool GetTextLength( HWND hWnd = NULL );

      /**
         Places text on the clipboard

         @param lpszBuffer pointer to a string where the text is to be put
         @param   hWnd window handle to be used by clipboard
         <br>
         @return TRUE       - Text was successfully copied from clipboard
         @return FALSE      - No text on the clipboard
      */
      bool SetText( const char *lpszBuffer, HWND hWND = NULL );
#else
      /**
         Retrieves text from the clipboard.

         @return TRUE - Text was successfully copied from clipboard
         @return FALSE - No text on the clipboard
      */
      bool GetText( void );

      /**
         Places text on the clipboard

         @param lpszBuffer pointer to a string where the text is to be put
         <br>
         @return TRUE       - Text was successfully copied from clipboard
         @return FALSE      - No text on the clipboard
      */
      bool SetText( const char *lpszBuffer );
#endif
};
}

#endif
