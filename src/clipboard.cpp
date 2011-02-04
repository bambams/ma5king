////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
//                   Clipboard written by Vladimir Kokovic.                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/clipboard.h"
using namespace MAS;

#ifdef ALLEGRO_UNIX
  #include <X11/X.h>
  #include <X11/Xlib.h>
  #include <X11/Xatom.h>
  #include <string.h>
#endif

Clipboard::Clipboard() : buffer( 0 ), bufSize( 0 ) {
#ifdef ALLEGRO_UNIX
  display = XOpenDisplay( NULL );
#endif
}

Clipboard::~Clipboard() {
#ifdef ALLEGRO_UNIX
  if ( (Display *)display ) { XCloseDisplay( (Display *)display );  display = 0; }
  if ( buffer )  { XFree( buffer );  buffer = 0; }
#else
  if ( buffer ) { delete [] buffer;  buffer = 0; }
#endif
}

#ifdef ALLEGRO_WINDOWS
////////////////////////////////////////////////////////////////////
// GetText
// - Retrieves text from the clipboard
////////////////////////////////////////////////////////////////////
//
// Parameters:
//  hWnd       - window handle to be used by clipboard
//
// Return Values:
//  TRUE       - Text was successfully copied from clipboard
//  FALSE      - No text on the clipboard
//
////////////////////////////////////////////////////////////////////

bool Clipboard::GetText( HWND hWnd ) {
  HGLOBAL hGlobal;    // Global memory handle
  LPSTR lpszData;     // Pointer to clipboard data
  unsigned long nSize;  // Size of clipboard data
  // First, open the clipboard. OpenClipboard() takes one
  // parameter, the handle of the window that will temporarily
  // be it's owner. If NULL is passed, the current process
  // is assumed.
  OpenClipboard( hWnd );
  // Request a pointer to the text on the clipboard.
  hGlobal = GetClipboardData( CF_TEXT );
  // If there was no text on the clipboard, we have
  // been returned a NULL handle.
  if ( hGlobal == NULL )  return false;
  // Now we have a global memory handle to the text
  // stored on the clipboard. We have to lock this global
  // handle so that we have access to it.
  lpszData = (LPSTR)GlobalLock( hGlobal );
  // Now get the size of the text on the clipboard.
  nSize = GlobalSize( hGlobal );
  // Make sure the text on the clipboard is not longer
  // that the buffer that was allocated for it. If it was
  // snip the text on the clipboard so that it fits.
  if ( nSize > (UINT)bufSize ) {
    if ( buffer )  delete buffer;
    buffer = new char[nSize + 1];
    bufSize = nSize;
  }
  // Now, copy the text into the return buffer. At the
  // end, we need to add a NULL string terminator.
  UINT i;
  for ( i = 0; i < nSize; ++i )
    *(buffer + i) = *(lpszData + i);
  *(buffer + i) = 0;
  // Now, simply unlock the global memory pointer
  // and close the clipboard.
  GlobalUnlock( hGlobal );
  CloseClipboard();
  return true;
}

////////////////////////////////////////////////////////////////////
// GetTextLength
// - Retrieves length of text on the clipboard
////////////////////////////////////////////////////////////////////
//
// Parameters:
//  pnSize     - pointer to unsigned long that will receive
//               the length of the text on the clipboard.
//               NOTE: Does not include NULL terminator.
//  hWnd       - window handle to be used by clipboard
//
// Return Values:
//  TRUE       - Text length was successfully returned.
//  FALSE      - No text on the clipboard
//
////////////////////////////////////////////////////////////////////

bool Clipboard::GetTextLength( HWND hWnd ) {
  HGLOBAL hGlobal;    // Global memory handle
  unsigned long nSize;  // Size of clipboard data
  LPSTR lpszData;     // Pointer to clipboard data
  // First, open the clipboard. OpenClipboard() takes one
  // parameter, the handle of the window that will temporarily
  // be it's owner. If NULL is passed, the current process
  // is assumed.
  OpenClipboard( hWnd );
  // Request a pointer to the text on the clipboard.
  hGlobal = GetClipboardData( CF_TEXT );
  // If there was no text on the clipboard, we have
  // been returned a NULL handle.
  if ( hGlobal == NULL )  return false;
  // Now we have a global memory handle to the text
  // stored on the clipboard. We have to lock this global
  // handle so that we have access to it.
  lpszData = (LPSTR)GlobalLock( hGlobal );
  // Now get the size of the text on the clipboard.
  nSize = GlobalSize( hGlobal );
  // Now, simply unlock the global memory pointer
  // and close the clipboard.
  GlobalUnlock( hGlobal );
  CloseClipboard();
  // Finally, save the length of the string we found
  // into the bufSize pointer and return.
  bufSize = nSize;
  return true;
}

////////////////////////////////////////////////////////////////////
// SetText
// - Places text on the clipboard
////////////////////////////////////////////////////////////////////
//
// Parameters:
//  lpszBuffer - pointer to a string where the text is to be put
//  hWnd       - window handle to be used by clipboard
//
// Return Values:
//  TRUE       - Text was successfully copied from clipboard
//  FALSE      - No text on the clipboard
//
////////////////////////////////////////////////////////////////////

bool Clipboard::SetText( const char *lpszBuffer, HWND hWnd ) {
  HGLOBAL hGlobal;    // Global memory handle
  LPSTR lpszData;     // Pointer to clipboard data
  unsigned long nSize;  // Size of clipboard data
  // First, open the clipboard. OpenClipboard() takes one
  // parameter, the handle of the window that will temporarily
  // be it's owner. If NULL is passed, the current process
  // is assumed. After opening, empty the clipboard so we
  // can put our text on it.
  OpenClipboard( hWnd );
  EmptyClipboard();
  // Get the size of the string in the buffer that was
  // passed into the function, so we know how much global
  // memory to allocate for the string.
  nSize = lstrlen( lpszBuffer );
  // Allocate the memory for the string.
  hGlobal = GlobalAlloc( GMEM_ZEROINIT, nSize + 1 );
  // If we got any error during the memory allocation,
  // we have been returned a NULL handle.
  if ( hGlobal == NULL )  return false;
  // Now we have a global memory handle to the text
  // stored on the clipboard. We have to lock this global
  // handle so that we have access to it.
  lpszData = (LPSTR)GlobalLock( hGlobal );
  // Now, copy the text from the buffer into the allocated
  // global memory pointer.
  for ( UINT i = 0; i < nSize + 1; ++i )
    *(lpszData + i) = *(lpszBuffer + i);
  // Now, simply unlock the global memory pointer,
  // set the clipboard data type and pointer,
  // and close the clipboard.
  GlobalUnlock( hGlobal );
  SetClipboardData( CF_TEXT, hGlobal );
  CloseClipboard();
  return true;
}
#else
#ifdef ALLEGRO_UNIX
bool Clipboard::GetText( void ) {
  if ( !(Display *)display )  return false;
  if ( buffer )  XFree( buffer );
  if ( (buffer = XFetchBytes( (Display *)display, (int *)&bufSize )) != NULL )  return true;
  return false;
}

bool Clipboard::SetText( const char *lpszBuffer ) {
  if ( !display )  return false;
  int len = strlen( lpszBuffer );
  /* Perform the cut.  If we don't call XSetSelectionOwner() before
  doing the cut, existing selections are not overridden. */
  XSetSelectionOwner( (Display *)display, XA_PRIMARY, None, CurrentTime );
  XStoreBytes( (Display *)display, lpszBuffer, len+1 );
  return true;
}
#else
// Internal clipboard for platforms that don't have a clipboard of their
// own (e.g. DOS) or a wrapper hasn't yet been written.
// Untested code!
bool Clipboard::GetText( void ) {
if (buffer) {
      return true;
   }
   else {
      return false;
   }
}

bool Clipboard::SetText( const char *lpszBuffer ) {
   if (buffer) {
      delete [] buffer;
      buffer = 0;
      bufSize = 0;
   }

   if (lpszBuffer) {
      int len = strlen( lpszBuffer );
      buffer = new char[len+1];
      strcpy(buffer, lpszBuffer);
      bufSize = len;
   }

   return true;
}
#endif      // !linux
#endif      // !windows
