#include "consolelog.h"
#include <format>

ConsoleWriter ConsoleLog::consoleWriter = ConsoleWriter();

ConsoleLog::ConsoleLog(std::string name): Log(name){

}

void ConsoleLog::write(const std::string& msg, const logging::LOG_LEVEL logLevel)
{
    if (logLevel < defaultLogLevel) return;

    std::string finalMsg;
    std::string colorTagStart;
    std::string colorTagEnd = std::format("\e[{}m", int(logging::COLORS::DEFAULT));

    if (logLevel == logging::LOG_LEVEL::ERROR){
        colorTagStart = std::format("\e[{}m", int(logging::COLORS::RED));
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
    return logging::LOGGER_TYPE::LOGGER_CONSOLE;
}
