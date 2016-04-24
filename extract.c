 
  // extract.c
 // funtions and types used to extract x,y,z values from a
 // string containing a url of the form
 // "http://almondbread.cse.unsw.edu.au:7191/tile_x3.14_y-0.141_z5.bmp"
 // initially by richard buckland
 // 13 April 2014
 // your name here: Gilbert Choi, Zhao Zheng
  
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <assert.h>
 #include "extract.h"
 
 int main () {

     char * message = "http://almondbread.cse.unsw.edu.au:7191/tile_x3.14_y-0.141_z5.bmp";

     triordinate dat = extract (message);
     
     printf ("dat is (%lf %lf %d)\n", dat.x, dat.y, dat.z);
    
     assert (dat.x == 3.14);
     assert (dat.y == -0.141);
     assert (dat.z == 5);
     printf("All tests passed\n");
  
     return EXIT_SUCCESS;
 }
 
 
triordinate extract (char *message){
   char* stringX;
   char* stringY;
   char* stringZ;
   
   sscanf(message,"http://almondbread.cse.unsw.edu.au:7191/tile_x%s_y%s_z%s.bmp",stringX,stringY,stringZ);
   printf("%s\n",stringX);
   triordinate result;
   result.x = myAtoD(stringX);
   result.y = myAtoD(stringY);
   result.z = myAtoL(stringZ);
  // printf("%lf,%lf,%d\n",stringX,stringY,stringZ);
   return result;
}






double myAtoD (char *message){
   char* decimalValues;
   char* unitValues;
   sscanf(message,"%s.%s",&decimalValues,&unitValues);
   double dubb;
   int i = strlen(decimalValues);
   while (i>0){
      dubb = decimalValues[i] - '0';
      dubb /=10;
      i--;
   }
   i = 0;
   double tops;
   while (unitValues[i] != NULL){
      tops = tops*10 + unitValues[i] - '0';
      i++;
   }
   dubb += tops;
   return dubb;
}

long myAtoL (char *message){
   long number=0;
   int i = 0;
   int placevalue = 0;
   while(message[i]!= '\0'){
      if(message[i]>= '0' && message[i]<='9'){
         number = number*10 + message[i];
         i++;
      }
      if(message[i] == '.'){
         i++;
         placevalue = 0;
      }
   }
   if(placevalue!=0){
      number = number / (placevalue*10); 
   }
   return number;
}

