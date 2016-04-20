    
     // extract.c
    // funtions and types used to extract x,y,z values from a
    // string containing a url of the form
    // "http://almondbread.cse.unsw.edu.au:7191/tile_x3.14_y-0.141_z5.bmp"
    // initially by richard buckland
    // 13 April 2014
    // your name here: Gilbert Choi
     
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <assert.h>
    #include "extract.h"
     
     remove_zeroes(double *number, char * result, int buf_len);
    int main (int argc, char *argv[]) {
        
        char * message = "http://almondbread.cse.unsw.edu.au:7191/tile_x3.14_y-0.141_z5.bmp";
     
        triordinate dat = extract (message);
     
        printf ("dat is (%lf %lf %d)\n", dat.x, dat.y, dat.z);
       
        assert (dat.x == 3.14);
        assert (dat.y == -0.141);
        assert (dat.z == 5);
     
        return EXIT_SUCCESS;
    }
    
    
   triordinate extract (char *message){
      triordinate input;
      
      sscanf(message,"http://almondbread.cse.unsw.edu.au:7191/tile_x%lf_y%lf_z%d.bmp",&input.x,&input.y,&input.z);
      char test[81];
      
      remove_zeroes(&input.x,test,81);
      remove_zeroes(&input.y,test,81);
      printf("%lf,%lf,%d\n",input.x,input.y,input.z);
      return input;
   }
   
   
   void
remove_zeroes(double *number, char * result, int buf_len)
{ 
    char * pos;
    int len;

    snprintf(result, buf_len, "%lf", *number);
    len = strlen(result);

    pos = result + len - 1;
 #if 0
/* according to Jon Cage suggestion, removing this part */
    while(*div != '.')
        div++;
#endif
    while(*pos == '0')
        *pos-- = '\0';

    if(*pos == '.')
        *pos = '\0';

}
   
   
   
   
   
   double myAtoD (char *message){
      double number =0;
      int i = 0;
      int placevalue = 0;
      while(message[i]!=EOF){
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
   
   long myAtoL (char *message){
      long number=0;
      int i = 0;
      int placevalue = 0;
      while(message[i]!=EOF){
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
