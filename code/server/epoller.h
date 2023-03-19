#ifndef EPOLLER_H
#define EPOLLER_H

#include <sys/epoll.h> //epoll_ctl()
#include <fcntl.h>     // fcntl()
#include <unistd.h>    // close()
#include <assert.h>    // close()
#include <vector>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

class Epoller
{
public:
    Epoller(int maxEvent = 1024);
    ~Epoller();

    bool addFd(int fd, uint32_t events);
    bool modFd(int fd, uint32_t events);
    bool delFd(int fd);

    int wait(int timeoutMs = -1);

    int getEeventFd(size_t i) const;

    uint32_t getEvents(size_t i) const;

private:
    int epollfd_;
    std::vector<struct epoll_event> events_;
};
#endif // EPOLLER_H