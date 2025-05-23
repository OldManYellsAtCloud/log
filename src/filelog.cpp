#include "filelog.h"
#include <filesystem>
#include <iostream>
#include <format>

bool FileLog::isValid() const
{
    return file.is_open();
}

bool FileLog::entryExists(std::string path)
{
    std::filesystem::directory_entry de {path};
    return de.exists();
}

std::string FileLog::getAvailableFileName(std::string name, std::string folder)
{
    std::string path = std::format("{}/{}.log", folder, name);
    int i = 0;
    while (entryExists(path)){
        path = std::format("{}/{}-{}.log", folder, name, i++);
    }
    return path;
}

unsigned int FileLog::getLogType() const
{
    return logging::LOGGER_TYPE::LOGGER_FILE;
}

FileLog::FileLog(std::string name, std::string folder) : Log(name)
{
    if (!entryExists(folder)){
        if (!std::filesystem::create_directory(folder)){
            std::cerr << "Could not create log folder! Not storing log file for "
                      << name << std::endl;
            return;
        }
    }

    std::string filePath = getAvailableFileName(name, folder);
    file = std::ofstream(filePath, std::ios_base::app);

    if (!isValid()){
        std::cerr << folder << "/" << name << ".log is not writable!" << std::endl;
    }
}

FileLog::~FileLog()
{
    if (isValid())
        file.close();
}

void FileLog::write(const std::string &msg, const logging::LOG_LEVEL logLevel)
{
    if (!isValid()) return;
    if (logLevel < defaultLogLevel) return;

    std::string finalMsg = std::format("{} - {}: {}", getTimeStamp(),
                                       getLogLevelString(logLevel), msg);
    file << finalMsg;
    if (!msg.ends_with("\n")) file << std::endl;
    file.flush();
}
