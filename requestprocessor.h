#ifndef REQUESTPROCESSOR_H
#define REQUESTPROCESSOR_H

#include <vector>
#include <cstdint>
#include "log.h"

class RequestProcessor
{
    std::string name;
    std::vector<Log*> loggers;

    void writeLog(std::string loggerName, std::string message, LOG_LEVEL level);

    bool validateRequestType(int requestType);
    bool validateLoggerType(int loggerType);
    bool validateLogLevel(int logLevel);

    void processNewLoggerRequest(std::vector<uint8_t>& buffer);
    void processLoggingRequest(std::vector<uint8_t>& buffer);

    std::string extractLoggerName(std::vector<uint8_t>& buffer, int& current_offset);
public:
    RequestProcessor(std::string selfName);
    ~RequestProcessor(){};

    void processRequest(std::vector<uint8_t>& buffer);
    void selfLog(std::string message, LOG_LEVEL level);
    void addLogger(std::string name, LOGGER_TYPE loggerType);
};

#endif // REQUESTPROCESSOR_H
