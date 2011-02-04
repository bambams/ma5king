// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_BITMAP_H
#define      MASKING_BITMAP_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "color.h"
#include "rect.h"
#include "error.h"
#include "maskingdll.h"

#define _MASKING_BITMAP_STORE_AND_SET_ALL(target)      \
   ALLEGRO_STATE state;                                \
   al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP  \
                        | ALLEGRO_STATE_BLENDER);      \
   al_set_target_bitmap(target);                       \
   al_set_blender(ALLEGRO_ADD,                         \
                  ALLEGRO_ALPHA,                       \
                  ALLEGRO_INVERSE_ALPHA);

#define _MASKING_BITMAP_STORE_AND_SET(target)          \
   ALLEGRO_STATE state;                                \
   al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);\
   al_set_target_bitmap(target);

#define _MASKING_BITMAP_RESTORE al_restore_state(&state);

namespace MAS {
/**
   A wrapper for the ALLEGRO_BITMAP structure.
   It has all the blitting, sprite drawing and primitive drawing functions,
   copy constructors, overloaded assignment operators and a few special GUI
   drawing functions (like drawing a pseudo 3D rectangle for example). At the
   time I wrote this class I thought it was a good idea to have wrappers for
   all the Allegro structures but I have since changed my mind. This class
   however is almost complete (some functions are missing though) and as far
   as I know it works and since I used it throughout the rest of the library
   I decided to keep it in anyway. I suggest you keep using the drawing and
   bitmap manipulation functions Allegro provides instead of learning the
   interface of this class though, but there's no harm using this class so
   if you want take a look at the actual header:
   \URL[bitmap.h]{"../../include/MASkinG/bitmap.h"}
*/
class MASKING_DLL_DECLSPEC Bitmap {
   public:
      // subtype of the bitmap
      enum Type {
         MEMORY,    // memory bitmap
         VIDEO,      // video bitmap
         UNDEFINED
      };

   protected:
      ALLEGRO_BITMAP *data;   // the actual bitmap data
      Type type;
      bool allocated;      // did we allocate the data or did we just make a pointer?
      Size thick;         // size of the central area (for tiled blit)

   public:
      // ctors & dtor
      Bitmap();
      Bitmap(int w, int h, Type t = VIDEO);
      Bitmap(const Size& s, Type t = VIDEO);
      Bitmap(const char *file, Type t = VIDEO);
      Bitmap(ALLEGRO_BITMAP *src);  //make a pointer!
      Bitmap(const Bitmap &src);
      virtual ~Bitmap();

      operator ALLEGRO_BITMAP*() const;
      Bitmap& operator=(ALLEGRO_BITMAP *bmp); // assignment from Allegro bitmap: makes a pointer!
      Bitmap& operator=(const Bitmap &bmp);   // assignment from another bitmap: makes a copy!
      virtual Bitmap& Set(ALLEGRO_BITMAP *bmp, bool copy=false, Type=UNDEFINED);
      virtual Bitmap& Set(const Bitmap &bmp, bool copy=false, Type=UNDEFINED);
      virtual int ThickX() const { return thick.w(); }
      virtual int ThickY() const { return thick.h(); }
      virtual const Size& Thick() const { return thick; }
      virtual void ThickX(int x) { thick.w(x); }
      virtual void ThickY(int y) { thick.h(y); }
      virtual void Thick(const Size &s) { thick = s; }

      // loading and saving functions
      virtual Error Load(const char *file, Type t = VIDEO);
      virtual void Save(const char *file);

      // basic bitmap manipulation functions
      Bitmap&      Create(const Size &s, Type t = VIDEO) { return Create(s.w(), s.h(), t); }
      Bitmap&      Create(int w, int h, Type t = VIDEO);
      Bitmap&      CreateSubBitmap(const Bitmap &parent, const Rect &r) { return CreateSubBitmap(parent, r.x(), r.y(), r.w(), r.h()); }
      Bitmap&      CreateSubBitmap(const Bitmap &parent, int x, int y, int w, int h);
      void      Destroy();
      int   w()      const { return al_get_bitmap_width(data); }
      int   h()      const { return al_get_bitmap_height(data); }
      Size   size()      const { return Size(w(), h()); }
      bool   IsMemory()   const { return al_get_bitmap_flags(data) & ALLEGRO_MEMORY_BITMAP; }
      bool   IsSubBitmap()   const { return al_is_sub_bitmap(data); }
      bool   IsVideo()   const { return !IsMemory(); }
      void   SetClip(const Rect &r)   { SetClip(r.x(), r.y(), r.x2(), r.y2()); }
      void   SetClip(int x1, int y1, int x2, int y2)   {
         _MASKING_BITMAP_STORE_AND_SET(data)
         al_set_clipping_rectangle(x1,y1,x2-x1+1,y2-y1+1);
         _MASKING_BITMAP_RESTORE}
      Rect   GetClip();

      // drawing primitives
      void   Putpixel(const Point &p, const Color &c) {   Putpixel(p.x(), p.y(), c); }
      Color  Getpixel(const Point &p) const { return Getpixel(p.x(), p.y()); }
      void   Line(const Point &p1, const Point &p2, const Color &c) { Line(p1.x(), p1.y(), p2.x(), p2.y(), c); }
      void   Vline(const Point &p, int y2, const Color &c) {   Vline(p.x(), p.y(), y2, c); }
      void   Hline(const Point &p, int x2, const Color &c) {   Hline(p.x(), p.y(), x2, c); }
      void   Rectangle(const Rect &r, const Color &c) {   Rectangle(r.x(), r.y(), r.x2(), r.y2(), c); }
      void   Rectfill(const Rect &r, const Color &c) {   Rectfill(r.x(), r.y(), r.x2(), r.y2(), c); }
      void   Triangle(const Point &p1, const Point &p2, const Point &p3, const Color &c);
      void   Circle(const Point &p, int r, const Color &c) {   Circle(p.x(), p.y(), r, c); }
      void   Circlefill(const Point &p, int r, const Color &c) {   Circlefill(p.x(), p.y(), r, c); }
      void   Ellipse(const Point &p, const Size &s, const Color &c) { Ellipse(p.x(), p.y(), s.w(), s.h(), c); }
      void   Ellipsefill(const Point &p, const Size &s, const Color &c) { Ellipsefill(p.x(), p.y(), s.w(), s.h(), c); }
      void   Arc(const Point &p, int a1, int a2, int r, const Color &c) { Arc(p.x(), p.y(), a1, a2, r, c);}
      /* TODO
      void   Spline(const int points[8], const Color &c) { spline(data, points, c); }
      void   Floodfill(const Point &p, const Color &c) { floodfill(data, p.x(), p.y(), c); }
      */

      void   Putpixel(int x, int y, const ALLEGRO_COLOR &c) {
         _MASKING_BITMAP_STORE_AND_SET_ALL(data)
         al_put_pixel(x+0.5, y+0.5, c);
         _MASKING_BITMAP_RESTORE
      }

      ALLEGRO_COLOR Getpixel(int x, int y) const { return al_get_pixel(data, x, y); }

      void   Line(int x1, int y1, int x2, int y2, const ALLEGRO_COLOR &c) {
         _MASKING_BITMAP_STORE_AND_SET_ALL(data)
         al_draw_line(x1+0.5, y1+0.5, x2+0.5, y2+0.5, c, 1);
         _MASKING_BITMAP_RESTORE
      }

      void   Vline(int x, int y1, int y2, const ALLEGRO_COLOR &c) {
         _MASKING_BITMAP_STORE_AND_SET_ALL(data)
         al_draw_line(x+0.5, y1+0.5, x+0.5, y2+0.5, c, 1);
         _MASKING_BITMAP_RESTORE
      }

      void   Hline(int x1, int y, int x2, const ALLEGRO_COLOR &c) {
         _MASKING_BITMAP_STORE_AND_SET_ALL(data)
         al_draw_line(x1+0.5, y+0.5, x2+0.5, y+0.5, c, 1);
         _MASKING_BITMAP_RESTORE
      }

      void   Rectangle(int x1, int y1, int x2, int y2, const ALLEGRO_COLOR &c) {
         _MASKING_BITMAP_STORE_AND_SET_ALL(data)
         al_draw_rectangle(x1+0.5, y1+0.5, x2+0.5, y2+0.5, c, 1);
         _MASKING_BITMAP_RESTORE
      }

      void   Rectfill(int x1, int y1, int x2, int y2, const ALLEGRO_COLOR &c) {
         _MASKING_BITMAP_STORE_AND_SET_ALL(data)
         al_draw_filled_rectangle(x1+0.5, y1+0.5, x2+0.5, y2+0.5, c);
         _MASKING_BITMAP_RESTORE
      }

      void   Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ALLEGRO_COLOR &c) {
         _MASKING_BITMAP_STORE_AND_SET_ALL(data)
         al_draw_triangle(x1+0.5, y1+0.5, x2+0.5, y2+0.5, x3+0.5, y3+0.5, c, 1);
         _MASKING_BITMAP_RESTORE
      }

      void   Circle(int x, int y, int r, const ALLEGRO_COLOR &c) {
         _MASKING_BITMAP_STORE_AND_SET_ALL(data)
         al_draw_circle(x+0.5, y+0.5, r, c, 1);
         _MASKING_BITMAP_RESTORE
      }

      void   Circlefill(int x, int y, int r, const ALLEGRO_COLOR &c) {
         _MASKING_BITMAP_STORE_AND_SET_ALL(data)
         al_draw_filled_circle(x+0.5, y+0.5, r, c);
         _MASKING_BITMAP_RESTORE
      }

      void   Ellipse(int x, int y, int rx, int ry, const ALLEGRO_COLOR &c) {
         _MASKING_BITMAP_STORE_AND_SET_ALL(data)
         al_draw_ellipse(x+0.5, y+0.5, rx, ry, c, 1);
         _MASKING_BITMAP_RESTORE
      }

      void   Ellipsefill(int x, int y, int rx, int ry, const ALLEGRO_COLOR &c) {
         _MASKING_BITMAP_STORE_AND_SET_ALL(data)
         al_draw_filled_ellipse(x+0.5, y+0.5, rx, ry, c);
         _MASKING_BITMAP_RESTORE
      }

      void   Arc(int x, int y, int a1, int a2, int r, const ALLEGRO_COLOR &c) {
         _MASKING_BITMAP_STORE_AND_SET_ALL(data)
         al_draw_arc(x+0.5, y+0.5, r, a1/255.f*2*ALLEGRO_PI, a2/255.f*2*ALLEGRO_PI, c, 1);
         _MASKING_BITMAP_RESTORE
      }
      /*  TODO
      void   Floodfill(int x, int y, int c) { floodfill(data, x, y, c); }
      */

      // blitting functions
      void   Clear() { al_clear_to_color(MAS::Color::white); }

      void   Clear(const Color &c) {
         _MASKING_BITMAP_STORE_AND_SET_ALL(data)
         al_clear_to_color(c);
         _MASKING_BITMAP_RESTORE
      }

      void   Blit(Bitmap &dest, const Point &pSrc, const Point &pDest, const Size &s) const {
         _MASKING_BITMAP_STORE_AND_SET_ALL(dest)
         al_draw_bitmap_region(data, pSrc.x(), pSrc.y(), s.w(), s.h(), pDest.x(), pDest.y(), 0);
         _MASKING_BITMAP_RESTORE
      }

      void   PlainBlit(Bitmap &dest, const Point &pSrc, const Point &pDest, const Size &s) const {
         _MASKING_BITMAP_STORE_AND_SET_ALL(dest)
         al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
         al_draw_bitmap_region(data, pSrc.x(), pSrc.y(), s.w(), s.h(), pDest.x(), pDest.y(), 0);
         _MASKING_BITMAP_RESTORE
      }

      void   StretchBlit(Bitmap &dest, const Rect &rSrc, const Rect &rDest) const {
         _MASKING_BITMAP_STORE_AND_SET_ALL(dest)
         al_draw_scaled_bitmap(data, rSrc.x(), rSrc.y(), rSrc.w(), rSrc.h(), rDest.x(), rDest.y(), rDest.w(), 
                               rDest.h(), 0);
         _MASKING_BITMAP_RESTORE
      }

      void   Blit(Bitmap &dest, int sx, int sy, int dx, int dy, int w, int h) {
         _MASKING_BITMAP_STORE_AND_SET_ALL(dest)
         al_draw_bitmap_region(data, sx, sy, w, h, dx, dy, 0);
         _MASKING_BITMAP_RESTORE
      }

      void   StretchBlit(Bitmap &dest, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh) {
         _MASKING_BITMAP_STORE_AND_SET_ALL(dest)
         al_draw_scaled_bitmap(data, sx, sy, sw, sh, dx, dy, dw, dh, 0);
         _MASKING_BITMAP_RESTORE
      }

      // special drawing functions
      void   DrawDottedRect(const Rect &r, const Color &c) { DrawDottedRect(r.x(), r.y(), r.x2(), r.y2(), c); }
      void   Draw3DFrame(const Rect &r, const Color &c1, const Color &c2, const Color &c3) { Draw3DFrame(r.x(), r.y(), r.x2(), r.y2(), c1, c2, c3); }
      void   Draw3DCircle(const Point &centre, int r, const Color &c1, const Color &c2, const Color &c3) { Draw3DCircle(centre.x(), centre.y(), r, c1, c2, c3); }
      void   DrawXMark(const Point &centre, int size, const Color &col) { DrawXMark(centre.x(), centre.y(), size, col); }
      void   DrawCheckMark(const Point &origin, const Color &col) { DrawCheckMark(origin.x(), origin.y(), col); }
      void   DrawArrow(const Point &origin, const Color &col, int dir) { DrawArrow(origin.x(), origin.y(), col, dir); }

      void   DrawDottedRect(int x1, int y1, int x2, int y2, const ALLEGRO_COLOR &c);
      void   Draw3DFrame(int x1, int y1, int x2, int y2,
                         const ALLEGRO_COLOR &c1, const ALLEGRO_COLOR &c2, const ALLEGRO_COLOR &c3);
      void   Draw3DCircle(int x, int y, int r,
                          const ALLEGRO_COLOR &c1, const ALLEGRO_COLOR &c2, const ALLEGRO_COLOR &c3);
      void   DrawXMark(int x, int y, int size, const ALLEGRO_COLOR &col);
      void   DrawCheckMark(int x, int y, const ALLEGRO_COLOR &col);
      void   DrawArrow(int x, int y, const ALLEGRO_COLOR &col, int dir);

      void TiledBlit(
         Bitmap &dest,   // destination
         int sx = 0,      // x offset of the source subbitmap
         int sy = 0,      // y offset of the source subbitmap
         int dx = 0,      // destination x
         int dy = 0,      // destination y
         int w = 0,      // blit width (0 ==> w=destination->w)
         int h = 0,      // blit height (0 ==> h=destination->h)
         int countx = 0,   // number of subbitmaps (horizontaly)
         int county = 0   // number of subbitmaps (verticaly)
      );
      void HorizontalTile(Bitmap &dest, int i, int x, int y, int w, int count);
      void VerticalTile(Bitmap &dest, int i, int x, int y, int h, int count);

      static Bitmap null;
};
}

#endif         //MASKING_BITMAP_H
