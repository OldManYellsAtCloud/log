#ifndef ENUMS_H
#define ENUMS_H

namespace logging {
enum REQUEST_TYPE {
    NEW_LOGGER = 0,
    LOG_MESSAGE = 1
};

enum LOG_LEVEL {
    DEBUG = 0, INFO, WARNING, ERROR, FATAL
};

enum COLORS {
    RED = 31,
    DEFAULT = 0
};

enum LOGGER_TYPE {
    LOGGER_CONSOLE = 0,
    LOGGER_FILE = 1
};
}
#endif // ENUMS_H
