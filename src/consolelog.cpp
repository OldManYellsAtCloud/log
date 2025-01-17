#include "consolelog.h"
#include <format>

ConsoleWriter ConsoleLog::consoleWriter = ConsoleWriter();

ConsoleLog::ConsoleLog(std::string name): Log(name){

}

void ConsoleLog::write(const std::string& msg, const LOG_LEVEL logLevel)
{
    std::string finalMsg;
    std::string colorTagStart;
    std::string colorTagEnd = std::format("\e[{}m", int(COLORS::DEFAULT));

    if (logLevel == ERROR){
        colorTagStart = std::format("\e[{}m", int(COLORS::RED));
    } else {
        colorTagStart = colorTagEnd;
    }

    finalMsg = std::format("{} - {} - {}: {}{}{}", getTimeStamp(), name_,
                           getLogLevelString(logLevel), colorTagStart, msg,
                           colorTagEnd);

    consoleWriter.write(finalMsg);
}

unsigned int ConsoleLog::getLogType() const
{
    return CONSOLE_LOG_TYPE;
}
