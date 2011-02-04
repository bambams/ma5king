////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

//#define DEBUGMODE
#include "../include/MASkinG/debug.h"
#include "../include/MASkinG/listboxex.h"

#define MAX(x,y)     (((x) > (y)) ? (x) : (y))
#define MIN(x,y)     (((x) < (y)) ? (x) : (y))
#define CLAMP(x,y,z) MAX((x), MIN((y), (z)))

using namespace MAS;


ListBoxEx::Header::Header() : Button(), columns(NULL), mouseColumn(-1), resizingColumn(-1) {
}


ListBoxEx::Header::~Header() {
}


void ListBoxEx::Header::Draw(Bitmap &canvas) {
// remember old clipping rectangle
   Rect oldClip = canvas.GetClip();

   // start depends on the scrolling status of the parent listbox
   int x = -((ListBoxEx *)GetParent())->GetXOffset();

   // for all items:
   int index = 0;
   for (std::vector<Column>::iterator i = columns->begin(); i != columns->end(); ++i, index++) {
      // width of the item; if -1, calculate width so that all items fit in the width of the box
      int dw;
      if (i->width != -1) {
         dw = i->width;
      }
      else {
         dw = w()/columns->size();

         // make sure the last one fills the list in case there was some round off error
         if (index == (int)columns->size()-1) {
            dw = w() - dw*(columns->size()-1);
         }

         // remember for next time
         i->width = dw;
      }

      // don't bother drawing items that are scrolled out of view (on the left)
      if (x + dw < 0) {
         x += dw;
         continue;
      }

      // get the button state
      int state = Disabled() ? 2 : ((Selected() && index == mouseColumn) ? 1 : ((HasFocus() && index == mouseColumn) ? 3 : 0));

      // set clipping rectangle for this item
      Rect clip(x, 0, dw, h());
      canvas.SetClip(clip);

      // draw the item
      DrawItemBackground(canvas, &(*i), state, clip);
      DrawItemContents(canvas, &(*i), state, clip);

      // advance to the next item; quite if the next item is already out of view (on the right)
      x += dw;
      if (x >= w()) {
         break;
      }
   }

   // draw some placeholder gfx to the right
   int dw = w() - x + 16;
   if (dw > 0) {
      Rect clip(x, 0, dw, h());
      canvas.SetClip(clip);
      DrawItemBackground(canvas, NULL, 0, clip);
   }

   // restore old clipping rectangle, just in case
   canvas.SetClip(oldClip);
}


void ListBoxEx::Header::DrawItemBackground(Bitmap &canvas, Column *item, int state, const Rect &dstRect) {
   Bitmap bmp = GetBitmap();
   if (bmp) {
      bmp.TiledBlit(canvas, 0, state, dstRect.x(), dstRect.y(), dstRect.w(), dstRect.h(), 1, 4);
   }
}


void ListBoxEx::Header::DrawItemContents(Bitmap &canvas, Column *item, int state, const Rect &dstRect) {
   // get the button colors and font
   Color fg = GetFontColor(state);
   Color bg = GetShadowColor(state);
   Font fnt = GetFont(state);

   // adjust x offset according to alignment
   int xx = dstRect.x();
   switch (item->alignment) {
      case 0:      xx += 4;            break;
      case 1:      xx += dstRect.w()-4;   break;
      case 2:      xx += dstRect.w()/2;   break;
   };

   // calculate y offset so that the text will be vertically centered in the dest rectangle
   int yy = dstRect.y() + (dstRect.h() - fnt.TextHeight())/2;

   // print the text
   fnt.GUITextout(canvas, item->title.c_str(), Point(xx, yy), fg, bg, GetTextMode(), item->alignment);
}


int ListBoxEx::Header::PreferredHeight() {
   Bitmap bmp = GetParent()->GetBitmap();
   if (bmp) {
      return bmp.h()/8 + 4;
   }
   else {
      return 16;
   }
}


void ListBoxEx::Header::MsgMousemove(const Point &d) {
   Button::MsgMousemove(d);

   if (resizingColumn != -1 && TestFlag(D_PRESSED)) {
      OnResize();
   }
   else {
      OnMousemove();
   }
}


void ListBoxEx::Header::OnResize() {
   // find where the resizing column ends
   int x = -((ListBoxEx *)GetParent())->GetXOffset();
   for (int i=0; i<=resizingColumn; i++) {
      x += (*columns)[i].width;
   }

   // find where the mouse is
   Point mp = GetMousePos();

   // the new width is the difference between the position of the
   // mouse and the old right border of the item
   int w = (*columns)[resizingColumn].width + mp.x() - x - resizeOffset;

   // width can't be less than 0
   w = MAX(0, w);

   // resize the column through the parent listbox so that the
   // scrollers get updated too
   ListBoxEx *lb = ((ListBoxEx *)GetParent());
   lb->SetColumnWidth(resizingColumn, w);
}


void ListBoxEx::Header::OnMousemove() {
   // find which header the mouse is over:
   // start depends on the parent's scroll offset
   int x = -((ListBoxEx *)GetParent())->GetXOffset();

   bool resizingEnabled = ((ListBoxEx *)GetParent())->IsColumnResizingEnabled();
   bool sortingEnabled = ((ListBoxEx *)GetParent())->IsAutosortEnabled();

   // no point in doing any work if nothing is enabled
   if (!(resizingEnabled || sortingEnabled)) {
      return;
   }

   // check each item
   Point mp = GetMousePos();
   int index = 0;
   for (std::vector<Column>::iterator i = columns->begin(); i != columns->end(); ++i, index++) {
      // width of this item; if -1 calculate the width as the header with / column count
      int dw = i->width != -1 ? i->width : w() / columns->size();

      // only bother if the column is visible (i.e. width > 0)
      if (dw > 0) {
         // check if mouse is inside this item's rectangle
         if (mp <= Rect(x, 0, dw, h()-1)) {
            // check if the mouse if close to the borders; if so, switch to resizing mode
            if (mp.x() <= x+4 && HasMouse() && resizingEnabled) {
               // we can't resize the -1st column
               if (index > 0) {
                  SetCursor(Skin::MOUSE_SIZE_HORIZONTAL);
                  resizingColumn = index - 1;
                  resizeOffset = mp.x() - x;
                  mouseColumn = -1;
                  return;
               }
            }

            if (mp.x() >= x+dw-5 && HasMouse() && resizingEnabled) {
               SetCursor(Skin::MOUSE_SIZE_HORIZONTAL);
               resizingColumn = index;
               resizeOffset = mp.x() - (x+dw);
               mouseColumn = -1;
               return;
            }

            // switch back to normal cursor
            SetCursor(Skin::MOUSE_NORMAL);
            resizingColumn = -1;

            // check if the mouse switched to a different column than it was before
            if (index != mouseColumn) {
               // only allow clicking the column headers if auto sort is on
               if (sortingEnabled) {
                  // remember the index and redraw
                  mouseColumn = index;

                  PlaySample(Skin::SAMPLE_GOTFOCUS);
               }
            }

            // we found where the mouse is -> nothing else to do
            return;
         }

         // advance to the next column
         x += dw;
      }
   }

   // if we got to here, it means the mouse is not over any of the columns

   // check if it just behind the last column
   if (mp.x() <= x+4 && index > 0 && HasMouse() && resizingEnabled) {
      SetCursor(Skin::MOUSE_SIZE_HORIZONTAL);
      resizingColumn = index - 1;
   }
   else {
      SetCursor(Skin::MOUSE_NORMAL);
      resizingColumn = -1;
   }

   if (mouseColumn != -1) {
      mouseColumn = -1;
   }
}


void ListBoxEx::Header::MsgLRelease() {
   if (resizingColumn < 0 || !HasMouse()) {
      SetCursor(Skin::MOUSE_NORMAL);
   }

   Button::MsgLRelease();
}


void ListBoxEx::Header::MsgLostmouse() {
   if (!TestFlag(D_PRESSED)) {
      resizingColumn = -1;
      SetCursor(Skin::MOUSE_NORMAL);
   }

   Button::MsgLostmouse();
}


ListBoxEx::Item::Item() : parent(NULL), selected(false), sortColumn(0), sortOrder(1), userData(0) {
}


ListBoxEx::Item::Item(const ListBoxEx::Item &rhs, intptr_t data) {
   cells = rhs.cells;
   parent = rhs.parent;
   selected = rhs.selected;
   sortColumn = rhs.sortColumn;
   sortOrder = rhs.sortOrder;
   userData = data;
}


ListBoxEx::Item::~Item() {
}


ListBoxEx::Item& ListBoxEx::Item::operator=(const ListBoxEx::Item &rhs) {
   cells = rhs.cells;
   parent = rhs.parent;
   selected = rhs.selected;
   sortColumn = rhs.sortColumn;
   sortOrder = rhs.sortOrder;
   userData = rhs.userData;
   return *this;
}


bool ListBoxEx::Item::operator<(const ListBoxEx::Item &rhs) const {
   return (stricmp(GetText(sortColumn), rhs.GetText(sortColumn))*sortOrder < 0) ? true : false;
}


int ListBoxEx::Item::h() {
   int bmp_h = 0;

   Bitmap bmp = parent->GetBitmap();
   if (bmp) {
      bmp_h = bmp.h()/8;
   }
   else {
      bmp_h = 12;
   }

   int fnt_h = 0;

   if (parent) {
      Font fnt = parent->GetFont(0);
      fnt_h = fnt.TextHeight() *3 / 2;
   }

   return MAX(bmp_h, fnt_h);
}


bool ListBoxEx::Item::IsSelected() {
   return selected;
}


void ListBoxEx::Item::Select() {
   selected = true;
}


void ListBoxEx::Item::Deselect() {
   selected = false;
}


ListBoxEx::Item* ListBoxEx::GetSelectedItem() {
   return GetItem(GetFirstSelectedIndex());
}


int ListBoxEx::GetSelectedIndex() {
   return GetFirstSelectedIndex();
}


void ListBoxEx::Item::Toggle() {
   selected = !selected;
}


void ListBoxEx::Item::DrawBackground(Bitmap &canvas, int state, const Rect &dstRect) {
   Bitmap bmp = parent->GetBitmap();
   if (bmp) {
      bmp.TiledBlit(canvas, 0, state, dstRect.x(), dstRect.y(), dstRect.w(), dstRect.h(), 1, 8);
   }
}


void ListBoxEx::Item::DrawItem(Bitmap &canvas, int i, int state, const Rect &dstRect) {
   if (parent->AreGridLinesEnabled()) {
      DrawGrid(canvas, i, state, dstRect);
   }

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

   Color fg = parent->GetFontColor(s);
   Color bg = parent->GetShadowColor(s);
   Font fnt = parent->GetFont(s);
   Color tm = parent->GetTextMode();

   int xx = dstRect.x();
   int yy = dstRect.y() + (dstRect.h() - fnt.TextHeight())/2;

   // adjust x offset according to alignment
   switch (parent->columns[i].alignment) {
      case 0:      xx += 4;            break;
      case 1:      xx += dstRect.w()-4;   break;
      case 2:      xx += dstRect.w()/2;   break;
   };

   fnt.GUITextout(canvas, GetText(i), Point(xx, yy), fg, bg, tm, parent->columns[i].alignment);
}


void ListBoxEx::Item::DrawGrid(Bitmap &canvas, int i, int state, const Rect &dstRect) {
   Color col = parent->GetSkin()->c_shad2;

   int x2 = dstRect.x2()-1;
   int y2 = dstRect.y2()-1;
   for (int x = dstRect.x(); x <= x2; x += 2) {
      canvas.Putpixel(x, y2, col);
   }

   for (int y = dstRect.y(); y <= y2; y += 2) {
      canvas.Putpixel(x2, y, col);
   }
}


void ListBoxEx::Item::InsertColumn(int index) {
   if (index < 0 || index > (int)cells.size()) {
      cells.push_back(std::string(""));
   }
   else {
      std::vector<std::string>::iterator i = cells.begin();
      for (int j=0; j<index; ++j, ++i);
      cells.insert(i, std::string(""));
   }
}


void ListBoxEx::Item::DeleteColumn(int index) {
   // only if index is valid
   if (index < (int)cells.size() && index >= 0) {
      // find the indexed column
      std::vector<std::string>::iterator i = cells.begin();
      for (int j=0; j<index; ++j, ++i);

      // remove the column
      cells.erase(i);
   }
}


void ListBoxEx::Item::SetText(const char *str, int index) {
   if (index >= 0 && index < (int)cells.size()) {
      cells[index] = str ? std::string(str) : std::string("");
   }
}


const char* ListBoxEx::Item::GetText(int index) const {
   if (index >= 0 && index < (int)cells.size()) {
      return cells[index].c_str();
   }
   else {
      return NULL;
   }
}


int ListBoxEx::Item::GetColumnCount() {
   return cells.size();
}



bool ListBoxEx::InputHandler::MsgChar(int c) {
   Widget::MsgChar(c);

   switch (c>>8) {
      case ALLEGRO_KEY_DOWN:      break;
      case ALLEGRO_KEY_UP:      break;

      default:
         return false;
   };

   return true;
}


void ListBoxEx::InputHandler::MsgGotfocus() {
   Widget::MsgGotfocus();
   GetParent()->PlaySample(Skin::SAMPLE_GOTFOCUS);
}


void ListBoxEx::InputHandler::MsgLostfocus() {
   Widget::MsgLostfocus();
   GetParent()->PlaySample(Skin::SAMPLE_LOSTFOCUS);
}


ListBoxEx::ListBoxEx()
   :ScrollBox(),
   header(NULL),
   focusedItem(-1),
   selectedItem(-1),
   showHeader(true),
   multiSelect(false),
   singleClick(false),
   iterationIndex(-1),
   autoSort(false),
   columnResizing(true),
   gridLines(false)
{
   SetHeader(&ownHeader);
   Add(inputHandler);
}


ListBoxEx::~ListBoxEx() {
   columns.clear();

   if (header != &ownHeader) {
      delete header;
      header = &ownHeader;
   }

   DeleteAllItems();
}


void ListBoxEx::SetHeader(Header *newHeader) {
   bool visible = false;

   if (header) {
      visible = !header->Hidden();
      Remove(*header);
   }

   if (!newHeader) {
      newHeader = &ownHeader;
   }

   header = newHeader;
   header->columns = &columns;
   Add(*header);

   if (visible) {
      header->Unhide();
   }
   else {
      header->Hide();
   }

   SetFlag(D_RESIZED);
}


ListBoxEx::Header* ListBoxEx::GetHeader() {
   return header;
}


void ListBoxEx::MsgInitSkin() {
   ScrollBox::MsgInitSkin();

   if (GetBitmapIndex() == -1) SetBitmap(Skin::LIST);
   for (int i=0; i<4; i++) {
      if (GetFontColor(i) == Color::transparent) SetFontColor(skin->fcol[Skin::INFO_LIST][i], skin->scol[Skin::INFO_LIST][i], i);
      if (GetFontIndex(i) == -1) SetFont(skin->fnt[Skin::INFO_LIST][i], i);
   }
   if (GetTextMode() == Color::transparent) SetTextMode(Color::transparent);
   if (GetSampleIndex(Skin::SAMPLE_GOTFOCUS) == -1) SetSample(Skin::SAMPLE_GOTFOCUS, Skin::SAMPLE_GOTFOCUS);
   if (GetSampleIndex(Skin::SAMPLE_LOSTFOCUS) == -1) SetSample(Skin::SAMPLE_LOSTFOCUS, Skin::SAMPLE_LOSTFOCUS);
   if (GetSampleIndex(Skin::SAMPLE_ACTIVATE) == -1) SetSample(Skin::SAMPLE_ACTIVATE, Skin::SAMPLE_ACTIVATE);
   if (GetSampleIndex(Skin::SAMPLE_SCROLL) == -1) SetSample(Skin::SAMPLE_SCROLL, Skin::SAMPLE_SCROLL);
}


void ListBoxEx::MsgGotfocus() {
   ScrollBox::MsgGotfocus();

   if (!(header->HasMouse() || vScroller.HasMouse() || hScroller.HasMouse())) {
      GiveFocusTo(&inputHandler);
   }
}


void ListBoxEx::DrawClientArea(Bitmap &dest, int xofst, int yofst) {
   ScrollBox::DrawClientArea(dest, xofst, yofst);

   Rect oldClip = dest.GetClip();

   int yy = -yofst;

   if (!header->Hidden()) {
      yy += header->h();
   }

   // for all items
   for (size_t i=0; i<items.size(); i++) {
      // skip items that are out of view
      if (yy + items[i]->h() < 0) {
         yy += items[i]->h();
         continue;
      }

      // find out which state the item is in
      int state = 0;
      if (multiSelect) {
         bool selected = items[i]->IsSelected();

         if (inputHandler.HasFocus()) {
            if ((int)i == focusedItem) {
               if (selected) {
                  state = 4;
               }
               else {
                  state = 3;
               }
            }
            else {
               if (selected) {
                  state = 5;
               }
               else {
                  state = 2;
               }
            }
         }
         else if (Disabled()) {
            state = selected ? 7 : 6;
         }
         else {
            state = selected ? 1 : 0;
         }
      }
      else {
         if (Disabled()) {
            state = 6;
         }
         else if ((int)i == selectedItem) {
            state = inputHandler.HasFocus() ? 5 : 1;
         }
      }

      // draw background
      Rect clip(0, yy, dest.w(), items[i]->h());
      dest.SetClip(clip);
      items[i]->DrawBackground(dest, state, clip);

      // for all subitems:
      int xx = -xofst;
      for (size_t j=0; j<columns.size(); j++) {
         // width of the item; if -1, calculate width so that all items fit in the width of the box
         int dw = columns[j].width != -1 ? columns[j].width : dest.w() / columns.size();

         // don't bother drawing items that are scrolled out of view (on the left)
         if (xx + dw < 0) {
            xx += dw;
            continue;
         }

         // set clipping rectangle for this item
         Rect clip(xx, yy, dw, items[i]->h());
         dest.SetClip(clip);

         // draw the item
         items[i]->DrawItem(dest, j, state, clip);

         // advance to the next item; quite if the next item is already out of view (on the right)
         xx += dw;
         if (xx >= dest.w()) {
            break;
         }
      }

      yy += items[i]->h();
      if (yy >= dest.h()) {
         break;
      }
   }

   dest.SetClip(oldClip);
}


void ListBoxEx::UpdateSize() {
   ScrollBox::UpdateSize();

   int offx = 0;
   int offy = 0;

   Bitmap bmp = panel.GetBitmap();
   if (bmp) {
      offx = (bmp.w() - bmp.ThickX())/2;
      offy = (bmp.h() - bmp.ThickY())/2;
   }

   int scrw = 0;
   if (!vScroller.Hidden()) {
      scrw = vScroller.w();
   }

   header->Shape(offx, offy, w() - 2*offx - scrw, header->PreferredHeight());
   UpdateScrollers();

   int hh = header->Hidden() ? 0 : header->h();
   inputHandler.Shape(offx, offy + hh, header->w(), h() - 2*offx - hh - (hScroller.Hidden() ? 0 : hScroller.h()));
}


void ListBoxEx::UpdateScrollers() {
   int width = 0;
   for (std::vector<Column>::iterator i = columns.begin(); i != columns.end(); ++i) {
      width += i->width != -1 ? i->width : header->w() / columns.size();
   }

   int height = 0;

   if (!header->Hidden()) {
      height += header->h();
   }

   for (std::vector<Item *>::iterator j = items.begin(); j != items.end(); ++j) {
      height += (*j)->h();
   }

   SetScrollSize(width, height);

   vScroller.SetVerticalIncrement(MAX(1, (height - h()) / 20));
   hScroller.SetHorizontalIncrement(MAX(1, (width - w()) / 20));
}


int ListBoxEx::InsertColumn(const char *title, int index, int align, int width) {
   int ret = index;

   // create new column item
   Column col;
   col.title = title ? std::string(title) : std::string("");
   col.alignment = align;
   col.width = width;
   col.sortOrder = 1;

   // insert new item to the column array
   if (index < 0 || index > (int)columns.size()) {
      // insert at the back if index is invalid
      columns.push_back(col);
      ret = columns.size()-1;
   }
   else {
      // iterate to the indexed column and insert there
      std::vector<Column>::iterator i = columns.begin();
      for (int j=0; j<index; ++j, ++i);
      columns.insert(i, col);
   }

   // update all the existing items
   for (std::vector<Item *>::iterator iter = items.begin(); iter != items.end(); ++iter) {
      (*iter)->InsertColumn(index);
   }

   if ((columns.size() > 1 || col.title != "") && showHeader) {
      header->Unhide();
      SetFlag(D_RESIZED);
   }

   // update the scrollers
   UpdateScrollers();

   return ret;
}


void ListBoxEx::DeleteColumn(int index) {
   // only if index is valid
   if (index < (int)columns.size() && index >= 0) {
      // find the indexed column
      std::vector<Column>::iterator i = columns.begin();
      for (int j=0; j<index; ++j, ++i);

      // remove the column
      columns.erase(i);

      // update all the items
      for (std::vector<Item *>::iterator iter = items.begin(); iter != items.end(); ++iter) {
         (*iter)->DeleteColumn(index);
      }

      if ((columns.size() < 1 && std::string(GetColumnTitle(0)) == "") || !showHeader) {
         if (!header->Hidden()) {
            header->Hide();
            SetFlag(D_RESIZED);
         }
      }

      // update the scrollers
      UpdateScrollers();
   }
}


int ListBoxEx::GetColumnCount() {
   return columns.size();
}


void ListBoxEx::SetColumnTitle(int index, const char *title) {
   if (index < (int)columns.size() && index >= 0) {
      columns[index].title = std::string(title);
   }
}


const char* ListBoxEx::GetColumnTitle(int index) {
   if (index < (int)columns.size() && index >= 0) {
      return columns[index].title.c_str();
   }
   else {
      return NULL;
   }
}


void ListBoxEx::SetColumnWidth(int index, int width) {
   if (index < (int)columns.size() && index >= 0) {
      columns[index].width = width;
      UpdateSize();
   }
}


int ListBoxEx::GetColumnWidth(int index) {
   if (index < (int)columns.size() && index >= 0) {
      return columns[index].width;
   }
   else {
      return -1;
   }
}


int ListBoxEx::InsertItem(Item *item, int index) {
   item->parent = this;
   int ret = index;

   // if there are no columns yet, add one
   if (columns.empty()) {
      InsertColumn("");
   }

   // add item to the items list
   if (index < 0 || index > (int)items.size()) {
      // at the back
      items.push_back(item);
      ret = items.size()-1;
   }
   else {
      // at the requested index
      std::vector<Item *>::iterator i = items.begin();
      for (int j=0; j<index; ++j, ++i);
      items.insert(i, item);
   }

   // make sure the item has all the columns
   while (item->GetColumnCount() < (int)columns.size()) {
      item->InsertColumn();
   }

   // when inserting the first item in single select mode also select it
   if (items.size() > 0 && selectedItem < 0 && !multiSelect) {
      Select(0);
   }

   // update the scrollers
   //UpdateScrollers();
   UpdateSize();
   UpdateSize();

   return ret;
}


int ListBoxEx::InsertItem(const char *item, int index) {
   index = InsertItem(new Item(), index);
   SetItem(index, 0, item);
   return index;
}


void ListBoxEx::DeleteItem(int index) {
   if (index < (int)items.size() && index >= 0) {
      std::vector<Item *>::iterator i = items.begin();
      for (int j=0; j<index; ++j, ++i);
      delete *i;
      items.erase(i);

      if (items.size() == 0) {
         selectedItem = -1;
      }

      // update the scrollers
      //UpdateScrollers();
      UpdateSize();
      UpdateSize();
   }
}


void ListBoxEx::DeleteAllItems() {
   while (!items.empty()) {
      Item *item = items.back();
      delete item;
      items.pop_back();
   }

   selectedItem = -1;
   UpdateScrollers();
}


void ListBoxEx::SetItem(int row, int col, const char *str) {
   if (row >= 0 && row < (int)items.size()) {
      // make sure there are enough columns
      while (col >= (int)columns.size()) {
         InsertColumn("");
      }

      items[row]->SetText(str, col);
   }
}


const char* ListBoxEx::GetItem(int row, int col) {
   if (row >= 0 && row < (int)items.size()) {
      return items[row]->GetText(col);
   }
   else {
      return NULL;
   }
}


ListBoxEx::Item* ListBoxEx::GetItem(int row) {
   if (row >= 0 && row < (int)items.size()) {
      return items[row];
   }
   else {
      return NULL;
   }
}


int ListBoxEx::GetItemCount() {
   return items.size();
}


void ListBoxEx::SetHeaderVisibility(bool on) {
   showHeader = on;

   if (showHeader) {
      header->Unhide();
   }
   else {
      header->Hide();
   }

   SetFlag(D_RESIZED);
}


bool ListBoxEx::GetHeaderVisibility() {
   return showHeader;
}


void ListBoxEx::EnableMultiSelect(bool on) {
   // When turning to singleselect mode, make sure exactly one item is selected.
   if (!on && multiSelect) {
      // find how many were selected
      int selCount = GetSelectedCount();

      // if more than one is selected, only keep the first one
      if (selCount > 1) {
         int index = GetFirstSelectedIndex();
         DeselectAllItems();
         Select(index);
      }
      else if (selCount == 1) {
         Select(GetFirstSelectedIndex());
      }
      // if none were selected but there are some items in the list,
      // select the first one
      else if (items.size() > 0) {
         Select(0);
      }
   }

   // turn to the requested mode
   multiSelect = on;
}


bool ListBoxEx::IsMultiSelectEnabled() {
   return multiSelect;
}


int ListBoxEx::GetSelectedCount() {
   // in multiselect mode count the selected items
   if (multiSelect) {
      int count = 0;
      for (std::vector<Item *>::iterator i = items.begin(); i != items.end(); ++i) {
         if ((*i)->IsSelected()) {
            ++count;
         }
      }
      return count;
   }
   // in singleselect mode only one or zero items are ever selected
   else {
      if (selectedItem >= 0) {
         return 1;
      }
      else {
         return 0;
      }
   }
}


bool ListBoxEx::IsSelected(int i) {
   if (i >= 0 && i < (int)items.size()) {
      return items[i]->IsSelected();
   }
   else {
      return false;
   }
}


void ListBoxEx::Select(int i) {
   if (i >= 0 && i < (int)items.size()) {
      if (!multiSelect && selectedItem >= 0) {
         items[selectedItem]->Deselect();
      }

      selectedItem = i;
      items[selectedItem]->Select();
   }
}


void ListBoxEx::Deselect(int i) {
   if (i >= 0 && i < (int)items.size() && multiSelect) {
      items[i]->Deselect();
   }
}


void ListBoxEx::Toggle(int i) {
   if (i >= 0 && i < (int)items.size() && multiSelect) {
      items[i]->Toggle();
   }
}


void ListBoxEx::DeselectAllItems() {
   if (multiSelect) {
      int index = GetFirstSelectedIndex();

      while (index != -1) {
         items[index]->Deselect();
         index = GetNextSelectedIndex();
      }
   }
}


int ListBoxEx::GetFirstSelectedIndex() {
   iterationIndex = -1;

   if (multiSelect) {
      // search for the first selected item
      int index = 0;
      for (std::vector<Item *>::iterator i = items.begin(); i != items.end(); ++i, index++) {
         if ((*i)->IsSelected()) {
            iterationIndex = index;
            break;
         }
      }
   }
   else {
      iterationIndex = selectedItem;
   }

   return iterationIndex;
}


int ListBoxEx::GetNextSelectedIndex() {
   if (multiSelect) {
      ++iterationIndex;

      // search for the next selected item
      for (; iterationIndex < (int)items.size(); iterationIndex++) {
         if (items[iterationIndex]->IsSelected()) {
            return iterationIndex;
         }
      }

      // if we came to here, no more items are selected
      return -1;
   }
   else {
      // in single select mode only one item can be selected
      return -1;
   }
}


void ListBoxEx::EnableSingleClick(bool on) {
   singleClick = on;
}


bool ListBoxEx::IsSingleClickEnabled() {
   return singleClick;
}



void ListBoxEx::HandleEvent(Widget &obj, int msg, intptr_t arg1, intptr_t arg2) {
   ScrollBox::HandleEvent(obj, msg, arg1, arg2);

   if (obj == inputHandler) {
      switch (msg) {
         case MSG_MOUSEMOVE:      OnMousemove();      break;
         case MSG_CHAR:         OnChar(arg1);      break;
         case MSG_LPRESS:      OnLPress();         break;
         case MSG_WHEEL:         OnWheel(arg1);      break;
         case MSG_DCLICK:      OnDClick();         break;
      };
   }
   else if (obj == *header) {
      if (msg == MSG_ACTIVATE && autoSort) {
         int col = header->mouseColumn;
         if (col >= 0 && col < (int)columns.size()) {
            Sort(col, columns[col].sortOrder);
            columns[col].sortOrder *= -1;
         }
      }
   }
}


void ListBoxEx::OnMousemove() {
   int oldFocus = focusedItem;
   focusedItem = GetMouseRowIndex();

   if (focusedItem != oldFocus) {
      // in single select mode also change selection if the mouse button is being pressed down
      if ((GetMouse()->buttons & 1) && inputHandler.HasFocus() && inputHandler.HasMouse()) {
         if (multiSelect) {
            Toggle(focusedItem);
         }
         else {
            Select(focusedItem);
         }
      }
   }
}


int ListBoxEx::GetMouseRowIndex() {
   if (!HasMouse() || header->HasMouse() || vScroller.HasMouse() || hScroller.HasMouse()) {
      return -1;
   }

   Bitmap bmp = panel.GetBitmap();

   if (!bmp) {
      return -1;
   }

   int offx = (bmp.w() - bmp.ThickX())/2;
   int offy = (bmp.h() - bmp.ThickY())/2;

   int hh = header->Hidden() ? 0 : header->h();
   int y = -GetYOffset() + hh + offy;
   int ww = w() - offx - (vScroller.Hidden() ? 0 : vScroller.w());

   Point mp = GetMousePos();

   for (int i=0; i<(int)items.size(); i++) {
      int dh = items[i]->h();

      if (dh > 0) {
         if (mp <= Rect(offx, y, ww, dh)) {
            return i;
         }

         y += dh;
      }
   }

   return -1;
}


int ListBoxEx::GetMouseColumnIndex() {
   if (!HasMouse() || vScroller.HasMouse() || hScroller.HasMouse()) {
      return -1;
   }

   Bitmap bmp = panel.GetBitmap();

   if (!bmp) {
      return -1;
   }

   int offx = (bmp.w() - bmp.ThickX())/2;
   int x = -GetXOffset() + offx;
   Point mp = GetMousePos();

   for (int i=0; i<(int)columns.size(); i++) {
      int dw = columns[i].width;

      if (dw > 0) {
         if (mp <= Rect(x, 0, dw, h())) {
            return i;
         }

         x += dw;
      }
   }

   return -1;
}


void ListBoxEx::OnChar(int c) {
   // in single select mode focus should follow selected item
   if (!multiSelect) {
      if (selectedItem != -1) {
         focusedItem = selectedItem;
      }
      else {
         focusedItem = 0;
      }
   }

   // remember old focus
   int newFocus = focusedItem;

   // handle keys
   switch (c>>8) {
      case ALLEGRO_KEY_UP:      --newFocus;               break;
      case ALLEGRO_KEY_DOWN:      ++newFocus;               break;
      case ALLEGRO_KEY_PGUP:      newFocus -= 16;            break;
      case ALLEGRO_KEY_PGDN:      newFocus += 16;            break;
      case ALLEGRO_KEY_HOME:      newFocus = 0;            break;
      case ALLEGRO_KEY_END:      newFocus = items.size();   break;

      case ALLEGRO_KEY_SPACE: {
         // in multi select mode space just toggles the current item
         if (multiSelect) {
            Toggle(focusedItem);
            PlaySample(Skin::SAMPLE_SCROLL);
            GetParent()->HandleEvent(*this, MSG_SCROLL, focusedItem);
         }
         // in single select mode space also activates the current item
         else {
            PlaySample(Skin::SAMPLE_ACTIVATE);
            GetParent()->HandleEvent(*this, callbackID >= 0 ? callbackID : MSG_ACTIVATE, selectedItem);
         }

         return;
      }
      break;

      case ALLEGRO_KEY_ENTER: {
         // in multiselect mode enter toggles the current item
         if (multiSelect) {
            Toggle(focusedItem);
         }

         // regardless of mode enter activates the current item
         PlaySample(Skin::SAMPLE_ACTIVATE);
         GetParent()->HandleEvent(*this, callbackID >= 0 ? callbackID : MSG_ACTIVATE, multiSelect ? focusedItem : selectedItem);

         return;
      }
   };

   // clamp focus to valid values
   if (newFocus < 0) {
      newFocus = 0;
   }
   else if (newFocus >= (int)items.size()) {
      newFocus = items.size()-1;
   }

   // did focus change?
   if (newFocus != focusedItem) {
      focusedItem = newFocus;
      ScrollToItem(focusedItem);

      // in single select mode also select the new focus item
      if (!multiSelect) {
         Select(newFocus);
         GetParent()->HandleEvent(*this, MSG_SCROLL, newFocus);
      }

      PlaySample(Skin::SAMPLE_SCROLL);
   }
}


void ListBoxEx::OnLPress() {
   if (focusedItem >= 0 && focusedItem < GetItemCount()) {
      bool changed = false;

      if (multiSelect) {
         Toggle(focusedItem);
         changed = true;
      }
      else {
         if (focusedItem != selectedItem) {
            Select(focusedItem);
            changed = true;
         }
      }

      // in single click mode, clicking activates the current item
      if (singleClick) {
         PlaySample(Skin::SAMPLE_ACTIVATE);
         GetParent()->HandleEvent(*this, callbackID >= 0 ? callbackID : MSG_ACTIVATE, focusedItem);
      }
      // in double click mode, clicking just changes selection
      else if (changed) {
         PlaySample(Skin::SAMPLE_SCROLL);
         GetParent()->HandleEvent(*this, MSG_SCROLL, focusedItem);
      }
   }
}


void ListBoxEx::OnWheel(int z) {
   int currentOffset = GetYOffset();

   int index = 0;
   int itemOffset=0;
   for (itemOffset=0; itemOffset<currentOffset;) {
      if (index >= (int)items.size()) {
         break;
      }

      itemOffset += items[index]->h();
      ++index;
   }

   index -= z;
   if (index < 0) {
      index = 0;
   }
   else if (index >= (int)items.size()) {
      index = (int)items.size()-1;
   }

   index = CLAMP(0, index, (int)items.size()-1);
   if (index >= 0 && index < (int)items.size()) {
      int old = GetYOffset();
      ScrollToItem(index, true);

      if (GetYOffset() != old) {
         PlaySample(Skin::SAMPLE_SCROLL);
         OnMousemove();
      }
   }
}


void ListBoxEx::OnDClick() {
   // in single click mode the item has already been activated, we don't
   // need to do it twice
   if (!singleClick) {
      PlaySample(Skin::SAMPLE_ACTIVATE);
      GetParent()->HandleEvent(*this, callbackID >= 0 ? callbackID : MSG_ACTIVATE, focusedItem);
   }
}


bool ListBoxEx::ScrollToItem(int index, bool force) {
   bool ret = false;

   if (index >= 0 && index < (int)items.size()) {
      // find the current scroll offset
      int currentOffset = GetYOffset();

      // find the position of the requested item
      int itemOffset = 0;
      for (int i=0; i<index; i++) {
         itemOffset += items[i]->h();
      }

      // scroll regardless of the status if scrolling is forced
      if (force) {
         ScrollToY(itemOffset);
         ret = true;
      }
      // only if item is not already visible
      else if (itemOffset >= currentOffset + inputHandler.h() - (header->Hidden() ? 0 : header->h()) || itemOffset < currentOffset) {
         // scrolling down
         if (currentOffset < itemOffset) {
            itemOffset = itemOffset - inputHandler.h() + items[index]->h();
         }

         // scroll to the requested item
         ScrollToY(itemOffset);
         ret = true;
      }
   }

   return ret;
}


void ListBoxEx::Sort(int column, int order) {
   // check just in case
   if (column >= (int)columns.size() || column < 0) {
      return;
   }

   // make sure each item knows the what the primary column and sort order are
   for (std::vector<Item *>::iterator iter = items.begin(); iter != items.end(); ++iter) {
      (*iter)->sortColumn = column;
      (*iter)->sortOrder = order;
   }

   // actually sort the list
   std::sort(items.begin(), items.end(), Item::cmp_sort);

   // in single select mode variable selectedItem tracks the index of the
   // only selected item which might have changed after the sort so we must
   // find it again
   if (!multiSelect && selectedItem != -1) {
      for (int i=0; i<(int)items.size(); i++) {
         if (items[i]->IsSelected()) {
            Select(i);
            break;
         }
      }
   }
}


void ListBoxEx::EnableAutosort(bool on) {
   autoSort = on;
}


bool ListBoxEx::IsAutosortEnabled() {
   return autoSort;
}


void ListBoxEx::EnableColumnResizing(bool on) {
   columnResizing = on;
}


bool ListBoxEx::IsColumnResizingEnabled() {
   return columnResizing;
}


void ListBoxEx::EnableGridLines(bool on) {
   gridLines = on;
}


bool ListBoxEx::AreGridLinesEnabled() {
   return gridLines;
}
