// ************************************************************************** //
//         MASkinG - Miran Amon's Skinnable GUI Library for Allegro           //
//    Copyright (c) 2002-2005 Miran Amon (miranamon@users.sourceforge.net)    //
//          Project websites: http://ferisrv5.uni-mb.si/~ma0747               //
//                  http://sourceforge.net/projects/masking                   //
// ************************************************************************** //

#ifndef      MASKING_ERROR_H
#define      MASKING_ERROR_H

#include "maskingdll.h"

namespace MAS {
/**
   Represents an error code functions can return and you can check.
   It contains some most common error codes but you can easily derive your own
   error class with your own errors. This class is most often used for checking
   whether installing some module or MASkinG itself was successfull or for
   checking if loading data has failed or something.
*/
class MASKING_DLL_DECLSPEC Error {
   public:
      /**
         The type of the error.
         An Error can be one of the following values:
<PRE>
   NONE            - there was no error
   MEMORY          - ran out of memory
   VIDEOMEMORY     - ran out of video memory
   SKIN_INI        - can't find a config file (skin)
   SKIN_DAT        - can't find or load a datafile (skin)
   NO_FILE         - file doesn't exist
   LOAD_FILE       - can't load a file
   INDEX           - wrong index for accessing an array or vector
   UNKNOWN         - an undetermined error (any values above UNKNOWN can be used by derived error classes)
</PRE>
      */
      enum Type {
         NONE,
         MEMORY,
         VIDEOMEMORY,
         SKIN_INI,
         SKIN_DAT,
         NO_FILE,
         LOAD_FILE,
         INDEX,
         UNKNOWN
      };

   protected:
      /**
         The actual error code of the Error object.
      */
      Type type;

   public:
      /**
         The constructor for making a new Error object.
         Pass one of the possible error codes as the parameter to create an error.
      */
      Error(Type t = NONE);

      virtual ~Error();

      /**
         Returns a string containing a short description of the error.
         This can be used to print to the screen to inform the user of an error.
         If you derive your own error class you need to implement this function
         like this:
<PRE>
      virtual const char *MyErrror::GetDescription() {
          if (type > UNKNOWN) {
              // return my own error description string
          }
          else {
              return Error::GetDescription();
          }
      }
</PRE>
      */
      virtual const char *GetDescription();

      /**
         Casts an error code to a bool value.
         If the error code represents a fatal error that requires the program to shut
         down this returns true otherwise it returns false. This enables you to do
         something like this:
<PRE>
      Error e = FunctionThatTriesToDoSomething();
      if (e) {
          ShutdownProgram();
      }
</PRE>
      */
      virtual operator bool() const;

      /**
         Compares two error codes.
         Return true if the error codes are the same.
      */
      virtual bool operator==(const Type &t);

      /**
         Compares two error codes.
         Return true if the error codes are different.
      */
      virtual bool operator!=(const Type &t);

      /**
         Prints an error message to the screen by using allegro_message() and exits the program.
         Call this when you catch a fatal error.
      */
      void Report();
};
}

#endif         //MASKING_ERROR_H
