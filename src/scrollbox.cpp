////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
//           ScrollBox written by Martijn "amarillion" van Iersel             //
////////////////////////////////////////////////////////////////////////////////


#include "../include/MASkinG/scrollbox.h"
using namespace MAS;

ScrollBoxHelper::ScrollBoxHelper() : parent (NULL) {
}


void ScrollBoxHelper::SetScrollBox(ScrollBox *p) {
parent = p;
}

void ScrollBoxHelper::Draw (Bitmap &canvas) {
   if (parent) parent->HelperDraw (canvas);
}


void ScrollBox::MsgInitSkin() {
   Dialog::MsgInitSkin();
   Bitmap panel = skin->GetBitmap (Skin::PANEL_SUNKEN);
   bufl = (panel.w() - panel.ThickX()) / 2;
   bufr = (panel.w() - panel.ThickX()) / 2;
   buft = (panel.h() - panel.ThickY()) / 2;
   bufb = (panel.h() - panel.ThickY()) / 2;
}

// scrolls to a certain area
// tries to make sure the area is visible
// usefull if another widget (e.g. a minimap) is used to scroll the scrollbox widget
void ScrollBox::ScrollToArea (int x, int y, int w, int h)
{
   if (x + w > GetXOffset() + GetClientWidth())
      SetPos (x + w - GetClientWidth(), GetYOffset());
   else if (x < GetXOffset())
      SetPos (x, GetYOffset());
   if (y + h > GetYOffset() + GetClientHeight())
      SetPos (GetXOffset(), y + h - GetClientHeight());
   else if (y < GetYOffset())
      SetPos (GetXOffset(), y);
}

// determine if point p is within the client area (so not on the scrollbars)
// usefull for determining on which area the mouse is.
bool ScrollBox::IsInClientArea(Point &p)
{
   return (
      p.x() >= GetClientLeft() &&
      p.x() < GetClientRight() &&
      p.y() >= GetClientTop() &&
      p.y() < GetClientBottom()
   );
}

// reset scrollers. called when
// scroll area changes in size (SetScrollSize)
// scroll options are changed
// client area changes in size
void ScrollBox::ResetScrollers() {
   hashscroller = false;
   hasvscroller = false;
   if (hopt == SCROLLER_AUTO) {
      if (scrollWidth > w() - (bufl + bufr)) {
         hashscroller = true;
      }

      if   (scrollWidth > w() - (bufl + bufr) - vScroller.w() &&
         (vopt == SCROLLER_ON ||
         (vopt == SCROLLER_AUTO && scrollHeight > h() - (buft + bufb)))) {
         hashscroller = true;
      }
   }
   else {
      hashscroller = (hopt == SCROLLER_ON);
   }

   if (vopt == SCROLLER_AUTO) {
      if (scrollHeight > h() - (buft + bufb)) {
         hasvscroller = true;
      }

      if   (scrollHeight > h() - (buft + bufb) - hScroller.h() &&
         (hopt == SCROLLER_ON ||
         (hopt == SCROLLER_AUTO && scrollWidth > w() - (bufl + bufr)))) {
         hasvscroller = true;
      }
   }
   else {
      hasvscroller = (vopt == SCROLLER_ON);
   }

   if (hashscroller) {
      hScroller.Unhide();
      hScroller.Shape (0, h() - hScroller.h(), w() - GetVScrollerSize(), hScroller.h());
      hScroller.SetRange (scrollWidth, GetClientWidth());
      hScroller.SetPosition (scrollX);
   }
   else {
      hScroller.Hide();
   }
   if (hasvscroller) {
      vScroller.Unhide();
      vScroller.Shape (w() - vScroller.w(), 0, vScroller.w(), h() - GetHScrollerSize());
      vScroller.SetRange (scrollHeight, GetClientHeight());
      vScroller.SetPosition (scrollY);
   }
   else {
      vScroller.Hide();
   }

   panel.Shape (0,0,w() - GetVScrollerSize(), h() - GetHScrollerSize());
   //helper.Shape (0,0,w() - GetVScrollerSize(), h() - GetHScrollerSize());
   helper.Shape(GetClientLeft(), GetClientTop(), GetClientWidth(), GetClientHeight());

   if (hashscroller && hasvscroller) {
      clrscr.Shape (w() - vScroller.w(), h() - hScroller.h(), vScroller.w(), hScroller.h());
      clrscr.Unhide();
   }
   else {
      clrscr.Hide();
   }
}


// sets the offset of the client and scroller position in pixels
void ScrollBox::SetPos (int x, int y) {
   if (scrollX != x || scrollY != y) {
      scrollX = x;
      scrollY = y;
      CheckPositionBounds();
   }
};


void ScrollBox::ScrollTo(int x, int y) {
   SetPos(x,y);
}

void ScrollBox::ScrollToX(int x) {
   SetPos(x,GetYOffset());
}

void ScrollBox::ScrollToY(int y) {
   SetPos(GetXOffset(),y);
}


// center view
void ScrollBox::Center () {
   SetPos (
      (scrollWidth - GetClientWidth()) / 2,
      (scrollHeight - GetClientHeight()) / 2);
}


// set options for scrollers, default = auto
void ScrollBox::SetScrollOption (int newhopt, int newvopt) {
   if (newhopt != hopt || newvopt != vopt) {
      hopt = newhopt;
      vopt = newvopt;
      ResetScrollers();
   }
}


ScrollBox::ScrollBox()  : CompoundWidget(),
   scrollWidth (0), scrollHeight (0), scrollX (0), scrollY (0),
   hopt (SCROLLER_AUTO), vopt (SCROLLER_AUTO), hashscroller (false), hasvscroller (false)
{
   helper.SetScrollBox(this);
   Add(clrscr);
   hScroller.SetOrientation(1);
   vScroller.SetOrientation(0);
   hScroller.Hide();
   vScroller.Hide();
   hScroller.SetIncrement(16);
   vScroller.SetIncrement(16);
   Add(hScroller);
   Add(vScroller);
   Add(panel);
   Add(helper);
   bufl = 0;
   bufr = 0;
   buft = 0;
   bufb = 0;
}


void ScrollBox::SetScrollSize(int width, int height) {
   if (scrollWidth != width || scrollHeight != height) {
      scrollWidth = width;
      scrollHeight = height;
      CheckPositionBounds();
      ResetScrollers ();
   }
}


// make sure the positions scrollx and scrolly are between 0 and scrollWidth / scrollHeight.
void ScrollBox::CheckPositionBounds() {
   if (scrollX >= scrollWidth - GetClientWidth())
      scrollX = scrollWidth - GetClientWidth() - 1;
   if (scrollX < 0)
      scrollX = 0;
   if (scrollY >= scrollHeight - GetClientHeight())
      scrollY = scrollHeight - GetClientHeight() - 1;
   if (scrollY < 0)
      scrollY = 0;
   hScroller.SetPosition (scrollX);
   vScroller.SetPosition (scrollY);
}


void ScrollBox::UpdateSize() {
   Dialog::UpdateSize();

   // size of parent dialog has changed.
   // see if scrollers need to be added / removed.
   CheckPositionBounds();
   ResetScrollers();
}

void ScrollBox::HandleEvent(Widget &obj, int msg, intptr_t arg1, intptr_t arg2) {
   Dialog::HandleEvent(obj, msg, arg1, arg2);
   if (msg == MSG_SCROLL) {
      if (obj == hScroller) {
         scrollX = arg1;
      }
      else if (obj == vScroller) {
         scrollY = arg1;
      }
      MsgScroll();
   }
}


// helperdraw is different from normal draw:
// normal draw is called before subwidgets
// helperdraw is called after subwidgets.
void ScrollBox::HelperDraw(Bitmap &canvas) {
   /*
   Bitmap client;
   client.CreateSubBitmap (canvas, GetClientLeft(), GetClientTop(), GetClientWidth(), GetClientHeight());
   DrawClientArea (client, scrollX, scrollY);
   */
   DrawClientArea (canvas, scrollX, scrollY);
   //client.Destroy();
}


int ScrollBox::GetHScrollerSize() {
   return hashscroller ? hScroller.h() : 0;
}


int ScrollBox::GetVScrollerSize() {
   return hasvscroller ? vScroller.w() : 0;
}


void ScrollBox::DrawClientArea (Bitmap &dest, int xofst, int yofst) {
}


int ScrollBox::GetClientBottom() { return h() - GetHScrollerSize() - bufb; }
int ScrollBox::GetClientRight() { return w() - GetVScrollerSize() - bufr; }
int ScrollBox::GetClientTop() { return buft; }
int ScrollBox::GetClientLeft() { return bufl; }
int ScrollBox::GetClientHeight() { return h() - GetHScrollerSize() - (buft + bufb); }
int ScrollBox::GetClientWidth() { return w() - GetVScrollerSize() - (bufl + bufr); }
int ScrollBox::GetXOffset() { return scrollX; }
int ScrollBox::GetYOffset() { return scrollY; }
Size ScrollBox::GetScrollSize () { return Size (scrollWidth, scrollHeight); }
int ScrollBox::GetHScrollOption() { return hopt; }
int ScrollBox::GetVScrollOption() { return vopt; }

void ScrollBox::MsgScroll() {
}
