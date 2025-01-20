#include "loggerfactory.h"
#include "consolelog.h"
#include "filelog.h"

Log *LoggerFactory::createLogger(std::string name, logging::LOGGER_TYPE loggerType)
{
    switch (loggerType){
    case logging::LOGGER_TYPE::LOGGER_CONSOLE:
        return std::move(new ConsoleLog(name));
    case logging::LOGGER_TYPE::LOGGER_FILE:
    default:
        return std::move(new FileLog(name));
    }
}
