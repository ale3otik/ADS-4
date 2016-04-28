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
#include "mutex_manual_barriers.hpp"
#include "sync_queue.hpp"

using std::vector;
using std::thread;
using std::cout;
using std::endl;

#define MUTEX_TEST
//#define QUEUE_TEST


#ifdef MUTEX_TEST
#define MOD ((int)1e9 + 7)
#define QNT ((int)1e4)

inline void change(int * value) {
    *value = (*value * 17 + 967890) % MOD;
    *value = (*value * 51 + 878987) % MOD;
    *value = (*value * 23 + 789098) % MOD;
    *value = (*value * 93 + 10998) % MOD;
    *value = (*value * 12 + 167890) % MOD;
    *value = (*value * 49 + 12123) % MOD;
    *value = (*value * 47 + 123322) % MOD;
    *value = (*value * 12 + 12312444) % MOD;
    *value = (*value * 49 + 12349893) % MOD;
    *value = (*value * 19 + 12323114) % MOD;
}

template <class M>
void inc(M * mutex, int * value , int * limit , int * thrcount) {
    for(int i = 0;i < QNT; ++i) {
        mutex->lock();
        change(value);
        mutex->unlock();
    }
    
}
//    while(*value < *limit) {
//        mutex->lock();
//        if(*value < *limit) {
//            ++*value;
//            ++*thrcount;
//        }
//        mutex->unlock();
//    }
//}

template <class M>
void check(int limit , int64_t nthreads) {
    M mutex;
    int value = 1;
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
    
    int exp = 1;
    
    for(int i = 0; i < nthreads * QNT; ++i) change(&exp);
    assert(value == exp);
//    assert(value == sum && value == limit);
}

void testFutex() {
    const int64_t limit = (int)1e5;
    const int ntests = 10;
    
    vector<int64_t> nthreads(ntests);
    vector<int64_t> ftime(ntests);
    vector<int64_t> mtime(ntests);
    vector<int64_t> strongtime(ntests);
    vector<int64_t> weaktime(ntests);
    
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
        
        start = std::chrono::system_clock::now();
        check<mutex_barrier_weak>(limit, nthreads[test]);
        end = std::chrono::system_clock::now();
        weaktime[test] = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        
        start = std::chrono::system_clock::now();
        check<mutex_barrier_strong>(limit, nthreads[test]);
        end = std::chrono::system_clock::now();
        strongtime[test] = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    }
    
    cout << "hardware_concurrency : " << std::thread::hardware_concurrency() << "\n\n";
    cout << "limit : " << limit << "\n\n";
    
    for(int i = 0; i < ntests; ++i){
        cout << "+ nthreads :"  << nthreads[i] << "\n\n";
        cout << ("        my futex" ) <<" time : " << ftime[i] << "ms\n" << endl;
        cout << ("      std::mutex" ) <<" time : " << mtime[i] << "ms\n" << endl;
        cout << ("    mutex strong" ) <<" time : " << strongtime[i] << "ms\n" << endl;
        cout << ("      mutex weak" ) <<" time : " << weaktime[i] << "ms\n" << endl;
//        cout <<  "   f/m ratio <" << ftime[i] / (float)mtime[i] << ">" << endl;
    }
}

#endif

#ifdef QUEUE_TEST
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

    assert(sum == ((qsize * (qsize+1)) / 2));
    cout << "queue works" << endl;
}
#endif

int main() {
#ifdef MUTEX_TEST
    testFutex();
#endif

#ifdef QUEUE_TEST
    test_queue();
#endif

    
    return 0;
}

