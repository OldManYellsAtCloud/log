#include "consolewriter.h"
#include <iostream>

ConsoleWriter::ConsoleWriter() {}

void ConsoleWriter::write(std::string msg)
{
    const std::lock_guard<std::mutex> lock(mut);
    std::cout << msg;
    if (!msg.ends_with('\n'))
        std::cout << std::endl;
}
