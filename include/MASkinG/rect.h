// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_RECT_H
#define      MASKING_RECT_H

#include "point.h"
#include "size.h"
#include "maskingdll.h"

namespace MAS {
/**
   A simple class that represents a rectangle in 2D space.
   It also has some overloaded operators for manipulating rectangles.

   @see Point, Size
*/
class MASKING_DLL_DECLSPEC Rect {
   protected:
      /**
         Top left point (origin).
      */
      Point origin;

      /**
         The size of the rectangle.
      */
      Size dim;

   public:
      /**
         Default constructor.
      */
      Rect();

      /**
         Constructor for making a rectangle from coordinates of origin, width and height.
      */
      Rect(int x, int y, int w, int h);

      /**
         Constructor for making a rectangle from the origin point and size.
      */
      Rect(const Point &p, const Size &s);

      virtual ~Rect();

      /**
         Operator for comparing two Rects.
         Two rectangles are equal if they have the same origin point and size.
      */
      bool operator==(const Rect &r) const;

      bool operator>(const Rect &r) const;
      bool operator>=(const Rect &r) const;
      bool operator>(const Point &p) const;
      bool operator>=(const Point &p) const;

      /** */void operator+=(const Point &p) { origin += p; }
      /** */void operator-=(const Point &p) { origin -= p; }
      /** */Rect operator+(const Point &p) const { return Rect(origin + p, dim); }
      /**
         Operators for doing simple arithmetic with Rects (i.e. moving).
      */
      Rect operator-(const Point &p) const { return Rect(origin - p, dim); }

      /**
         Returns true if this rectangle intersects with the passed rectangle (excluding the edges).
      */
      bool Intersects(const Rect &r) const;

      /** */Point topLeft() const { return origin; }
      /** */Point bottomRight() const { return Point(origin.x() + dim.w(), origin.y() + dim.h()); }
      /** */Size size() const { return dim; }
      /** */int x() const { return origin.x(); }
      /** */int y() const { return origin.y(); }
      /** */int x2() const { return origin.x() + dim.w(); }
      /** */int y2() const { return origin.y() + dim.h(); }
      /** */int w() const { return dim.w(); }
      /** */int h() const { return dim.h(); }
      /** */void topLeft(const Point& p) { origin = p; }
      /** */void bottomRight(const Point& p) { x2(p.x()); y2(p.y()); }
      /** */void size(const Size& s) { dim = s; }
      /** */void x(int v) { origin.x(v); }
      /** */void y(int v) { origin.y(v); }
      /** */void x2(int v) { dim.w(v - origin.x()); }
      /** */void y2(int v) { dim.h(v - origin.y()); }
      /** */void w(int v) { dim.w(v); }
      /**
         Bsic get and set functions.
      */
      void h(int v) { dim.h(v); }
};
}

#endif         //MASKING_RECT
