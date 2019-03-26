#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include "connection.h"
#include "event_loop.h"
#include "logger.h"
#include "event.h"

#define mu_malloc malloc
#define mu_free free

static void event_readable_callback(int fd, event* ev, void* arg)
{
    connection* conn = (connection*)arg;
}

static void event_writable_callback(int fd, event* ev, void* arg)
{
    connection* conn = (connection*)arg;
}

connection* connection_create(event_loop* loop, int connfd, connection_callback_pt read_cb)
{
    connection* conn = (connection* )mu_malloc(sizeof(connection));
    if (conn == NULL)  {
        debug_ret("file: %s, line: %d", __FILE__, __LINE__);
        return NULL;
    }

    conn->fd = connfd;
    conn->readable_callback = read_cb;

    event* ev = (event*)event_create(connfd,  EPOLLIN | EPOLLPRI, event_readable_callback, 
                            conn, event_writable_callback, conn);
    if (ev == NULL)  {
        debug_ret("file: %s, line: %d", __FILE__, __LINE__);
        mu_free(conn);
        return NULL;
    }

    event_add_io(loop->epoll_fd, ev);

    return conn;    
}