//
//  futex.hpp
//  Futex
//
//  Created by Alex on 16.03.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#ifndef futex_hpp
#define futex_hpp
#include <thread>
#include <atomic>

struct BadUnlockException: public std::exception {
    const char * what() const throw();
};

class futex {
public:
    futex();
    void lock();
    void unlock();
    bool try_lock();
private:
    std::atomic<int> _cur_thread_id;
    int _get_thread_id() const;
};


#endif /* futex_hpp */