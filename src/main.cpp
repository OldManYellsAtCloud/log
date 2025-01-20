#include "requestprocessor.h"
#include "socketlistener.h"
#include <thread>
#include <signal.h>
#include "enums.h"
using namespace std;

namespace {
std::jthread socketListenerThread;
}

void termination_cleanup(int signum){
    socketListenerThread.request_stop();
}

int main()
{
    struct sigaction cleanup_action;
    cleanup_action.sa_handler = termination_cleanup;
    sigemptyset(&cleanup_action.sa_mask);
    cleanup_action.sa_flags = 0;

    sigaction(SIGINT, &cleanup_action, nullptr);
    sigaction(SIGTERM, &cleanup_action, nullptr);
    sigaction(SIGHUP, &cleanup_action, nullptr);

    RequestProcessor rp{"main_logger"};
    // add a logger for ourselves too
    rp.addLogger("main_logger", logging::LOGGER_FILE);

    SocketListener sl {&rp};

    socketListenerThread = std::jthread(&SocketListener::startListening, &sl);
    socketListenerThread.join();
    return 0;
}
