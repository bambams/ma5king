////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/animator.h"
#include "../include/MASkinG/settings.h"


MAS::Animator::Animator()
   :frames(0),
   progress(1)
{
}

   
MAS::Animator::~Animator() {
   buffer.Destroy();
   back.Destroy();
}


void MAS::Animator::Create(Bitmap &bmp, int length) {
   if (!buffer || (bmp.w() != buffer.w() || bmp.h() != buffer.h())) {
      buffer.Destroy();
      buffer.Create(bmp.w(), bmp.h());
      buffer.Clear(Color::transparent);

      back.Destroy();
      back.Create(bmp.w(), bmp.h());
      back.Clear(Color::transparent);
   }
   else if (!back) {
      back.Destroy();
      back.Create(bmp.w(), bmp.h());
      back.Clear(Color::transparent);
   }
   
   frames = length * MAS::Settings::logicFrameRate / 1000;
   frames = frames < 1 ? 1 : frames;
   progress = 1;
}


bool MAS::Animator::Update() {
   ++progress;
   if (progress >= frames) {
      buffer.Blit(back, 0, 0, 0, 0, back.w(), back.h());
   }
   return progress > frames ? false : true;
}


void MAS::Animator::Reset() {
   frames = 0;
   progress = 1;

   back.Destroy();
}


void MAS::Animator::Draw(Bitmap &canvas, int type) {
   back.Blit(canvas, 0, 0, 0, 0, back.w(), back.h());
   
   switch (type) {
      case NONE: {
         buffer.Blit(canvas, 0, 0, 0, 0, buffer.w(), buffer.h());
         break;
      }

      case BLEND: {
         ALLEGRO_STATE state;
         al_store_state(&state, ALLEGRO_STATE_ALL);
         al_set_target_bitmap(canvas);
         al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
         al_draw_tinted_bitmap(buffer, al_map_rgba(0, 0, 0, (progress<<8)*(progress)/(frames*frames)), 0, 0, 0);
         al_restore_state(&state);
         break;
      }

      case SLIDE_DOWN: {
         int y = (buffer.h())*(frames - progress)/frames;
         buffer.Blit(canvas, 0, y, 0, 0, buffer.w(), buffer.h() - y);
         break;
      }

      case SLIDE_UP: {
         int y = (buffer.h())*(frames - progress)/frames;
         buffer.Blit(canvas, 0, 0, 0, y, buffer.w(), buffer.h() - y);
         break;
      }

      case SLIDE_LEFT: {
         int x = (buffer.w())*(frames - progress)/frames;
         buffer.Blit(canvas, 0, 0, x, 0, buffer.w() - x, buffer.h());
         break;
      }

      case SLIDE_RIGHT: {
         int x = (buffer.w())*(frames - progress)/frames;
         buffer.Blit(canvas, x, 0, 0, 0, buffer.w() - x, buffer.h());
         break;
      }

      case ROLL_DOWN: {
         int h = buffer.h()*progress/frames;
         buffer.Blit(canvas, 0, 0, 0, 0, buffer.w(), h);
         break;
      }

      case ROLL_UP: {
         int h = buffer.h()*progress/frames;
         buffer.Blit(canvas, 0, buffer.h() - h, 0, buffer.h() - h, buffer.w(), h);
         break;
      }

      case ROLL_LEFT: {
         int w = buffer.w()*progress/frames;
         buffer.Blit(canvas, buffer.w() - w, 0, buffer.w() - w, 0, w, buffer.h());
         break;
      }

      case ROLL_RIGHT: {
         int w = buffer.w()*progress/frames;
         buffer.Blit(canvas, 0, 0, 0, 0, w, buffer.h());
         break;
      }

      case BLINDS_DOWN: {
         int h = buffer.h();
         int w = buffer.w();
         int pr = (progress<<2)/frames;
         int count = 0;
         while (true) {
            buffer.Blit(canvas, 0, pr+count, 0, pr+count, w, 1);
            count += 4;
            if (count >= h - pr)
               break;
         }
         break;
      }

      case BLINDS_UP: {
         int h = buffer.h();
         int w = buffer.w();
         int pr = ((frames-progress)<<2)/frames;
         int count = 0;
         while (true) {
            buffer.Blit(canvas, 0, pr+count, 0, pr+count, w, 1);
            count += 4;
            if (count >= h - pr)
               break;
         }
         break;
      }

      case BLINDS_LEFT: {
         int h = buffer.h();
         int w = buffer.w();
         int pr = ((frames-progress)<<2)/frames;
         int count = 0;
         while (true) {
            buffer.Blit(canvas, pr+count, 0, pr+count, 0, 1, h);
            count += 4;
            if (count >= w - pr)
               break;
         }
         break;
      }

      case BLINDS_RIGHT: {
         int h = buffer.h();
         int w = buffer.w();
         int pr = (progress<<2)/frames;
         int count = 0;
         while (true) {
            buffer.Blit(canvas, pr+count, 0, pr+count, 0, 1, h);
            count += 4;
            if (count >= w - pr)
               break;
         }
         break;
      }

      case OPEN_VERTICAL: {
         // top half
         int h = buffer.h()>>1;
         int y = h - progress*h/frames;
         buffer.Blit(canvas, 0, 0, 0, y, buffer.w(), h - y);
         // bottom half
         h = buffer.h()>>1;
         y = h + (frames - progress)*h/frames;
         buffer.Blit(canvas, 0, y, 0, h, buffer.w(), buffer.h() - y);
         break;
      }

      case OPEN_HORIZONTAL: {
         // left half
         int w = buffer.w()>>1;
         int x = w - progress*w/frames;
         buffer.Blit(canvas, 0, 0, x, 0, w - x, buffer.h());
         // rigth half
         w = buffer.w()>>1;
         x = w + (frames - progress)*w/frames;
         buffer.Blit(canvas, x, 0, w, 0, buffer.w() - x, buffer.h());
         break;
      }

      case OPEN_FROM_CENTRE: {
         int w = buffer.w()>>1;
         int h = buffer.h()>>1;
         int r1w = progress*w/frames;
         int r1h = progress*h/frames;
         int r2w = (frames - progress)*w/frames;
         int r2h = (frames - progress)*h/frames;
         buffer.Blit(canvas, 0, 0, w - r1w, h - r1h, r1w, r1h);         // top left
         buffer.Blit(canvas, 0, h + r2h, w - r1w, h, r1w, h - r2h);      // bottom left
         buffer.Blit(canvas, w + r2w, 0, w, h - r1h, w - r2w, r1h);      // top right
         buffer.Blit(canvas, w + r2w, h + r2h, w, h, w - r2w, h - r2h);   // bottom right
         break;
      }

      case ROLL_OPEN_VERTICAL: {
         int h = buffer.h();
         int y = (frames - progress)*(h>>1)/frames;
         buffer.Blit(canvas, 0, y, 0, y, buffer.w(), h - (y<<1));
         break;
      }

      case ROLL_OPEN_HORIZONTAL: {
         int w = buffer.w();
         int x = (frames - progress)*(w>>1)/frames;
         buffer.Blit(canvas, x, 0, x, 0, w - (x<<1), buffer.h());
         break;
      }

      case ROLL_OPEN_FROM_CENTRE: {
         int w = buffer.w();
         int x = (frames - progress)*(w>>1)/frames;
         int h = buffer.h();
         int y = (frames - progress)*(h>>1)/frames;
         buffer.Blit(canvas, x, y, x, y, w - (x<<1), h - (y<<1));
         break;
      }

      case CLOSE_VERTICAL: {
         int h = buffer.h()>>1;
         int y = (frames - progress)*h/frames;
         buffer.Blit(canvas, 0, y, 0, 0, buffer.w(), h - y);      // top half
         buffer.Blit(canvas, 0, h, 0, h + y, buffer.w(), h - y);   // bottom half
         break;
      }

      case CLOSE_HORIZONTAL: {
         int w = buffer.w()>>1;
         int x = (frames - progress)*w/frames;
         buffer.Blit(canvas, x, 0, 0, 0, w - x, buffer.h());      // left half
         buffer.Blit(canvas, w, 0, w + x, 0, w - x, buffer.h());   // right half
         break;
      }

      case CLOSE_TO_CENTRE: {
         int w = buffer.w()>>1;
         int h = buffer.h()>>1;
         int rw = (frames - progress)*w/frames;
         int rh = (frames - progress)*h/frames;
         int w2 = w - rw;
         int h2 = h - rw;
         buffer.Blit(canvas, rw, rh, 0, 0, w2, h2);         // top left
         buffer.Blit(canvas, rw, h, 0, h + rh, w2, h2);      // bottom left
         buffer.Blit(canvas, w, rh, w + rw, 0, w2, h2);      // top right
         buffer.Blit(canvas, w, h, w + rw, h + rh, w2, h2);   // bottom right
         break;
      }

      case ROLL_CLOSE_VERTICAL: {
         int h = buffer.h()>>1;
         int r = (frames - progress)*h/frames;
         buffer.Blit(canvas, 0, 0, 0, 0, buffer.w(), progress*h/frames);   // top
         buffer.Blit(canvas, 0, h + r, 0, h + r, buffer.w(), h - r);      // bottom
         break;
      }

      case ROLL_CLOSE_HORIZONTAL: {
         int w = buffer.w()>>1;
         int r = (frames - progress)*w/frames;
         buffer.Blit(canvas, 0, 0, 0, 0, progress*w/frames, buffer.h());   // left
         buffer.Blit(canvas, w +r, 0, w + r, 0, w - r, buffer.h());      // right
         break;
      }

      case ROLL_CLOSE_TO_CENTRE: {
         int w = buffer.w()>>1;
         int h = buffer.h()>>1;
         int r1w = progress*w/frames;
         int r1h = progress*h/frames;
         int r2w = (frames - progress)*w/frames;
         int r2h = (frames - progress)*h/frames;
         int w2 = w + r2w;
         int w3 = w - r2w;
         int h2 = h + r2h;
         int h3 = h - r2h;
      
         buffer.Blit(canvas, 0, 0, 0, 0, r1w, r1h);      // top left
         buffer.Blit(canvas, 0, h2, 0, h2, r1w, h3);      // bottom left
         buffer.Blit(canvas, w2, 0, w2, 0, w3, r1h);      // top right
         buffer.Blit(canvas, w2, h2, w2, h2, w3, h3);   // bottom right
         break;
      }

      case ROLL_CLOSE_TO_CENTRE2: {
         int w = buffer.w()>>1;
         int h = buffer.h()>>1;
         int r1w = progress*w/frames;
         int r1h = progress*h/frames;
         int r2w = (frames - progress)*w/frames;
         int r2h = (frames - progress)*h/frames;
      
         buffer.Blit(canvas, 0, 0, 0, 0, buffer.w(), r1h);               // top
         buffer.Blit(canvas, 0, h + r2h, 0, h + r2h, buffer.w(), h - r2h);   // bottom
         buffer.Blit(canvas, 0, 0, 0, 0, r1w, buffer.h());               // left
         buffer.Blit(canvas, w + r2w, 0, w + r2w, 0, w - r2w, buffer.h());   // right
         break;
      }
   };
}


bool MAS::Animator::Animating() {
   return (progress < frames);
}


MAS::Bitmap& MAS::Animator::GetBuffer() {
   return buffer;
}
