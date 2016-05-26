//
//  ThreadPool.hpp
//  RayTracing
//
//  Created by Alex on 26.05.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#ifndef ThreadPool_h
#define ThreadPool_h
#include <iostream>
#include <queue>
#include <thread>
#include <condition_variable>
#include <vector>
#include <mutex>
#include <exception>
#include <memory>

using std::queue;
using std::mutex;
using std::condition_variable;
using std::exception;
using std::thread;

template <class T>
class sync_queue {
public:
    sync_queue(unsigned int capacity);
    sync_queue(const sync_queue<T> &) = delete;
    void push(T item);
    bool pop(T& item);
    void shutdown();
    
private:
    unsigned int capacity_;
    queue<T> queue_;
    
    mutex mutex_;
    condition_variable queue_full_;
    condition_variable queue_empty_;
    bool is_shutdown_;
};

class shutdownException: public exception{
    virtual const char* what() const throw() {
        return "shutdownException";
    }
};

template<class T>
sync_queue<T>::sync_queue(unsigned int capacity):
capacity_(capacity),
is_shutdown_(false) {}

template<class T>
void sync_queue<T>::push(T item) {
    std::unique_lock<mutex> lock(mutex_);
    queue_full_.wait(lock, [this] () {return queue_.size() != capacity_ || is_shutdown_; });
    
    if (is_shutdown_) throw shutdownException();
    
    queue_.push(move(item));
    queue_empty_.notify_one();
}

template<class T>
bool sync_queue<T>::pop(T& item) {
    std::unique_lock<mutex> lock(mutex_);
    queue_empty_.wait(lock, [this] () {return queue_.size() != 0 || is_shutdown_; });
    if (is_shutdown_ && queue_.empty()) return false;
    item = move(queue_.front());
    queue_.pop();
    queue_full_.notify_one();
    return true;
}

template<class T>
void sync_queue<T>::shutdown(){
    is_shutdown_ = true;
    queue_empty_.notify_all();
    queue_full_.notify_all();
}

/*****************THREAD POOOL*************************/
template<class T>
class thread_pool {
public:
    thread_pool(size_t threadsNumber = std::thread::hardware_concurrency());
    thread_pool(const thread_pool<T>& threadPool) = delete;
    void submit(std::function<T()> functionToPerform);
    void shutdown();
private:
    bool is_working_;
    size_t thread_qnt_;
    std::vector<thread> workers_;
    sync_queue<std::function<T()> > queue_;
    
};


template<class T>
thread_pool<T>::thread_pool(size_t threadsNumber): queue_(-1){
    is_working_ = true;
    thread_qnt_ = threadsNumber;
    if (thread_qnt_ == 0) thread_qnt_ = std::thread::hardware_concurrency();
    std::cout << thread_qnt_ <<std::endl;
    auto worker = [this] () -> void {
        std::function<T()> task;
        
        while(queue_.pop(task)){
            task();
        }
    };
    
    for (size_t i = 0; i < threadsNumber; ++i)
        workers_.emplace_back(thread(worker));
}


template<class T>
void thread_pool<T>::submit(std::function<T()> taskFunction) {
    if (!is_working_) throw shutdownException();
    std::function<T()> task(taskFunction);
    queue_.push(move(task));
}

template<class T>
void thread_pool<T>::shutdown(){
    queue_.shutdown();
    is_working_ = false;
    for (size_t i = 0; i < workers_.size(); ++i){
        if (workers_[i].joinable())
            workers_[i].join();
    }
}

#endif /* ThreadPool_h */
