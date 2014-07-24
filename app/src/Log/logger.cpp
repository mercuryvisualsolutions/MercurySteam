#include "logger.h"

Log::Logger::Logger()
{
    _init();
}

Log::Logger::Logger(const std::string &sessionId) :
    Logger()
{
    _sessionId = sessionId;
}

Log::Logger::~Logger()
{
    delete _logWidget;
}

void Log::Logger::log(const std::string &message, Ms::Log::LogMessageType type, Log::LogMessageContext context) const
{
    if(context & LogMessageContext::Client)
    {
        _logWidget->log(message, type);
    }
    if(context & LogMessageContext::Server)
    {
        _globalLogger->log(message, type);
    }
}

unsigned int Log::Logger::maxOnScreenLogMessages() const
{
    return _logWidget->maxLogMessages();
}

void Log::Logger::setMaxOnScreenLogMessages(unsigned int value)
{
    _logWidget->setMaxLogMessages(value);
}

Ms::Widgets::MLogWidget *Log::Logger::logWidget() const
{
    return _logWidget;
}

Ms::Log::MLogger *Log::Logger::globalLogger() const
{
    return _globalLogger;
}

void Log::Logger::setGlobalLogger(Ms::Log::MLogger *logger)
{
    _globalLogger = logger;
}

const std::string Log::Logger::sessionId() const
{
    return _sessionId;
}

void Log::Logger::setSessionId(const std::string &sessionId)
{
    _sessionId = sessionId;
}

bool Log::Logger::operator==(const Log::Logger &other)
{
    return _sessionId == other.sessionId();
}

void Log::Logger::_init()
{
    _sessionId = "";
    _logWidget = new Ms::Widgets::MLogWidget();
}
