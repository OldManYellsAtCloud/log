#include "log.h"
#include <ctime>
#include <format>

std::string Log::getTimeStamp(){
    time_t now = time(0);
    tm *localtimeNow = localtime(&now);
    std::string timestamp = std::format("{}-{:02}-{:02} {:02}:{:02}:{:02}",
                                localtimeNow->tm_year + 1900,
                                localtimeNow->tm_mon + 1, localtimeNow->tm_mday,
                                localtimeNow->tm_hour, localtimeNow->tm_min,
                                localtimeNow->tm_sec);
    return timestamp;
}

std::string Log::getLogLevelString(logging::LOG_LEVEL logLevel)
{
    switch(logLevel){
    case logging::LOG_LEVEL::ERROR: return "ERROR";
    case logging::LOG_LEVEL::INFO: return "INFO";
    case logging::LOG_LEVEL::DEBUG: return "DEBUG";
    case logging::LOG_LEVEL::FATAL: return "FATAL";
    case logging::LOG_LEVEL::WARNING: return "WARNING";
    default: return "N/A";
    }

}

std::string Log::getName()
{
    return name_;
}


