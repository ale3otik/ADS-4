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
    _cur_thread_id = -1;
}

void futex::lock() {
    int empty_id = -1;
    int new_thread = _get_thread_id();
    while(!_cur_thread_id.compare_exchange_weak(empty_id, new_thread)) {
        empty_id = -1;
        std::this_thread::yield();
    }
}

bool futex::try_lock() {
    int new_thread = _get_thread_id();
    int empty_id = -1;
    if(_cur_thread_id.compare_exchange_weak(empty_id, new_thread)) {
        return true;
    }
    return false;
}

void futex::unlock() {
    int new_thread = _get_thread_id();
    if(!_cur_thread_id.compare_exchange_weak(new_thread, -1)) {
        throw new BadUnlockException;
    }
}

inline int futex::_get_thread_id() const {
    std::stringstream sstream;
    std::string str;
    sstream << std::this_thread::get_id();
    return (int)std::stoll(sstream.str(), nullptr, 16);
}