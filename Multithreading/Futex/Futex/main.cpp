//
//  main.cpp
//  Futex
//
//  Created by Alex on 16.03.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <cassert>
#include <chrono>
#include "futex.hpp"
using std::vector;
using std::thread;
using std::cout;
using std::endl;

template <class M>
void inc(M * mutex, int * value , int * limit , int * thrcount) {
    while(*value < *limit) {
        mutex->lock();
        if(*value < *limit) {
            ++*value;
            ++*thrcount;
        }
        mutex->unlock();
    }
}

template <class M>
void check(int limit , int64_t nthreads) {
    M mutex;
    int value = 0;
    int thrcount = 0;
    vector<thread> threads;
    for(int i = 0 ;  i < nthreads ;++ i) {
        threads.push_back(thread(inc<M>, &mutex , &value , &limit , &thrcount));
    }

    for(auto & x : threads) {
        x.join();
    }
    assert(value == thrcount && value == limit);
}

int main() {

    const int64_t limit = (int)1e7;
    const int ntests = 10;
    
    vector<int64_t> nthreads(ntests);
    vector<int64_t> ftime(ntests);
    vector<int64_t> mtime(ntests);
    
    for(int i = 0; i < ntests; ++i) {
        nthreads[i] = i + 1;
    }
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    for(int test = 0; test < ntests; ++test){
            start = std::chrono::system_clock::now();
            check<futex>(limit, nthreads[test]);
            end = std::chrono::system_clock::now();
            ftime[test] = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    
            start = std::chrono::system_clock::now();
            check<std::mutex>(limit, nthreads[test]);
            end = std::chrono::system_clock::now();
            mtime[test] = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    }
    
    cout << "hardware_concurrency : " << std::thread::hardware_concurrency() << "\n";
    cout << "limit : " << limit << "\n\n";
    
    for(int i = 0; i < ntests; ++i){
        cout << "nthreads :"  << nthreads[i] << "\n";
        cout << ("   futex" ) <<" time : " << ftime[i] << "ms" << endl;
        cout << ("   mutex" ) <<" time : " << mtime[i] << "ms" << endl;
        cout <<  "   f/m ratio <" << ftime[i] / (float)mtime[i] << ">" << endl;

    }
    
    return 0;
}

