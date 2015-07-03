#include <string>
#include <thread>

#include "logger.h"
#include "server.h"
#include "parser.h"
#include "led.h"

int main(int argc, char *argv[])
{
    LOGGER_CONF("", Logger::screen_on, DBG_DEBUG, DBG_DEBUG);

    Led led;
    Server server;

    std::thread serverThread(Server::thread, std::ref(server), std::ref(led));
    serverThread.join();

    return 0;
}

