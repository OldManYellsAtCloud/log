#include "filelog.h"
#include <filesystem>
#include <iostream>
#include <format>

bool FileLog::isValid() const
{
    return file.is_open();
}

bool FileLog::folderExists(std::string folder)
{
    std::filesystem::directory_entry de {folder};
    return de.exists();
}

unsigned int FileLog::getLogType() const
{
    return FILE_LOG_TYPE;
}

FileLog::FileLog(std::string name, std::string folder) : Log(name)
{
    if (!folderExists(folder)){
        std::cerr << folder << " does not exist! Not creating file log for "
                  << name << std::endl;
        return;
    }

    file = std::ofstream(std::format("{}/{}.log", folder, name), std::ios_base::app);

    if (!isValid()){
        std::cerr << folder << "/" << name << ".log is not writable!" << std::endl;
    }
}

FileLog::~FileLog()
{
    if (isValid())
        file.close();
}

void FileLog::write(const std::string &msg, const LOG_LEVEL logLevel)
{
    if (!isValid()) return;

    std::string finalMsg = std::format("{} - {}: {}", getTimeStamp(),
                                       getLogLevelString(logLevel), msg);
    file << finalMsg;
    if (!msg.ends_with("\n")) file << std::endl;
    file.flush();
}
