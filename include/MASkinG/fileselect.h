// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_FILESELECT_H
#define      MASKING_FILESELECT_H

#include "../MASkinG.h"
#include "maskingdll.h"

namespace MAS {
   /* From A4 internals */
   struct al_ffblk        /* file info block for the al_find*() routines */
   {
      int attrib;         /* actual attributes of the file found */
      time_t time;        /* modification time of file */
      long size;          /* size of file */
      char name[512];     /* name of file */
      void *ff_data;      /* private hook */
   };

/**
   Internal helper widget for the FileSelector.

   Draws a fileselector's file list entry with an appropriate icon indicating
   wether a file is actually a file or a directory.

   @see FileSelect
*/
class MASKING_DLL_DECLSPEC ListItemFile : public ListItemString {
   public:
      /**
         Constructor for making a list item from an allegro al_ffblk struct.
      */
      ListItemFile(const struct al_ffblk *info);
      virtual ~ListItemFile();
      /**
         Draws the list item with an appropriate icon.
      */
      void Draw(Bitmap &canvas, Bitmap &skinBitmap, int state);
      /**
         The underlying al_ffblk struct describing the file (name, attributes).
      */
      struct al_ffblk *file;
};

/**
   The standard fileselector dialog.
   A file selector is a dialog that allows you to browse your file system and
   select a single file from it.

   Note: currently the file selector has "issues" and may not always work properly.
*/
class MASKING_DLL_DECLSPEC FileSelect : public Window {
   protected:
      // the underlying widgets that make up the dialog
      Panel box;
      ListBox listFiles;
      Label tFile;
      EditBox eFile;
      Label tMask;
      ComboBox listMask;
      ListBox listDrives;
      CheckBox cArch, cDirec, cHidden, cSystem, cReadOnly;
      Button bOK, bCancel;

      // various internal helper functions
      void FillDriveList();
      void FillFileList();
      void FillMaskList(const char *mask);
      void UpdatePath();
      void EnterDir();
      void EnterDrive();
      void OnFileSelChange();
      void RemoveLastFileName(char *path);

      bool alreadyAddedAFilename;

   public:
      /**
         File selector constructor. Sets up a file selector dialog. All
         parameters are optional but you can pass a title string for the
         dialog, the initial path, and the file mask string and flags to
         determine which files should be visible initially. If the initial
         path is NULL the current working directory will be used. The file
         mask string should be a list of file types separated with
         vertical pipes (the '|' character). Every file type should have
         two parts: a short description and a semicolon spearated list of
         extensions surrounded by round braces. The flags should be a
         combination of standard Allegro FA_* flags. For example:
<pre>
      FileSelect dlg("Open file...",
                     NULL,
                        "All bitmaps (*.bmp;*.pcx;*.tga)|
                         BMP files (*.bmp)|
                         PCX files (*.pcx)|
                         TGA files (*.tga)|
                         All files (*.*)",
                     FA_ARCH);
</pre>
         will create an "open file" dialog that will start in the current
         working directory and will be able to list archives with either
         all known bitmap extensions, all bmp files, all pcx files, all
         tga files or all files.
      */
      FileSelect(const char *title = "Select file", const char *startDir = NULL, const char *mask = "All files (*.*)", int flags = 0xFF);
      /**
      */
      virtual char *Popup(Dialog *p, int = INT_MAX, int = INT_MAX);
      /**
         Pops up a file selector at the given position. Returns the full
         path to the selected file or NULL if the dialog is canceled or escaped.
      */
      virtual char *Popup(Dialog *p, const Point &pos);
      /**
         Handles various events such as list box selections, check box
         clicks, button clicks, etc.
      */
      void HandleEvent(Widget &obj, int msg, intptr_t arg1=0, intptr_t arg2=0);
};
}

#endif      //MASKING_FILESELECT_H
