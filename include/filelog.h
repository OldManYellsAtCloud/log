#ifndef FILELOG_H
#define FILELOG_H

#include "log.h"
#include <fstream>

#define FILE_LOG_TYPE 2

class FileLog : public Log
{
private:
    std::ofstream file;
    bool isValid() const;
    static bool entryExists(std::string path);
    static std::string getAvailableFileName(std::string name, std::string folder);
protected:
    unsigned int getLogType() const override;
public:
    FileLog(std::string name, std::string folder = "/tmp/logs");
    ~FileLog();
    void write(const std::string &msg, const LOG_LEVEL logLevel) override;
};

#endif // FILELOG_H
