//
//  sync_queue.hpp
//  Futex
//
//  Created by Alex on 17.03.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#ifndef sync_queue_hpp
#define sync_queue_hpp

#include <stdio.h>
#include "futex.hpp"
#include <queue>
template<class T>
class SyncQueue {
public:
    SyncQueue();
    void push(T elem);
    void pop();
    T front();
    T back();
    size_t size() const;
private:
    std::queue<T> _q;
    futex _ftx;
    
};

template <class T>
SyncQueue<T>::SyncQueue():
_ftx() {}

template <class T>
void SyncQueue<T>::push(T elem) {
    _ftx.lock();
    _q.push(elem);
    _ftx.unlock();
}

template <class T>
void SyncQueue<T>::pop() {
    _ftx.lock();
    _q.pop();
    _ftx.unlock();
}

template <class T>
T SyncQueue<T>::front() {
    _ftx.lock();
    T & elem =_q.front();
    _ftx.unlock();
    return elem;
}

template <class T>
T SyncQueue<T>::back() {
    _ftx.lock();
    T & elem =_q.back();
    _ftx.unlock();
}

template <class T>
size_t SyncQueue<T>::size() const{
    return _q.size();
}

#endif /* sync_queue_hpp */
