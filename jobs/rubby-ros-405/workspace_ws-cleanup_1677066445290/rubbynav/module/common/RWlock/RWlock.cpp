//
// Created by lcy on 21-6-15.
//
#include "RWlock.h"

RWLock::RWLock(bool writeFirst):
        WRITE_FIRST(writeFirst),
        m_write_thread_id(),
        m_lockCount(0),
        m_writeWaitCount(0){
}

int RWLock::readLock() {
    if (this_thread::get_id() != this->m_write_thread_id) {
        int count;
        if (WRITE_FIRST)
            do {
                while ((count = m_lockCount) == WRITE_LOCK_STATUS || m_writeWaitCount > 0);
            } while (!m_lockCount.compare_exchange_weak(count, count + 1));
        else
            do {
                while ((count = m_lockCount) == WRITE_LOCK_STATUS);
            } while (!m_lockCount.compare_exchange_weak(count, count + 1));
    }
    return m_lockCount;
}

int RWLock::readUnlock() {
    if (this_thread::get_id() != this->m_write_thread_id)
        --m_lockCount;
    return m_lockCount;
}

int RWLock::writeLock() {
    if (this_thread::get_id() != this->m_write_thread_id) {
        ++m_writeWaitCount;
        for(int zero = FREE_STATUS;!this->m_lockCount.compare_exchange_weak(zero,WRITE_LOCK_STATUS);zero = FREE_STATUS);
        --m_writeWaitCount;
        m_write_thread_id=this_thread::get_id();
    }
    return m_lockCount;
}

int RWLock::writeUnlock(){
    if(this_thread::get_id() != this->m_write_thread_id){
        throw runtime_error("writeLock/Unlock mismatch");
    }
    assert(WRITE_LOCK_STATUS == m_lockCount || FREE_STATUS == m_lockCount);
    m_write_thread_id = NULL_THEAD;
    m_lockCount.store(FREE_STATUS);
    return m_lockCount;
}

const std::thread::id RWLock::NULL_THEAD;

