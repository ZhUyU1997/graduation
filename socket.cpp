#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>

#include <iostream>
#include <string>

#define MAX_EVENTS 10

class SocketServer
{
private:
    int socketFd = -1;
    int epollFd = -1;

public:
    SocketServer(std::string ip, uint16_t port)
    {
        socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        struct sockaddr_in serv_addr = {0};
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
        serv_addr.sin_port = htons(port);
        int ret = bind(socketFd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

        if (ret)
        {
            printf("Failed to bind port %d! ERR(%d)\n", port, ret);
            exit(1);
        }
    }
    ~SocketServer()
    {
        close(epollFd);
        close(socketFd);
    }
    void listen(int n)
    {
        ::listen(socketFd, n);
    }
    bool select(int sec = -1)
    {
        fd_set rfds;
        struct timeval tv;
        int retval;

        FD_ZERO(&rfds);
        FD_SET(socketFd, &rfds);

        tv.tv_sec = sec;
        tv.tv_usec = 0;

        retval = ::select(socketFd + 1, &rfds, NULL, NULL, (sec == -1) ? NULL : &tv);

        if (retval < 0)
        {
            perror("select()");
            return false;
        }
        else if (retval)
            return true;
        else
            return false;
    }

    bool epoll_init()
    {
        struct epoll_event ev;

        epollFd = epoll_create1(0);
        if (epollFd == -1)
        {
            perror("epoll_create1");
            return false;
        }

        ev.events = EPOLLIN;
        ev.data.fd = socketFd;
        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, socketFd, &ev) == -1)
        {
            perror("epoll_ctl: listen_sock");
            return false;
        }
        return false;
    }

    bool epoll_add(int fd)
    {
        struct epoll_event ev;

        ev.events = EPOLLIN | EPOLLRDHUP;
        ev.data.fd = fd;
        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &ev) == -1)
        {
            perror("epoll_ctl: listen_sock");
            return false;
        }
        return true;
    }

    bool epoll_del(int fd)
    {
        struct epoll_event ev;
        ev.data.fd = fd;
        if (epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, &ev) == -1)
        {
            perror("epoll_ctl: listen_sock");
            return false;
        }
        return true;
    }
    void epoll_loop()
    {
        struct epoll_event events[MAX_EVENTS];
        epoll_init();
        for (;;)
        {
            int nfds = epoll_wait(epollFd, events, MAX_EVENTS, -1);
            if (nfds == -1)
            {
                perror("epoll_wait");
                return;
            }
            for (int n = 0; n < nfds; ++n)
            {
                struct epoll_event ev = events[n];
                if (ev.data.fd == socketFd)
                {
                    int clnt_sock = this->accept();
                    epoll_add(clnt_sock);
                }
                else
                {
                    if (ev.events & EPOLLRDHUP)
                    {
                        printf("[-]Close\n");
                        close(ev.data.fd);
                    }
                    else if (ev.events & EPOLLIN)
                    {
                        char buf[100] = {0};
                        int n = read(ev.data.fd, buf, 100);
                        std::cout << buf << '\n';
                    }
                }
            }
        }
    }

    std::string getIP(struct sockaddr_in addr)
    {
        return inet_ntoa(addr.sin_addr);
    }

    int accept()
    {
        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_size = sizeof(clnt_addr);
        int clientFd = ::accept(socketFd, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        std::cout << "[+]Accept:" << getIP(clnt_addr) << ":" << clnt_addr.sin_port << '\n';
        return clientFd;
    }
};

int main()
{
    SocketServer server("127.0.0.1", 62345);
    server.listen(20);
    server.epoll_loop();

    return 0;
}
