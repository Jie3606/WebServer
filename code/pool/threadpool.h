#ifndef THREADPOLL_H
#define THREADPOLL_H

#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <functional>
#include <future>

class ThreadPoll
{
public:
    ThreadPoll(int maxSize = 8) : poll(std::make_shared<Poll>())
    {
        for (int i = 0; i < maxSize; i++)
        {
            std::thread([poll_ = poll]
                        {
                std::unique_lock<std::mutex> lock(poll_->mtx);
                while(true)
                {
                    if(!poll_->task.empty())
                    {
                        auto task = std::move(poll_->task.front());
                        poll_->task.pop();
                        lock.unlock();
                        task();
                        lock.lock();
                    }
                    else if(poll_->isClosed) break;
                    else poll_->cv.wait(lock);
                } })
                .detach();
        }
    }
    ThreadPoll() = default;
    ThreadPoll(ThreadPoll &&) = default;
    ~ThreadPoll()
    {
        if (poll)
        {
            std::unique_lock<std::mutex> lock(poll->mtx);
            poll->isClosed = true;
        }
        poll->cv.notify_all();
    }
    template <class _Ty>
    void addTask(_Ty &&task)
    {
        {
            std::lock_guard<std::mutex> lock(poll->mtx);
            poll->task.emplace(std::forward<_Ty>(task));
        }
        poll->cv.notify_one();
    }

private:
    struct Poll
    {
        std::mutex mtx;
        std::condition_variable cv;
        std::queue<std::function<void()>> task;
        bool isClosed;
    };
    std::shared_ptr<Poll> poll;
};

#endif