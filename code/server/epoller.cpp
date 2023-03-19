#include "epoller.h"

Epoller::Epoller(int maxEvent) : epollfd_(epoll_create(maxEvent)), events_(maxEvent)
{
    assert(epollfd_ > 0 && events_.size() > 0);
}
Epoller::~Epoller()
{
    close(epollfd_);
    
}

bool Epoller::addFd(int fd, uint32_t events)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = events;
    int ret = epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &event);
    return ret == 0;
}

bool Epoller::modFd(int fd, uint32_t events)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = events;
    int ret = epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &event);
    return ret == 0;
}
bool Epoller::delFd(int fd)
{
    struct epoll_event event = {0};
    int ret = epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, &event);
    return ret == 0;
}
int Epoller::wait(int timeoutMs = -1)
{
    return epoll_wait(epollfd_, &*events_.begin(), events_.size(), timeoutMs);
}

int Epoller::getEeventFd(size_t i) const
{
    assert(i < events_.size() && i >= 0);
    return events_[i].data.fd;
}

uint32_t Epoller::getEvents(size_t i) const
{
    assert(i < events_.size() && i >= 0);
    return events_[i].events;
}