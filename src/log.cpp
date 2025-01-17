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

std::string Log::getLogLevelString(LOG_LEVEL logLevel)
{
    switch(logLevel){
    case ERROR: return "ERROR";
    case INFO: return "INFO";
    case DEBUG: return "DEBUG";
    case FATAL: return "FATAL";
    case WARNING: return "WARNING";
    default: return "N/A";
    }

}

std::string Log::getName()
{
    return name_;
}


