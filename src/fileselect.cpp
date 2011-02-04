////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/fileselect.h"
#include "../include/MASkinG/util.h"

MAS::ListItemFile::ListItemFile(const al_ffblk *info) : MAS::ListItemString(), file(NULL) {
   file = new al_ffblk;
   file->attrib = info->attrib;
   file->time = info->time;
   file->size = info->size;
   strcpy(file->name, info->name);
   text = file->name;
}


MAS::ListItemFile::~ListItemFile() {
   if (file) {
      free(file);
      file = NULL;
      text = NULL;
   }
}


void MAS::ListItemFile::Draw(Bitmap &canvas, Bitmap &bmp, int state) {
   ListItem::Draw(canvas, bmp, state);

   if (text) {
      int s;
      switch (state) {
         case 0:
         case 1:
            s = 0;   break;
         case 2:
         case 3:
            s = 3;   break;
         case 4:
         case 5:
            s = 1;   break;
         case 6:
         case 7:
            s = 2;   break;
      };

      // get the button colors and font
      Color fg = parent->GetParent()->GetFontColor(s);
      Color bg = parent->GetParent()->GetShadowColor(s);
      Font f = parent->GetParent()->GetFont(s);

      if (strcmp(text, ".") && strcmp(text, "..")) {
         int yy = (h() - 14)/2;
         if (file->attrib & FA_DIREC) {
            /* TODO: polyons
            static int points[] = { 6, yy+1, 11, yy+1, 15, yy+5, 3, yy+5 };
            static int points2[] = { 7, yy+2, 10, yy+2, 14, yy+4, 4, yy+4 };
            polygon(canvas, 4, points, Color(110,105,80));
            polygon(canvas, 4, points2, Color(240,230,120));
            */
            canvas.Rectfill(4, yy+3, 17, yy+12, Color(240,230,120));
            canvas.Rectangle(4, yy+3, 17, yy+12, Color(96,88,64));
         }
         else {
            canvas.Rectfill(6, yy+1, 15, yy+12, Color::white);
            canvas.Rectangle(6, yy+1, 15, yy+12, Color::black);
         }
      }
      f.GUITextout(canvas, text, 21, (bmp.h()/8 - f.TextHeight())/2, fg, bg, Color::transparent, 0);
   }
}


//static bool lockPath = false;
MAS::FileSelect::FileSelect(const char *title, const char *path, const char *mask, int flags)
   :Window(),
   alreadyAddedAFilename(false)
{
   this->title.SetText(title);
   ClearFlag(D_RESIZABLE);

   int ww = 400;
   int hh = 300;

   clientArea->Resize(ww,hh);
   Resize(ww,hh);

   bOK.Setup(       292, 240,  96,  22, ALLEGRO_KEY_O, D_EXIT, "&OK");
   bCancel.Setup(   292, 266,  96,  22, ALLEGRO_KEY_C, D_EXIT, "&Cancel");
   cArch.Setup(     292, 122,  96,  22, ALLEGRO_KEY_A, 0,      "&Archives");
   cDirec.Setup(    292, 144,  96,  22, ALLEGRO_KEY_D, D_SELECTED,"&Directories");
   cHidden.Setup(   292, 166,  96,  22, ALLEGRO_KEY_H, 0,      "&Hidden");
   cSystem.Setup(   292, 188,  96,  22, ALLEGRO_KEY_S, 0,      "&System");
   cReadOnly.Setup( 292, 210,  96,  22, ALLEGRO_KEY_R, 0,      "&Read only");

   if (!path || strcmp(path, "")==0) {
      ALLEGRO_PATH *cwd = al_get_current_directory();
      eFile.Setup(      52, 240, 232,  22, 0,     0,      al_path_cstr(cwd, ALLEGRO_NATIVE_PATH_SEP), 4096);
   }
   else {
      eFile.Setup(      52, 240, 232,  22, 0,     0,      path, 4096);
   }
   listDrives.Shape(292,  12,  96, 102);
   listFiles.Shape(  12,  12, 272, 220);
   listMask.Setup(   52, 266, 232,  22, 0, 0, 0);
   tFile.Setup(      12, 244,  40,  22, 0,     0,      "file:", 2);
   tMask.Setup(      12, 270,  40,  22, 0,     0,      "mask:", 2);

   if (flags & FA_ARCH)   cArch.Select();
   if (flags & FA_HIDDEN)   cHidden.Select();
   if (flags & FA_SYSTEM)   cSystem.Select();
   if (flags & FA_RDONLY)   cReadOnly.Select();

   Add(box);
   Add(listFiles);
   Add(tFile);
   Add(eFile);
   Add(tMask);
   Add(listMask);
   Add(listDrives);
   Add(cArch);
   Add(cDirec);
   Add(cHidden);
   Add(cSystem);
   Add(cReadOnly);
   Add(bOK);
   Add(bCancel);

   FillMaskList(mask);
   FillDriveList();
   FillFileList();
}


char *MAS::FileSelect::Popup(MAS::Dialog *p, const Point &pos) {
   return Popup(p, pos.x(), pos.y());
}


char *MAS::FileSelect::Popup(MAS::Dialog *p, int xx, int yy) {
   SetParent(p);
   Centre();
   if (xx != INT_MAX) {
      x(xx);
      if (yy != INT_MAX) {
         y(yy);
      }
   }

   box.Shape(4, 4, clientArea->w()-8, clientArea->h()-8);

   char *path = eFile.GetText();
   int l = strlen(path);
   if (l>3 && path[l-1]!='\\' && path[l-1]!='/') {
      alreadyAddedAFilename = true;
   }
   else {
      alreadyAddedAFilename = false;
   }

   if (listDrives.Size() > 0) {
      int i=0;
      while (true) {
         char *drive = listDrives.GetItem(i)->GetText();
         if (drive[0] == path[0] || drive[0]+32 == path[0]) {
            listDrives.Select(i);
            break;
         }
         ++i;
      }
   }

   //TODO: popups
   //Widget *lastObject = Window::Popup(p, xx, yy, &listFiles);
   Widget *lastObject = NULL;
   if (lastObject && lastObject != &bCancel && lastObject != &iconExit) {
      return eFile.GetText();
   }
   else {
      return NULL;
   }
}


void MAS::FileSelect::HandleEvent(Widget &obj, int msg, intptr_t arg1, intptr_t arg2) {
   Window::HandleEvent(obj,msg,arg1,arg2);

   switch (msg) {
      case MSG_ACTIVATE:
         if (obj == cArch ||
            obj == cDirec ||
            obj == cHidden ||
            obj == cReadOnly ||
            obj == cSystem ||
            obj == listMask) {
            FillFileList();
         }
         else if (obj == listFiles) {
            EnterDir();
         }
         else if (obj == eFile) {
            UpdatePath();
         }
         else if (obj == listDrives) {
            EnterDrive();
         }
         break;

      case MSG_SCROLL:
         if (obj == listFiles) {
            OnFileSelChange();
         }
         break;
   };
}


void MAS::FileSelect::FillDriveList() {
   listDrives.DeleteAllItems();
   listDrives.Disable();
/* TODO: file selector */
#if 0

#if (defined ALLEGRO_DOS) || (defined ALLEGRO_WINDOWS)
   char buf[256];
   usprintf(buf, uconvert_ascii("A:\\\0", buf));

   int c=0;
   for (int i=0; i<26; i++) {
      if (_al_drive_exists(i)) {
         usetat(buf, 0, 'A' + i);
         listDrives.InsertItem(new ListItemString(buf), c);
         c++;
      }
   }
   listDrives.Enable();
#endif

#endif
}

/* TODO: file selector */
#if 0
static bool cmp_file_item(const MAS::ListItem *li1, const MAS::ListItem *li2) {
   MAS::ListItemFile *i1 = (MAS::ListItemFile *)li1;
   MAS::ListItemFile *i2 = (MAS::ListItemFile *)li2;
   if ((i1->file->attrib & FA_DIREC) && !(i2->file->attrib & FA_DIREC)) {
      return true;
   }
   else if (!(i1->file->attrib & FA_DIREC) && (i2->file->attrib & FA_DIREC)) {
      return false;
   }
   else {
      return (ustricmp(i1->file->name, i2->file->name) < 0) ? true : false;
   }
}
#endif


void MAS::FileSelect::FillFileList() {
/* TODO: file selector */
#if 0

   lockPath = true;
   listFiles.DeleteAllItems();

   char path[4096];
   char *tmp;
   strcpy(path, eFile.GetText());
   tmp = get_filename(path);
   usetat(tmp, 0, '\0');
   fix_filename_slashes(path);

   struct al_ffblk info;
   int attrib;
   int done;
   int i=0;

   char tmp2[4096];
   // get the directories
   if (cDirec.Selected()) {
      put_backslash(path);
      ustrcat(path, uconvert_ascii("*.*", tmp2));
      attrib = 0xff;
      if (!cArch.Selected())      attrib &= ~FA_ARCH;
      if (!cHidden.Selected())   attrib &= ~FA_HIDDEN;
      if (!cSystem.Selected())   attrib &= ~FA_SYSTEM;
      if (!cReadOnly.Selected())   attrib &= ~FA_RDONLY;

      done = al_findfirst(path, &info, attrib);
      while (!done) {
         if (info.attrib & FA_DIREC) {
            listFiles.InsertItem(new ListItemFile(&info), i);
            i++;
         }
         done = al_findnext(&info);
      }
      al_findclose(&info);
   }

   // get all other files
   usetat(tmp, 0, '\0');
   attrib = 0xff;
   attrib &= ~FA_DIREC;
   if (!cArch.Selected())      attrib &= ~FA_ARCH;
   if (!cHidden.Selected())   attrib &= ~FA_HIDDEN;
   if (!cSystem.Selected())   attrib &= ~FA_SYSTEM;
   if (!cReadOnly.Selected())   attrib &= ~FA_RDONLY;

   char pattern[4096];
   char mask[4096];
   if (listMask.list.GetSelectedItem()) {
      strcpy(mask, listMask.list.GetSelectedItem()->GetText());
   }
   else {
      strcpy(mask, "All files (*.*)");
   }
   char *tok = ustrtok(mask, uconvert_ascii("(", tmp2));
   tok = ustrtok(NULL, uconvert_ascii(";", tmp2));

   while (tok) {
      int len = strlen(tok);
      if (ugetat(tok, uoffset(tok, -1)) == ')')
         usetat(tok, uoffset(tok, -1), '\0');

      strcpy(pattern, path);
      ustrcat(pattern, tok);

      done = al_findfirst(pattern, &info, attrib);
      while (!done) {
         listFiles.InsertItem(new ListItemFile(&info), i);
         i++;
         done = al_findnext(&info);
      }
      al_findclose(&info);
      tok = ustrtok(NULL, uconvert_ascii(";", tmp2));
   }

   listFiles.Sort(cmp_file_item);
   lockPath = false;

#endif
}


void MAS::FileSelect::FillMaskList(const char *mask) {
   listMask.list.DeleteAllItems();
   if (!mask) {
      listMask.list.InsertItem(new ListItemString("All files (*.*)"), 0);
   }
   else {
      char buf[4096];
	  char *last;
      strcpy(buf, mask);
      char *tok = tokenize(buf, "|", &last);
      int i=0;
      while (tok) {
         listMask.list.InsertItem(new ListItemString(tok), i);
         i++;
         tok = tokenize(NULL, "|", &last);
      }
   }

   listMask.list.Select(0);
}


void MAS::FileSelect::UpdatePath() {
/* TODO: file selector */
#if 0

   char *path = eFile.GetText();
   int l = strlen(path);
   fix_filename_slashes(path);

   if (l<3) return;

   if (ugetat(path,l-1)=='\\' || ugetat(path,l-1)=='/') {
      usetat(path,l-1,'\0');
   }

   struct al_ffblk info;
   al_findfirst(path, &info, 0xFF);
   if (info.attrib & FA_DIREC) {
      usetat(path, strlen(path), '/');
      fix_filename_slashes(path);
      FillFileList();
      eFile.Redraw();
   }
   else if (l>3) {
      eFile.SendMessage(MSG_LOSTMOUSE);
      Close();
   }
   alreadyAddedAFilename = false;

#endif
}


void MAS::FileSelect::EnterDir() {
/* TODO: file selector */
#if 0

   if (!listFiles.GetSelectedItem()) return;
   char *file = listFiles.GetSelectedItem()->GetText();
   char *path = eFile.GetText();
   char tmp2[16];

   // if the selected file is the ., do nothing
   if (strcmp(file, uconvert_ascii(".", tmp2)) == 0) {
      return;
   }
   // if the selected file is the .., go one dir up
   else if (strcmp(file, uconvert_ascii("..", tmp2)) == 0) {
      if (alreadyAddedAFilename) {
         RemoveLastFileName(path);
      }
      RemoveLastFileName(path);
   }

   // if the last string in the path is a directory, remove the trailing slash
   int l = strlen(path);
   if (l>3 && (ugetat(path,l-1)=='\\' || ugetat(path,l-1)=='/')) {
      usetat(path, uoffset(path, -1), '\0');
   }

   // go into the directory or close if it's a file
   struct al_ffblk info;
   al_findfirst(path, &info, 0xFF);
   if (info.attrib & FA_DIREC || strcmp(file, uconvert_ascii("..", tmp2)) == 0) {
      put_backslash(path);
      FillFileList();
      eFile.ScrollTo(4096);
      alreadyAddedAFilename = false;
   }
   else {
      Close();
   }
   al_findclose(&info);

#endif
}


void MAS::FileSelect::EnterDrive() {
   eFile.SetText(listDrives.GetSelectedItem()->GetText());
   FillFileList();
}


void MAS::FileSelect::OnFileSelChange() {
/* TODO: file selector */
#if 0

   if (!listFiles.GetFocusedItem() || lockPath) return;
   char *file = listFiles.GetFocusedItem()->GetText();
   char *path = eFile.GetText();
   char tmp[16];

   // append the filename to the path
   if (strcmp(file, uconvert_ascii(".", tmp)) != 0 && strcmp(file, uconvert_ascii("..", tmp)) != 0) {
      // if a filename is already at the end of the path remove it
      if (alreadyAddedAFilename) {
         RemoveLastFileName(path);
      }

      // append the filename to the end of the path and remember that
      ustrcat(path, file);
      alreadyAddedAFilename = true;

      // if the filename is a directory, append a trailing slash as well
      struct al_ffblk info;
      al_findfirst(path, &info, FA_DIREC | FA_ARCH | FA_HIDDEN | FA_SYSTEM | FA_RDONLY | FA_LABEL);
      if (info.attrib & FA_DIREC && (strcmp(file, uconvert_ascii(".", tmp)) != 0 || strcmp(file, uconvert_ascii("..", tmp)) != 0)) {
         put_backslash(path);
         //FillFileList();
      }
   }

   // redraw the filename box
   eFile.ScrollTo(4096);

#endif
}


void MAS::FileSelect::RemoveLastFileName(char *path) {
/* TODO: file selector */
#if 0

   usetat(path, uoffset(path, -1), '\0');
   char *tmpfile = get_filename(path);
   usetat(tmpfile, 0, '\0');
   put_backslash(path);
   alreadyAddedAFilename = false;

#endif
}
