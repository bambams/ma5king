////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
//                     Modifications by Vladimir Kokovic.                    //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/debug.h"
#include "../include/MASkinG/menu.h"
#include "../include/MASkinG/settings.h"
#include "../include/MASkinG/util.h"

std::vector<MAS::MenuItem *> MAS::Menu::itemsHistory;
std::vector<MAS::Menu *> MAS::Menu::menusHistory;

MAS::MenuItem::MenuItem(const char *txt, Menu *ch)
:Widget(),
   text(NULL),
   child(ch),
   callbackID(-1),
   topBar(true),
   delayTimer(0),
   allocated(false)
{
   SetText(txt);
}


MAS::MenuItem::MenuItem(const char *txt, int id)
   :Widget(),
   text(NULL),
   child(NULL),
   callbackID(id),
   topBar(true),
   delayTimer(0),
   allocated(false)
{
   SetText(txt);
}


MAS::MenuItem::~MenuItem() {
   if (text) {
      delete [] text;
      text = NULL;
   }

   child = NULL;
}


void MAS::MenuItem::MsgInitSkin() {
   Widget::MsgInitSkin();
   for (int i=0; i<4; i++) {
      if (GetFontColor(i) == Color::transparent) SetFontColor(skin->fcol[Skin::INFO_MENU][i], skin->scol[Skin::INFO_MENU][i], i);
      if (GetFontIndex(i) == -1) SetFont(skin->fnt[Skin::INFO_MENU][i], i);
   }
   if (GetTextMode() == Color::transparent) SetTextMode(Color::transparent);
   if (GetSampleIndex(Skin::SAMPLE_GOTFOCUS) == -1) SetSample(Skin::SAMPLE_GOTFOCUS, Skin::SAMPLE_GOTFOCUS);
   if (GetSampleIndex(Skin::SAMPLE_LOSTFOCUS) == -1) SetSample(Skin::SAMPLE_LOSTFOCUS, Skin::SAMPLE_LOSTFOCUS);
}


bool MAS::MenuItem::MsgWantfocus() {
   Widget::MsgWantfocus();
   return text ? true : false;
}


bool MAS::MenuItem::MsgWantTab() {
   Widget::MsgWantTab();
   return false;
}


void MAS::MenuItem::MsgGotfocus() {
   Widget::MsgGotfocus();
   delayTimer = 0;
}


void MAS::MenuItem::MsgLostfocus() {
   Widget::MsgLostfocus();
   delayTimer = -1;
}


void MAS::MenuItem::MsgTick() {
   Widget::MsgTick();
   if (HasFocus() && !topBar && child && delayTimer >= 0) {
      delayTimer += 1000/MAS::Settings::logicFrameRate;
      if (delayTimer >= skin->menuDelay) {
         delayTimer = -1;
         GetParent()->HandleEvent(*this, MSG_ACTIVATE);
      }
   }
}


void MAS::MenuItem::MsgKey() {
   Widget::MsgKey();
   GetParent()->HandleEvent(*this, MSG_ACTIVATE);
}


bool MAS::MenuItem::MsgChar(int c) {
   Widget::MsgChar(c);
   return true;
}


void MAS::MenuItem::MsgLPress() {
   Widget::MsgLPress();
   SetFlag(D_PRESSED);
}


void MAS::MenuItem::MsgLRelease() {
   Widget::MsgLRelease();
   if (TestFlag(D_PRESSED)) {
      ClearFlag(D_PRESSED);
      if (HasMouse()) {
         GetParent()->HandleEvent(*this, MSG_ACTIVATE);
      }
   }
}


void MAS::MenuItem::Draw(Bitmap &canvas) {
   int off, my;
   char *buf, *tok, *last;

   // if this item is the parent of a currently open child menu
   // it should be drawn as if it has focus (note: this is a big
   // ugly hack (tm))
   Menu *openChildMenu =  ((Menu *)GetParent())->openChildMenu;
   bool open = false;
   if (openChildMenu && openChildMenu->parentMenu == this) {
      open = true;
   }

   // get the button state
   int state;
   if (topBar) {
      state = Disabled() ? 2 : ((Selected() || open) ? 3 : 0);
   }
   else {
      state = Disabled() ? 2 : ((HasFocus() || open) ? 3 : (Selected() ? 1 : 0));
   }

   // get the button colors and font
   Color fg = GetFontColor(state);
   Color bg = GetShadowColor(state);
   Font f = GetFont(state);

   // Get the subbitmap count and offset for a particular type of menu item
   int vcount;
   if (topBar) {
      vcount = 2;
      //off = (Selected() || HasFocus()) ? 1 : 0;
      off = (HasFocus() || open) ? 1 : 0;
   }
   else {
      vcount = 7;
      if (HasFocus() || open) {
         if (child)            off = 4;
         else if (Selected())   off = 6;
         else               off = 1;
      }
      else {
         if (child)            off = 3;
         else if (Selected())   off = 5;
         else               off = 0;
      }
   }

   // Draw a normal text menu item
   SetBitmap(topBar ? Skin::MENU_BUTTON : Skin::MENU_ITEM);
   Bitmap bmp = GetBitmap();

   if (text) {
      my = (h() - f.TextHeight())/2;

      // The menu button
      bmp.TiledBlit(canvas, 0, off, 0, 0, w(), h(), 1, vcount);

      // The menu text (left part)
      buf = strdup(text);
      tok = tokenize(buf, "\t", &last);
      if (topBar) {
         f.GUITextoutEx(canvas, tok, 4, my, fg, bg, GetTextMode(), 0);
      }
      else {
         f.GUITextoutEx(canvas, tok, 16, my, fg, bg, GetTextMode(), 0);
      }

      // The menu text (right part)
      tok = tokenize(NULL, "", &last);
      if (tok) {
         f.GUITextout(canvas, tok, w() - f.TextLength(tok) - 12, my, fg, bg, GetTextMode(), 0);
      }

      free(buf);
   }
   // It's not a text item, it's a separator
   else {
      bmp.TiledBlit(canvas, 0, 2, 0, 0, w(), h(), 1, vcount);
   }
}


char *MAS::MenuItem::GetText() {
   return text;
}


void MAS::MenuItem::SetText(const char *t) {
   if (text) {
      delete [] text;
      text = NULL;
   }

   if (t) {
      text = new char[1+strlen(t)];
      strcpy(text, t);
   }
}


void MAS::MenuItem::UpdateSize() {
   if (text) {
      Font f = GetFont(Skin::NORMAL);
      int l = f.TextLength(text) + (topBar ? 8 : 64);
      Resize(l, skin->menuHeight);
   }
}



//////////////////////////////////////////////////

MAS::Menu::Menu()
   :Dialog(),
   topBar(true),
   parentMenu(NULL),
   openChildMenu(NULL)
{
   Dialog::Add(back);
   w(8);
   back.w(8);
   minSize = MAS::Size(0,0);
}


MAS::Menu::~Menu() {
   /*
   while (widgets.size() > 1) {
      Remove(0);
   }
   */
}


void MAS::Menu::MsgInitSkin() {
   Dialog::MsgInitSkin();
   if (GetBitmapIndex() == -1) SetBitmap(Skin::MENU_BACK);
   back.SetBitmap(bitmap);
   UpdateSize();
   SetAnimationProperties(skin->menuAnimationLength, skin->menuAnimationType);
   if (GetSampleIndex(Skin::SAMPLE_ACTIVATE) == -1) SetSample(Skin::SAMPLE_ACTIVATE, Skin::SAMPLE_ACTIVATE);
   if (GetSampleIndex(Skin::SAMPLE_OPEN) == -1) SetSample(Skin::SAMPLE_OPEN, Skin::SAMPLE_OPEN);
   if (GetSampleIndex(Skin::SAMPLE_CLOSE) == -1) SetSample(Skin::SAMPLE_CLOSE, Skin::SAMPLE_CLOSE);
}


void MAS::Menu::MsgLostmouse() {
   Dialog::MsgLostmouse();
}


void MAS::Menu::MsgLostfocus() {
   Dialog::MsgLostfocus();
}


bool MAS::Menu::MsgWantfocus() {
   return true;
}


bool MAS::Menu::MsgWantTab() {
   return false;
}


bool MAS::Menu::MsgXChar(int c) {
   Dialog::MsgXChar(c);

   if (openChildMenu || !topBar) {
      return false;
   }
   else {
      int x = MenuAltKey(c);
      if (!x) {
         return false;
      }

      for (size_t i=1; i<widgets.size(); i++) {
         if (MenuKeyShortcut(x, ((MenuItem *)widgets[i])->GetText())) {
            HandleEvent(*widgets[i], MSG_ACTIVATE);
            MenuItem *item = (MenuItem *)widgets[i];
            parent->MoveFocusTo(this);
            MoveFocusTo(item);
            break;
         }
      }
   }

   return true;
}


bool MAS::Menu::MsgChar(int c) {
   Dialog::MsgChar(c);

   switch (c>>8) {
      case ALLEGRO_KEY_ESCAPE:
         GetRootMenu()->Close();
         MoveFocusTo(NULL);
         GetParent()->MoveFocusTo(NULL);
         break;

      case ALLEGRO_KEY_TAB:
         return true;
         break;

      case ALLEGRO_KEY_LEFT:
         if (topBar) {
            MoveFocus(3);
         }
         else {
            if (parentMenu) {
               Close();
               GetParent()->MoveFocusTo(parentMenu);
               if (parentMenu->topBar) {
                  parentMenu->SendMessage(MSG_CHAR, c);
               }
            }
         }
         break;

      case ALLEGRO_KEY_RIGHT:
         if (topBar) {
            MoveFocus(2);
         }
         else {
            if (openChildMenu) {
               // Find enabled widget
               MenuItem *it;
               int i = 0;
               while( (it = openChildMenu->GetItem(i)) )
               {
                  if( !it->Disabled() && it->text)
                     break;
                  i++;
               }

               // do nothing if no widgets is enabled
               if( it ) {
                  openChildMenu->MoveFocusTo(it);
                  openChildMenu->GetParent()->MoveFocusTo(openChildMenu);
               }
            }
            else if (parentMenu) {
               Close();
               GetParent()->MoveFocusTo(parentMenu);
               parentMenu->SendMessage(MSG_CHAR, c);
            }
         }
         break;

      case ALLEGRO_KEY_UP:
         if (!topBar) {
            MoveFocus(5);
         }
         break;

      case ALLEGRO_KEY_DOWN:
         if (!topBar) {
            MoveFocus(4);
         }
         else {
            if (openChildMenu) {
               // Find enabled widget
               MenuItem *it;
               int i = 0;
               while( (it = openChildMenu->GetItem(i)) )
               {
                  if( !it->Disabled() && it->text)
                     break;
                  i++;
               }

               // do nothing if no widgets is enabled
               if( it ) {
                  openChildMenu->MoveFocusTo(it);
                  openChildMenu->GetParent()->MoveFocusTo(openChildMenu);
               }
            }
         }
         break;

      case ALLEGRO_KEY_SPACE:
      case ALLEGRO_KEY_ENTER:
         if (focusObject) {
            HandleEvent(*focusObject, MSG_ACTIVATE);
         }
         break;

      default: {
         if (openChildMenu) {
            return openChildMenu->MsgChar(c);
         }
         else {
            for (size_t i=1; i<widgets.size(); i++) {
               if (MenuKeyShortcut(c, ((MenuItem *)widgets[i])->GetText())) {
                  MenuItem *item = (MenuItem *)widgets[i];
                  parent->MoveFocusTo(this);
                  MoveFocusTo(item);
                  //TODO
                  //simulate_keypress(ALLEGRO_KEY_ENTER<<8);
                  return true;
               }
            }
         }
         return false;
      }
   }

   return true;
}

// dir: 2 - right, 3 - left, 4 - down, 5 - up (0 and 1 are not used (TAB and shift+TAB)
void MAS::Menu::MoveFocus(int dir) {
   size_t i=1;
   while (i<widgets.size()) {
      if (widgets[i]->HasFocus()) break;
      ++i;
   }

   size_t start = i;
   int di = -1-2*(dir%2-1);
   while (true) {
      i += di;
      if( i == start ) // avoid cycling
         break;
      if (i >= widgets.size()) {
         i = 1;
      }
      else if (i < 1) {
         i = widgets.size()-1;
      }
      //i = MID(1, i, widgets.size()-1);
      MenuItem *it = (MenuItem*)widgets[i];
      if (it->text && !it->Disabled() ) {
         Dialog::MoveFocus(focusObject, it);
         break;
      }
   }
}


bool MAS::Menu::MenuKeyShortcut(int c, const char *s) {
//TODO
#if 0
   int d;

   if (!s) return false;
   while ((d = ugetxc(&s)) != 0) {
      if (d == '&') {
         d = ugetc(s);
         if ((d != '&') && (tolower(d) == tolower(c & 0xFF)))
            return true;
      }
   }
#endif
   return false;
}


int MAS::Menu::MenuAltKey(int k) {
   static unsigned char alt_table[] =
   {
      ALLEGRO_KEY_A, ALLEGRO_KEY_B, ALLEGRO_KEY_C, ALLEGRO_KEY_D, ALLEGRO_KEY_E, ALLEGRO_KEY_F, ALLEGRO_KEY_G, ALLEGRO_KEY_H, ALLEGRO_KEY_I,
      ALLEGRO_KEY_J, ALLEGRO_KEY_K, ALLEGRO_KEY_L, ALLEGRO_KEY_M, ALLEGRO_KEY_N, ALLEGRO_KEY_O, ALLEGRO_KEY_P, ALLEGRO_KEY_Q, ALLEGRO_KEY_R,
      ALLEGRO_KEY_S, ALLEGRO_KEY_T, ALLEGRO_KEY_U, ALLEGRO_KEY_V, ALLEGRO_KEY_W, ALLEGRO_KEY_X, ALLEGRO_KEY_Y, ALLEGRO_KEY_Z
   };

   AL_CONST char *s;
   int c, d;

   if (k & 0xFF)
      return 0;

   k >>= 8;

   //TODO
   //c = scancode_to_ascii(k);
   c = 0;
   if (c) {
      k = c;
   }
   else {
      for (c=0; c<(int)sizeof(alt_table); c++) {
         if (k == alt_table[c]) {
            k = c + 'a';
            break;
         }
      }

      if (c >= (int)sizeof(alt_table))
      return 0;
   }

//TOD
#if 0
   for (size_t i=1; i<widgets.size(); i++) {
      s = ((MenuItem *)widgets[i])->GetText();
      while ((d = ugetxc(&s)) != 0) {
         if (d == '&') {
            d = ugetc(s);
            if ((d != '&') && (tolower(d) == tolower(k))) {
               return k;
            }
         }
      }
   }
#endif

   return 0;
}


MAS::Dialog::ActionType MAS::Menu::GetAction() {
   if (mouseObject == &back) {
      return MOVE;
   }
   else {
      return NONE;
   }
}


void MAS::Menu::HandleEvent(Widget& obj, int msg, intptr_t arg1, intptr_t arg2) {
   Dialog::HandleEvent(obj, msg, arg1, arg2);

   switch (msg) {
      case MSG_ACTIVATE:
         if (&obj == openChildMenu) {
            Menu *m = GetRootMenu();
            if (!m->parentMenu) {
               m->Close();
            }
         }
         else {
            MenuItem *item = (MenuItem *)(&obj);
            if (item->child) {
               if (openChildMenu) {
                  if (openChildMenu == item->child) {
                     if (topBar && skin->focus != 2) {
                        CloseChildMenu();
                        obj.Deselect();
                        MoveFocusTo(NULL);
                        PlaySample(Skin::SAMPLE_CLOSE);
                     }
                  }
                  else {
                     if (skin->focus != 2) {
                        CloseChildMenu();
                        OpenChildMenu(item);
                        obj.Select();
                        MoveFocusTo(item);
                        PlaySample(Skin::SAMPLE_OPEN);
                     }
                  }
               }
               else {
                  OpenChildMenu(item);
                  obj.Select();
                  PlaySample(Skin::SAMPLE_OPEN);
               }
            }
            else if (item->callbackID >= 0) {
               PlaySample(Skin::SAMPLE_ACTIVATE);
               itemsHistory.push_back( item );
               menusHistory.push_back( this );
               //TODO
               //simulate_keypress(ALLEGRO_KEY_ESC<<8);
               GetParent()->HandleEvent(*GetRootMenu(), item->callbackID);
               if ( !itemsHistory.empty() ) {
                  itemsHistory.clear();
                  menusHistory.clear();
               }
            }
         }
         break;

      case MSG_GOTFOCUS: {
         MenuItem *item = (MenuItem *)(&obj);
         if (openChildMenu && openChildMenu != item->child) {
            CloseChildMenu();

            if (item->child) {
               OpenChildMenu(item);
               PlaySample(Skin::SAMPLE_OPEN);
            }
            else {
               PlaySample(Skin::SAMPLE_CLOSE);
            }

            if (topBar) {
               obj.Select();
            }
         }
         break;
      }

      case MSG_LOSTMOUSE: {
         if (theSkin->focus == 0)
         if (!MouseInMenu() && !MouseInChildMenu()) {
            if (!close) {
               close = true;
               GetRootMenu()->Close();
               close = false;
            }
         }
         break;
      }

      case MSG_LOSTFOCUS: {
         if (!MouseInMenu() && !MouseInChildMenu()) {
            if (!close) {
               close = true;
               GetRootMenu()->Close();
               close = false;
            }
         }
         break;
      }

      /*
      case MSG_LOSTFOCUS: {
         if (obj == *this) {
            if (!openChildMenu) {
               GetRootMenu()->Close();
            }
            else if (!(openChildMenu->HasFocus())) {
               GetRootMenu()->Close();
            }
         }
         break;
      }
      */

      case MSG_CHAR:
         if (obj != *this) {
            MsgChar(arg1);
         }
         break;
   };
}


bool MAS::Menu::MouseInMenu() {
   Point mp = GetMousePos();
   Rect r = Rect(0, 0, w(), h());
   //if (HasMouse()) {
   if (mp <= r) {
      return true;
   }
   else if (parentMenu) {
      return ((MAS::Menu *)parentMenu->GetParent())->MouseInMenu();
   }
   else {
      return false;
   }
}


bool MAS::Menu::MouseInChildMenu() {
   if (!openChildMenu) {
      return false;
   }
   //else if (openChildMenu->HasMouse()) {
   else {
      Point mp = openChildMenu->GetMousePos();
      Rect r = Rect(0, 0, openChildMenu->w(), openChildMenu->h());
      if (mp <= r) {
         return true;
      }
      else {
         return openChildMenu->MouseInChildMenu();
      }
   }
}


void MAS::Menu::CloseChildMenu() {
   Menu *tmp = openChildMenu;

   if (tmp) {
      tmp->CloseChildMenu();
   }
   else {
      return;
   }

   tmp = openChildMenu;
   if (tmp->parentMenu) {
      tmp->parentMenu->Deselect();
   }
   tmp->MoveFocusTo(NULL);
   tmp->ResetAnimation();
   openChildMenu = NULL;
   GetParent()->Remove(*tmp);
}


void MAS::Menu::OpenChildMenu(MenuItem *item) {
   int cx = topBar ? item->x() : item->x() + item->w();//*3/4;
   int cy = topBar ? y2() : item->y();// + item->h()/2;
   item->child->Popup(GetParent(), item, cx, cy);
   openChildMenu = item->child;
   itemsHistory.push_back( item );
   menusHistory.push_back( this );
}


void MAS::Menu::GetHistory( std::vector<MenuItem *> * & iHistory, std::vector<Menu *> * & mHistory ) {
   iHistory = &itemsHistory;
   mHistory = &menusHistory;
}


MAS::Menu *MAS::Menu::GetRootMenu() {
   if (parentMenu) {
      return ((Menu *)parentMenu->GetParent())->GetRootMenu();
   }
   else {
      return this;
   }
}


void MAS::Menu::Add(MenuItem *item) {
   Dialog::Add(*item);
   UpdateSize();
}


void MAS::Menu::Add(const char *text, Menu &child) {
   MenuItem *item = new MenuItem(text, &child);
   item->allocated = true;
   Add(item);
}


void MAS::Menu::Add(const char *text, int id) {
   MenuItem *item = new MenuItem(text, id);
   item->allocated = true;
   Add(item);
}


void MAS::Menu::Remove(int i) {
   if (i >= 0 && i < (int)widgets.size()-1) {
      MenuItem *item = (MenuItem *)widgets[i+1];
      Dialog::Remove(*widgets[i+1]);
      if (item->allocated) {
         delete item;
      }
   }
}


void MAS::Menu::RemoveByID(int ID) {
   int i = GetItemByID(ID);
   if (i >= 0) {
      Remove(i);
   }
}


MAS::MenuItem *MAS::Menu::GetItem(int i) {
   if (i >= 0 && i < (int)widgets.size()-1) {
      return (MenuItem *)widgets[i+1];
   }

   return NULL;
}


int MAS::Menu::GetItemByID(int ID) {
   int size = widgets.size();
   for (int i=1; i<size; i++) {
      if (((MenuItem *)widgets[i])->callbackID == ID) {
         return i-1;
      }
   }

   return -1;
}


void MAS::Menu::UpdateSize() {
   Dialog::UpdateSize();

   size_t i;
   if (topBar) {
      int xx = 0;
      int yy = 0;
      int ww = 0;
      int hh = skin->menuHeight;

      for (i=1; i<widgets.size(); i++) {
         MenuItem *item = (MAS::MenuItem *)(widgets[i]);
         item->Place(xx, yy);
         item->topBar = true;
         item->UpdateSize();
         xx += item->w();
         ww += item->w();
      }

      Resize(ww+2, hh+2);
      back.Resize(w(), h());
   }
   else {
      if (bitmap < 0) return;
      //Bitmap bmp = back.GetBitmap();

      int xx = 2;
      int yy = 2;
      int ww = 0;
      int hh = 0;

      for (i=1; i<widgets.size(); i++) {
         MenuItem *item = (MAS::MenuItem *)(widgets[i]);
         item->topBar = false;
         item->UpdateSize();
         if (item->w() > ww) {
            ww = item->w();
         }
      }

      for (i=1; i<widgets.size(); i++) {
         MenuItem *item = (MAS::MenuItem *)(widgets[i]);
         item->Shape(xx, yy, ww, skin->menuHeight);
         yy += item->h();
         hh += item->h();
      }

      Rect r = *Root();
      xx = x() - parent->x();
      if (xx + ww > r.x2()) {
         if (parentMenu) {
            xx = parentMenu->GetParent()->x() /*+ parentMenu->w()/4*/ - ww;
            if ( xx < 0 ) {
               if ( parentMenu->topBar )  xx = r.x2() - ww - 4;
               if ( xx < 0 )  xx = 0;
            }
         }
         else {
            xx = r.w() - ww - 4;
            if ( xx < 0 )  xx = 0;
         }
      }
      yy = y() - parent->y();
      if (yy + hh > r.y2()) {
         yy = r.h() - hh - 1;
      }
      //Shape(MAX(xx, 0), MAX(yy, 0), ww + 4, hh + 4);
      Shape(xx, yy, ww + 4, hh + 4);
      back.Resize(ww + 4, hh + 4);
   }
}


void MAS::Menu::Popup(MAS::Dialog *parentDialog, MAS::MenuItem *parentMenu, int x, int y) {
   parentDialog->Remove(*this);
   topBar = false;
   this->parentMenu = parentMenu;
   parentDialog->Add(*this);
   Place(x - parent->x(), y - parent->y());
   TakeFocusFrom(focusObject);
   UpdateSize();
   Animate();
   PlaySample(Skin::SAMPLE_OPEN);
}


void MAS::Menu::Popup(MAS::Dialog *parentDialog, MAS::MenuItem *parentMenu, const Point &p) {
   Popup(parentDialog, parentMenu, p.x(), p.y());
}


void MAS::Menu::Close() {
   ResetAnimation();
   CloseChildMenu();
   MoveFocusTo(NULL);
   if (!parentMenu) {
      GetParent()->MoveFocusTo(NULL);
   }
   if (!topBar) {
      GetParent()->Remove(*this);
   }
}


void MAS::Menu::Enable(int i)   { widgets[i+1]->ClearFlag(D_DISABLED); }
void MAS::Menu::Disable(int i)   { widgets[i+1]->SetFlag(D_DISABLED); }
void MAS::Menu::Check(int i)   { widgets[i+1]->SetFlag(D_SELECTED); }
void MAS::Menu::Uncheck(int i)   { widgets[i+1]->ClearFlag(D_SELECTED); }
bool MAS::Menu::Disabled(int i)   { return (widgets[i+1]->TestFlag(D_DISABLED)); }
bool MAS::Menu::Checked(int i)   { return (widgets[i+1]->TestFlag(D_SELECTED)); }
