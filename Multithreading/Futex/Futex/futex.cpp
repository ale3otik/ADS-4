//  futex.cpp
//  Futex
//
//  Created by Alex on 16.03.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#include "futex.hpp"
#include <sstream>

//- if true, it replaces the contained value with val (like store).
//- if false, it replaces expected with the contained value .

const char *  BadUnlockException::what() const throw() {
    return "Wrong lock-unlock sequence";
}

futex::futex() {
    cur_thread_id_ = -1;
}

void futex::lock() {
    int64_t empty_id = -1;
    int64_t new_thread = get_thread_id_();
    while(!cur_thread_id_.compare_exchange_weak(empty_id, new_thread)) {
        empty_id = -1;
        std::this_thread::yield();
    }
}

bool futex::try_lock() {
    int64_t new_thread = get_thread_id_();
    int64_t empty_id = -1;
    if(cur_thread_id_.compare_exchange_weak(empty_id, new_thread)) {
        return true;
    }
    return false;
}

void futex::unlock() {
    int64_t new_thread = get_thread_id_();
    if(!cur_thread_id_.compare_exchange_weak(new_thread, -1)) {
        throw new BadUnlockException;
    }
}

inline int64_t futex::get_thread_id_() const {
    std::stringstream sstream;
    std::string str;
//    int x = static_cast<int>(std::this_thread::get_id());
    sstream << std::this_thread::get_id();
    return (int64_t)std::stoll(sstream.str(), nullptr, 16);
}