#ifndef SERVER_H
#define SERVER_H

#include <string>

#include "led.h"

class Server
{
    private:
        enum ServerState_
        {
            S_ok_ = 0,
            S_error_ = 1L << 0,
            S_dead_ = 1L << 1,
            S_eof_ = 1L << 1,
        };

        ServerState_ state_;
        std::string fifoIn_;
        std::string fifoOut_;
        int in_;

        static const std::string defaultInFifo;
        static const std::string defaultOutFifo;
        static const int PollTimeout;
        void setState(ServerState_ state);

    public:
        typedef ServerState_ ServerState;
        static const ServerState server_ok = S_ok_;
        static const ServerState server_error = S_error_;
        static const ServerState server_dead = S_dead_;
        static const ServerState server_eof = S_eof_;

        Server();
        Server(const std::string &fifoIn, const std::string &fifoOut_);
        ~Server();

        static void thread(Server &server, Led &led);

        void clear();
        void init();
        void reinit();
        bool isAlive() const;
        bool error() const;
        bool eof() const;
        bool ok() const;
        void stop();
        bool wait();
        int recv(std::string &query);
        void send(const std::string &response);
        ServerState state() const;

};

#endif // SERVER_H

