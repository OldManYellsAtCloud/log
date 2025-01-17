#ifndef CONSOLEWRITER_H
#define CONSOLEWRITER_H

#include <string>
#include <mutex>

class ConsoleWriter
{
private:
    std::mutex mut;

public:
    ConsoleWriter();
    ~ConsoleWriter(){};

    void write(std::string msg);
};

#endif // CONSOLEWRITER_H
