// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_SCROLLOBJ_H
#define      MASKING_SCROLLOBJ_H

#include "maskingdll.h"

namespace MAS {
/**
   Implements the basic functionality of a widget that can scroll things.
   That's not a widget that can be scrolled, but a widget that does the
   scrolling! ScrollObject is quite generic so it works with doubles instead
   of ints like most scroll widgets (sliders, scrollers, etc.) so if you
   use it to scroll integer values, you'll have to do a bit of casting.
*/
class MASKING_DLL_DECLSPEC ScrollObject {
   protected:
      /**
         The minimum value the object can scroll to.
      */
      double min;
      /**
         The maximum value the object can scroll to.
      */
      double max;
      /**
         The current position of the scroll object.
      */
      double pos;
      /**
         The increment value of the scroll object.
      */
      double inc;

   public:
      ScrollObject();
      virtual ~ScrollObject();

      // set functions
      /**
         Sets the minimum value to which the object can scroll. Default is 0.
      */
      virtual void SetMin(double min);
      /**
         Sets the maximum value to which the object can scroll. Default is 100.
      */
      virtual void SetMax(double max);
      /**
         A shortcut for SetMin() and SetMax().
      */
      virtual void SetRange(double min, double max);
      /**
         Sets the position of the scroll object. The position is clipped to the
         object's range. Default is 0.
      */
      virtual void SetPosition(double pos);
      /**
         Sets the value by which the object can scroll. Default is 1. Must be positive!
      */
      virtual void SetIncrement(double inc);
      /**
         A shortcut for all of the above functions.
      */
      virtual void Setup(double min, double max, double pos, double inc);
      /**
         Increments the object's position by the increment value.
         The new position is clipped by the maximum value.
      */
      virtual void Increment();
      /**
         Decrements the object's position by the increment value.
         The new position is clipped by the minimum value.
      */
      virtual void Decrement();

      // get functions
      /**
         Returns the minimum value of the scroll object.
      */
      virtual double GetMin();
      /**
         Returns the maximum value of the scroll object.
      */
      virtual double GetMax();
      /**
         Returns the scroll object's range (i.e. max - min).
      */
      virtual double GetRange();
      /**
         Returns the position of the scroll object.
      */
      virtual double GetPosition();
      /**
         Returns the increment value.
      */
      virtual double GetIncrement();
};
}

#endif         //MASKING_SCROLLOBJ_H
