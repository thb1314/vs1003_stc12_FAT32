#include "board_init.h"
#include <STC12C5A60S2.H>

void ZNX_Board_Init()  
{
 int i=0;

 //==================================================================
 P23=P20=0; //SMG-DIN,DZ-DINH,DZ-DINL=0
 for(i=0;i<16;i++) //向数码管、点阵的HC595中写入全0，以防它们乱显
 {
  P21=0; //SCK=0
  P21=1; //SCK=1
 }
 
 P27=0; //SMG-LC=0
 P27=1; //SMG-LC=1
 P34=0; //DZ-LC=0
 P34=1; //DZ-LC=1 
 
 //=============以上代码为ZN-X板上数码管、点阵显示清0===================
}