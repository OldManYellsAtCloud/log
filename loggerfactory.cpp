#include "loggerfactory.h"
#include "consolelog.h"
#include "filelog.h"

Log *LoggerFactory::createLogger(std::string name, LOGGER_TYPE loggerType)
{
    switch (loggerType){
    case LOGGER_TYPE::LOGGER_CONSOLE:
        return std::move(new ConsoleLog(name));
    case LOGGER_TYPE::LOGGER_FILE:
    default:
        return std::move(new FileLog(name));
    }
}
