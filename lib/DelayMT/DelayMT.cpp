
#include "Arduino.h"
#include "DelayMT.h"
 

DelayMT::DelayMT(int n)  // Constructor
             {M =millis()+n;st=0;timeDelay=n;}

void DelayMT::SetDelayMT( int n)
             {M =millis()+n;st=0;timeDelay=n;}

void DelayMT::resetDelayMT()
             {M =millis()+timeDelay;st=0;}

 int DelayMT::StDelayMT()  //estado del delay
             {
             if(st)return 1;
              
             if(millis()<M)
                return 0;
              else
                {
                st=1;  
                return (1) ;  
                }  
              }             

