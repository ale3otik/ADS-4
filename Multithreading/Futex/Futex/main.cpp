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
#include "futex.hpp"
using std::vector;
using std::thread;
const int nthreads = 10;
void check_lock(futex * f , int ind) {
    f->lock();
    std::cout << "I'm thread number " << ind << "\n";
    f->unlock();
}

void check_try_lock(futex * f , int ind) {
    if(f->try_lock()) {
        std::cout << "I CAN !! " << ind << "\n";
        f->unlock();
    }
}
int main() {
    futex f;
    vector<thread> th;
    for(int i = 0 ;  i < nthreads ;++ i) {
        th.push_back(thread(check_lock , &f , i));
    }
    
    for(auto & x : th) {
        x.join();
    }
    
    std::cout<< "_____________" << std::endl;
    th.clear();
    for(int i = 0 ;  i < nthreads ;++ i) {
        th.push_back(thread(check_try_lock , &f , i));
    }
    
    for(auto & x : th) {
        x.join();
    }
    std::cout << std::endl;
    return 0;
}

