//
//  main.cpp
//  cacheTest
//
//  Created by Alex on 21.04.16.
//  Copyright © 2016 Alex. All rights reserved.
//

/**
 *
 * Expected results:
 *
 * ping_pong:8.746 (c)
 * NO_ping_pong:2.269 (c)
 *
 **/

#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using std::chrono::system_clock;

#define QUANTITY (int)1e9
#define LEN (int)1e5

void counter(int * p) {
    for(int i = 0; i < QUANTITY; ++i){
        ++ *p;
    }
}

long long no_ping_pong(void) {
    int last = 1e5 - 1;
    int data[LEN];
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = system_clock::now();
    thread td1(counter , data);
    thread td2(counter , data + last);
    td1.join();
    td2.join();
    
    end = system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds> (end-start).count();
}

long long ping_pong(void) {
    int data[LEN];
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = system_clock::now();
    thread td1(counter , data);
    thread td2(counter , data + 1);
    
    td1.join();
    td2.join();
    
    end = system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds> (end-start).count();
}

int main() {
    long long t1 = ping_pong();
    long long t2 = no_ping_pong();
    cout << "\nping_pong:" << (float) t1 / (1000.0) << endl;
    cout << "NO_ping_pong:" << (float) t2 / (1000.0) << endl;
    return 0;
}
