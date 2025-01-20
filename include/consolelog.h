#ifndef CONSOLELOG_H
#define CONSOLELOG_H

#include "log.h"
#include "consolewriter.h"

#define CONSOLE_LOG_TYPE 1

class ConsoleLog : public Log
{
private:
    static ConsoleWriter consoleWriter;
protected:
    unsigned int getLogType() const override;
public:
    ConsoleLog(std::string name);
    void write(const std::string& msg, const logging::LOG_LEVEL logLevel = logging::LOG_LEVEL::INFO) override;
};

#endif // CONSOLELOG_H
