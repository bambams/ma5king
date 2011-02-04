////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/mouse.h"
#include "../include/MASkinG/settings.h"
#include "../include/MASkinG/size.h"


MAS::Cursor *MAS::Mouse::globalCursor = NULL;

MAS::Mouse::Mouse() :
   hidden(true),
   pos(-10000,-10000),
   buttons(0),
   z(0)
{
}


MAS::Mouse::~Mouse() {
   Hide();
}


void MAS::Mouse::CalculateGeometry() {
   // calculate position of the sprite
   spritePos.x(pos.x() - cur->focus.x());
   spritePos.y(pos.y() - cur->focus.y());

   // calculate position of the shadow
   shadowPos.x(spritePos.x() + cur->shadowOffset.x() - cur->PADDING);
   shadowPos.y(spritePos.y() + cur->shadowOffset.y() - cur->PADDING);
}


void MAS::Mouse::SetCursor(Cursor &cur) {
   // set the pointer
   this->cur = &cur;

   // calculate positions of the sprite, shadow and buffer
   CalculateGeometry();
}


MAS::Cursor* MAS::Mouse::GetCursor() {
   return cur;
}


void MAS::Mouse::Show(Bitmap &canvas) {
   if (!MAS::Settings::showMouse) return;

   // draw shadow
   if (MAS::Settings::mouseShadow) {
      cur->Shadow().Blit(canvas, 0, 0, 
                         shadowPos.x(), shadowPos.y(),
                         cur->Shadow().w(), cur->Shadow().h());
   }

   // draw sprite
   cur->Sprite().Blit(canvas, 0, 0,
                      spritePos.x(), spritePos.y(),
                      cur->Sprite().w(), cur->Sprite().h());

   hidden = false;
}


void MAS::Mouse::Hide() {
   if (!MAS::Settings::showMouse) return;

   hidden = true;
}


void MAS::Mouse::UpdateState(ALLEGRO_MOUSE_EVENT *event, ALLEGRO_EVENT_TYPE type) {
   switch (type) {
      case ALLEGRO_EVENT_MOUSE_AXES: {
         // get the new position of the mouse
         pos = Point(event->x, event->y);
         // read the position of the wheel
         z = event->z;
         // update positions of the sprite and shadow
         CalculateGeometry();                                     }
      break;

      case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
         if (event->button == 1)
            buttons &= ~1;
         if (event->button == 2)
            buttons &= ~4;
         if (event->button == 3)
            buttons &= ~2;
      break;

      case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
         if (event->button == 1)
            buttons |= 1;
         if (event->button == 2)
            buttons |= 4;
         if (event->button == 3)
            buttons |= 2;
      break;
   }
}

void MAS::Mouse::Update() {
   // update cursor animation
   cur->Update();
}
