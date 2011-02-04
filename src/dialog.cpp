////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG.h"

#include "../include/MASkinG/dialog.h"
#include "../include/MASkinG/settings.h"
#include <cstdlib>
#include <algorithm>
#include <list>

#define MAX(x,y)     (((x) > (y)) ? (x) : (y))

#ifdef SendMessage
#undef SendMessage
#endif


MAS::MouseState MAS::Dialog::mouseState;
MAS::Mouse *MAS::Dialog::mouse = NULL;
MAS::Widget *previousFocusObject = NULL;
bool lockFocus = false;


MAS::Dialog::Dialog()
   :MAS::Widget(),
   running(false),
   focusObject(NULL),
   lastFocusObject(NULL),
   mouseObject(NULL),
   joyOn(false),
   escapeExits(true),
   arrowKeysMoveFocus(true),
   tabKeyMovesFocus(true),
   close(false),
   action(NONE)
{
   display = al_get_current_display();
   int dw = al_get_display_width(display);
   int dh = al_get_display_height(display);
   Shape(0, 0, dw, dh);
   widgets.reserve(50);
   SetFlag(D_HASCHILDREN | D_TOPLEVEL);
   Widget::SetParent(this);
   minSize = MAS::Size(20, 8);
   tooltipObject = &defaultTooltipObject;
   tooltipObject->Hide();
}


//TODO implement popups
#if 0
class DummyBack : public MAS::Widget {
   private:
      MAS::Bitmap back;

   public:
      void MsgStart() {
         Widget::MsgStart();
         back.Create(SCREEN_W, SCREEN_H);
         blit(screen, back, 0, 0, 0, 0, back.w(), back.h());
         Shape(0,0,SCREEN_W,SCREEN_H);
      }

      void MsgEnd() {
         Widget::MsgEnd();
         back.Destroy();
      }

      void Draw(MAS::Bitmap &canvas) {
         back.Blit(canvas, 0, 0, 0, 0, back.w(), back.h());
      }
};

class DummyDlg : public MAS::Dialog {
   public:
      MAS::Widget *ret;
      MAS::Dialog *dlg;
      int xx, yy;
      MAS::Widget *f;

   private:
      DummyBack back;

   public:
      DummyDlg() : MAS::Dialog(), ret(NULL), dlg(NULL), xx(0), yy(0), f(NULL) {
         Add(back);
      }

      void MsgStart() {
         Dialog::MsgStart();
         ret = dlg->Popup(this, xx, yy, f);
         Close();
      }
};

MAS::Widget *MAS::Dialog::Popup(MAS::Dialog *p, int xx, int yy, MAS::Widget *f) {
   SetCursor(Skin::MOUSE_NORMAL);
   SetFlag(D_WINDOW);
   if (!p) {
      DummyDlg *dlg = new DummyDlg;
      dlg->dlg = this;
      dlg->xx = xx;
      dlg->yy = yy;
      dlg->f = f;
      dlg->Execute();
      Widget *ret = dlg->ret;
      delete dlg;
      SetParent(this);
      return ret;
   }
   else {
      Dialog *r = p->Root();
      Widget *previousFocusObject = r != p ? p : r->GetFocusObject();
      Widget *previousMouseObject = r != p ? p : r->GetMouseObject();
      r->HandleKeyboardInput();
      SetParent(r);
      if (xx != INT_MAX) x(xx);
      if (yy != INT_MAX) y(yy);
      Widget *ret = f;
      close = false;
      int oldf = Flags();
      SetFlag(D_MODAL);
      Add(*tooltipObject);
      r->Add(*this);
      if (f) {
         MoveMouse(GetMouseObject(), f);
         MoveFocus(GetFocusObject(), f);
      }
      else {
         r->MsgMousemove(Point(0,0));
      }
      while (!close) r->MsgIdle();
      ret = GetFocusObject();
      Remove(*tooltipObject);
      r->Remove(*this);
      SetFlag(oldf);
      if (previousMouseObject) {
         r->MoveMouse(r->GetMouseObject(), previousMouseObject);
      }
      else {
         r->mouseObject = NULL;
      }
      if (previousFocusObject) {
         r->MoveFocusTo(previousFocusObject);
      }
      else {
         r->focusObject = NULL;
      }
      r->MsgMousemove(Point(0,0));
      return ret;
   }
}


MAS::Widget *MAS::Dialog::Popup(MAS::Dialog *p, const MAS::Point &pos, MAS::Widget *f) {
   return Popup(p, pos.x(), pos.y(), f);
}
#endif


void MAS::Dialog::Centre() {
// FIXEM | Review me | Removed this then A49 API changed.
//   ALLEGRO_DISPLAY *old_d = al_get_current_display();
//   al_set_current_display(display);
   int dw = al_get_display_width(display);
   int dh = al_get_display_width(display);
//   if (old_d != display)
//      al_set_current_display(old_d);

   MAS::Rect r = (parent != this && parent) ? *parent : MAS::Rect(0,0,dw,dh);
   x(r.x() + (r.w() - w())/2);
   y(r.y() + (r.h() - h())/2);
}


bool MAS::Dialog::AlreadyAdded(const Widget &w) {
   std::vector<Widget *>::iterator i;
   for (i = widgets.begin(); i != widgets.end(); ++i) {
      if (*i == &w) {
         return true;
      }
   }

   return false;
}


bool MAS::Dialog::Add(Widget &w) {
   if (AlreadyAdded(w)) {
      return false;
   }

   // add the object to the list
   widgets.push_back(&w);

   // let the object know of its parent dialog
   w.SetParent(this);

   // send the object a start message if it is being added
   // while the dialog is already running
   if (running) {
      // make sure the widget has a skin
      if (!w.skin) {
         w.SetSkin(skin);
      }

      w.MsgInitSkin();
      w.MsgStart();
   }

   if (w.TestFlag(D_MODAL)) {
      MoveMouse(mouseObject, &w);
      MoveFocus(focusObject, &w);
   }

   return true;
}


void MAS::Dialog::Remove(Widget &w, bool del) {
   // look for the object
   std::vector<Widget *>::iterator i;
   for (i=widgets.begin(); i != widgets.end(); ++i) {
      if (*i == &w) {
         if (del) {
            deletedWidgets.push_back(&w);
         }
         else {
            if (running) {
               w.MsgEnd();
            }

            // take mouse and focus away from the focus object if necessary
            TakeMouseFrom(*i);
            TakeFocusFrom(*i);

            // remove the object
            widgets.erase(i);
         }

         break;
      }
   }
}


void MAS::Dialog::BringToTop(MAS::Widget &w) {
   if (parent != this) {
      parent->BringToTop(*this);
   }

   std::vector<MAS::Widget *>::iterator i;
   for (i = widgets.begin(); i != widgets.end(); ++i) {
      if (*i == &w) {
         break;
      }
   }

   // Windows are moved all the way to the top, other widgets are
   // moved only to the first window.
   if (w.IsWindow()) {
      widgets.erase(i);
      widgets.push_back(&w);
   }
   else {
      i = widgets.erase(i);
      for (; i != widgets.end(); ++i) {
         if ((*i)->IsWindow()) {
            break;
         }
      }
      widgets.insert(i, &w);
   }
}


// sends out a message to all the widgets in the dialog
void MAS::Dialog::DialogMessage(int msg, intptr_t arg) {
   int force = (
      (msg == MSG_TICK) ||
      (msg == MSG_TIMER) ||
      (msg == MSG_START) ||
      (msg == MSG_END) ||
      (msg == MSG_INITSKIN)
   );

   std::vector<MAS::Widget *> lwidgets = widgets;
   
   std::vector<MAS::Widget *>::iterator i;
   for (i=lwidgets.begin(); i != lwidgets.end(); ++i) {
      if (force || !(*i)->Hidden()) {
         (*i)->SendMessage(msg, arg);
      }
   }
}


void MAS::Dialog::DoRedraw() {
   std::vector<Widget *>::iterator i;

   for (i = widgets.begin(); i != widgets.end(); ++i) {
      MAS::Widget *w = *i;
      if (!w->Hidden()) {
         w->MsgDraw();
      }
   }

   if (parent == this) {
      Bitmap canvas = GetCanvas(this);
      mouse->Show(canvas);
      ReleaseCanvas(canvas);
   }
}


void MAS::Dialog::CheckGeometry() {
   // check if any widget needs to be redrawn
   std::vector<Widget *>::iterator i;
   MAS::Widget *w;
   int f = Flags();
   if ((f & D_MOVED) && (f & D_RESIZED)) {
      MsgShape();
      ClearFlag(D_MOVED | D_RESIZED);
   }
   else if (f & D_MOVED) {
      MsgMove();
      ClearFlag(D_MOVED);
   }
   else if (f & D_RESIZED) {
      MsgResize();
      ClearFlag(D_RESIZED);
   }

   for (i=widgets.begin(); i != widgets.end(); ++i) {
      w = *i;
      f = w->Flags();
      if ((f & D_MOVED) && (f & D_RESIZED)) {
         w->MsgShape();
         w->ClearFlag(D_MOVED | D_RESIZED);
      }
      else if (f & D_MOVED) {
         w->MsgMove();
         w->ClearFlag(D_MOVED);
      }
      else if (f & D_RESIZED) {
         w->MsgResize();
         w->ClearFlag(D_RESIZED);
      }

      if (w->HasChildren()) {
         ((Dialog *)w)->CheckGeometry();
      }
   }
}


void MAS::Dialog::CheckForSkinChange() {
   MAS::Widget *w;

   if (Flags() & D_CHANGEDSKIN) {
      MsgInitSkin();
   }
   else {
      // check if any widget's skin has changed
      for (std::vector<Widget *>::iterator i = widgets.begin(); i != widgets.end(); ++i) {
         w = *i;
         if (w->TestFlag(D_CHANGEDSKIN)) {
            w->MsgInitSkin();
         }

         if (w->HasChildren()) {
            ((Dialog *)w)->CheckForSkinChange();
         }
      }
   }
}


void MAS::Dialog::CheckFocusLock() {
   MAS::Widget *w;

   // check if any widget locks focus
   for (std::vector<Widget *>::iterator i = widgets.begin(); i != widgets.end(); ++i) {
      w = *i;
      if (w->TestFlag(D_PRESSED)) {
         lockFocus = true;
         return;
      }

      if (w->HasChildren()) {
         ((Dialog *)w)->CheckFocusLock();
      }
   }
}


void MAS::Dialog::SetSkin(MAS::Skin *skin) {
   Widget::SetSkin(skin);

   MAS::Widget *w;
   std::vector<Widget *>::iterator i;
   for (i=widgets.begin(); i != widgets.end(); ++i) {
      w = *i;
      w->SetSkin(this->skin);

      if (w->HasChildren()) {
         ((Dialog *)w)->CheckForSkinChange();
      }
   }
}


void MAS::Dialog::MsgStart() {
   Widget::MsgStart();
   running = true;

   if (parent == this) {
      Add(*tooltipObject);

      SetSkin(skin);
      CheckForSkinChange();

      if (!mouse) {
         mouse = new MAS::Mouse;
      }

      SetCursor(MAS::Skin::MOUSE_NORMAL);

      // find the object with the mouse and give it the focus or whatever
      if (MAS::Settings::showMouse) {
         mouseState.doubleClickState = 0;
         mouseState.doubleClickDelay = MAS::Settings::logicFrameRate * MAS::Settings::doubleClickDelay / 1000;
         mouseState.doubleClickTick = 0;

         mouseObject = FindMouseObject();
         if (mouseObject) {
            mouseObject->MsgGotmouse();
         }
         if (!focusObject) {
            GiveFocusTo(mouseObject);
         }
      }
   }

   // send out a MSG_START to all child widgets
   DialogMessage(MSG_START);

   action = NONE;
}


void MAS::Dialog::MsgEnd() {
   DialogMessage(MSG_END);
   Widget::MsgEnd();
   running = false;

   if (parent == this) {
      Remove(*tooltipObject);
   }
}


void MAS::Dialog::MsgDraw() {
   Widget::MsgDraw();
   DialogMessage(MSG_DRAW);
}


void MAS::Dialog::MsgClick() {
   Widget::MsgClick();

   Widget *focusObject = this->focusObject;
   Widget *mouseObject = this->mouseObject;

   if (mouseObject) {
      // switch focus if necessary
      if (mouseObject != focusObject) {
         if (mouseObject->MsgWantfocus()) {
            MoveFocus(focusObject, mouseObject);
         }
      }

      // send a MSG_CLICK
      mouseObject->MsgClick();

      if (mouseObject && mouseObject->GetParent() != this) {
         mouseObject->GetParent()->MsgClick();
      }
   }
}


void MAS::Dialog::MsgDClick() {
   Widget::MsgDClick();

   Widget *mouseObject = this->mouseObject;

   if (mouseObject) {
      mouseObject->MsgDClick();

      if (mouseObject && mouseObject->GetParent() != this) {
         mouseObject->GetParent()->MsgDClick();
      }
   }
}


bool MAS::Dialog::MsgWantfocus() {
   Widget::MsgWantfocus();
   return false;
}


void MAS::Dialog::MsgGotfocus() {
   SetFlag(D_GOTFOCUS);
   GetParent()->HandleEvent(*this, MSG_GOTFOCUS);
}


void MAS::Dialog::MsgLostfocus() {
   ClearFlag(D_GOTFOCUS);
   GetParent()->HandleEvent(*this, MSG_LOSTFOCUS);
   lastFocusObject = NULL;
    if (focusObject) {
      focusObject->MsgLostfocus();
      focusObject = NULL;
   }
}


void MAS::Dialog::MsgGotmouse() {
   Widget::MsgGotmouse();
}


void MAS::Dialog::MsgLostmouse() {
   Widget::MsgLostmouse();

   if (!TestFlag(D_PRESSED)) {
      SelectAction(GetAction());
   }

   if (mouseObject) {
      mouseObject->MsgLostmouse();
      mouseObject = NULL;
   }
}


void MAS::Dialog::MsgWheel(int z) {
   Widget::MsgWheel(z);

   Widget *focusObject = this->focusObject;
   Widget *mouseObject = this->mouseObject;

   if (mouseObject) {
      mouseObject->MsgWheel(z);

      if (mouseObject && mouseObject->GetParent() != this) {
         mouseObject->GetParent()->MsgWheel(z);
      }
   }
   if (focusObject && focusObject != mouseObject && focusObject->TestFlag(D_PRESSED)) {
      focusObject->MsgWheel(z);
   }
}


void MAS::Dialog::MsgLPress() {
   Widget::MsgLPress();

   Widget *focusObject = this->focusObject;
   Widget *mouseObject = this->mouseObject;

   if (focusObject && focusObject != mouseObject) {
      if (focusObject->Flags() & D_PRESSED) {
         focusObject->MsgLPress();
      }
   }
   if (mouseObject) {
      mouseObject->MsgLPress();

      if (mouseObject && mouseObject->GetParent() != this) {
         mouseObject->GetParent()->MsgLPress();
      }
   }

   actionOrigin = GetMousePos();
   SetFlag(D_PRESSED);
}


void MAS::Dialog::MsgMPress() {
   Widget::MsgMPress();

   Widget *focusObject = this->focusObject;
   Widget *mouseObject = this->mouseObject;

   if (focusObject && focusObject != mouseObject) {
      if (focusObject->Flags() & D_PRESSED) {
         focusObject->MsgMPress();
      }
   }
   if (mouseObject) {
      mouseObject->MsgMPress();

      if (mouseObject && mouseObject->GetParent() != this) {
         mouseObject->GetParent()->MsgMPress();
      }
   }
}


void MAS::Dialog::MsgRPress() {
   Widget::MsgRPress();

   Widget *focusObject = this->focusObject;
   Widget *mouseObject = this->mouseObject;

   if (focusObject && focusObject != mouseObject) {
      if (focusObject->Flags() & D_PRESSED) {
         focusObject->MsgRPress();
      }
   }
   if (mouseObject) {
      mouseObject->MsgRPress();

      if (mouseObject && mouseObject->GetParent() != this) {
         mouseObject->GetParent()->MsgRPress();
      }
   }
}


void MAS::Dialog::MsgLRelease() {
   Widget::MsgLRelease();
   ClearFlag(D_PRESSED);

   Widget *focusObject = this->focusObject;
   Widget *mouseObject = this->mouseObject;

   if (focusObject && focusObject != mouseObject) {
      if (focusObject->Flags() & D_PRESSED) {
         focusObject->MsgLRelease();
      }
   }
   if (mouseObject) {
      mouseObject->MsgLRelease();

      if (mouseObject && mouseObject->GetParent() != this) {
         mouseObject->GetParent()->MsgLRelease();
      }
   }
}


void MAS::Dialog::MsgMRelease() {
   Widget::MsgMRelease();

   Widget *focusObject = this->focusObject;
   Widget *mouseObject = this->mouseObject;

   if (focusObject && focusObject != mouseObject) {
      if (focusObject->Flags() & D_PRESSED) {
         focusObject->MsgMRelease();
      }
   }
   if (mouseObject) {
      mouseObject->MsgMRelease();

      if (mouseObject && mouseObject->GetParent() != this) {
         mouseObject->GetParent()->MsgMRelease();
      }
   }
}


void MAS::Dialog::MsgRRelease() {
   Widget::MsgRRelease();

   Widget *focusObject = this->focusObject;
   Widget *mouseObject = this->mouseObject;

   if (focusObject && focusObject != mouseObject) {
      if (focusObject->Flags() & D_PRESSED) {
         focusObject->MsgRRelease();
      }
   }
   if (mouseObject) {
      mouseObject->MsgRRelease();

      if (mouseObject && mouseObject->GetParent() != this) {
         mouseObject->GetParent()->MsgRRelease();
      }
   }
}


void MAS::Dialog::MsgTimer(ALLEGRO_TIMER_EVENT *t) {
   Widget::MsgTimer(t);
   DialogMessage(MSG_TIMER, (intptr_t)t);
}


void MAS::Dialog::MsgTick() {
   Widget::MsgTick();

   if (mouse) {
      mouse->Update();
   }

   // send out a MSG_TICK
   DialogMessage(MSG_TICK);

   // only the top level dialog can do the dialog manager thingy
   if (parent == this) {
      // see if any widget's skin was changed
      CheckForSkinChange();

      // see if any widget was moved and/or resized
      CheckGeometry();

      lockFocus = false;
   }

   while (!deletedWidgets.empty()) {
      Widget *tmp = deletedWidgets.back();

      std::vector<Widget *>::iterator iter = widgets.begin();
      for (; iter != widgets.end() && (*iter) != tmp; ++iter);

      if (iter != widgets.end()) {
         Remove(*tmp);
         delete tmp;
      }

      deletedWidgets.pop_back();
   }
}


void MAS::Dialog::MsgMove() {
   Widget::MsgMove();
   UpdateSize();
}



void MAS::Dialog::MsgResize() {
   Widget::MsgResize();
   UpdateSize();
}



void MAS::Dialog::MsgShape() {
   Widget::MsgShape();
   UpdateSize();
}

void MAS::Dialog::UpdateSize() {
   std::vector<MAS::Widget *>::iterator i;
   MAS::Widget *w;
   for (i = widgets.begin(); i != widgets.end(); ++i) {
      w = *i;
      w->SendMessage(MSG_SHAPE);
      // move the widget to the right absolute position
      w->UpdatePosition();

      if (w->HasChildren()) {
         ((Dialog *)w)->UpdateSize();
      }
   }
}


void MAS::Dialog::MsgMousemove(const MAS::Point& d) {
   Widget::MsgMousemove(d);

   // move or resize the dialog
   if (TestFlag(D_PRESSED)) {
      Rect oldRect = *this;
      Point d2 = GetMousePos() - actionOrigin;
      int x2 = MAX(x() - parent->x() + d2.x(), 0);
      int y2 = MAX(y() - parent->y() + d2.y(), 0);
      int w2, h2;
      if (action == MOVE && TestFlag(D_MOVABLE)) {
         Place(x2, y2);
         MsgMove();
      }
      else if ((action != NONE && TestFlag(D_RESIZABLE))) {
         switch (action) {
            case RESIZE_UP_LEFT:
               w2 = MAX(w() + x() - x2 - parent->x(), minSize.w());
               h2 = MAX(h() + y() - y2 - parent->y(), minSize.h());
               x2 = MAX(this->x2() - w2 - parent->x(), 0);
               y2 = MAX(this->y2() - h2 - parent->y(), 0);
               Resize(w2, h2);
               Place(x2, y2);
               MsgMove();
               MsgResize();
               break;

            case RESIZE_DOWN_RIGHT:
               w2 = MAX(w() + d2.x(), minSize.w());
               h2 = MAX(h() + d2.y(), minSize.h());
               actionOrigin.x(actionOrigin.x() + w2 - w());
               actionOrigin.y(actionOrigin.y() + h2 - h());
               Resize(w2, h2);
               MsgResize();
               break;

            case RESIZE_UP:
               h2 = MAX(h() + y() - y2 - parent->y(), minSize.h());
               y2 = MAX(this->y2() - h2 - parent->y(), 0);
               h(h2);
               y(y2);
               MsgMove();
               MsgResize();
               break;

            case RESIZE_DOWN:
               h2 = MAX(h() + d2.y(), minSize.h());
               actionOrigin.y(actionOrigin.y() + h2 - h());
               h(h2);
               MsgResize();
               break;

            case RESIZE_UP_RIGHT:
               w2 = MAX(w() + d2.x(), minSize.w());
               h2 = MAX(h() + y() - y2 - parent->y(), minSize.h());
               y2 = MAX(this->y2() - h2 - parent->y(), 0);
               actionOrigin.x(actionOrigin.x() + w2 - w());
               Resize(w2, h2);
               y(y2);
               MsgMove();
               MsgResize();
               break;

            case RESIZE_DOWN_LEFT:
               w2 = MAX(w() + x() - x2 - parent->x(), minSize.w());
               h2 = MAX(h() + d2.y(), minSize.h());
               x2 = MAX(this->x2() - w2 - parent->x(), 0);
               actionOrigin.y(actionOrigin.y() + h2 - h());
               Resize(w2, h2);
               x(x2);
               MsgMove();
               MsgResize();
               break;

            case RESIZE_LEFT:
               w2 = MAX(w() + x() - x2 - parent->x(), minSize.w());
               x2 = MAX(this->x2() - w2 - parent->x(), 0);
               w(w2);
               x(x2);
               MsgMove();
               MsgResize();
               break;

            case RESIZE_RIGHT:
               w2 = MAX(w() + d2.x(), minSize.w());
               actionOrigin.x(actionOrigin.x() + w2 - w());
               w(w2);
               MsgResize();
               break;

            default:
               return;
         };

         MsgShape();
      }
   }
   else {
      SelectAction(GetAction());
   }

   Widget *focusObject = this->focusObject;
   Widget *mouseObject = this->mouseObject;

   // send a MSG_MOUSEMOVE message to the widget with the mouse/focus
   if (focusObject) {
      focusObject->MsgMousemove(d);
   }
   if (!lockFocus && mouseObject) {
      if (mouseObject != focusObject) {
         mouseObject->MsgMousemove(d);

         if (mouseObject->GetParent() != this) {
            mouseObject->GetParent()->MsgMousemove(d);
         }
      }
   }

   if (parent == this || IsWindow() || TestFlag(D_MOVABLE) || TestFlag(D_RESIZABLE)) {
      if ((!mouseObject || !mouseObject->TestFlag(D_MODAL))) {
         // see which widget is under the mouse
         MAS::Widget *newMouseObject = FindMouseObject();

         // see if mouse object has changed
         if (newMouseObject != mouseObject) {
            MoveMouse(mouseObject, newMouseObject);
         }

         // switch input focus if necessary
         mouseObject = this->mouseObject;
         if (!lockFocus && newMouseObject != focusObject && !(focusObject && focusObject->TestFlag(D_PRESSED))) {
            switch (skin->focus) {
               case 0:
                  // focus always follows mouse (if an object looses the mouse, it looses focus)
                  if (mouseObject != focusObject) {
                     MoveFocus(focusObject, mouseObject);
                  }
                  break;

               case 1:
                  // focus follows the mouse but only when the mouse object wants to take focus
                  if (mouseObject != focusObject) {
                     if (mouseObject && mouseObject->MsgWantfocus()) {
                        MoveFocus(focusObject, mouseObject);
                     }
                  }
                  break;

               case 2:
                  // focus doesn't follow the mouse
                  break;
            };
         }
      }
   }
}


void MAS::Dialog::MsgInitSkin() {
   DialogMessage(MSG_INITSKIN);
   Widget::MsgInitSkin();
}


void MAS::Dialog::HandleMouseInput(ALLEGRO_MOUSE_EVENT *event, ALLEGRO_EVENT_TYPE type) {
   if (focusObject && focusObject->TestFlag(D_MODAL)) {
      ((MAS::Dialog *)focusObject)->HandleMouseInput(event, type);
      return;
   }

   if (!mouse)
      return;

   /* Save the state of the mouse before the update. */
   int click = 0;
   Point mousePos = mouse->pos;
   int buttons = mouse->buttons;
   int z = mouse->z;

   mouse->UpdateState(event, type);

   if (mousePos != mouse->pos) {
      // the mouse has moved!
      MsgMousemove(mouse->pos - mousePos);
   }

   if (!(buttons & 1) && (mouse->buttons & 1)) {
      // left mouse button was pressed
      MsgClick();
      MsgLPress();
      click |= 1;
   }
   else if ((buttons & 1) && !(mouse->buttons & 1)) {
      // left mouse button was released
      MsgLRelease();
   }

   if (!(buttons & 2) && (mouse->buttons & 2)) {
      // right mouse button was pressed
      MsgClick();
      MsgRPress();
      click |= 2;
   }
   else if ((buttons & 2) && !(mouse->buttons & 2)) {
      // right mouse button was released
      MsgRRelease();
   }

   if (!(buttons & 4) && (mouse->buttons & 4)) {
      // middle mouse button was pressed
      MsgClick();
      MsgMPress();
      click |= 4;
   }
   else if ((buttons & 4) && !(mouse->buttons & 4)) {
      // middle mouse button was released
      MsgMRelease();
   }

   // handle double clicks
   int bit = 0;
   switch (mouseState.doubleClickState) {
      case 1:   bit = 1; break;
      case 2:   bit = 2; break;
      case 3:   bit = 4; break;
   };

   switch (mouseState.doubleClickState) {
      case 0:
         if (click & 1) mouseState.doubleClickState = 1; else
         if (click & 2) mouseState.doubleClickState = 2; else
         if (click & 4) mouseState.doubleClickState = 3;
         break;

      case 1:
      case 2:
      case 3:
         if (click & bit) {
            mouseState.doubleClickState = 4;
         }
         else {
            ++mouseState.doubleClickTick;
            if (mouseState.doubleClickTick >= mouseState.doubleClickDelay) {
               mouseState.doubleClickTick = 0;
               mouseState.doubleClickState = 0;
            }
         }
         break;

      case 4:
         MsgDClick();
         mouseState.doubleClickTick = 0;
         mouseState.doubleClickState = 0;
         break;
   };

   CheckFocusLock();

   // handle mouse scroller button
   if (z != mouse->z) {
      MsgWheel((mouse->z - z) * MAS::Settings::scrollingSpeed);
   }
}


void MAS::Dialog::HandleKeyboardInput(ALLEGRO_KEYBOARD_EVENT *event) {
   if (focusObject && focusObject->TestFlag(D_MODAL)) {
      ((MAS::Dialog *)focusObject)->HandleKeyboardInput(event);
      return;
   }

   int cAscii = event->unichar == (unsigned int)-1 ? 0 : event->unichar;
   int cScan  = event->keycode;

//TODO: do the joystick
#if 0
   // fake joystick input by converting it to key presses
   if (MAS::Settings::useJoystick) {
      if (joyOn) {
         rest(1000/MAS::Settings::logicFrameRate);
      }

      poll_joystick();

      if (joyOn) {
         if ((!joy[0].stick[0].axis[0].d1) && (!joy[0].stick[0].axis[0].d2) &&
            (!joy[0].stick[0].axis[1].d1) && (!joy[0].stick[0].axis[1].d2) &&
            (!joy[0].button[0].b) && (!joy[0].button[1].b))
         {
            joyOn = false;
            rest(1000/MAS::Settings::logicFrameRate);
         }
         cAscii = cScan = 0;
      }
      else {
         if (joy[0].stick[0].axis[0].d1) {
            cAscii = 0;
            cScan = ALLEGRO_KEY_LEFT;
            joyOn = true;
         }
         else if (joy[0].stick[0].axis[0].d2) {
            cAscii = 0;
            cScan = ALLEGRO_KEY_RIGHT;
            joyOn = true;
         }
         else if (joy[0].stick[0].axis[1].d1) {
            cAscii = 0;
            cScan = ALLEGRO_KEY_UP;
            joyOn = true;
         }
         else if (joy[0].stick[0].axis[1].d2) {
            cAscii = 0;
            cScan = ALLEGRO_KEY_DOWN;
            joyOn = true;
         }
         else if ((joy[0].button[0].b) || (joy[0].button[1].b)) {
            cAscii = ' ';
            cScan = ALLEGRO_KEY_SPACE;
            joyOn = true;
         }
         else {
            cAscii = cScan = 0;
         }
      }
   }
#endif

   SendKeyboardMessages(cAscii, cScan, event->modifiers);
}


bool MAS::Dialog::CheckKeyboardShortcuts(int cAscii, int cScan, int mod) {
   if (!(mod & ALLEGRO_KEYMOD_CTRL)) return false;

   std::vector<MAS::Widget *>::iterator i;
   for (i = widgets.begin(); i != widgets.end(); ++i) {
      Widget *w = *i;
      if (w->HasChildren()) {
         if (((MAS::Dialog *)w)->CheckKeyboardShortcuts(cAscii, cScan, mod)) {
            return true;
         }
      }

      if ((((cAscii > 0) && (cAscii <= 255) &&
         (tolower((*i)->key) == tolower(cAscii)))) &&
         (!((*i)->flags & (D_HIDDEN | D_DISABLED))))   {
         (*i)->MsgKey();
         return true;
      }
   }

   return false;
}


bool MAS::Dialog::SendKeyboardMessages(int cAscii, int cScan, int mod) {
   int cCombo = (mod << 16) | (cScan<<8) | ((cAscii <= 255) ? cAscii : '^');

   Widget *focusObject = this->focusObject;

   // send messages
   if (focusObject) {
      if (focusObject->HasChildren()) {
         if (((MAS::Dialog *)focusObject)->SendKeyboardMessages(cAscii, cScan, mod)) {
            return true;
         }
      }

      if (focusObject->MsgChar(cCombo)) {
         return true;
      }

      if (focusObject->MsgUChar(cAscii)) {
         return true;
      }
   }

   // keyboard shortcuts
   if (CheckKeyboardShortcuts(cAscii, cScan, mod)) {
      return true;
   }

   // broadcast key
   std::vector<MAS::Widget *>::iterator i;
   for (i = widgets.begin(); i != widgets.end(); ++i) {
      if (!((*i)->flags & (D_HIDDEN | D_DISABLED))) {
         if ((*i)->MsgXChar(cCombo)) {
            return true;
         }
      }
   }

   // handle <CR> and <SPACE>
   if (((cAscii == '\r') || (cAscii == '\n') || (cAscii == ' ')) && (focusObject)) {
      focusObject->MsgKey();
      return true;
   }

   // handle <ESC>
   if (cAscii == 27 && escapeExits && (parent == this || IsWindow())) {
      Close();
      if (close) {
         TakeFocusFrom(focusObject);
      }
      return true;
   }

   // move focus
   if ((parent == this || IsWindow()) && !lockFocus) {
      int dir;
      switch (cScan) {
         case ALLEGRO_KEY_TAB:   dir = (mod & ALLEGRO_KEYMOD_SHIFT) ? 1 : 0;      break;
         case ALLEGRO_KEY_RIGHT: dir = 2;   break;
         case ALLEGRO_KEY_LEFT:  dir = 3;   break;
         case ALLEGRO_KEY_DOWN:  dir = 4;   break;
         case ALLEGRO_KEY_UP:    dir = 5;   break;
         default:      return false;
      }

      if ((dir < 2 && tabKeyMovesFocus) || (arrowKeysMoveFocus && dir >= 2)) {
         MoveFocus(dir);
      }
      return true;
   }

   return false;
}


void MAS::Dialog::ProcessEvent(ALLEGRO_EVENT *event) {
   switch (event->type) {
      case ALLEGRO_EVENT_TIMER:
         MsgTimer(&event->timer);
      break;
      case ALLEGRO_EVENT_DISPLAY_CLOSE:
         Close();
      break;
      case ALLEGRO_EVENT_MOUSE_AXES:
      case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
      case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
         if (MAS::Settings::showMouse) {
            HandleMouseInput(&event->mouse, event->type);
         }
      break;
      case ALLEGRO_EVENT_KEY_DOWN:
      case ALLEGRO_EVENT_KEY_REPEAT:
         if (MAS::Settings::useKeyboard) {
            HandleKeyboardInput(&event->keyboard);
         }
      break;
   }
}


MAS::Widget *MAS::Dialog::FindMouseObject() {
   if (!widgets.empty()) {
      std::vector<MAS::Widget *>::iterator i = widgets.end();
      Widget *w = NULL;

      for (; i != widgets.begin();) {
         --i;
         w = *i;

         if (!w->Hidden() && !w->Disabled() ) {
            if (w->HasChildren()) {
               if (!w->IsWindow()) {
                  Widget *tmp = ((Dialog *)w)->FindMouseObject();
                  if (tmp) {
                     return tmp;
                  }
               }
               else {
                  Widget *tmp = ((Dialog *)w)->FindMouseObject();
                  if (tmp) {
                     return w;
                  }
               }
            }

            if (GetMouse()->pos <= *w && w->MsgWantmouse()) {
               return w;
            }
         }
      }
   }

   return NULL;
}


void MAS::Dialog::GiveFocusTo(MAS::Widget *w) {
   if (w) {
      if (w->MsgWantfocus() && !w->Hidden() && !w->Disabled() && w != this) {
         w->MsgGotfocus();
         focusObject = w;

         if (w->IsWindow() && w != this) {
            BringToTop(*w);
         }
      }
      else if (w->GetParent() != this) {
         GiveFocusTo(w->GetParent());
      }
   }
}


void MAS::Dialog::TakeFocusFrom(MAS::Widget *w) {
   if (w && !w->TestFlag(D_MODAL) && focusObject && (w == focusObject || focusObject->IsChildOf(w))) {
      focusObject->MsgLostfocus();
      lastFocusObject = NULL;   // this should not be here but if it is removed,
                     //programs crash when dynamically adding and removing
                     //widgets that have input focus
      focusObject = NULL;
   }

   if (parent != this) {
      parent->TakeFocusFrom(w);
   }
}


void MAS::Dialog::MoveFocus(MAS::Widget *src, MAS::Widget *dest) {
   //if (parent == this || IsWindow()) {
   if (parent == this || TestFlag(D_TOPLEVEL)) {
      TakeFocusFrom(src);
      GiveFocusTo(dest);
   }
   else {
      parent->MoveFocus(src, dest);
   }
}


void MAS::Dialog::MoveFocusTo(MAS::Widget *dest) {
   MoveFocus(focusObject, dest);
}


void MAS::Dialog::GiveMouseTo(MAS::Widget *w) {
   if (w) {
      if (w->MsgWantmouse() && !w->Hidden() && !w->Disabled() && w != this) {
         w->MsgGotmouse();
         mouseObject = w;
      }
      else if (w->GetParent() != this) {
         GiveMouseTo(w->GetParent());
      }
   }
}


void MAS::Dialog::TakeMouseFrom(MAS::Widget *w) {
   if (w && mouseObject && (w == mouseObject || mouseObject->IsChildOf(w))) {
      mouseObject->MsgLostmouse();
      mouseObject = NULL;
   }

   if (parent != this) {
      parent->TakeMouseFrom(w);
   }
}


void MAS::Dialog::MoveMouse(MAS::Widget *src, MAS::Widget *dest) {
   //if (parent == this || IsWindow()) {
   if (parent == this || TestFlag(D_TOPLEVEL)) {
      TakeMouseFrom(src);
      GiveMouseTo(dest);
   }
   else {
      parent->MoveMouse(src, dest);
   }
}

static int s_cmp_right(const MAS::Widget *d1, const MAS::Widget *d2) {
	int ret = (d2->x() - d1->x()) + abs(d1->y() - d2->y()) * 2;
	return (d1->x() >= d2->x()) ? ret += 0x10000 : ret;
}

static int s_cmp_left(const MAS::Widget *d1, const MAS::Widget *d2) {
	int ret = (d1->x() - d2->x()) + abs(d1->y() - d2->y()) * 2;
	return (d1->x() <= d2->x()) ? ret += 0x10000 : ret;
}

static int s_cmp_down(const MAS::Widget *d1, const MAS::Widget *d2) {
	int ret = (d2->y() - d1->y()) + abs(d1->x() - d2->x()) * 2;
	return (d1->y() >= d2->y()) ? ret += 0x10000 : ret;
}

static int s_cmp_up(const MAS::Widget *d1, const MAS::Widget *d2) {
	int ret = (d1->y() - d2->y()) + abs(d1->x() - d2->x()) * 2;
	return (d1->y() <= d2->y()) ? ret += 0x10000 : ret;
}

// comparison function for list::sort()
static bool cmp_obj_list(const MAS::MAS_OBJ_LIST *obj1, const MAS::MAS_OBJ_LIST *obj2) {
	return obj1->diff < obj2->diff;
}

// dir:
// 0 - TAB
// 1 - shift+TAB
// 2 - RIGHT
// 3 - LEFT
// 4 - DOWN
// 5 - UP
void MAS::Dialog::MoveFocus(int dir) {
   if (!TestFlag(D_TOPLEVEL) && parent != this) {
      parent->MoveFocus(dir);
      return;
   }

   // a pointer to the comparison function
   int (*cmp)(const MAS::Widget *d1, const MAS::Widget *d2);

   // choose a comparison function
   switch (dir) {
      case 0:  cmp = s_cmp_right;     break;
      case 1:  cmp = s_cmp_left;      break;
      case 2:  cmp = s_cmp_right;     break;
      case 3:  cmp = s_cmp_left;      break;
      case 4:  cmp = s_cmp_down;      break;
      case 5:  cmp = s_cmp_up;        break;
      default: return;
   };

   // a temporary list of rectangles
   std::vector<MAS_OBJ_LIST *> objList;
   Widget *f = GetFocusObject();
   int c=0;

   // fill temporary table
   FillObjectList(objList, cmp, f, c);

   // sort table
   std::sort(objList.begin(), objList.end(), cmp_obj_list);

   // find an object to give the focus to
   std::vector<MAS_OBJ_LIST *>::iterator j;
   for (j = objList.begin(); j != objList.end(); ++j) {
      if ((*j)->w->MsgWantTab()) {
         MoveFocus(focusObject, (*j)->w);
         break;
      }
   }

   // clean up
   while (!objList.empty()) {
      j = objList.end();
      --j;
      delete *j;
      objList.pop_back();
   }
}


void MAS::Dialog::FillObjectList(std::vector<MAS_OBJ_LIST *> &objList, int (*cmp)(const MAS::Widget *, const MAS::Widget *), MAS::Widget *f, int &c) {
   std::vector<MAS::Widget *>::iterator i;
   Widget *w;
   for (i = widgets.begin(); i != widgets.end(); ++i) {
      w = *i;
      if (!w->Hidden() && !w->Disabled() && !(w->IsWindow())) {
         if (w->HasChildren()) {
            ((Dialog *)w)->FillObjectList(objList, cmp, f, c);
         }
         if (!w->HasFocus()) {
            MAS_OBJ_LIST *item = new MAS_OBJ_LIST;
            item->w = w;
            if (f) {
               item->diff = cmp(f, w);
            }
            else {
               item->diff = c;
            }
            objList.push_back(item);
            ++c;
         }
      }
   }
}


MAS::Dialog *MAS::Dialog::Root() {
   if (parent == this) {
      return this;
   }
   else {
      return parent->Root();
   }
}


MAS::Dialog *MAS::Dialog::RootWindow() {
   if (parent == this || IsWindow()) {
      return this;
   }
   else {
      return parent->RootWindow();
   }
}


void MAS::Dialog::HandleEvent(Widget &obj, int msg, intptr_t arg1, intptr_t arg2) {
   switch (msg) {
      case MSG_ACTIVATE: {
         if (obj.WillExit()) {
            Close();
         }
      }
      break;
   };

   if (parent != this && !IsWindow()) {
      parent->HandleEvent(obj, msg, arg1, arg2);
   }
}


ALLEGRO_BITMAP *MAS::Dialog::GetCanvas(Point &offset) {
   if ((flags & D_ANIMATING) && !animator->Animating()) {
      return NULL;
   }

   if (!(flags & D_ANIMATING) && animator->Animating()) {
      offset -= topLeft();
      return animator->GetBuffer();
   }
   else if (parent != this) {
      return parent->GetCanvas(offset);
   }
   else {
      return al_get_target_bitmap();
   }
}


ALLEGRO_BITMAP *MAS::Dialog::GetCanvas(MAS::Widget *obj) {
   Point p = obj->topLeft();
   ALLEGRO_BITMAP *pCanvas = GetCanvas(p);
   int cw = al_get_bitmap_width(pCanvas);
   int ch = al_get_bitmap_height(pCanvas);
   if (pCanvas && p.x() < cw && p.y() < ch) {
      int ww = p.x() + obj->w() > x2() ? x2() - p.x() : obj->w();
      int hh = p.y() + obj->h() > y2() ? y2() - p.y() : obj->h();
      if (ww <= 0 || hh <= 0) {
         return NULL;
      }
      else {
         return al_create_sub_bitmap(pCanvas, p.x() < 0 ? 0 : p.x(), p.y() < 0 ? 0 : p.y(), ww, hh);
      }
   }
   else {
      return NULL;
   }
}


MAS::Mouse *MAS::Dialog::GetMouse() const {
   return mouse;
}


void MAS::Dialog::SetMouse(MAS::Mouse *m) {
   if (mouse) {
      m->pos = mouse->pos;
      m->buttons = mouse->buttons;
      m->z = mouse->z;

      delete mouse;
      mouse = m;
      SetCursor(MAS::Skin::MOUSE_NORMAL);
   }
   else {
      mouse = m;
   }
}


MAS::Point MAS::Dialog::GetOffset() const {
   if (parent == this) {
      return Point(0,0);
   }
   else {
      return relativePos + parent->GetOffset();
   }
}


void MAS::Dialog::Close() {
   if (MsgClose()) {
      if (parent == this || TestFlag(D_MODAL)) {
         close = true;
      }
      else {
         GetParent()->HandleEvent(*this, MSG_CLOSE);
      }
   }
}


void MAS::Dialog::SetMouseCursor(int index) {
   // set the mouse cursor
   if (mouse) {
      mouse->SetCursor(skin->GetCursor(index));
   }
}


bool MAS::Dialog::HasMouse() {
   if (Widget::HasMouse()) return true;

   for (std::vector<Widget *>::iterator i = widgets.begin(); i != widgets.end(); ++i) {
      if ((*i)->HasMouse()) {
         return true;
      }
   }

   return false;
}


bool MAS::Dialog::HasFocus() {
   if (Widget::HasFocus()) return true;

   for (std::vector<Widget *>::iterator i = widgets.begin(); i != widgets.end(); ++i) {
      if ((*i)->HasFocus()) {
         return true;
      }
   }

   return false;
}


void MAS::Dialog::Hide() {
   Widget::Hide();
   for (std::vector<Widget *>::iterator i = widgets.begin(); i != widgets.end(); ++i) {
      (*i)->Hide();
   }
}


void MAS::Dialog::Unhide() {
   Widget::Unhide();
   for (std::vector<Widget *>::iterator i = widgets.begin(); i != widgets.end(); ++i) {
      (*i)->Unhide();
   }
}


void MAS::Dialog::Disable() {
   Widget::Disable();
   for (std::vector<Widget *>::iterator i = widgets.begin(); i != widgets.end(); ++i) {
      (*i)->Disable();
   }
}


void MAS::Dialog::Enable() {
   Widget::Enable();
   for (std::vector<Widget *>::iterator i = widgets.begin(); i != widgets.end(); ++i) {
      (*i)->Enable();
   }
}


MAS::Dialog::ActionType MAS::Dialog::GetAction() {
   if (parent != this && mouseObject != focusObject)
      return MOVE;
   else
      return NONE;
}


void MAS::Dialog::SelectAction(MAS::Dialog::ActionType action) {
   if (this->action != action) {
      this->action = action;

      switch (action) {
         case NONE:
         case MOVE:
            SetCursor(Skin::MOUSE_NORMAL);
            break;

         case RESIZE_UP_LEFT:
         case RESIZE_DOWN_RIGHT:
            SetCursor(Skin::MOUSE_SIZE_DIAGONAL1);
            break;

         case RESIZE_UP:
         case RESIZE_DOWN:
            SetCursor(Skin::MOUSE_SIZE_VERTICAL);
            break;

         case RESIZE_UP_RIGHT:
         case RESIZE_DOWN_LEFT:
            SetCursor(Skin::MOUSE_SIZE_DIAGONAL2);
            break;

         case RESIZE_LEFT:
         case RESIZE_RIGHT:
            SetCursor(Skin::MOUSE_SIZE_HORIZONTAL);
            break;
      };
   }
}


MAS::Widget *MAS::Dialog::GetFocusObject() const {
   if (focusObject && focusObject->HasChildren()) {
      return ((Dialog *)focusObject)->GetFocusObject();
   }
   else {
      return focusObject;
   }
}


MAS::Widget *MAS::Dialog::GetMouseObject() const {
   if (mouseObject && mouseObject->HasChildren()) {
      return ((Dialog *)mouseObject)->GetMouseObject();
   }
   else {
      return mouseObject;
   }
}


void MAS::Dialog::SetTooltipObject(MAS::Tooltip *tooltipObject) {
   Remove(*tooltipObject);
   if (tooltipObject) {
      this->tooltipObject = tooltipObject;
   }
   else {
      this->tooltipObject = &defaultTooltipObject;
   }
   Add(*tooltipObject);
}


bool MAS::Dialog::MsgWantTab() {
   return false;
}
