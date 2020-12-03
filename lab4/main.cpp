#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <threads.h>
#include <fstream>
#include <string>
#include <cassert>
//#define N 5000
using namespace std;

//if N not define ./prog -n [N] -l [l]
//if N difine ./prog -n1 -bs [BLOCKSIZE]
// ./prog -n1 -bs 0

void dgemm(double **a, double **b, double **c, int N){

    for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++){
      		c[i][j] = 0;
      		for (int k = 0; k < N; k++)
        		c[i][j] += a[i][k] * b[k][j];
    	}
  	}
}

void dgemm_opt_1(double **a, double **b, double **c, int N){

    for (int i = 0; i < N; i++){
		for (int k = 0; k < N; k++){
      		c[i][k] = 0;
      		for (int j = 0; j < N; j++)
        		c[i][k] += a[i][j] * b[j][k];
    	}
  	}
}

void dgemm_opt_2(double **a, double **b, double **c, int N, int blocksize){

    for(int jj=0;jj<N;jj+= blocksize){
        for(int kk=0;kk<N;kk+= blocksize){
            for(int i=0;i<N;i++){
                for(int j = jj; j<((jj+blocksize)>N?N:(jj+blocksize)); j++){
                    for(int k = kk; k<((kk+blocksize)>N?N:(kk+blocksize)); k++){
                        c[i][j] += a[i][k]*b[k][j];
                    }
                }
            }
        }
    }
}

int matrix(int N, int blocksize)
{
	int S, i, j;
	double **a, **b, **c;
	
    srand(time(NULL));
	a = new double*[N];
	for (i = 0; i < N; i++){
		a[i] = new double[N];
	}

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			a[i][j] = rand();
		}
	}
/*
	for (int i = 0; i < N; i++){
    	for (int j = 0; j < N; j++){
      		cout << a[i][j] << " ";
		}
    	cout << "\n";
	}

    cout << "B!" << endl;
*/
	b = new double*[N];
	for (i = 0; i < N; i++){
		b[i] = new double[N];
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			b[i][j] = rand();
		}
	}
/*
    for (int i = 0; i < N; i++){
    	for (int j = 0; j < N; j++){
      		cout << b[i][j] << " ";
		}
    	cout << "\n";
	}
*/
	c = new double*[N];
  	for (int i = 0; i < N; i++){ 
    	c[i] = new double[N];
	}

    dgemm(a,b,c, N);
    //dgemm_opt_1(a, b, c, N);
    //dgemm_opt_2(a, b, c, N, blocksize);

/*
	cout << "dgemm" << "\n";
	for (int i = 0; i < N; i++){
    	for (int j = 0; j < N; j++){
      		cout << c[i][j] << " ";
		}
    	cout << "\n";
	}
*/
	return 0;
}

int main(int argc, char* argv[])
{
	struct timespec t1;
    struct timespec t2;
	int blocksize = atoi(argv[5]);
	//int n = atoi(argv[2]);
	if(string(argv[1]) == "-n"){
    	int N = atoi(argv[2]);
		//int *n = new int[N];
		double *time = new double[N];
		int l = atoi(argv[4]);
		int l1 = 1;
		while(l1 <= l){
      //  n[l1] = N;
        	clock_gettime(CLOCK_REALTIME, &t1);
        	matrix(N, blocksize);
        	clock_gettime(CLOCK_REALTIME, &t2);
        	time[l1] = (t2.tv_nsec - t1.tv_nsec) / 1000000000.0 + (t2.tv_sec - t1.tv_sec);
        	//blocksize = blocksize + 500;
        	//cout << "time = " << time[l1] << " s" << endl;
        	N = N + 1;
        	l1++;
    	}
		
		l1 = 1;
		N = atoi(argv[2]);
		ofstream fout(argv[6]);
    	fout << "N;time" << endl;
		while(l1 <= l){
        	fout << N;
        	fout << ";" << time[l1] << endl;
			N = N + 1;
			l1++;
		}
	}
	
	if(string(argv[1]) == "-n1"){

		int N = 1000;
		blocksize = atoi(argv[3]);
		double time;
		if(blocksize == 0){
			clock_gettime(CLOCK_REALTIME, &t1);
        	matrix(N, blocksize);
        	clock_gettime(CLOCK_REALTIME, &t2);
        	time = (t2.tv_nsec - t1.tv_nsec) / 1000000000.0 + (t2.tv_sec - t1.tv_sec);
        	cout << "time = " << time << " s" << endl;
    		
    		ofstream fout(argv[5]);
    		fout << "N;time" << endl;
        	fout << N;
        	fout << ";" << time;
		} else {
			int l = atoi(argv[5]);
			int l1 = 1;
			double *time = new double[N];
			while(l1 <= l){
        		clock_gettime(CLOCK_REALTIME, &t1);
        		matrix(N, blocksize);
        		clock_gettime(CLOCK_REALTIME, &t2);
        		time[l1] = (t2.tv_nsec - t1.tv_nsec) / 1000000000.0 + (t2.tv_sec - t1.tv_sec);
        		cout << "time = " << time[l1] << " s" << endl;
        		blocksize = blocksize + 100;
        		l1++;
    		}

			blocksize = atoi(argv[3]);
			double min_time = time[1];
			for(int i = 1; i < l; i++){
				if(time[i] < min_time){
					min_time = time[i];
					blocksize = blocksize + 100;
				}
			}

			ofstream fout(argv[7]);
			fout << "N;blocksize;time" << endl;
        	fout << N;
			fout << ";" << blocksize;
        	fout << ";" << min_time << endl;
		}
	}
	
	if(string(argv[1]) == "-n2"){
		int N = 2000;
		blocksize = atoi(argv[3]);
		double time;
		if(blocksize == 0){
			clock_gettime(CLOCK_REALTIME, &t1);
        	matrix(N, blocksize);
        	clock_gettime(CLOCK_REALTIME, &t2);
        	time = (t2.tv_nsec - t1.tv_nsec) / 1000000000.0 + (t2.tv_sec - t1.tv_sec);
        	//cout << "time = " << time << " s" << endl;
    		
    		ofstream fout(argv[5], ios_base::app);
    		fout << endl;
        	fout << N;
        	fout << ";" << time;
		} else {
			int l = atoi(argv[5]);
			int l1 = 1;
			double *time = new double[N];
			while(l1 <= l){
        		clock_gettime(CLOCK_REALTIME, &t1);
        		matrix(N, blocksize);
        		clock_gettime(CLOCK_REALTIME, &t2);
        		time[l1] = (t2.tv_nsec - t1.tv_nsec) / 1000000000.0 + (t2.tv_sec - t1.tv_sec);
        		//cout << "time = " << time[l1] << " s" << endl;
        		blocksize = blocksize + 200;
        		l1++;
			}	
			
			blocksize = atoi(argv[3]);
			double min_time = time[1];
			for(int i = 1; i < l; i++){
				if(time[i] < min_time){
					min_time = time[i];
					blocksize = blocksize + 200;
				}
			}
			
			ofstream fout(argv[7], ios_base::app);
        	fout << N;
			fout << ";" << blocksize;
        	fout << ";" << min_time << endl;		
		}
	}
	
	if(string(argv[1]) == "-n5"){
		int N = 5000;
		blocksize = atoi(argv[3]);
		double time;
		if(blocksize == 0){
			clock_gettime(CLOCK_REALTIME, &t1);
        	matrix(N, blocksize);
        	clock_gettime(CLOCK_REALTIME, &t2);
        	time = (t2.tv_nsec - t1.tv_nsec) / 1000000000.0 + (t2.tv_sec - t1.tv_sec);
        	//cout << "time = " << time << " s" << endl;
    		
    		ofstream fout(argv[5], ios_base::app);
    		fout << endl;
        	fout << N;
        	fout << ";" << time;
		} else {
			int l = atoi(argv[5]);
			int l1 = 1;
			double *time = new double[N];

			while(l1 <= l){
        		clock_gettime(CLOCK_REALTIME, &t1);
        		matrix(N, blocksize);
        		clock_gettime(CLOCK_REALTIME, &t2);
        		time[l1] = (t2.tv_nsec - t1.tv_nsec) / 1000000000.0 + (t2.tv_sec - t1.tv_sec);
        		//cout << "time = " << time[l1] << " s" << endl;
        		blocksize = blocksize + 500;
        		l1++;
			}

			blocksize = atoi(argv[3]);
			double min_time = time[1];
			for(int i = 1; i < l; i++){
				if(time[i] < min_time){
					min_time = time[i];
					blocksize = blocksize + 500;
				}
			}
			
			ofstream fout(argv[7], ios_base::app);
        	fout << N;
			fout << ";" << blocksize;
        	fout << ";" << min_time << endl;
		}
	}

}
