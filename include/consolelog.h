#ifndef CONSOLELOG_H
#define CONSOLELOG_H

#include "log.h"
#include "consolewriter.h"

class ConsoleLog : public Log
{
private:
    static ConsoleWriter consoleWriter;
public:
    unsigned int getLogType() const override;
    ConsoleLog(std::string name);
    void write(const std::string& msg, const logging::LOG_LEVEL logLevel = logging::LOG_LEVEL::INFO) override;
};

#endif // CONSOLELOG_H
