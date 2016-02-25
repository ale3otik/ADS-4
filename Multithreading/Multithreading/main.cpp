#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include <chrono>
using namespace std;
using chrono::system_clock;
mutex m;
int ans;

void hello(shared_ptr<vector<int> > v, int i) {
    for(int x = i*100; x < i*100 + 100; ++x) {
        m.lock();
        ans += (*v)[x];
        m.unlock();
    }
}

int main2() {
    thread t[10];
    shared_ptr<vector<int> > v(new vector<int> (1000,1));
    
    time_t tm = system_clock::to_time_t(system_clock::now());
    for (int i = 0 ; i < 10000; ++i) {
        t[i] = thread(hello,v,i);
    }
    
    tm = system_clock::to_time_t(system_clock::now()) - tm;
    
    
    for (int i = 0; i < 10; ++i) {
        t[i].join();
        //        free(t[i]);
    }
    
    cout << ans << endl;
    cout << tm << endl;
    
    // практика  просуммировать синусы и числа параллельно (за log)
    
    // prefix_sum за Log
    // сортировка за линию
    // log ^ k
    return 0;
}