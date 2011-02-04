// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_ALLEGRO_SAMPLE_H
#define      MASKING_ALLEGRO_SAMPLE_H

#include <allegro5/allegro.h>
#include "error.h"
#include "maskingdll.h"

class ALLEGRO_SAMPLE;

namespace MAS {
/**

   TODO XXX: I've replaced ALLEGRO_SAMPLE with ALLEGRO_ALLEGRO_SAMPLE in this file just to stop
   poping up compiler errors from this file. <milan>


   A wrapper for the Allegro ALLEGRO_SAMPLE structure.
   At the time I wrote this class I thought it was a good idea to have wrappers
   for all the Allegro structures but I have since changed my mind. This class
   however is almost complete and as far as I know it works and since I used it
   throughout the rest of the library I decided to keep it in anyway. I suggest
   you keep using the sample manipulation functions Allegro provides instead of
   learning the interface of this class though, but if you want to use it then
   take a look at the actual header:
   \URL[sample.h]{"../../include/MASkinG/sample.h"}
*/
class MASKING_DLL_DECLSPEC Sample {
   protected:
      ALLEGRO_SAMPLE *data;
      bool allocated;

   public:
      Sample();
      Sample(int bits, bool stereo, int freq, int len);
      Sample(const char *file);
      Sample(ALLEGRO_SAMPLE *smp);
      Sample(const Sample &smp);
      virtual ~Sample();

      Sample& Set(ALLEGRO_SAMPLE *smp, bool copy=false);
      Sample& Set(const Sample &smp, bool copy=false);

      // some handy overloaded operators
      //operator bool() const;               // does sample data exist?
      operator ALLEGRO_SAMPLE*() const;
      Sample& operator=(ALLEGRO_SAMPLE *smp);         // assignment from Allegro sample: makes a pointer!
      Sample& operator=(const Sample &smp);   // assignment from another sample: makes a copy!

      // loading and saving functions
      virtual Error Load(const char *file);
      virtual void Save(const char *file);

      // basic sample manipulation functions
      Sample& Create(int bits, bool stereo, int freq, int len);
      void Destroy();
      int Play(int vol=255, int pan=127, int freq=1000, bool loop=false) const;
      void Adjust(int vol, int pan, int freq, bool loop) const;
      void Stop() const;

      int bits() const;
      bool stereo() const;
      int freq() const;
      int len() const;
};
}

#endif         //MASKING_ALLEGRO_SAMPLE
