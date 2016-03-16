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
    int64_t result_time[4];
    const int64_t limit = (int)1e7;
    int64_t nthreads[] = {std::thread::hardware_concurrency() / 2 , std::thread::hardware_concurrency() * 2};
    std::chrono::time_point<std::chrono::system_clock> start, end;
    for(int i = 0; i < 2 ; ++i){
        start = std::chrono::system_clock::now();
        check<futex>(limit, nthreads[i]);
        end = std::chrono::system_clock::now();
        result_time[i] = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    }
    
    for(int i = 2; i < 4 ; ++i){
        start = std::chrono::system_clock::now();
        check<std::mutex>(limit, nthreads[i%2]);
        end = std::chrono::system_clock::now();
        result_time[i] = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    }
    
    cout << "hardware_concurrency : " << std::thread::hardware_concurrency() << "\n";
    cout << "limit : " << limit << "\n\n";
    
    for(int i = 0; i < 4; ++i){
        cout << (i < 2 ? "futex" : "mutex" ) <<" nthreads :" << nthreads[i%2] << " time : " << result_time[i] << "ms" << endl;
    }
    
    return 0;
}

