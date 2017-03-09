/*
Simple program for approximating value of pi using the midpoint rule
*/
#include<stdio.h>
#include<math.h>

#define A 0
#define B 1
#define N 10

//fcalc
double fcalc(double x){
	double f;
	f = sqrt(1-x*x);
	printf("%.03lf\t%.03lf\n",x,f);
	return f;
}

//main function
int main(){
	double dx,x,fsum;
	int k;
	
	dx = (B-A)/N;
	
	for(k=1;k<N;k++){
		x = A+(k-0.5)*dx;
		fsum = fsum+(dx*fcalc(x));
	}
	printf("%.03lf",fsum);
	return 0;
}

