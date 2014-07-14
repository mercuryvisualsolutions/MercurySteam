#include "MLogger.h"

#include <Wt/WDateTime>

Ms::Log::MLogger::MLogger()
{
}

void Ms::Log::MLogger::log(const std::string &message, Ms::Log::LogMessageType type)
{
    std::string str = "";

    if(type == LogMessageType::Info)
    {
        str = "[Info]";

    }
    else if(type == LogMessageType::Warning)
    {
        str = "[Warn]";

    }
    else if(type == LogMessageType::Error)
    {
        str = "[Error]";

    }
    else if(type == LogMessageType::Fatal)
    {
        str = "[Fatal]";
    }

    str += "[" + Wt::WDateTime::currentDateTime().toString().toUTF8() + "] -> " + message;

    std::cout << str << std::endl;
}
