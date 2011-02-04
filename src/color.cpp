////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/color.h"
#include <cmath>

#define MIN(x,y)     (((x) < (y)) ? (x) : (y))

MAS::Color MAS::Color::white;
MAS::Color MAS::Color::black;
MAS::Color MAS::Color::red;
MAS::Color MAS::Color::green;
MAS::Color MAS::Color::blue;
MAS::Color MAS::Color::cyan;
MAS::Color MAS::Color::magenta;
MAS::Color MAS::Color::yellow;
MAS::Color MAS::Color::darkgray;
MAS::Color MAS::Color::gray;
MAS::Color MAS::Color::lightgray;
MAS::Color MAS::Color::orange;
MAS::Color MAS::Color::brown;
MAS::Color MAS::Color::purple;
MAS::Color MAS::Color::maroon;
MAS::Color MAS::Color::darkgreen;
MAS::Color MAS::Color::darkblue;
MAS::Color MAS::Color::transparent;


MAS::Color::Color(const ALLEGRO_COLOR &c)
   :col(c)
{
}


MAS::Color::Color(int r, int g, int b) {
   Make(r,g,b);
}


MAS::Color::Color(int r, int g, int b, int a) {
   Make(r,g,b,a);
}


MAS::Color::Color(float h, float s, float v) {
   int r,g,b;
   hsv_to_rgb(h,s,v, &r,&g,&b);
   Make(r,g,b);
}


MAS::Color::Color(const char *str) {
   Make(str);
}

////////////////////////////////////////////////////////////////////////////////
// Converts a string to a color where string is in standard rgb
// format (e.g. 64,128,200 or 200 150 125)
void MAS::Color::Make(const char *str) {
   char *str2 = new char[1+strlen(str)];
   int r = 0;
   int g = 0;
   int b = 0;
   int a = 255;

   if (!strcmp(str, "-1")) {
      delete [] str2;
      col = MAS::Color::transparent;
      return;
   }
   else {
      strcpy(str2, str);
      char *tok;

      tok = strtok(str2, ", ;");
      if (tok) {
         r = strtol(tok, NULL, 10);

         tok = strtok(0, ", ;");
         if (tok) {
            g = strtol(tok, NULL, 10);

            tok = strtok(0, ", ;");
            if (tok) {
               b = strtol(tok, NULL, 10);

               tok = strtok(0, ", ;");
               if (tok) {
                  a = strtol(tok, NULL, 10);
               }
            }
         }
      }
   }

   delete [] str2;
   Make(r, g, b, a);
}



void MAS::Color::Make(int r, int g, int b) {
   col = al_map_rgb(r,g,b);
}


void MAS::Color::Make(int r, int g, int b, int a) {
   col = al_map_rgba(r,g,b,a);
}


MAS::Color::operator ALLEGRO_COLOR() const {
   return col;
}


MAS::Color::operator bool() const {
   return !(col.a == 0);
}


void MAS::Color::operator=(const ALLEGRO_COLOR &c) {
   col.r = c.r;
   col.g = c.g;
   col.b = c.b;
   col.a = c.a;
}


int MAS::Color::r() const {
   return col.r * 255;
}


int MAS::Color::g() const {
   return col.g * 255;
}


int MAS::Color::b() const {
   return col.b * 255;
}


int MAS::Color::a() const {
   return col.a * 255;
}


void MAS::Color::r(int _r) {
   col.r = _r / 255.f;
}


void MAS::Color::g(int _g) {
   col.g = _g / 255.f;
}


void MAS::Color::b(int _b) {
   col.b = _b / 255.f;
}


void MAS::Color::a(int _a) {
   col.a = _a / 255.f;
}


void MAS::Color::Colorize(const Color &c, int alpha) {
   float a = (float)alpha/256.0f;

   // convert the pixel to HSV
   float hp, sp, vp;
   ToHSV(hp, sp, vp);

   // convert the color to HSV
   float hc, sc, vc;
   c.ToHSV(hc, sc, vc);

   // make a new rgb pixel
   int r,g,b;
   HSVToRGB(hc, sp*sc, vp*vc, r,g,b);

   // blend the new color with the old one
   r = (int)(r*a + this->r()*(1-a));
   g = (int)(g*a + this->g()*(1-a));
   b = (int)(b*a + this->b()*(1-a));
   Make(r,g,b);
}


/* hsv_to_rgb:
 *  Converts from HSV colorspace to RGB values.
 *  Coppied from A4.
 */
void MAS::Color::hsv_to_rgb(float h, float s, float v, int *r, int *g, int *b)
{
   float f, x, y, z;
   int i;

   ALLEGRO_ASSERT(s >= 0 && s <= 1);
   ALLEGRO_ASSERT(v >= 0 && v <= 1);

   v *= 255.0f;

   if (s == 0.0f) { /* ok since we don't divide by s, and faster */
      *r = *g = *b = v + 0.5f;
   }
   else {
      h = fmod(h, 360.0f) / 60.0f;
      if (h < 0.0f)
         h += 6.0f;

      i = (int)h;
      f = h - i;
      x = v * s;
      y = x * f;
      v += 0.5f; /* round to the nearest integer below */
      z = v - x;

      switch (i) {

    case 6:
    case 0:
       *r = v;
       *g = z + y;
       *b = z;
       break;

    case 1:
       *r = v - y;
       *g = v;
       *b = z;
       break;

    case 2:
       *r = z;
       *g = v;
       *b = z + y;
       break;

    case 3:
       *r = z;
       *g = v - y;
       *b = v;
       break;

    case 4:
       *r = z + y;
       *g = z;
       *b = v;
       break;

    case 5:
       *r = v;
       *g = z;
       *b = v - y;
       break;
      }
   }
}


/* rgb_to_hsv:
 *  Converts an RGB value into the HSV colorspace.
 *  Coppied from A4.
 */
void MAS::Color::rgb_to_hsv(int r, int g, int b, float *h, float *s, float *v)
{
   int delta;

   ALLEGRO_ASSERT(r >= 0 && r <= 255);
   ALLEGRO_ASSERT(g >= 0 && g <= 255);
   ALLEGRO_ASSERT(b >= 0 && b <= 255);

   if (r > g) {
      if (b > r) {
         /* b>r>g */
         delta = b-g;
         *h = 240.0f + ((r-g) * 60) / (float)delta;
         *s = (float)delta / (float)b;
         *v = (float)b * (1.0f/255.0f);
      }
      else {
         /* r>g and r>b */
         delta = r - MIN(g, b);
         *h = ((g-b) * 60) / (float)delta;
         if (*h < 0.0f)
            *h += 360.0f;
         *s = (float)delta / (float)r;
         *v = (float)r * (1.0f/255.0f);
      }
   }
   else {
      if (b > g) {
         /* b>g>=r */
         delta = b-r;
         *h = 240.0f + ((r-g) * 60) / (float)delta;
         *s = (float)delta / (float)b;
         *v = (float)b * (1.0f/255.0f);
      }
      else {
         /* g>=b and g>=r */
         delta = g - MIN(r, b);
         if (delta == 0) {
            *h = 0.0f;
            if (g == 0)
               *s = *v = 0.0f;
            else {
               *s = (float)delta / (float)g;
               *v = (float)g * (1.0f/255.0f);
            } 
         }
         else {
            *h = 120.0f + ((b-r) * 60) / (float)delta;
            *s = (float)delta / (float)g;
            *v = (float)g * (1.0f/255.0f);
         }
      }
   }
}


void MAS::Color::InitBasicColors()
{
   MAS::Color::white       = al_map_rgb(255,255,255);
   MAS::Color::black       = al_map_rgb(  0,  0,  0);
   MAS::Color::red         = al_map_rgb(255,  0,  0);
   MAS::Color::green       = al_map_rgb(  0,255,  0);
   MAS::Color::blue        = al_map_rgb(  0,  0,255);
   MAS::Color::cyan        = al_map_rgb(  0,255,255);
   MAS::Color::magenta     = al_map_rgb(255,  0,255);
   MAS::Color::yellow      = al_map_rgb(255,255,  0);
   MAS::Color::darkgray    = al_map_rgb( 96, 96, 96);
   MAS::Color::gray        = al_map_rgb(128,128,128);
   MAS::Color::lightgray   = al_map_rgb(192,192,192);
   MAS::Color::orange      = al_map_rgb(255,170,  0);
   MAS::Color::brown       = al_map_rgb(150, 96,  0);
   MAS::Color::purple      = al_map_rgb(144,  0,168);
   MAS::Color::maroon      = al_map_rgb(128,  0,  0);
   MAS::Color::darkgreen   = al_map_rgb(  0,128,  0);
   MAS::Color::darkblue    = al_map_rgb(  0,  0,128);
   MAS::Color::transparent = al_map_rgba(0, 0, 0, 0);
}
