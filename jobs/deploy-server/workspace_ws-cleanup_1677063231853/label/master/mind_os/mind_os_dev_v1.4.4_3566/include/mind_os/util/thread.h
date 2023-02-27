#pragma once

#include <chrono>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include "rate.h"

namespace mind_os {

namespace util {


class Thread
{

public:
    class Sleeper
    {
        std::mutex mtxCv;
        std::condition_variable cv;
    public:

        template<typename _Rep, typename _Period>
        bool sleep(const std::chrono::duration<_Rep, _Period>& rtime)
        {
            std::cv_status flag;
            std::unique_lock <std::mutex> lck(mtxCv);
            flag = cv.wait_for(lck, rtime);

            return flag == std::cv_status::timeout;
        }

        void interrupt()
        {
            cv.notify_one();
        }
    } ;

    Thread();

    virtual ~Thread();

    static long int getTid();

    void start();

    template<typename _Rep, typename _Period>
    bool sleep(const std::chrono::duration<_Rep, _Period>& rtime)
    {
        return sleeper.sleep(rtime);
    }

    void setRate(double hz);

    bool regularSleep();
    bool sleep();
    bool wakeup();

    void interrupt(bool blocked = true);

    bool isRunning();
    bool isInterrupted();

protected:
    virtual void run() = 0;

private:
    Sleeper sleeper;
    std::thread *th;
    std::atomic_bool running;
    std::atomic_bool interrupted;
    std::shared_ptr<Rate> rate;

    void innerRun();
};

}}
