

#include <iostream>
#include "omp.h"
#include <time.h>
#include <chrono>
#include <cstdlib>
#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

const int N = 4096; 
const int Nthr = 4;
float f(int i) {return(float)i;}

int main(void){
    omp_set_num_threads(Nthr);
    float* a = new float[N];
    float *sumt  = new float[Nthr+1];
    sumt[0] = 0.0;
    float sum = 0.0;

    double rTimeFinal = omp_get_wtime();
    double rTime1 = omp_get_wtime();
    
    /* added*/
    #pragma omp parallel for schedule(static) firstprivate(sum)
    for(int i = 0; i<N; i++){
        sum = rand() % RAND_MAX; 
        a[i] = sum;
        sumt[omp_get_thread_num()+1] = sum;
    }
    rTime1 = omp_get_wtime() - rTime1;

    double rTime2 = omp_get_wtime();
    for (int i = 1; i<Nthr; i++){
        sumt[i] += sumt[i-1];
    }
    rTime2 = omp_get_wtime() - rTime2;

    double rTime3 = omp_get_wtime();
    
    /*add*/
    #pragma omp parallel for schedule(static)
    for(int i=0; i<N; i++){
        a[i] += sumt[omp_get_thread_num()];
    }
    rTime3 = omp_get_wtime() - rTime3;

    rTimeFinal = omp_get_wtime() - rTimeFinal;

    for(int i=0; i<N; i++){
        cout << a[i] << endl;
    }

    cout << "Step 1 runtime: " << rTime1 << "Seconds" << endl;
    cout << "Step 2 runtime: " << rTime2 << "Seconds" << endl;
    cout << "Step 3 runtime: " << rTime3 << "Seconds" << endl;
    cout << "Total runtime: " << rTimeFinal << "Seconds" << endl;
    
    delete[]sumt;
    delete[] a;
    return 0;
}
