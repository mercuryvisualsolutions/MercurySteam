#include "MLogger.h"

#include <Wt/WDateTime>

Ms::Log::MLogger::MLogger()
{
    _init();
}

Ms::Log::MLogger::~MLogger()
{
    delete _logWidget;
}

unsigned int Ms::Log::MLogger::maxClientLogMessages()
{
    return _logWidget->maxLogMessages();
}

void Ms::Log::MLogger::setMaxClientLogMessages(unsigned int value)
{
    _logWidget->setMaxLogMessages(value);
}

Ms::Log::Widgets::MLogWidget *Ms::Log::MLogger::creteClientLoggerWidget()
{
    if(!_logWidget)
        _logWidget = new Ms::Log::Widgets::MLogWidget();

    return _logWidget;
}

void Ms::Log::MLogger::log(const std::string &message, Ms::Log::LogMessageType type, Ms::Log::LogMessageContext context)
{
    if(context & Ms::Log::LogMessageContext::Client)
    {
        _logWidget->log(message, type);
    }

    if(context & Ms::Log::LogMessageContext::Server)
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
}

Ms::Log::Widgets::MLogWidget *Ms::Log::MLogger::logWidget()
{
    return _logWidget;
}

void Ms::Log::MLogger::_init()
{
    _logWidget = nullptr;
}

