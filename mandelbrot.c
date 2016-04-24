//Task2B
//Zhao Zheng and Gilbert Choi

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "pixelColor.h"
#include "mandelbrot.h"
#include <math.h>

#define MAX_ITERATIONS 256

//serveBMP defines
#define SIMPLE_SERVER_VERSION 1.0
#define REQUEST_BUFFER_SIZE 1000
#define DEFAULT_PORT 1917
#define NUMBER_OF_PAGES_TO_SERVE 10
// after serving this many pages the server will halt

//chessboard defines
#define BYTES_PER_PIXEL 3
#define BITS_PER_PIXEL (BYTES_PER_PIXEL*8)
#define NUMBER_PLANES 1
#define PIX_PER_METRE 2835
#define MAGIC_NUMBER 0x4d42
#define NO_COMPRESSION 0
#define OFFSET 54
#define DIB_HEADER_SIZE 40
#define NUM_COLORS 0
#define SIZE 512

typedef unsigned char  bits8;
typedef unsigned short bits16;
typedef unsigned int   bits32;

typedef struct _complexNumber complex;
struct _complexNumber{
   double real;
   double imaginary;
};


int escapeSteps(double x, double y);
static complex multiply (complex a, complex b);
//serveBMP
int waitForConnection (int serverSocket);
int makeServerSocket (int portno);
void serveBMP (int socket);
double zoomdivision(int z); //how many decimals should I?

 //serveBMP.c
int main (int argc, char *argv[]) {
      
   printf ("************************************\n");
   printf ("Starting simple server %f\n", SIMPLE_SERVER_VERSION);
   printf ("Serving bmps since 2012\n");   
   
   int serverSocket = makeServerSocket (DEFAULT_PORT);   
   printf ("Access this server at http://localhost:%d/\n", DEFAULT_PORT);
   printf ("************************************\n");
   
   char request[REQUEST_BUFFER_SIZE];
   
   int numberServed = 0;
   while (numberServed < NUMBER_OF_PAGES_TO_SERVE) {
      
      printf ("*** So far served %d pages ***\n", numberServed);
      
      int connectionSocket = waitForConnection (serverSocket);
      // wait for a request to be sent from a web browser, open a new
      // connection for this conversation
      
      // read the first line of the request sent by the browser  
      int bytesRead;
      bytesRead = read (connectionSocket, request, (sizeof request)-1);
      assert (bytesRead >= 0); 
      // were we able to read any data from the connection?
            
      // print entire request to the console 
      printf (" *** Received http request ***\n %s\n", request);
      
      //send the browser a simple html page using http
      printf (" *** Sending http response ***\n");
      
      //only serveBMP if a .bmp request
      double x,y;
      int z;
      if(sscanf(request,"http://almondbread.cse.unsw.edu.au:7191/tile_x%lf_y%lf_z%d.bmp",&x,&y,&z)==3){
         serveBMP(connectionSocket);
      }else{
         //use the tile.js
      }
      //else: I could make it http page of file not found?
      
      // close the connection after sending the page- keep aust beautiful
      close(connectionSocket);
      
      numberServed++;
   } 
   
   // close the server connection after we are done- keep aust beautiful
   printf ("** shutting down the server **\n");
   close (serverSocket);
   
   return EXIT_SUCCESS; 
}

/*   {
     0x42,0x4d,0x5a,0x00,0x00,0x00,0x00,0x00,
     0x00,0x00,0x36,0x00,0x00,0x00,0x28,0x00,
     0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,
     0x00,0x00,0x01,0x00,0x18,0x00,0x00,0x00,
     0x00,0x00,0x24,0x00,0x00,0x00,0x13,0x0b,
     0x00,0x00,0x13,0x0b,0x00,0x00,0x00,0x00,
     0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x07,
     0xff,0x07,0x07,0x07,0x07,0x07,0xff,0x00,
     0x00,0x0e,0x07,0x07,0x07,0x66,0x07,0x07,
     0x07,0x07,0x07,0x00,0x00,0x0d,0x07,0x07,
     0x07,0x07,0x07,0x07,0xff,0xff,0xff,0x00,
     0x00,0x0d};
*/


// start the server listening on the specified port number
int makeServerSocket (int portNumber) { 
   
   // create socket
   int serverSocket = socket (AF_INET, SOCK_STREAM, 0);
   assert (serverSocket >= 0);   
   // error opening socket
   
   // bind socket to listening port
   struct sockaddr_in serverAddress;
   memset ((char *) &serverAddress, 0,sizeof (serverAddress));
   
   serverAddress.sin_family      = AF_INET;
   serverAddress.sin_addr.s_addr = INADDR_ANY;
   serverAddress.sin_port        = htons (portNumber);
   
   // let the server start immediately after a previous shutdown
   int optionValue = 1;
   setsockopt (
      serverSocket,
      SOL_SOCKET,
      SO_REUSEADDR,
      &optionValue, 
      sizeof(int)
   );

   int bindSuccess = 
      bind (
         serverSocket, 
         (struct sockaddr *) &serverAddress,
         sizeof (serverAddress)
      );
   
   assert (bindSuccess >= 0);
   // if this assert fails wait a short while to let the operating 
   // system clear the port before trying again
   
   return serverSocket;
}

// wait for a browser to request a connection,
// returns the socket on which the conversation will take place
int waitForConnection (int serverSocket) {
   // listen for a connection
   const int serverMaxBacklog = 10;
   listen (serverSocket, serverMaxBacklog);
   
   // accept the connection
   struct sockaddr_in clientAddress;
   socklen_t clientLen = sizeof (clientAddress);
   int connectionSocket = 
      accept (
         serverSocket, 
         (struct sockaddr *) &clientAddress, 
         &clientLen
      );
   
   assert (connectionSocket >= 0);
   // error on accept
   
   return (connectionSocket);
}


int escapeSteps(double x,double y){
   int n = 0;
   double distance = 0;
   complex z;
   z.real = 0;
   z.imaginary = 0;

   while (distance < 2 && n < MAX_ITERATIONS) {
      z = multiply(z,z);
      z.real   += x;
      z.imaginary+=y;
      distance = sqrt( (z.real * z.real) + (z.imaginary * z.imaginary));
      n++;
   }
//   printf("assert case:%lf,%lf, returning:%d",x,y,n);
   return n;
      
}

static complex multiply (complex a, complex b) {
   complex answer;
   answer.real = a.real*b.real - a.imaginary*b.imaginary;
   answer.imaginary = a.real*b.imaginary + a.imaginary*b.real;
   return answer;
}


void serveBMP (int socket) {
   char* message;
   
   // first send the http response header
   
   // (if you write stings one after another like this on separate
   // lines the c compiler kindly joins them togther for you into
   // one long string)
   message = "HTTP/1.0 200 OK\r\n"
                "Content-Type: image/bmp\r\n"
                "\r\n";
   printf ("about to send=> %s\n", message);
   write (socket, message, strlen (message));
   
   //getting the coordinate information
   double x,y;
   int z;
   //sscanf(message,"localhost:7191/tile_x%lf_y%lf_z%d.bmp",&x,&y,&z)
   //need to improve this parsing
   x = 0;
   y = 0;
   z = 4;
   // now send the BMP
   unsigned char bmpheader[] = {
     0x42,0x4d,0x36,0x00,0x0c,0x00,0x00,0x00,
     0x00,0x00,0x36,0x00,0x00,0x00,0x28,0x00,
     0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,
     0x00,0x00,0x01,0x00,0x18,0x00,0x00,0x00,
     0x00,0x00,0x24,0x00,0x0c,0x00,0x13,0x0b,
     0x00,0x00,0x13,0x0b,0x00,0x00,0x00,0x00,
     0x00,0x00,0x00,0x00,0x00,0x00      
   };
   write (socket,bmpheader,sizeof(bmpheader));
   
   //now for the real BMP
   int position = 0;
   int row = 0;
   int col = 0;
   double div = zoomdivision(z);
   double absiscae = x;
//   double ordinate = y;//safe storage of the values of the centre
   x = x - 256*div;
   y = y - 256*div;
   while(position <= SIZE*SIZE*BYTES_PER_PIXEL){
      if(col == SIZE -1){
         col = 0;
         row++;
         y+=div;
         x = absiscae;
      }
      col++;
      int steps = escapeSteps(x,y);
      unsigned char bmp[] = {
         stepsToRed(steps),
         stepsToBlue(steps),
         stepsToGreen(steps)
                            };
      write (socket, bmp, sizeof(bmp));
      x+=div;
      
      position++;   
   }
}

double zoomdivision(int zoom){
   //trying to make 2^-zoom
   long double magnification = 1;
   while(zoom >0 ){
      magnification /= 2;
      zoom--;
   }
   double division = magnification;
   //used long double for division to keep dps right
   return division;
}