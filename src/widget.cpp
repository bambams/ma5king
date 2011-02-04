////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/debug.h"
#include "../include/MASkinG/widget.h"
#include "../include/MASkinG/dialog.h"
#include "../include/MASkinG/settings.h"

#ifdef SendMessage
#undef SendMessage
#endif


MAS::Widget::Widget()
:MAS::Rect(0,0,0,0),
   flags(0),
   key(0),
   callbackID(-1),
   bitmap(-1),
   parent(NULL),
   animator(NULL),
   animLength(1),
   animType(Animator::NONE),
   tooltipText(NULL)
{
   skin = NULL;
   SetSkin(MAS::theSkin);

   SetAnimator(new Animator);

   normalizedRect = Rect(-1,-1,-1,-1);
}


MAS::Widget::~Widget() {
   parent = NULL;
   skin = NULL;
   if (animator) {
      delete animator;
      animator = NULL;
   }
}


void MAS::Widget::x(int i, bool normalized) {
   if (normalized) {
      normalizedRect.x(i);
   }
   else {
      int realPos = i < 0 ? GetParent()->w() + i : i;
      relativePos.x(realPos);
      if (parent && parent != this) {
         Point p = parent->GetOffset();
         Rect::x(realPos + p.x());
      }
      else {
         Rect::x(realPos);
      }
      //normalizedRect.x(-1);
   }

   SetFlag(D_MOVED);
}


void MAS::Widget::y(int i, bool normalized) {
   if (normalized) {
      normalizedRect.y(i);
   }
   else {
      int realPos = i < 0 ? GetParent()->h() + i : i;
      relativePos.y(realPos);
      if (parent && parent != this) {
         Point p = parent->GetOffset();
         Rect::y(realPos + p.y());
      }
      else {
         Rect::y(realPos);
      }
      //normalizedRect.y(-1);
   }

   SetFlag(D_MOVED);
}


void MAS::Widget::w(int i, bool normalized) {
   if (normalized) {
      normalizedRect.w(i);
   }
   else {
      Rect::w(i);
      //normalizedRect.w(-1);
   }

   SetFlag(D_RESIZED);
}


void MAS::Widget::h(int i, bool normalized) {
   if (normalized) {
      normalizedRect.h(i);
   }
   else {
      Rect::h(i);
      //normalizedRect.h(-1);
   }

   SetFlag(D_RESIZED);
}


void MAS::Widget::UpdatePosition() {
   if (parent && parent != this) {
      Point p = parent->GetOffset();
      Rect::x(relativePos.x() + p.x());
      Rect::y(relativePos.y() + p.y());
   }
}


int MAS::Widget::SendMessage(int msg, intptr_t c) {
   int ret = D_O_K;

   // convert the Allegro message constant to a MASkinG message handler
   switch (msg) {
      case (MSG_START):   MsgStart();   Animate();   break;
      case (MSG_END):      MsgEnd();      break;
      case (MSG_DRAW):   MsgDraw();      break;
      case (MSG_CLICK):   MsgClick();      break;
      case (MSG_DCLICK):   MsgDClick();   break;
      case (MSG_KEY):      MsgKey();      break;
      case (MSG_CHAR):   if (MsgChar(c))      ret |= D_USED_CHAR;      break;
      case (MSG_UCHAR):   if (MsgUChar(c))   ret |= D_USED_CHAR;      break;
      case (MSG_XCHAR):   if (MsgXChar(c))   ret |= D_USED_CHAR;      break;
      case (MSG_WANTFOCUS):   if (MsgWantfocus())   ret |= D_WANTFOCUS;   break;
      case (MSG_GOTFOCUS):   MsgGotfocus();      break;
      case (MSG_LOSTFOCUS):   MsgLostfocus();      break;
      case (MSG_GOTMOUSE):   MsgGotmouse();      break;
      case (MSG_LOSTMOUSE):   MsgLostmouse();      break;
      case (MSG_RADIO):      MsgRadio(c);      break;
      case (MSG_WHEEL):      MsgWheel(c);      break;
      case (MSG_LPRESS):      MsgLPress();      break;
      case (MSG_MPRESS):      MsgMPress();      break;
      case (MSG_RPRESS):      MsgRPress();      break;
      case (MSG_LRELEASE):   MsgLRelease();      break;
      case (MSG_MRELEASE):   MsgMRelease();      break;
      case (MSG_RRELEASE):   MsgRRelease();      break;
      case (MSG_MOVE):      MsgMove();         break;
      case (MSG_RESIZE):      MsgResize();      break;
      case (MSG_SHAPE):      MsgShape();         break;
      case (MSG_TIMER):      MsgTimer((ALLEGRO_TIMER_EVENT *)c); break;
      case (MSG_TICK):      MsgTick();         break;
      case (MSG_INITSKIN):   MsgInitSkin();      break;
      case (MSG_CLOSE):      if (MsgClose()) ret |= D_CLOSE;      break;
      case (MSG_WANTMOUSE):   if (MsgWantmouse())   ret |= D_WANTFOCUS;   break;
      case (MSG_WANTTAB):      if (MsgWantTab())   ret |= D_WANTFOCUS;   break;
   };

   return ret;
}


void MAS::Widget::MsgStart()      {
   UpdatePosition();
   GetParent()->HandleEvent(*this, MSG_START);
}

void MAS::Widget::MsgEnd()         {
   GetParent()->HandleEvent(*this, MSG_END);
   ClearFlag(D_GOTFOCUS | D_GOTMOUSE | D_CHANGEDSKIN | D_MOVED | D_RESIZED);
   ResetAnimation();
}


void MAS::Widget::MsgDraw() {
   Bitmap canvas = GetCanvas();

   if (canvas) {
      if ((flags & D_ANIMATING) && !animator->Animating()) {
         animator->Create(canvas, animLength);
         Bitmap aBuffer = animator->GetBuffer();
         if (aBuffer) {
            Draw(aBuffer);
            animator->Draw(canvas, animType);
         }
         ClearFlag(D_ANIMATING);
      }
      else if (!(flags & D_ANIMATING) && animator->Animating()) {
         animator->Draw(canvas, animType);
      }
      else {
         Draw(canvas);
      }

      ReleaseCanvas(canvas);
   }

   GetParent()->HandleEvent(*this, MSG_DRAW);
}


void MAS::Widget::Draw(Bitmap &canvas) {
}

void MAS::Widget::MsgClick()      { GetParent()->HandleEvent(*this, MSG_CLICK); }
void MAS::Widget::MsgDClick()      { GetParent()->HandleEvent(*this, MSG_DCLICK); }
void MAS::Widget::MsgKey()          { GetParent()->HandleEvent(*this, MSG_KEY); }
bool MAS::Widget::MsgChar(int c)   { GetParent()->HandleEvent(*this, MSG_CHAR, c);   return false; }
bool MAS::Widget::MsgUChar(int c)   { GetParent()->HandleEvent(*this, MSG_UCHAR, c);   return false; }
bool MAS::Widget::MsgXChar(int xc)   { GetParent()->HandleEvent(*this, MSG_XCHAR, xc);   return false; }
bool MAS::Widget::MsgWantfocus()   { GetParent()->HandleEvent(*this, MSG_WANTFOCUS);   return false; }
bool MAS::Widget::MsgWantmouse()   { GetParent()->HandleEvent(*this, MSG_WANTMOUSE);   return true; }

void MAS::Widget::MsgGotfocus()      {
   SetFlag(D_GOTFOCUS);
   GetParent()->HandleEvent(*this, MSG_GOTFOCUS);
   PlaySample(Skin::SAMPLE_GOTFOCUS);
}

void MAS::Widget::MsgLostfocus()   {
   ClearFlag(D_GOTFOCUS);
   GetParent()->HandleEvent(*this, MSG_LOSTFOCUS);
   PlaySample(Skin::SAMPLE_LOSTFOCUS);
}

void MAS::Widget::MsgGotmouse()      { SetFlag(D_GOTMOUSE); GetParent()->HandleEvent(*this, MSG_GOTMOUSE); }
void MAS::Widget::MsgLostmouse()   { ClearFlag(D_GOTMOUSE); GetParent()->HandleEvent(*this, MSG_LOSTMOUSE); }
void MAS::Widget::MsgRadio(int r)   { GetParent()->HandleEvent(*this, MSG_RADIO, r); }
void MAS::Widget::MsgWheel(int wh)   { GetParent()->HandleEvent(*this, MSG_WHEEL, wh); }
void MAS::Widget::MsgLPress()      { GetParent()->HandleEvent(*this, MSG_LPRESS); }
void MAS::Widget::MsgMPress()      { GetParent()->HandleEvent(*this, MSG_MPRESS); }
void MAS::Widget::MsgRPress()      { GetParent()->HandleEvent(*this, MSG_RPRESS); }
void MAS::Widget::MsgLRelease()      { GetParent()->HandleEvent(*this, MSG_LRELEASE); }
void MAS::Widget::MsgMRelease()      { GetParent()->HandleEvent(*this, MSG_MRELEASE); }
void MAS::Widget::MsgRRelease()      { GetParent()->HandleEvent(*this, MSG_RRELEASE); }
void MAS::Widget::MsgTimer(ALLEGRO_TIMER_EVENT *t)   { GetParent()->HandleEvent(*this, MSG_TIMER, (intptr_t)t); }

void MAS::Widget::MsgTick() {
   if (animator->Animating()) {
      if (!animator->Update()) {
         StopAnimating();
      }
   }
}

void MAS::Widget::MsgInitSkin() {
   GetParent()->HandleEvent(*this, MSG_INITSKIN);
   ClearFlag(D_CHANGEDSKIN);
}

void MAS::Widget::MsgMove() {
   GetParent()->HandleEvent(*this, MSG_MOVE);
   ClearFlag(D_MOVED);

   if (normalizedRect.x() != -1) {
      x(GetParent()->w()*normalizedRect.x()/100);
   }

   if (normalizedRect.y() != -1) {
      y(GetParent()->h()*normalizedRect.y()/100);
   }
}

void MAS::Widget::MsgResize() {
   GetParent()->HandleEvent(*this, MSG_RESIZE);
   ClearFlag(D_RESIZED);

   if (normalizedRect.w() != -1) {
      w(GetParent()->w()*normalizedRect.w()/100);
   }

   if (normalizedRect.h() != -1) {
      h(GetParent()->h()*normalizedRect.h()/100);
   }
}

void MAS::Widget::MsgShape() {
   GetParent()->HandleEvent(*this, MSG_SHAPE);
   ClearFlag(D_MOVED | D_RESIZED);

   if (normalizedRect.x() != -1) {
      x(GetParent()->w()*normalizedRect.x()/100);
   }

   if (normalizedRect.y() != -1) {
      y(GetParent()->h()*normalizedRect.y()/100);
   }

   if (normalizedRect.w() != -1) {
      w(GetParent()->w()*normalizedRect.w()/100);
   }

   if (normalizedRect.h() != -1) {
      h(GetParent()->h()*normalizedRect.h()/100);
   }
}

void MAS::Widget::MsgMousemove(const Point& d) {
   GetParent()->HandleEvent(*this, MSG_MOUSEMOVE, d.x(), d.y());
}
bool MAS::Widget::MsgClose() {      return true;   }
bool MAS::Widget::MsgWantTab() {   return MsgWantfocus();   }


void MAS::Widget::Setup(int x, int y, int w, int h, int key, int flags) {
   Shape(x, y, w, h);
   this->key = key;
   this->flags |= flags;
}


ALLEGRO_BITMAP *MAS::Widget::GetCanvas() {
   return parent->GetCanvas(this);
}


void MAS::Widget::ReleaseCanvas(ALLEGRO_BITMAP *bmp) {
   if (bmp && al_is_sub_bitmap(bmp)) al_destroy_bitmap(bmp);
}


void MAS::Widget::SetSkin(MAS::Skin *skin) {
   int i;
   for (i=0; i<4; i++) {
      fontInfo.fntCol[i] = Color::transparent;
      fontInfo.shdCol[i] = Color::transparent;
      fontInfo.fnt[i] = -1;
   }
   fontInfo.textMode = Color::transparent;

   for (i=0; i<7; i++) {
      sample[i] = -1;
   }

   SetFlag(D_CHANGEDSKIN);
   if (skin) {
      this->skin = skin;
   }
   else {
      this->skin = MAS::theSkin;
   }
}


MAS::Skin *MAS::Widget::GetSkin() {
   return skin;
}


MAS::Point MAS::Widget::GetMousePos() const {
   return parent->GetMouse()->pos - topLeft();
}


void MAS::Widget::SetCursor(int i) {
   if (parent) {
      parent->SetMouseCursor(i);
   }
}


void MAS::Widget::PlaySample(int i) const {
   if (MAS::Settings::guiSound && sample[i] >= 0) {
      skin->PlaySample(sample[i]);
   }
}


void MAS::Widget::SetAnimator(Animator *a) {
   if (!a) {
      return;
   }

   if (animator) {
      delete animator;
   }

   animator = a;
}


void MAS::Widget::SetAnimationProperties(int length, int type) {
   animLength = length;
   animType = type;
}


void MAS::Widget::Animate() {
   StopAnimating();
   if (animType != Animator::NONE) {
      SetFlag(D_ANIMATING);
   }
}


void MAS::Widget::Animate(int type) {
   animType = type;
   Animate();
}


void MAS::Widget::StopAnimating() {
   ClearFlag(D_ANIMATING);
}


void MAS::Widget::ResetAnimation() {
   ClearFlag(D_ANIMATING);
   animator->Reset();
}


void MAS::Widget::SetTooltipText(const char *text) {
   if (tooltipText) {
      delete [] tooltipText;
      tooltipText = NULL;
   }

   if (text) {
      tooltipText = new char[1+strlen(text)];
      strcpy(tooltipText, text);
   }
}


const char *MAS::Widget::GetTooltipText() {
   if (tooltipText) {
      return tooltipText;
   }
   else if (parent != this) {
      return parent->GetTooltipText();
   }

   return NULL;
}


int MAS::Widget::x()   const { return Rect::x(); }
int MAS::Widget::y()   const { return Rect::y(); }
int MAS::Widget::x2()   const { return Rect::x2(); }
int MAS::Widget::y2()   const { return Rect::y2(); }
int MAS::Widget::w()   const { return Rect::w(); }
int MAS::Widget::h()   const { return Rect::h(); }

MAS::Point MAS::Widget::RelativePos() const { return relativePos; }
int MAS::Widget::Key()      const { return key; };
int MAS::Widget::Flags()      const { return flags; };
bool MAS::Widget::TestFlag(int i)   const { return ((flags & i) == i); }
bool MAS::Widget::HasFocus() { return ((flags & D_GOTFOCUS) ? true : false); }
bool MAS::Widget::HasMouse() { return ((flags & D_GOTMOUSE) ? true : false); }
bool MAS::Widget::Hidden()   const { return ((flags & D_HIDDEN) ? true : false); }
bool MAS::Widget::WillExit() const { return ((flags & D_EXIT) ? true : false); }
bool MAS::Widget::Selected() const { return ((flags & D_SELECTED) ? true : false); }
bool MAS::Widget::Disabled() const { return ((flags & D_DISABLED) ? true : false); }
bool MAS::Widget::HasChildren() const { return ((flags & D_HASCHILDREN) ? true : false); }
bool MAS::Widget::IsWindow() const { return ((flags & D_WINDOW) ? true : false); }
MAS::Dialog *MAS::Widget::GetParent() const { return parent; }

bool MAS::Widget::IsChildOf(MAS::Widget *w) const {
   if (parent == w) {
      return true;
   }
   else if (parent != this) {
      return parent->IsChildOf(w);
   }
   else {
      return false;
   }
}


void MAS::Widget::Key(int i)       { key = i; }
void MAS::Widget::SetFlag(int i)   { flags |= i; }
void MAS::Widget::SetFlags(int f)   { flags = f; }
void MAS::Widget::ClearFlag(int i)   { flags &= ~i; }
void MAS::Widget::Enable()         { flags &= ~D_DISABLED; }
void MAS::Widget::Disable()         { flags |= D_DISABLED; }

void MAS::Widget::Hide() {
   flags |= D_HIDDEN;
}

void MAS::Widget::Unhide()         { flags &= ~D_HIDDEN; }
void MAS::Widget::MakeExit()      { flags |= D_EXIT; }
void MAS::Widget::DontExit()      { flags &= ~D_EXIT; }
void MAS::Widget::Place(int nx, int ny, bool normalized)   { x(nx,normalized); y(ny,normalized); }
void MAS::Widget::Place(const Point& p, bool normalized) { x(p.x(),normalized); y(p.y(),normalized); }
void MAS::Widget::Resize(int nw, int nh, bool normalized) { w(nw,normalized); h(nh,normalized); }
void MAS::Widget::Resize(const Size& s, bool normalized) { w(s.w(),normalized); h(s.h(),normalized); }
void MAS::Widget::Shape(int nx, int ny, int nw, int nh, bool normalized) { Resize(nw, nh, normalized); Place(nx, ny, normalized); }
void MAS::Widget::Shape(const Point& p, const Size& s, bool normalized) { Resize(s, normalized); Place(p, normalized); }
void MAS::Widget::Shape(const Rect &r, bool normalized) { Shape(r.x(), r.y(), r.w(), r.h(), normalized); }
void MAS::Widget::Select()         { flags |= D_SELECTED; }
void MAS::Widget::Deselect()      { flags &= ~D_SELECTED; }
void MAS::Widget::SetParent(Dialog *f) { parent = f; }

bool MAS::Widget::operator==(const Widget& obj) const { return (this == &obj); }
bool MAS::Widget::operator!=(const Widget& obj) const { return (this != &obj); }

void MAS::Widget::SetTextMode(Color m)   { fontInfo.textMode = m;   }
void MAS::Widget::SetFontColor(Color col, Color shd, int type) { fontInfo.fntCol[type] = col; fontInfo.shdCol[type] = shd; }
void MAS::Widget::SetFont(int f, int type) { fontInfo.fnt[type] = f; }
MAS::Font& MAS::Widget::GetFont(int type) { return skin->GetFont(fontInfo.fnt[type] >= 0 ? fontInfo.fnt[type] : 0); }
MAS::Color& MAS::Widget::GetTextMode() { return fontInfo.textMode; }
MAS::Color& MAS::Widget::GetFontColor(int type) { return fontInfo.fntCol[type]; }
MAS::Color& MAS::Widget::GetShadowColor(int type) { return fontInfo.shdCol[type]; }
int MAS::Widget::GetFontIndex(int type) { return fontInfo.fnt[type]; }
void MAS::Widget::SetBitmap(int i) { bitmap = i; }
int MAS::Widget::GetBitmapIndex() const { return bitmap; }
MAS::Bitmap& MAS::Widget::GetBitmap() const { return skin->GetBitmap(bitmap); }
void MAS::Widget::SetSample(int i, int event) { sample[event] = i; }
int MAS::Widget::GetSampleIndex(int event) const { return sample[event]; }
MAS::Sample& MAS::Widget::GetSample(int event) const { return skin->GetSample(sample[event]); }


void MAS::Widget::SetCallbackID(int ID) {
   callbackID = ID;
}


int MAS::Widget::GetCallbackID() {
   return callbackID;
}
