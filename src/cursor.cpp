////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/cursor.h"
#include "../include/MASkinG/settings.h"


int MAS::Cursor::PADDING = 2;
MAS::Color MAS::Cursor::shadowColor;
MAS::Point MAS::Cursor::shadowOffset = MAS::Point(2,0);


// a simple function that applies a blur effect on a bitmap
//  - Bitmap &bmp: the input bitmap
//  - int radius:  blur radius
void MAS::Cursor::Blur(Bitmap &bmp, int radius) {
   int x, y;
   int i, j;
   int r,g,b;
   
   int div = (2*radius+1)*(2*radius+1);

   Bitmap tmp2(bmp.w(), bmp.h(), Bitmap::MEMORY);
   tmp2.Clear(Color::transparent);
   bmp.Blit(tmp2, 0, 0, 0, 0, bmp.w(), bmp.h());

   Bitmap tmp(Size(bmp.w() + 2*radius, bmp.h() + 2*radius), Bitmap::MEMORY);
   tmp.Clear(Color::black);
   bmp.Blit(tmp, Point(0,0), Point(radius, radius), bmp.size());

   for (y=radius; y<tmp.h()-radius; y++) {
      for (x=radius; x<tmp.w()-radius; x++) {
         r = g = b = 0;
         for (j=-radius; j<=radius; j++) {
            for (i=-radius; i<=radius; i++) {
               Color col = tmp.Getpixel(Point(x+i, y+j));
               r += col.r();
               g += col.g();
               b += col.b();
            }
         }
         
         tmp2.Putpixel(Point(x-radius, y-radius), Color(r/div, g/div, b/div));
      }
   }

   bmp.Clear();
   tmp2.Blit(bmp, 0, 0, 0, 0, bmp.w(), bmp.h());
}


// creates a cursor shadow from a cursor bitmap
void MAS::Cursor::MakeShadow() {
   int x, y, i;

   Point pOffset(PADDING, PADDING);
   Point pOrigin(0,0);
   for (i=0; i<frameCount; i++) {
      Bitmap bmp;

      if (MAS::Settings::mouseShadow) {
         bmp.Create(Size(sprite[i]->w() + 2*PADDING, sprite[i]->h() + 2*PADDING), Bitmap::MEMORY);
      
         // create the alpha channel for the shadow by masking the cursor and bluring the results
         Bitmap alpha(bmp.size(), Bitmap::MEMORY);
         alpha.Clear(Color::transparent);
         sprite[i]->Blit(alpha, pOrigin, pOffset, sprite[i]->size());

         for (y=0; y<bmp.h(); y++) {
            for (x=0; x<bmp.w(); x++) {
               Point p(x,y);
               if (alpha.Getpixel(p) == Color::transparent)
                  alpha.Putpixel(p, Color::black);
               else
                  alpha.Putpixel(p, Color::white);
            }
         }

         Blur(alpha, PADDING);

         // write the alpha channel to the shadow bitmap
         for (y=0; y<bmp.h(); y++) {
            for (x=0; x<bmp.w(); x++) {
               Point p(x,y);
               bmp.Putpixel(p, Color(shadowColor.r(), shadowColor.g(), shadowColor.b(), alpha.Getpixel(p).r()));
            }
         }
      
         alpha.Destroy();
      }
      else {
         bmp.Create(1, 1);
         bmp.Putpixel(0, 0, Color::transparent);
      }

      Bitmap *result = new Bitmap(bmp.w(), bmp.h());
	  bmp.Blit(*result, 0, 0, 0, 0, bmp.w(), bmp.h());
      shadow.push_back(result);
   }
}



MAS::Cursor::Cursor()
   :focus(0,0),
   frameCount(0),
   frame(0),
   interval(1)
{
   MAS::Cursor::shadowColor = MAS::Color(96,96,96);
}

      
MAS::Cursor::~Cursor() {
   Destroy();
}


void MAS::Cursor::Destroy() {
   int i;
   for (i=0; i<frameCount; i++) {
      delete sprite[i];
      delete shadow[i];
   }
   sprite.clear();
   shadow.clear();
   
   frameCount = 0;
   frame = 0;
}


MAS::Cursor& MAS::Cursor::Create(const Bitmap &bmp, int n) {
   int i;

   Destroy();
   frameCount = n;
   for (i=0; i<n; i++) {
      Bitmap *newBmp = new Bitmap(Size(bmp.w()/n, bmp.h()));
      bmp.PlainBlit(*newBmp, Point(i*bmp.w()/n,0), Point(0,0), Size(bmp.w()/n,bmp.h()));
      sprite.push_back(newBmp);
   }
   
   MakeShadow();
   
   return *this;
}


MAS::Cursor& MAS::Cursor::Create() {
   Bitmap bmp(12,21, MAS::Bitmap::MEMORY);
   bmp.Clear(Color::transparent);
   bmp.Line( 0,  0,  0, 16, Color::white);
   bmp.Line( 0, 16,  4, 12, Color::white);
   bmp.Line( 4, 12,  7, 19, Color::white);
   bmp.Line( 8, 20,  9, 20, Color::white);
   bmp.Line(10, 19,  7, 12, Color::white);
   bmp.Line( 7, 11, 11, 11, Color::white);
   bmp.Line(11, 11,  0,  0, Color::white);
//TODO: implement this!
//   bmp.Floodfill(3, 8, Color::black);
   return Create(bmp, 1);
}

      
void MAS::Cursor::SetFocus(int x, int y) {
   focus.x(x);
   focus.y(y);
}

      
void MAS::Cursor::Update() {
   frame += interval;
   while (frame >= frameCount) {
      frame -= frameCount;
   }
}


void MAS::Cursor::SetAnimationInterval(int i) {
   interval = 1000.0f/(float)(MAS::Settings::logicFrameRate*i);
}


void MAS::Cursor::ReadSkinData(ALLEGRO_CONFIG *config) {
   const char *str;

#define GET_CONFIG_STR(key, def) \
   (str = al_get_config_value(config, "MOUSE", key)) ? strtol(str, NULL, 10) : def

   shadowColor = MAS::Color((str = al_get_config_value(config, "MOUSE",  "shadowColor")) ? str : "96,96,96");
   PADDING = GET_CONFIG_STR("shadowSize", 2);
   int x = GET_CONFIG_STR("shadowOffsetX", 2);
   int y = GET_CONFIG_STR("shadowOffsetY", 0);
   shadowOffset = MAS::Point(x,y);

#undef GET_CONFIG_STR
}


MAS::Bitmap& MAS::Cursor::Sprite() const {
   return *sprite[(int)frame];
}


MAS::Bitmap& MAS::Cursor::Shadow() const {
   return *shadow[(int)frame];
}


void MAS::Cursor::SetFocus(const MAS::Point &p) {
   SetFocus(p.x(), p.y());
}


const MAS::Point& MAS::Cursor::GetFocus() {
   return focus;
}
