////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

//#define DEBUGMODE
#include "../include/MASkinG/debug.h"
#include "../include/MASkinG/tabpanel.h"


void MAS::TabButton::MsgInitSkin() {
Button::MsgInitSkin();

   for (int j=0; j<4; j++) {
      SetFontColor(skin->fcol[Skin::INFO_TAB][j], skin->scol[Skin::INFO_TAB][j], j);
      SetFont(skin->fnt[Skin::INFO_TAB][j], j);
   }
   SetTextMode(Color::transparent);
   SetBitmap(Skin::TAB_BUTTON);
}


void MAS::TabButton::Draw(Bitmap &canvas) {
   Button::Draw(canvas);
}


bool MAS::TabButton::MsgWantTab() {
   Widget::MsgWantTab();
   return false;
}


MAS::TabPanel::TabPanel() : Dialog() {
   Add(box);
   selectedWidget = NULL;
   ClearFlag(D_TOPLEVEL);
}


MAS::TabPanel::~TabPanel() {
   while (!tabArray.empty()) {
      //Detach(tabArray.begin()->widget);
      delete tabArray.begin()->button;
      tabArray.erase(tabArray.begin());
   }
}


void MAS::TabPanel::MsgInitSkin() {
   Dialog::MsgInitSkin();

   box.SetBitmap(Skin::TAB_WINDOW);
}


void MAS::TabPanel::UpdateSize() {
   Dialog::UpdateSize();

   Font f = skin->GetFont(skin->fnt[Skin::INFO_TAB][0]);
   int bh = f.TextHeight() + 8;
   int x = 0;
   int y = 0;

   Bitmap boxBmp = skin->GetBitmap(Skin::TAB_WINDOW);
   int offx = (boxBmp.w() - boxBmp.ThickX())/2;
   int offy = (boxBmp.h() - boxBmp.ThickY())/2;
   box.Shape(0, bh-2, w(), h()-bh+2);

   int n = tabArray.size();
   for (int i=0; i<n; i++) {
      TabButton *b = tabArray[i].button;
      int bw = f.GUITextLength(b->GetText()) + bh;
      b->Shape(x, y, bw, bh);
      tabArray[i].widget->Shape(offx, bh + offy, w() - 2*offx, h() - 2*offy - bh);
      x += bw;
   }
}


void MAS::TabPanel::Attach(Widget *w, const char *title) {
   TabItem newItem;
   newItem.widget = w;
   newItem.button = new TabButton;
   newItem.button->SetText(title != NULL ? title : "tab");
   newItem.button->SetFlag(D_TOGGLE);
   tabArray.push_back(newItem);
   Add(*(newItem.button));
   if (!selectedWidget) {
      selectedWidget = w;
      Add(*w);
      newItem.button->Select();
   }
   w->ClearFlag(D_TOPLEVEL);
   UpdateSize();
}


void MAS::TabPanel::Detach(Widget *widget) {
   int n = tabArray.size();
   for (int i=0; i<n; i++) {
      Widget *b = tabArray[i].button;
      Widget *w = tabArray[i].widget;

      if (w == widget) {
         if (selectedWidget == w) {
            Remove(*w);
            selectedWidget = NULL;
         }
         Remove(*b);
         UpdateSize();
         break;
      }
   }
}


void MAS::TabPanel::HandleEvent(Widget &obj, int msg, intptr_t arg1, intptr_t arg2) {
   Dialog::HandleEvent(obj, msg, arg1, arg2);

   if (msg == MSG_ACTIVATE) {
      int n = tabArray.size();
      for (int i=0; i<n; i++) {
         if (tabArray[i].button == &obj) {
            OpenTab(i);
            break;
         }
      }
   }
}


void MAS::TabPanel::OpenTab(int i) {
   if (i < 0 || i >= (int)tabArray.size()) {
      return;
   }

   if (selectedWidget) {
      int j = GetOpenTab();
      if (j != -1) {
         tabArray[j].button->Deselect();
         TakeFocusFrom(tabArray[j].button);
      }
      Remove(*selectedWidget);
   }
   selectedWidget = tabArray[i].widget;
   Add(*selectedWidget);
   tabArray[i].button->Select();
   GetParent()->HandleEvent(*this, MSG_SCROLL, i);
}


void MAS::TabPanel::NextTab() {
   int sel = GetOpenTab();
   int count = GetTabCount();

   if (sel == -1) {
      if (count > 0) {
         OpenTab(0);
      }
   }
   else {
      ++sel;
      sel %= count;
      OpenTab(sel);
   }
}


void MAS::TabPanel::PreviousTab() {
   int sel = GetOpenTab();
   int count = GetTabCount();

   if (sel == -1) {
      if (count > 0) {
         OpenTab(count-1);
      }
   }
   else {
      --sel;
      if (sel < 0) {
         sel += count;
      }
      OpenTab(sel);
   }
}


int MAS::TabPanel::GetOpenTab() {
   for (size_t j=0; j<tabArray.size(); j++) {
      if (tabArray[j].widget == selectedWidget) {
         return j;
      }
   }

   return -1;
}


int MAS::TabPanel::GetTabCount() {
   return tabArray.size();
}
