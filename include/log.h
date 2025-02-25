#ifndef LOG_H
#define LOG_H

#include <string>
#include "enums.h"


class Log
{
protected:
    std::string name_;
    static std::string getTimeStamp();
    static std::string getLogLevelString(logging::LOG_LEVEL logLevel);
    logging::LOG_LEVEL defaultLogLevel;
public:
    /*
     * getLogType is a flag that allows differentiating the
     * different derived classes, used for equality checking.
     * This should return the same value for each derived classes.
     * (Same, as within one derived class. E.g. file_logger should
     * always return file_logger type)
     */
    virtual unsigned int getLogType() const = 0;
    Log(std::string name){name_ = name; defaultLogLevel = logging::LOG_LEVEL::DEBUG;}
    void setLogLevel(logging::LOG_LEVEL newLevel){defaultLogLevel = newLevel;}
    std::string getName();
    virtual ~Log(){};
    virtual void write(const std::string& msg, const logging::LOG_LEVEL logLevel) = 0;

    virtual bool operator=(Log* other){
        return name_ == other->name_ && getLogType() == other->getLogType();
    }
};

#endif // LOG_H
