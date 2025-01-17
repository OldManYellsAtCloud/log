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

    sockaddr_un sockaddr {
        .sun_family = AF_UNIX,
        .sun_path = "/tmp/log_sock"
    };

    socketFd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (socketFd < 0) {
        requestProcessor_->selfLog(std::format("Could not open socket: {}",
                                               strerror(errno)), LOG_LEVEL::ERROR);
        return;
    }

    ret = bind(socketFd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    if (ret < 0){
        requestProcessor_->selfLog(std::format("Could not bind socket: {}",
                                               strerror(errno)), LOG_LEVEL::ERROR);
        return;
    }

    ret = listen(socketFd, 100);
    if (ret < 0){
        requestProcessor_->selfLog(std::format("Could not mark socket for listening: {}",
                                               strerror(errno)), LOG_LEVEL::ERROR);
    }

    pfd[0].fd = socketFd;
    pfd[0].events = POLLIN;
    requestProcessor_->selfLog("SocketListener started up", LOG_LEVEL::INFO);
}

void SocketListener::startListening(std::stop_token st)
{
    int accept_sock;
    int recv_bytes;
    int current_message_size;

    std::vector<uint8_t> fullRequest;
    std::array<uint8_t, 1024> buffer;

    while (!st.stop_requested()){
        poll(pfd, 1, 500);

        if (!(pfd[0].revents & POLLIN)) continue;

        accept_sock = accept(socketFd, nullptr, nullptr);
        if (accept_sock < 0) {
            requestProcessor_->selfLog(std::format("Accept errored out: {}",
                                                   strerror(errno)), LOG_LEVEL::ERROR);
            continue;
        }

        current_message_size = 0;
        fullRequest.clear();
        while ((recv_bytes = recv(accept_sock, buffer.data(), sizeof(buffer), 0)) > 0){
            current_message_size += recv_bytes;
            // if it's too big, just drop it.
            if (current_message_size > MAX_RESPONSE_SIZE) {
                close(accept_sock);
                break;
            }
            fullRequest.insert(fullRequest.end(), buffer.begin(), buffer.begin() + recv_bytes);
        }

        if (current_message_size <= MAX_RESPONSE_SIZE){
            requestProcessor_->processRequest(fullRequest);
        }
    }

    requestProcessor_->selfLog("SocketListener cleanup start.", LOG_LEVEL::INFO);
    close(socketFd);
    unlink("/tmp/log_sock");
    requestProcessor_->selfLog("SocketListener closing.", LOG_LEVEL::INFO);
}
