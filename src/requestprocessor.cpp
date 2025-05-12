#include "requestprocessor.h"
#include "loggerfactory.h"
#include <string.h>
#include <format>

RequestProcessor::RequestProcessor(std::string selfName) : name {selfName} {}

void RequestProcessor::addLogger(std::string name, logging::LOGGER_TYPE loggerType)
{
    for (Log* l: loggers){
        if (l->getName() == name && l->getLogType() == loggerType)
            return;
    }
    loggers.push_back(LoggerFactory::createLogger(name, loggerType));
}

void RequestProcessor::writeLog(std::string loggerName, std::string message, logging::LOG_LEVEL level)
{
    for (auto logger: loggers){
        if (logger->getName() == loggerName)
            logger->write(message, level);
    }
}

bool RequestProcessor::validateRequestType(int requestType)
{
    return requestType == logging::REQUEST_TYPE::LOG_MESSAGE ||
           requestType == logging::REQUEST_TYPE::NEW_LOGGER;
}

bool RequestProcessor::validateLoggerType(int loggerType)
{
    return loggerType == logging::LOGGER_TYPE::LOGGER_CONSOLE ||
           loggerType == logging::LOGGER_TYPE::LOGGER_FILE;
}

bool RequestProcessor::validateLogLevel(int logLevel)
{
    return logLevel == logging::LOG_LEVEL::DEBUG || logLevel == logging::LOG_LEVEL::ERROR ||
           logLevel == logging::LOG_LEVEL::FATAL || logLevel == logging::LOG_LEVEL::INFO ||
           logLevel == logging::LOG_LEVEL::WARNING;
}

void RequestProcessor::selfLog(std::string message, logging::LOG_LEVEL level)
{
    writeLog(name, message, level);
}

void RequestProcessor::processRequests(std::vector<uint8_t> &buffer)
{
    int requestType;
    int current_idx = 0;
    while (current_idx < buffer.size()) {
        if (buffer.size() < sizeof(requestType)){
            selfLog("Invalid request, can't extract request type", logging::LOG_LEVEL::ERROR);
            return;
        } else {
            memcpy(&requestType, buffer.data() + current_idx, sizeof(requestType));
            current_idx += sizeof(requestType);
        }

        if (!validateRequestType(requestType)){
            selfLog(std::format("Unknown request type: {}", requestType), logging::LOG_LEVEL::ERROR);
            return;
        }

        switch (requestType){
        case logging::REQUEST_TYPE::NEW_LOGGER:
            processNewLoggerRequest(buffer, current_idx);
            break;
        case logging::REQUEST_TYPE::LOG_MESSAGE:
            processLoggingRequest(buffer, current_idx);
            break;
        default:
            selfLog(std::format("Unknown request type: {}", requestType), logging::LOG_LEVEL::ERROR);
        }
    }
}

void RequestProcessor::processNewLoggerRequest(std::vector<uint8_t> &buffer, int &current_offset)
{
    selfLog("Starting add new logger", logging::LOG_LEVEL::DEBUG);
    int loggerNameLength;
    if (buffer.size() < current_offset + sizeof(loggerNameLength)){
        selfLog("Can't extract new logger name size", logging::LOG_LEVEL::ERROR);
        return;
    } else {
        memcpy(&loggerNameLength, buffer.data() + current_offset, sizeof(loggerNameLength));
        current_offset += sizeof(loggerNameLength);
    }

    char loggerName[loggerNameLength + 1];
    if (buffer.size() < current_offset + loggerNameLength){
        selfLog("Can't extract new logger name", logging::LOG_LEVEL::ERROR);
        return;
    } else {
        memcpy(loggerName, buffer.data() + current_offset, loggerNameLength);
        loggerName[loggerNameLength] = '\0';
        current_offset += loggerNameLength;
    }

    int loggerType;
    if (buffer.size() < current_offset + sizeof(loggerType)){
        selfLog("Can't extract logger type", logging::LOG_LEVEL::ERROR);
        return;
    } else {
        memcpy(&loggerType, buffer.data() + current_offset, sizeof(loggerType));
        current_offset += sizeof(loggerType);
    }

    if (!validateLoggerType(loggerType)){
        selfLog(std::format("Unknow logger type: {}", loggerType), logging::LOG_LEVEL::ERROR);
        return;
    }

    //selfLog(std::format("Adding new logger: {}, type: {}", std::string(loggerName),
    //                    int(loggerType)), LOG_LEVEL::DEBUG);
    addLogger(loggerName, logging::LOGGER_TYPE(loggerType));
}

void RequestProcessor::processLoggingRequest(std::vector<uint8_t> &buffer, int& current_offset)
{
    int loggerNameLength;
    if (buffer.size() < current_offset + sizeof(loggerNameLength)){
        selfLog("Can't extract logger name size", logging::LOG_LEVEL::ERROR);
        return;
    } else {
        memcpy(&loggerNameLength, buffer.data() + current_offset, sizeof(loggerNameLength));
        current_offset += sizeof(loggerNameLength);
    }

    char loggerName[loggerNameLength + 1];
    if (buffer.size() < current_offset + loggerNameLength){
        selfLog("Can't extract logger name", logging::LOG_LEVEL::ERROR);
        return;
    } else {
        memcpy(loggerName, buffer.data() + current_offset, loggerNameLength);
        current_offset += loggerNameLength;
        loggerName[loggerNameLength] = '\0';
    }

    int msgLength;
    if (buffer.size() < current_offset + sizeof(msgLength)){
        selfLog("Can't extract message size", logging::LOG_LEVEL::ERROR);
        return;
    } else {
        memcpy(&msgLength, buffer.data() + current_offset, sizeof(msgLength));
        current_offset += sizeof(msgLength);
    }

    char msg[msgLength + 1];
    if (buffer.size() < current_offset + msgLength){
        selfLog("Can't extract log message", logging::LOG_LEVEL::ERROR);
        return;
    } else {
        memcpy(msg, buffer.data() + current_offset, msgLength);
        msg[msgLength] = '\0';
        current_offset += msgLength;
    }

    int logLevel;
    if (buffer.size() < current_offset + sizeof(logLevel)){
        selfLog("Can't extract log level, using INFO", logging::LOG_LEVEL::ERROR);
        logLevel = logging::LOG_LEVEL::INFO;
    } else {
        memcpy(&logLevel, buffer.data() + current_offset, sizeof(logLevel));
        current_offset += sizeof(logLevel);
    }

    if (!validateLogLevel(logLevel)){
        selfLog(std::format("Unknown log level: {}, using INFO", logLevel), logging::LOG_LEVEL::ERROR);
        logLevel = logging::LOG_LEVEL::INFO;
    }

    //selfLog(std::format("Adding message name: {}, msg: {}", std::string(loggerName),
    //                    std::string(msg)), LOG_LEVEL::DEBUG);

    writeLog(loggerName, msg, logging::LOG_LEVEL(logLevel));
}


