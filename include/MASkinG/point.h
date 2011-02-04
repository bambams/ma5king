// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_POINT_H
#define      MASKING_POINT_H

#include "maskingdll.h"

namespace MAS {
class Rect;

/**
   S simple class that holds two integers representing a point in 2D space.
   It also has a number of functions and overloaded operators for manipulating
   point objects.

   @see Size
*/
class MASKING_DLL_DECLSPEC Point {
   protected:
      /**
         The underlaying x coordinate.
      */
      int posx;

      /**
         The underlaying y coordinate.
      */
      int posy;

   public:
      /**
         Default constructor.
      */
      Point(int x=0, int y=0);

      /**   */bool operator==(const Point &p) const;
      /**
         Operators for comparing two points.
      */
      bool operator!=(const Point &p) const;

      /**   */bool operator<(const Rect &r) const;
      /**
         Operators for checking whether the point is inside a rectangle or not.
         The < operator checks the inside of the rectangle only, while <= checks
         for borders as well.
      */
      bool operator<=(const Rect &r) const;

      /**   */void operator+=(const Point &p) { posx += p.x(); posy += p.y(); }
      /**   */void operator-=(const Point &p) { posx -= p.x(); posy -= p.y(); }
      /**   */Point operator+(const Point &p) const {
         Point res;
         res.x(p.x() + posx);
         res.y(p.y() + posy);
         return res;
      }
      /**
         Operators for doing simple vector arithmetic with two points.

         Examples:
<pre>
      Point a(10, 20);
      Point b(30, 40);

      Point c = a + b;   // c = (40, 60)
      Point d = b - a;   // d = (20, 20)
      c += a;            // c = (50, 80)
      c -= b;            // c = (20, 40)
</pre>
      */
      Point operator-(const Point &p) const {
         Point res;
         res.x(posx - p.x());
         res.y(posy - p.y());
         return res;
      }


      /**   */int x() const { return posx; }
      /**   */int y() const { return posy; }
      /**   */void x(int v) { posx = v; }
      /**
         Get and set functions.
      */
      void y(int v) { posy = v; }
};
}

#endif         //MASKING_POINT_H
