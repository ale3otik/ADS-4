//
//  mutex_manual_barriers.cpp
//  Futex
//
//  Created by Alex on 21.04.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#include "mutex_manual_barriers.hpp"
#include "futex.hpp"
#include <sstream>

/*
 *
 * Weak memory barrier
 * use only with volatile!
 *
 */

mutex_barrier_weak::mutex_barrier_weak() {
    cur_thread_id_ = -1;
}

void mutex_barrier_weak::lock() {
    int64_t empty_id = -1;
    int64_t new_thread = get_thread_id_();
    while(!cur_thread_id_.compare_exchange_weak(empty_id, new_thread, std::memory_order_relaxed)) {
        empty_id = -1;
        std::this_thread::yield();
    }
}

bool mutex_barrier_weak::try_lock() {
    int64_t new_thread = get_thread_id_();
    int64_t empty_id = -1;
    if(cur_thread_id_.compare_exchange_strong(empty_id, new_thread, std::memory_order_relaxed)) {
        return true;
    }
    return false;
}

void mutex_barrier_weak::unlock() {
    int64_t new_thread = get_thread_id_();
    if(!cur_thread_id_.compare_exchange_strong(new_thread, -1, std::memory_order_relaxed)) {
        throw new BadUnlockException;
    }
}

inline int64_t mutex_barrier_weak::get_thread_id_() const {
    std::thread::id thread_id = std::this_thread::get_id();
    return *((int *)(&thread_id));
}

/*
 *
 * Strong safe memory barrier
 *
 */

mutex_barrier_strong::mutex_barrier_strong() {
    cur_thread_id_ = -1;
}

void mutex_barrier_strong::lock() {
    int64_t empty_id = -1;
    int64_t new_thread = get_thread_id_();
    while(!cur_thread_id_.compare_exchange_weak(empty_id, new_thread, std::memory_order_acquire)) {
        empty_id = -1;
        std::this_thread::yield();
    }
}

bool mutex_barrier_strong::try_lock() {
    int64_t new_thread = get_thread_id_();
    int64_t empty_id = -1;
    if(cur_thread_id_.compare_exchange_strong(empty_id, new_thread, std::memory_order_acquire)) {
        return true;
    }
    return false;
}

void mutex_barrier_strong::unlock() {
    int64_t new_thread = get_thread_id_();
    if(!cur_thread_id_.compare_exchange_strong(new_thread, -1, std::memory_order_release)) {
        throw new BadUnlockException;
    }
}

inline int64_t mutex_barrier_strong::get_thread_id_() const {
    std::thread::id thread_id = std::this_thread::get_id();
    return *((int *)(&thread_id));
}
