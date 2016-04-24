//pixelColor.c
//Team Zhao Zheng and Gilbert Choi 
//for Task2B

#include "pixelColor.h"

unsigned char stepsToRed (int steps){
   unsigned char red;
   red = steps%50;
   return red;
}
unsigned char stepsToBlue (int steps){
   unsigned char blue;
   blue = steps%31;
   return blue;
}
unsigned char stepsToGreen (int steps){
   unsigned char green;
   green = 250*(steps%2);
   return green;
}