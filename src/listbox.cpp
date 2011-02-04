////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/listbox.h"
#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
MAS::ListItem::ListItem() : parent(NULL), selected(false) {
}


MAS::ListItem::~ListItem() {
}

void MAS::ListItem::Draw(Bitmap &canvas, Bitmap &bmp, int state) {
   bmp.TiledBlit(canvas, 0, state, 0, 0, canvas.w(), canvas.h(), 1, 8);
}


int MAS::ListItem::h() {
   Bitmap bmp = parent->GetBitmap();
   if (bmp) {
      return bmp.h()/8;
   }
   else {
      return 12;
   }
}


char *MAS::ListItem::GetText() const {
   return NULL;
}


////////////////////////////////////////////////////////////////////////////////
MAS::ListItemString::ListItemString() : MAS::ListItem(), text(NULL) {
}


MAS::ListItemString::ListItemString(const char *t) : MAS::ListItem(), text(NULL) {
   SetText(t);
}


MAS::ListItemString::~ListItemString() {
   if (text) {
      delete [] text;
      text = NULL;
   }
}


void MAS::ListItemString::SetText(const char *t) {
   if (text) {
      delete [] text;
      text = NULL;
   }

   text = new char[1+strlen(t)];
   strcpy(text, t);
}


void MAS::ListItemString::Draw(Bitmap &canvas, Bitmap &bmp, int state) {
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

      f.GUITextout(canvas, text, 3, (h() - f.TextHeight())/2, fg, bg, Color::transparent, 0);
   }
}


char *MAS::ListItemString::GetText() const {
   return text;
}


////////////////////////////////////////////////////////////////////////////////
MAS::ListView::ListView()
   :Widget(),
   offset(0),
   focus(-1),
   multiSelect(false),
   singleClick(0)
{
}


MAS::ListView::~ListView() {
   while (!items.empty()) {
      delete items[0];
      items.erase(items.begin());
   }
}


void MAS::ListView::MsgInitSkin() {
   Widget::MsgInitSkin();
   if (GetBitmapIndex() == -1) SetBitmap(Skin::LIST);
   if (GetSampleIndex(Skin::SAMPLE_GOTFOCUS) == -1) SetSample(Skin::SAMPLE_GOTFOCUS, Skin::SAMPLE_GOTFOCUS);
   if (GetSampleIndex(Skin::SAMPLE_LOSTFOCUS) == -1) SetSample(Skin::SAMPLE_LOSTFOCUS, Skin::SAMPLE_LOSTFOCUS);
   if (GetSampleIndex(Skin::SAMPLE_ACTIVATE) == -1) SetSample(Skin::SAMPLE_ACTIVATE, Skin::SAMPLE_ACTIVATE);
   if (GetSampleIndex(Skin::SAMPLE_SCROLL) == -1) SetSample(Skin::SAMPLE_SCROLL, Skin::SAMPLE_SCROLL);
}


void MAS::ListView::Draw(Bitmap &canvas) {
   int i = offset;
   int y = 0;
   Bitmap bmp = GetBitmap();

   while (true) {
      if (i >= (int)(int)items.size()) {
         break;
      }

      int hh = items[i]->h();
      if (y > h()) {
         break;
      }

      // get the state of the item
      int state = 0;
      if (multiSelect) {
         if (HasFocus()) {
            if (i == focus) {
               if (items[i]->selected) {
                  state = 4;
               }
               else {
                  state = 3;
               }
            }
            else {
               if (items[i]->selected) {
                  state = 5;
               }
               else {
                  state = 2;
               }
            }
         }
         else if (Disabled()) {
            state = (items[i]->selected) ? 7 : 6;
         }
         else {
            state = items[i]->selected ? 1 : 0;
         }
      }
      else {
         if (Disabled()) {
            state = 6;
         }
         else if (i == focus) {
            state = HasFocus() ? 5 : 1;
         }
      }

      // draw the item
      Bitmap sub;
      if (y < canvas.h()) {
         sub.CreateSubBitmap(canvas, 0, y, w(), hh);
         if (sub) {
            items[i]->Draw(sub, bmp, state);
            sub.Destroy();
         }

         if (i == focus && skin->drawDots) {
            canvas.DrawDottedRect(1, y+1, w()-2, y+hh-2, skin->c_font);
         }
      }

      y += hh;
      i++;
   }
}


bool MAS::ListView::MsgWantfocus() {
   Widget::MsgWantfocus();
   return (TotalItems() > 0);
}


bool MAS::ListView::MsgChar(int c) {
   Widget::MsgChar(c);

   int newf = focus;
   switch (c>>8) {
      case ALLEGRO_KEY_UP:   --newf;               break;
      case ALLEGRO_KEY_DOWN:   ++newf;               break;
      case ALLEGRO_KEY_PGUP:   newf -= VisibleItems();   break;
      case ALLEGRO_KEY_PGDN:   newf += VisibleItems();   break;
      case ALLEGRO_KEY_HOME:   newf = 0;            break;
      case ALLEGRO_KEY_END:   newf = TotalItems()-1;   break;

      case ALLEGRO_KEY_ENTER: {
         if (focus >= 0 && focus < (int)items.size()) {
            if (multiSelect) {
               items[focus]->selected = true;
            }
            PlaySample(Skin::SAMPLE_ACTIVATE);
            GetParent()->HandleEvent(*this, MSG_ACTIVATE, focus);
         }
         break;
      }

      case ALLEGRO_KEY_SPACE: {
         if (focus >= 0 && focus < (int)items.size()) {
            if (multiSelect) {
               items[focus]->selected = !items[focus]->selected;
               PlaySample(Skin::SAMPLE_SCROLL);
               GetParent()->HandleEvent(*this, MSG_ACTIVATE, focus);
            }
            else {
               PlaySample(Skin::SAMPLE_ACTIVATE);
               GetParent()->HandleEvent(*this, MSG_ACTIVATE, focus);
            }
         }
         break;
      }

      default:
         return false;
   };

   newf = (newf < 0) ? 0 : ((newf > TotalItems()-1) ? TotalItems()-1 : newf);
   if (newf != focus) {
      SetFocus(newf);
      PlaySample(Skin::SAMPLE_SCROLL);
   }

   return true;
}


int MAS::ListView::FindMouseItem() {
   Point mp = GetMousePos();
   int i, y;
   for (i=offset, y=0; i<(int)items.size(); i++) {
      y += items[i]->h();
      if (mp.y() <= y) {
         return i;
      }
   }

   return -1;
}


void MAS::ListView::MsgLPress() {
   Widget::MsgLPress();

   int mouseItem = FindMouseItem();
   if (mouseItem != -1) {
      if (!multiSelect) {
         if (mouseItem != focus) {
            if (mouseItem != focus) {
               SetFocus(mouseItem);
               PlaySample(Skin::SAMPLE_SCROLL);
            }
         }
      }
      else {
         if (Selected(mouseItem)) {
            Deselect(mouseItem);
         }
         else {
            Select(mouseItem);
         }
         PlaySample(Skin::SAMPLE_SCROLL);
         GetParent()->HandleEvent(*this, MSG_ACTIVATE, mouseItem);
      }
   }
}


void MAS::ListView::MsgLRelease() {
   Widget::MsgLRelease();

   if (singleClick) {
      MsgDClick();
   }
}


void MAS::ListView::MsgDClick() {
   Widget::MsgDClick();
   int mouseItem = FindMouseItem();

   if (mouseItem != -1) {
      PlaySample(Skin::SAMPLE_ACTIVATE);
      GetParent()->HandleEvent(*this, MSG_ACTIVATE, mouseItem);
   }
}


void MAS::ListView::MsgMousemove(const Point &d) {
   Widget::MsgMousemove(d);
   if (multiSelect || singleClick) {
      int mouseItem = FindMouseItem();
      if (mouseItem != -1 && mouseItem != focus) {
         SetFocus(mouseItem);
         PlaySample(Skin::SAMPLE_SCROLL);
      }
   }
}


void MAS::ListView::DeselectAllItems() {
   for (std::vector<ListItem *>::iterator i = items.begin(); i != items.end(); ++i) {
      (*i)->selected = false;
   }
}


int MAS::ListView::TotalItems() {
   return (int)items.size();
}


int MAS::ListView::VisibleItems() {
   if (bitmap == -1) {
      return 0;
   }

   int i = offset;
   int y = 0;

   while (true) {
      if (i >= (int)items.size()) {
         break;
      }

      int hh = items[i]->h();
      if (y + hh > h()) {
         y += hh;
         break;
      }

      y += hh;
      i++;
   }

   if (y < h()) {
      while (true) {
         --offset;

         if (offset < 0) {
            offset = 0;
            break;
         }

         int hh = items[offset]->h();
         if (y + hh > h()) {
            ++offset;
            break;
         }

         y += hh;
      }
   }

   return i - offset;
}


bool MAS::ListView::Selected(int i) {
   if (i >= 0 && i < (int)items.size()) {
      return items[i]->selected;
   }
   return false;
}


void MAS::ListView::Select(int i) {
   if (i >= 0 && i < (int)items.size()) {
      if (!multiSelect) {
         Deselect(focus);
      }
      items[i]->selected = true;
      focus = i;
   }
}


void MAS::ListView::Deselect(int i) {
   if (i >= 0 && i < (int)items.size()) {
      items[i]->selected = false;
   }
}


int MAS::ListView::Selection() {
   if (!multiSelect) {
      return focus;
   }
   else {
      for (int i=0; i<(int)items.size(); i++) {
         if (items[i]->selected) return i;
      }
   }
   return 0;
}


void MAS::ListView::ScrollToSelection() {
   if (multiSelect) return;

   int newOffset = Selection();
   int vi = VisibleItems();

   if (newOffset < offset) {
      offset = newOffset;
   }
   else if (newOffset >= offset + vi) {
      offset = newOffset - vi + 1;
   }
}


int MAS::ListView::Focus() {
   return focus;
}


void MAS::ListView::SetFocus(int i) {
   if (!multiSelect) {
      Select(i);
   }
   else {
      focus = i;
   }

   GetParent()->HandleEvent(*this, MSG_SCROLL, focus);
}


void MAS::ListView::InsertItem(MAS::ListItem *item, int i) {
   item->parent = this;
   if (i >= (int)items.size()) {
      items.push_back(item);
   }
   else {
      std::vector<ListItem *>::iterator iter = items.begin();
      for (int j=0; j<i; j++, ++iter);
      items.insert(iter, item);
   }

   /*
   // move the focus if necessary
   if (i <= focus) SetFocus(focus+1);
   */

   // set focus to the first item if there is only one in the list
   if ((int)items.size() == 1) {
      SetFocus(0);
   }
}


MAS::ListItem *MAS::ListView::GetItem(int i) {
   if (i >= (int)items.size() || i < 0) {
      return NULL;
   }
   else {
      return items[i];
   }
}


MAS::ListItem *MAS::ListView::GetFocusedItem() {
   if (focus >= 0) {
      return GetItem(focus);
   }
   else {
      return NULL;
   }
}


MAS::ListItem *MAS::ListView::GetSelectedItem() {
   return GetItem(Selection());
}


void MAS::ListView::DeleteItem(int i) {
   MAS::ListItem *item = GetItem(i);
   if (item) {
      delete item;
      std::vector<ListItem *>::iterator iter = items.begin();
      int j;
      for (j=0; j<i; ++j, ++iter);
      items.erase(iter);
      int newFocus = focus;
      if (focus > j || focus >= (int)items.size()) {
         --newFocus;
         newFocus = newFocus < 0 ? 0 : newFocus;
         SetFocus(newFocus);
      }
      else if (items.empty()) {
         focus = -1;
      }

      if (Offset() + VisibleItems() > TotalItems() && offset > 0) {
         --offset;
      }
   }
}


void MAS::ListView::DeleteAllItems() {
   while (!items.empty()) {
      DeleteItem(0);
   }
}


int MAS::ListView::Offset() {
   return offset;
}


bool cmp_list_item(const MAS::ListItem *i1, const MAS::ListItem *i2) {
   return (stricmp(i1->GetText(), i2->GetText()) < 0) ? true : false;
}


void MAS::ListView::Sort() {
   //std::sort(items.begin(), items.end(), cmp_list_item);
   Sort(cmp_list_item);
}


void MAS::ListView::Sort(bool (*cmp)(const MAS::ListItem *, const MAS::ListItem *)) {
   std::sort(items.begin(), items.end(), cmp);
}


bool MAS::ListView::GetSingleClick() {
   return singleClick;
}


void MAS::ListView::SetSingleClick(bool single) {
   singleClick = single;
}


////////////////////////////////////////////////////////////////////////////////
MAS::ListBox::ListBox() : MAS::CompoundWidget() {
   list = new ListView();
   Add(box);
   Add(*list);
   Add(vScroller);
}


MAS::ListBox::ListBox(MAS::ListView *l) : MAS::CompoundWidget() {
   list = l;
   Add(box);
   Add(*list);
   Add(vScroller);
}


MAS::ListBox::~ListBox() {
   delete list;
}


void MAS::ListBox::MsgInitSkin() {
   Dialog::MsgInitSkin();
   if (GetBitmapIndex() == -1) SetBitmap(Skin::PANEL_SUNKEN);
   box.SetBitmap(bitmap);
   for (int i=0; i<4; i++) {
      if (GetFontColor(i) == Color::transparent) SetFontColor(skin->fcol[Skin::INFO_LIST][i], skin->scol[Skin::INFO_LIST][i], i);
      if (GetFontIndex(i) == -1) SetFont(skin->fnt[Skin::INFO_LIST][i], i);
   }
   if (GetTextMode() == Color::transparent) SetTextMode(Color::transparent);

   UpdateSize();
}


void MAS::ListBox::UpdateSize() {
   // show the scroller and shrink the box
   Bitmap bmpBack = skin->GetBitmap(Skin::SCROLL_VBACK);
   if (!bmpBack) {
      return;
   }

   box.Resize(w(), h());

   // setup the list view area
   Bitmap bmpBox = skin->GetBitmap(Skin::PANEL_SUNKEN);
   int bh = (bmpBox.w() - bmpBox.ThickX())/2;
   int bv = (bmpBox.h() - bmpBox.ThickY())/2;
   list->Shape(bh, bv, box.w() - 2*bh, box.h() - 2*bv);

   int ti = list->TotalItems();
   int vi = list->VisibleItems();

   if (ti > vi) {
      // skrink the box and the list view area a bit
      box.w(box.w() - bmpBack.w());
      list->w(list->w() - bmpBack.w());

      // setup the scroller
      vScroller.Setup(w() - bmpBack.w(), 0, bmpBack.w(), h(), 0, 0, ti, vi, 0, 0);
      ScrollToSelection();
      if (!Hidden()) {
         vScroller.Unhide();
      }
   }
   else {
      // hide the scroller
      vScroller.Hide();
   }

   Dialog::UpdateSize();
}


void MAS::ListBox::InsertItem(MAS::ListItem *item, int i) {
   list->InsertItem(item, i);
   UpdateSize();
}


MAS::ListItem *MAS::ListBox::GetItem(int i) {
   return list->GetItem(i);
}


MAS::ListItem *MAS::ListBox::GetFocusedItem() {
   return list->GetFocusedItem();
}


MAS::ListItem *MAS::ListBox::GetSelectedItem() {
   return list->GetSelectedItem();
}


void MAS::ListBox::DeleteItem(int i) {
   list->DeleteItem(i);
   UpdateSize();
}


void MAS::ListBox::DeleteAllItems() {
   list->DeleteAllItems();
   UpdateSize();
}


bool MAS::ListBox::Selected(int i) {
   return list->Selected(i);
}


void MAS::ListBox::Select(int i) {
   list->Select(i);
   ScrollToSelection();
}


void MAS::ListBox::Deselect(int i) {
   list->Deselect(i);
   ScrollToSelection();
}



int MAS::ListBox::Selection() {
   return list->Selection();
}


void MAS::ListBox::ScrollToSelection() {
   list->ScrollToSelection();
   vScroller.SetPosition(list->Offset());
}


int MAS::ListBox::Focus() {
   return list->Focus();
}


void MAS::ListBox::SetFocus(int i) {
   list->SetFocus(i);
   ScrollToSelection();
}


void MAS::ListBox::Sort() {
   list->Sort();
   ScrollToSelection();
}


void MAS::ListBox::Sort(bool (*cmp)(const MAS::ListItem *, const MAS::ListItem *)) {
   list->Sort(cmp);
   ScrollToSelection();
}


bool MAS::ListBox::GetSingleClick() {
   return list->GetSingleClick();
}


void MAS::ListBox::SetSingleClick(bool single) {
   list->SetSingleClick(single);
}


void MAS::ListBox::HandleEvent(MAS::Widget &w, int msg, intptr_t arg1, intptr_t arg2) {
   Dialog::HandleEvent(w, msg, arg1, arg2);

   if (msg == MSG_WHEEL && w == *list) {
      if (!vScroller.Hidden()) {
         vScroller.HandleEvent(*this, MSG_WHEEL, arg1);
      }
   }
   else if (msg == MSG_SCROLL) {
      if (w == *list) {
         bool doScroll = false;

         if (list->focus < list->offset) {
            list->offset = list->focus;
            doScroll = true;
         }
         else {
            int vi = list->VisibleItems();
            if (list->offset + vi <= list->focus && vi) {
               list->offset = list->focus - vi + 1;
               doScroll = true;
            }
         }

         if (doScroll) {
            //vScroller.SetPosition(list->offset);
            ScrollToSelection();
         }

         GetParent()->HandleEvent(*this, msg, list->focus);
      }

      else if (w == vScroller) {
         list->offset = (int)vScroller.GetPosition();
      }
   }
   else if (msg == MSG_ACTIVATE && w == *list) {
      GetParent()->HandleEvent(*this, callbackID >= 0 ? callbackID : MSG_ACTIVATE, arg1);
   }
}


void MAS::ListBox::SetMultiSelect(bool m) {
   list->multiSelect = m;
}


void MAS::ListBox::Dump() {
   TRACE("list contents:\n");
   for (std::vector<ListItem *>::iterator i = list->items.begin(); i != list->items.end(); ++i) {
      TRACE((*i)->GetText());
      TRACE("\n");
   }
}


int MAS::ListBox::Size() {
   return (int)list->items.size();
}


void MAS::ListBox::MsgGotfocus() {
   Dialog::MsgGotfocus();
   GiveFocusTo(list);
}


void MAS::ListBox::MsgLostfocus() {
   Dialog::MsgLostfocus();
   TakeFocusFrom(list);
}
