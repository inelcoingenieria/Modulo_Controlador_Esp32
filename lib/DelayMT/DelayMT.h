#ifndef DelayMT_h
#define DelayMT_h


#include "Arduino.h" 

class DelayMT  //delay multi tarea
   {  private:
          long M ;   
           int st;
           int timeDelay;
      public:
           DelayMT(int n) ;
 
          void SetDelayMT( int n);

          void resetDelayMT();

          int StDelayMT();
  } ;

#endif
