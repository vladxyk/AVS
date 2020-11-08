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
using namespace std;
//./memory_test –m RAM –b 1024| Kb –l 10
//               1  2   3     4  5   6  7

int main(int argc, char *argv[]){
    int n, l;
    struct timespec tw1;
    struct timespec tw2;
    struct timespec tr1;
    struct timespec tr2;

    if (string(argv[5]) == "Kb"){
        n = atoi(argv[4]) * 1024 / sizeof(int);
        l = atoi(argv[7]);
    }

    else if (string(argv[5]) == "Mb"){
        n = atoi(argv[4]) * 1024 * 1024 / sizeof(int);
        l = atoi(argv[7]);
    }

    else{ 
        n = atoi(argv[4]) / sizeof(int);
        l = atoi(argv[6]);
    }
    
    int *arr1 = new int[n];
    int *write = new int[n];
    float *tw = new float[n];
    float *tr = new float[n];
    float *awt = new float[n];
    float *art = new float[n];
    float *wbw = new float[n];
    float *rbw = new float[n];
    float *aew = new float[n];
    float *rew = new float[n];
    float *aer = new float[n];
    float *rer = new float[n];

    for(int i = 0; i < n; i++){
        arr1[i] = rand() % 10;
    }

    //WRITEWRITEWRITEWRITEWRITEWRITE

    int l1 = 1;
    while(l1 <= l){
        clock_gettime(CLOCK_REALTIME, &tw1);
        for(int i = 0; i < n; i++){
            write[i] = arr1[i];
        }
        clock_gettime(CLOCK_REALTIME, &tw2);
        //tw[l1] = (tw2 - tw1);
        tw[l1] = (tw2.tv_nsec - tw1.tv_nsec) / 1000000000.0 + (tw2.tv_sec - tw1.tv_sec);
	    //cout << l1 << ". WriteTime = " << tw[l1] << " s" << endl;
        l1++;
    }
    //cout << endl;
    
    l1 = 1;
    awt[0] = 0;
    int c = 1;
    while(l1<=l){
        awt[l1] = awt[l1-1] + tw[l1];
        l1++;
    }

    l1 = 1;
    while(l1<=l){
        awt[l1] = awt[l1]/c;
        c++;
        l1++;
    }
    
    l1 = 1;
    while(l1 <= l){
        //cout << l1 << ". AWT = " << awt[l1];
        wbw[l1] = (n / awt[l1]) * pow(10,6);
        //cout << "   " << l1 << ". WBW = " << wbw[l1];
        aew[l1] = abs(awt[l1] - tw[l1]);
        //cout << "   " << l1 << ". AE = " << aew[l1];
        rew[l1] = (aew[l1] / awt[l1]) * 100;
        //cout << "   " << l1 << ". RE = " << rew[l1] << endl;  
        l1++;
    }
  
    //READREADREADREADREADREAD

    l1 = 1;
    while(l1 <= l){
        clock_gettime(CLOCK_REALTIME, &tr1);
        for(int i = 0; i<n; i++){
            arr1[i];
        }
        clock_gettime(CLOCK_REALTIME, &tr2);
        tr[l1] = (tr2.tv_nsec - tr1.tv_nsec) / 1000000000.0 + (tr2.tv_sec - tr1.tv_sec);
        //cout << l1 << ". ReadTime = " << tr[l1] << " s" << endl;
        l1++;
    }
    
    art[0] = 0;
    c = 1;
    l1 = 1;
    while(l1<=l){
        art[l1] = art[l1-1] + tr[l1];
        l1++;
    }

    l1 = 1;
    while(l1<=l){
        art[l1] = art[l1]/c;
        c++;
        l1++;
    }

    l1 = 1;
    while(l1 <= l){
        //cout << l1 << ". ART = " << art[l1];
        rbw[l1] = (n / art[l1]) * pow(10,6);
        //cout << "   " << l1 << ". RBW = " << rbw[l1];
        aer[l1] = abs(art[l1] - tr[l1]);
        //cout << "   " << l1 << ". AE = " << aer[l1];
        rer[l1] = (aer[l1] / art[l1]) * 100;
        //cout << "   " << l1 << ". RE = " << rer[l1] << endl;  
        l1++;
    }
    
    ofstream fout(argv[8]);
    fout << "MemoryType;BlockSize;ElementType;BufferSize;LaunchNum;Timer;WriteTime;AverageWriteTime;WriteBandwidth;AbsError(write);RelError(write);ReadTime;AverageReadTime;ReadBandwidth;AbsError(read);RelError(read)";
    fout << endl;
    l1 = 1;
    while(l1 <= l){
        fout << argv[2];
        fout << " ;" << argv[4];
        fout << " ;" << "int";
        fout << " ;" << "1";
        fout << " ;" << l1; 
        fout << " ;" << "clock_gettime()";
        //WRITEWRITEWRITEWRITEWRITE
        fout << " ;" << tw[l1];
        fout << " ;" << awt[l1];
        fout << " ;" << wbw[l1];
        fout << " ;" << aew[l1];
        fout << " ;" << rew[l1];
        //READREADREADREADREADREAD
        fout << " ;" << tr[l1];
        fout << " ;" << art[l1];
        fout << " ;" << rbw[l1];
        fout << " ;" << aer[l1];
        fout << " ;" << rer[l1] << endl;
        l1++;
    }

}

