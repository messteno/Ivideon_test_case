#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <poll.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#include "logger.h"
#include "server.h"
#include "parser.h"

const std::string Server::defaultInFifo = "/tmp/l0";
const std::string Server::defaultOutFifo = "/tmp/l1";
const int Server::PollTimeout = 1000;

Server::Server() :
    state_(server_dead),
    fifoIn_(defaultInFifo),
    fifoOut_(defaultOutFifo),
    in_(-1)
{
}

Server::Server(const std::string &fifoIn, const std::string &fifoOut) :
    state_(server_dead),
    fifoIn_(fifoIn),
    fifoOut_(fifoOut),
    in_(-1)
{
}

Server::~Server()
{
    state_ = server_dead;
    if (in_ != -1)
    {
        close(in_);
        in_ = -1;
    }
}

void Server::thread(Server &server, Led &led)
{
    server.clear();
    server.init();

    while (server.isAlive())
    {
        DEBUG(DBG_DEBUG, "Led: " << led);

        bool event = server.wait();
        if (!event)
            continue;

        if (!server.isAlive())
            break;

        std::string query;
        server.recv(query);

        if (server.error() || server.eof())
        {
            server.reinit();
            continue;
        }

        DEBUG(DBG_INFO, "Server recv query: " << query);

        Parser parser(query);
        auto command = parser.parse();

        std::string response;
        if (!command)
            response = Command::FailedResponse;
        else
        {
            command->process(led);
            response = command->response();
        }

        DEBUG(DBG_INFO, "Server response: " << response);
        server.send(response);
    }
}

void Server::init()
{
    if (isAlive())
        return;

    mkfifo(fifoIn_.c_str(), 0666);

    in_ = open(fifoIn_.c_str(), O_RDONLY | O_NONBLOCK);
    if (in_ == -1)
    {
        DEBUG(DBG_CRITICAL, "Cannot open fifo: " << fifoIn_);
        return;
    }
    setState(server_ok);
}

void Server::clear()
{
    setState(server_dead);

    if (in_ != -1)
        close(in_);
    in_ = -1;
}

void Server::stop()
{
    setState(server_dead);
}

void Server::reinit()
{
    clear();
    init();
}

bool Server::isAlive() const
{
    return (this->state() & server_dead) == 0;
}

bool Server::error() const
{
    return (this->state() & server_error) != 0;
}

bool Server::eof() const
{
    return (this->state() & server_eof) != 0;
}

bool Server::ok() const
{
    return (this->state() == server_ok);
}

void Server::setState(ServerState state)
{
    state_ = state;
}

Server::ServerState Server::state() const
{
    return state_;
}

bool Server::wait()
{
    if (!isAlive())
        return false;

    setState(server_ok);

    struct pollfd pfd[1];
    pfd[0].fd = in_;
    pfd[0].events = POLLIN;
    pfd[0].revents = 0;

    int poll_result = poll(pfd, 1, PollTimeout);

    switch (poll_result) 
    {
        case -1:
            setState(server_error);
            return false;
        case 0:
            return false;
    }

    return true;
}

int Server::recv(std::string &query)
{
    unsigned char c;
    int readed = 0;
    while (1)
    {
        int ret = read(in_, &c, 1);
        if (ret == 1)
        {
            readed += 1;
            if (c != '\n')
                query += c;
            else
                break;
        }
        else if (ret == 0)
        {
            setState(server_eof);
            break;
        }
        else if (ret == -1 && errno != EAGAIN)
        {
            setState(server_error);
            DEBUG(DBG_WARN, "Read error: %s", strerror(errno));
            break;
        }
    }
    return readed;
}

void Server::send(const std::string &response)
{
    int out = open(fifoOut_.c_str(), O_WRONLY);
    if (out == -1)
    {
        DEBUG(DBG_ERROR, "Cannot open fifo: %s (%s)", fifoOut_.c_str(),
                                                      strerror(errno));
        return;
    }

    write(out, response.c_str(), response.length());
    write(out, "\n", 1);
    close(out);
}

