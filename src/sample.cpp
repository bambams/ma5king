////////////////////////////////////////////////////////////////////////////////
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
////////////////////////////////////////////////////////////////////////////////

#include "../include/MASkinG/sample.h"
#include <string.h>

/*
TODO: all of this
*/

MAS::Sample::Sample()
:data(NULL),
   allocated(false)
{
}


MAS::Sample::Sample(int bits, bool stereo, int freq, int len)
   :data(NULL)
{
   //Create(bits, stereo, freq, len);
}


MAS::Sample::Sample(const char *file)
   :data(NULL)
{
   //Load(file);
}


MAS::Sample::Sample(ALLEGRO_SAMPLE *smp) {
   data = smp;
   allocated = false;
}


MAS::Sample::Sample(const Sample &smp) {
   data = smp.data;
   allocated = false;
}


MAS::Sample::~Sample() {
   //Destroy();
}


MAS::Sample& MAS::Sample::Set(ALLEGRO_SAMPLE *smp, bool copy) {
#if 0
   Destroy();
   if (copy && smp) {
      Create(smp->bits, smp->stereo?true:false, smp->freq, smp->len);
      if (data) {
         memcpy(data->data, smp->data, smp->len);
         allocated = true;
      }
      else {
         Create(8,false,11025,0);
         allocated = false;
      }
   }
   else {
      data = smp;
      allocated = false;
   }
   return *this;
#endif
   return *this;
}


MAS::Sample& MAS::Sample::Set(const Sample &smp, bool copy) {
#if 0
   Destroy();
   if (copy && smp) {
      Create(smp.bits(), smp.stereo(), smp.freq(), smp.len());
      if (data) {
         memcpy(data->data, smp.data->data, smp.len());
         allocated = true;
      }
      else {
         Create(8,false,11025,0);
         allocated = false;
      }
   }
   else {
      data = smp.data;
      allocated = false;
   }
   return *this;
#endif
   return *this;
}


MAS::Error MAS::Sample::Load(const char *file) {
#if 0
   if (!file_exists(file, FA_RDONLY | FA_HIDDEN | FA_SYSTEM | FA_ARCH, NULL)) {
      return Error(Error::NO_FILE);
   }

   SAMPLE *tmp = load_sample(file);
   if (!tmp) {
      return Error(Error::LOAD_FILE);
   }

   Destroy();
   data = tmp;
   allocated = true;

   return Error(Error::NONE);
#endif
   return Error(Error::NO_FILE);
}


void MAS::Sample::Save(const char *file) {
#if 0
#if ALLEGRO_SUB_VERSION >= 1
   save_sample(file, data);
#endif
#endif
}


MAS::Sample& MAS::Sample::Create(int bits, bool stereo, int freq, int len) {
#if 0
   if (len > 0) {
      data = create_sample(bits, (int)stereo, freq, len);
      allocated = true;
   }
   else {
      data = NULL;
      allocated = false;
   }
   return *this;
#endif
   return *this;
}


void MAS::Sample::Destroy() {
#if 0
   if (data && allocated) {
      destroy_sample(data);
      data = NULL;
      allocated = false;
   }
#endif
}


MAS::Sample::operator ALLEGRO_SAMPLE*() const {
   return data;
}


MAS::Sample& MAS::Sample::operator=(ALLEGRO_SAMPLE *smp) {
#if 0
   Destroy();
   data = smp;
   allocated = false;
   return *this;
#endif
   return *this;
}


MAS::Sample& MAS::Sample::operator=(const Sample &smp) {
#if 0
   Destroy();
   data = smp.data;
   allocated = false;
   return *this;
#endif
   return *this;
}


int MAS::Sample::Play(int vol, int pan, int freq, bool loop) const {
#if 0
   if (data)
      return play_sample(data, vol, pan, freq, (int)loop);
   else
      return -1;
#endif
   return -1;
}


void MAS::Sample::Adjust(int vol, int pan, int freq, bool loop) const {
   //if (data) adjust_sample(data, vol, pan, freq, loop);
}


void MAS::Sample::Stop() const {
   //if (data) stop_sample(data);
}


int MAS::Sample::bits() const {
/*   if (data)
      return data->bits;
   else*/
      return 0;
}


bool MAS::Sample::stereo() const {
/*   if (data)
      return data->stereo ? true : false;
   else*/
      return false;
}


int MAS::Sample::freq() const {
/*   if (data)
      return data->freq;
   else*/
      return 0;
}


int MAS::Sample::len() const {
/*   if (data)
      return data->len;
   else*/
      return 0;
}
