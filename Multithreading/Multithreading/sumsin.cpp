//
//  sumsin.cpp
//  Multithreading
//
//  Created by Alex on 25.02.16.
//  Copyright © 2016 Alex. All rights reserved.
//
#include <iostream>
#include <mutex>
#include <thread>
#include <cmath>
#include <vector>
#include <chrono>
#include <cassert>

using namespace std;
using chrono::system_clock;

double sum = 0.0;

const int N = 1e8;
const int nthreads = 1e8;
mutex mtx;
#define COUNTSIN
void counter(shared_ptr<vector<double> > v, int l , int r) {
    double ans = 0;
    for(int i = l; i < r + 1; ++i) {
#ifndef COUNTSIN
        ans += (*v)[i];
#else
        ans += sin((double)(*v)[i]);
#endif
    }
    
    mtx.lock();
    sum += ans;
    mtx.unlock();
}

int main() {
    srand(2281488);
    cout.precision(20);
    shared_ptr<vector<double> > v(new vector<double> (N));
    for(int i = 0 ; i < (*v).size(); ++i){
        (*v)[i] = rand();
    }
    
    vector<thread> td(nthreads);
    int len = N / nthreads;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = system_clock::now();
    
    for(int i = 0 ; i < nthreads; ++i) {
        int l = i * len;
        int r = (i != nthreads-1) ? l + len - 1 : N - 1;
        
        td[i] = thread(counter,v,l,r);
    }
    
    for (int i = 0; i < nthreads; ++i) {
        td[i].join();
    }
    
    end = system_clock::now();
    long long timer = std::chrono::duration_cast<std::chrono::milliseconds> (end-start).count();
    
    double smpsum = 0;
    for(int i = 0 ; i < (*v).size(); ++i){
#ifndef COUNTSIN
        smpsum += (*v)[i];
#else
        smpsum += sin((*v)[i]);
#endif
    }

    cout << "simple: " << smpsum << "\ngenerl: "<< sum <<endl;
//    assert(smpsum == sum);
    cout << "\nTIME : " << timer / (1000.0) << endl;
    return 0;
}