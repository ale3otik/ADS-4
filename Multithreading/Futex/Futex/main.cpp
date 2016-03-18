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
#include <array>
#include "futex.hpp"
#include "sync_queue.hpp"

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
    vector<int> thrcount(nthreads , 0);
    vector<thread> threads;
    for(int i = 0 ;  i < nthreads ;++ i) {
        threads.push_back(thread(inc<M>, &mutex , &value , &limit , &thrcount[i]));
    }

    for(auto & x : threads) {
        x.join();
    }
    int64_t sum = 0;
    for(int i = 0; i < nthreads; ++i) {
        sum += thrcount[i];
    }
    
    assert(value == sum && value == limit);
}

void testA() {
    const int64_t limit = (int)1e5;
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
}

template <class T>
void test_queue_thread(SyncQueue<T> * q, int * qnt , int limit) {
    for(int i = 0; i < limit; ++i) {
        int qval = q->getpop();
        *qnt += qval;
    }
}

void test_queue() {
    SyncQueue<int> q;
    const int nthreads = 10;
    const int64_t qsize = 100;
    vector<thread> threads;
    for (int i = 1 ; i < qsize + 1; ++i) {
        q.push(i);
    }

    vector<int> qnt(nthreads);
    for(int i = 0; i < nthreads; ++i) {
        qnt[i] = 0;
        int limit = qsize / nthreads;
        threads.push_back(thread(test_queue_thread<int>, &q, &(qnt[i]), limit));
    }
    
    for(auto & x : threads) {
        x.join();
    }
    
    int64_t sum = 0;
    for(int i = 0; i < nthreads; ++i) {
        sum += qnt[i];
    }
    
//    cout << (qsize * (qsize+1)) / 2 << "\n";

    assert(sum == ((qsize * (qsize+1)) / 2));
}

int main() {
//    testA();
    test_queue();

    
    return 0;
}

