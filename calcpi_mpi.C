/*
Approximating the value of pi using the midpoint rule - MPI
*/

#include<stdio.h>
#include<math.h>
#include<mpi.h>

#define N 16000000
#define ROOT 0

//intergration range
double a=0.0;
double b=1.0;

//main function
int main(int argc, char* argv[]){
    double dx, x;
    double fsum=0.0,local_sum=0.0,pi;
	int i,k;
	int nprocs, myid, ierr;
	MPI_Status status;
	int p_size, p_start, p_end;
	double t_start, t_end, t=0.0;
	int itr=50;

	ierr=MPI_Init(&argc, &argv);
	ierr=MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	ierr=MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	//start iterations to take an average of execution time
	for(k=0;k<itr;k++){
	  pi=0.0;
	  local_sum=0.0;
	  fsum=0.0;
	
		//start time on root process
		if(myid==ROOT){
		  t_start = MPI_Wtime();
		}
		
		//dx has to present on all processes
		dx = (b-a)/N;
		
		//calculate the workload of each processor
		if(myid<(N % nprocs)){
		  p_size = (N/nprocs) + 1;
		}else{
		  p_size = (N/nprocs);
		}
		
		if(myid==ROOT){
		  p_start = 0;
		}else if((myid-1)<(N % nprocs)){
		  p_start = (myid * (N/nprocs)) + myid;
		}else{
		  p_start = (myid * (N/nprocs))+(N % nprocs);
		}

		p_end = p_start + p_size - 1;

		//calculate local sum on each process
		for(i=p_start;i<=p_end;i++){
		  x = a+(i+0.5)*dx;
		  local_sum = local_sum + (dx*sqrt(1-x*x));
		}

		//reduction operation to calculate global sum
		ierr=MPI_Reduce(&local_sum, &fsum, 1, MPI_DOUBLE, MPI_SUM, ROOT,MPI_COMM_WORLD);

		if(myid==ROOT){
		  pi = 4*fsum;

		  //stop time
		  t_end = MPI_Wtime();
		  //add time at every iteration
		  t = t + (t_end - t_start);
		}
	}

	if(myid==ROOT){
  	printf("time = %.12lf\t%.03lf\t%d\n", t/itr,pi,N%nprocs);
	}

	ierr=MPI_Finalize();

	return 0;
}

