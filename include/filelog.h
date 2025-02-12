#ifndef FILELOG_H
#define FILELOG_H

#include "log.h"
#include <fstream>

class FileLog : public Log
{
private:
    std::ofstream file;
    bool isValid() const;
    static bool entryExists(std::string path);
    static std::string getAvailableFileName(std::string name, std::string folder);
public:
    unsigned int getLogType() const override;
    FileLog(std::string name, std::string folder = "/tmp/logs");
    ~FileLog();
    void write(const std::string &msg, const logging::LOG_LEVEL logLevel) override;
};

#endif // FILELOG_H
