// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_SIZE_H
#define      MASKING_SIZE_H

#include "maskingdll.h"

namespace MAS {
/**
   Almost exactly the same as Point.
   The only difference is that instead of x and y coordinates this class has
   w and h values representing width and height and there are no operators
   for checking containment in a rectangle.

   @see Point
*/
class MASKING_DLL_DECLSPEC Size {
   private:
      int wide;
      int high;

   public:
      Size(int w=0, int h=0);

      bool operator==(const Size &s) const;
      bool operator!=(const Size &p) const;
      void operator+=(const Size &s) { wide += s.w(); high += s.h(); }
      void operator-=(const Size &s) { wide -= s.w(); high -= s.h(); }
      Size operator+(const Size &s) const {
         Size res;
         res.w(s.w() + wide);
         res.h(s.h() + high);
         return res;
      }
      Size operator-(const Size &s) const {
         Size res;
         res.w(wide - s.w());
         res.h(high - s.h());
         return res;
      }

      int w() const { return wide; }
      int h() const { return high; }
      void w(int v) { wide = v; }
      void h(int v) { high = v; }
};
}

#endif         //MASKING_SIZE_H
