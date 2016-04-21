//
//  mutex_manual_barriers.hpp
//  Futex
//
//  Created by Alex on 21.04.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#ifndef mutex_manual_barriers_hpp
#define mutex_manual_barriers_hpp
#include <thread>
#include <atomic>


class mutex_barrier_weak {
public:
    mutex_barrier_weak();
    void lock();
    void unlock();
    bool try_lock();
private:
    std::atomic<int64_t> cur_thread_id_;
    int64_t get_thread_id_() const;
};

class mutex_barrier_strong {
public:
    mutex_barrier_strong();
    void lock();
    void unlock();
    bool try_lock();
private:
    std::atomic<int64_t> cur_thread_id_;
    int64_t get_thread_id_() const;
};




#endif /* mutex_manual_barriers_hpp */
