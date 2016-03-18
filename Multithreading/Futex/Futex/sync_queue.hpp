//
//  syncq_ueue.hpp
//  Futex
//
//  Created by Alex on 17.03.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#ifndef syncq_queue_hpp
#define syncq_queue_hpp

#include "futex.hpp"
#include <queue>

template<class T>
class SyncQueue {
public:
    SyncQueue();
    void push(const T & elem);
    void pop();
    T getpop();
    size_t size() const;
private:
    std::queue<T> q_;
    std::mutex mtx_;
    std::condition_variable popcv_;
    
};


template <class T>
SyncQueue<T>::SyncQueue():
mtx_() {}

template <class T>
void SyncQueue<T>::push(const T & elem) {
    std::lock_guard<std::mutex> locker(mtx_);
    q_.push(elem);
    popcv_.notify_one();
}

template <class T>
void SyncQueue<T>::pop() {
    std::unique_lock<std::mutex> locker(mtx_);
    while(q_.size() == 0) {
        popcv_.wait(locker);
    }
    q_.pop();
}

template <class T>
T SyncQueue<T>::getpop() {
    std::unique_lock<std::mutex> locker(mtx_);
    while(q_.size() == 0) {
        popcv_.wait(locker);
    }
    
    T result(q_.front());
    q_.pop();
    return result;

}

template <class T>
size_t SyncQueue<T>::size() const{
    return q_.size();
}

#endif /* syncq_queue_hpp */
