
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <unordered_map>
#include <fcntl.h>  // fcntl()
#include <unistd.h> // close()
#include <assert.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "epoller.h"

class WebServer
{
public:
    WebServer(
        int port, int trigMode, int timeoutMS, bool OptLinger,
        int sqlProt, const char *sqlUser, const char *sqlPwd,
        const char *dbName, int connPollNum, int threadNum,
        int openLog, int logLevel, int logQueSize);

    ~WebServer();

    void start();

private:
    bool initSocket();
    void initEventMode(int trigMode);
    void addClient(int fd, sockaddr_in addr);

    void dealListen();
    void dealWrite(HttpConn *client);
    void dealRead(HttpConn *client);

    void sendError(int fd, const char *error);
    void extentTime(HttpConn *client);
    void closeConn(HttpConn *client);

    void onRead(HttpConn *client);
    void onWrite(HttpConn *client);
    void onProcess(HttpConn *client);

    static const int MAX_FD = 65536;

    static int setFdNonblock(int fd);

    int port_;
    bool openLinger_;
    int timeoutMS_;
    bool isClose_;
    int listenFd_;
    char *srcDir_;

    uint32_t listenEvent_;
    uint32_t connEvent_;

    std::unique_ptr<Epoller> epoller_;
    std::unique_ptr<ThreadPool> threadPool_;
    std::unique_ptr<HttpConn> users_;
    std::unique_ptr<SqlConnPool> sqlConnPool_;
};

#endif // WEBSERVER_H