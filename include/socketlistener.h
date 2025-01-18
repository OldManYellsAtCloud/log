#ifndef SOCKETLISTENER_H
#define SOCKETLISTENER_H

#include <vector>
#include <cstdint>
#include <thread>
#include <poll.h>
#include "requestprocessor.h"

#define MAX_RESPONSE_SIZE 1024 * 100

class SocketListener
{
private:
    int socketFd;
    RequestProcessor* requestProcessor_;

    struct pollfd pfd[1];

    std::vector<std::thread> clientSocketThreads;
    void clientSocketThread(std::stop_token* st, int clientSocketFd);

    bool isValidRequest(int requestType);
    bool isValidLoggerType(int loggerType);
    int getRequestType(std::vector<uint8_t>& buffer);
    void processRequest(std::vector<uint8_t>& buffer);
    void processNewLoggerRequest(std::vector<uint8_t>& buffer);

public:
    SocketListener(RequestProcessor* requestProcessor);
    void startListening(std::stop_token st);
};

#endif // SOCKETLISTENER_H
