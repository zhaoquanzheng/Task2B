//Task2B
//Zhao Zheng and Gilbert Choi

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "mandelbrot.h"
#include <math.h>

#define MAX_ITERATIONS 256

typedef struct _complexNumber complex;
struct _complexNumber{
   double real;
   double imaginary;
};

//complex add (complex a, complex b);
//complex multiply (complex a, complex b);

int escapeSteps(double x,double y){
   int n = 0;
   double distance = 0;
   complex z;
//   c.real = x;
//   c.imaginary = y;
   z.real = 0;
   z.imaginary = 0;
//   distance = sqrt(x*x + y*y);  
/*   if(distance < 2){
      n++;
   }
*/
   printf("%lf\n",distance);
   double a,b;
   while (distance < 2 && n < MAX_ITERATIONS) {
//      printf("the point is at %lf,%lf, the mod is %ld\n",z.real,z.imaginary,distance);
//      printf("c is at (%lf,%lf)\n",c.real,c.imaginary);
//      printf("n = %d, ",n);
      a = z.real;
      b = z.imaginary;
      z.real = a*a - b*b;
      z.imaginary = (2*a)*b;
      z.real   += x;
      z.imaginary+=y;
      distance = sqrt( (z.real * z.real) + (z.imaginary * z.imaginary));
      printf("%lf,%lf\ndistance: %lf\n",z.real,z.imaginary,distance);
      n++;
   }
   printf("assert case:%lf,%lf, returning:%d",x,y,n);
   return n;
      
}
/*
complex add (complex a, complex b) {
   complex sum;
   sum.real = a.real + b.real;
   sum.imaginary = a.imaginary + b.imaginary;
   return sum;
}

complex multiply (complex a, complex b) {
   complex answer;
   answer.real = a.real*b.real - a.imaginary*b.imaginary;
   answer.imaginary = a.real*b.imaginary + a.imaginary*b.real;
   return answer;
}
*/
