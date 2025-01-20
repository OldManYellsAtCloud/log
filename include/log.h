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
    /*
     * getLogType is a flag that allows differentiating the
     * different derived classes, used for equality checking.
     * This should return the same value for each derived classes.
     */
    virtual unsigned int getLogType() const = 0;
public:
    Log(std::string name){name_ = name;};
    std::string getName();
    virtual ~Log(){};
    virtual void write(const std::string& msg, const logging::LOG_LEVEL logLevel) = 0;

    virtual bool operator=(Log* other){
        return name_ == other->name_ && getLogType() == other->getLogType();
    }

};

#endif // LOG_H
