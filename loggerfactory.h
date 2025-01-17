#ifndef LOGGERFACTORY_H
#define LOGGERFACTORY_H

#include "log.h"

class LoggerFactory
{
public:
    static Log* createLogger(std::string name, LOGGER_TYPE loggerType);
};

#endif // LOGGERFACTORY_H
