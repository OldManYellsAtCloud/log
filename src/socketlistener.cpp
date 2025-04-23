#include "socketlistener.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <cstdint>
#include <unistd.h>
#include <array>
#include <format>

SocketListener::SocketListener(RequestProcessor* requestProcessor) :
                                requestProcessor_{requestProcessor}
{
    int ret;

    unlink("/tmp/log_sock");

    sockaddr_un sockaddr {
        .sun_family = AF_UNIX,
        .sun_path = "/tmp/log_sock"
    };

    socketFd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (socketFd < 0) {
        requestProcessor_->selfLog(std::format("Could not open socket: {}",
                                               strerror(errno)), logging::LOG_LEVEL::ERROR);
        return;
    }

    ret = bind(socketFd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    if (ret < 0){
        requestProcessor_->selfLog(std::format("Could not bind socket: {}",
                                               strerror(errno)), logging::LOG_LEVEL::ERROR);
        return;
    }

    ret = listen(socketFd, 100);
    if (ret < 0){
        requestProcessor_->selfLog(std::format("Could not mark socket for listening: {}",
                                               strerror(errno)), logging::LOG_LEVEL::ERROR);
    }

    pfd[0].fd = socketFd;
    pfd[0].events = POLLIN;
    requestProcessor_->selfLog("SocketListener started up", logging::LOG_LEVEL::INFO);
}

void SocketListener::clientSocketThread(std::stop_token *st, int clientSocketFd)
{
    std::vector<uint8_t> fullRequest;
    std::array<uint8_t, 10240> buffer;

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 500000;
    setsockopt(clientSocketFd, SOL_SOCKET, SO_RCVTIMEO, (void*)&tv, sizeof(tv));

    int recv_bytes = -1;
    struct pollfd clientFd[1];
    clientFd[0].fd = clientSocketFd;
    clientFd[0].events = POLLIN;

    while (!st->stop_requested() && recv_bytes != 0){
        poll(clientFd, 1, 1000);
        if (!(clientFd[0].revents & POLLIN)) continue;

        int current_message_size = 0;
        fullRequest.clear();
        while ((recv_bytes = recv(clientSocketFd, buffer.data(), buffer.size(), 0)) > 0){
            requestProcessor_->selfLog(std::format("message part: {}", recv_bytes), logging::LOG_LEVEL::INFO);
            current_message_size += recv_bytes;
            // if it's too big, just drop it.
            if (current_message_size < MAX_RESPONSE_SIZE) {
                fullRequest.insert(fullRequest.end(), buffer.begin(), buffer.begin() + recv_bytes);
            }
        }

        if (current_message_size <= MAX_RESPONSE_SIZE && current_message_size > 0){
            requestProcessor_->selfLog(std::format("Message size: {}", current_message_size), logging::LOG_LEVEL::DEBUG);
            requestProcessor_->processRequests(fullRequest);
        } else {
            requestProcessor_->selfLog(std::format("Too big message dropped, size: {}", current_message_size), logging::LOG_LEVEL::WARNING);
        }
    }

    close(clientSocketFd);
}

void SocketListener::startListening(std::stop_token st)
{
    int accept_sock;

    while (!st.stop_requested()){
        poll(pfd, 1, 500);

        if (!(pfd[0].revents & POLLIN)) continue;
        requestProcessor_->selfLog("Socket event", logging::LOG_LEVEL::DEBUG);

        accept_sock = accept(socketFd, nullptr, nullptr);
        if (accept_sock < 0) {
            requestProcessor_->selfLog(std::format("Accept errored out: {}",
                                                   strerror(errno)), logging::LOG_LEVEL::ERROR);
            continue;
        }

        clientSocketThreads.emplace_back(&SocketListener::clientSocketThread, this, &st, accept_sock);
    }

    requestProcessor_->selfLog("SocketListener cleanup start.", logging::LOG_LEVEL::INFO);

    for (auto& s: clientSocketThreads) s.join();
    close(socketFd);
    unlink("/tmp/log_sock");

    requestProcessor_->selfLog("SocketListener closing.", logging::LOG_LEVEL::INFO);
}
