#include "logger.h"

Log::Logger::Logger()
{
    init();
}

Log::Logger::Logger(const std::string &sessionId) :
    Logger()
{
    m_sessionId = sessionId;
}

Log::Logger::~Logger()
{
    delete m_logWidget;
}

void Log::Logger::log(const std::string &message, Ms::Log::LogMessageType type, Log::LogMessageContext context) const
{
    if(context & LogMessageContext::Client)
    {
        m_logWidget->log(message, type);
    }
    if(context & LogMessageContext::Server)
    {
        m_globalLogger->log(message, type);
    }
}

unsigned int Log::Logger::maxOnScreenLogMessages() const
{
    return m_logWidget->maxLogMessages();
}

void Log::Logger::setMaxOnScreenLogMessages(unsigned int value)
{
    m_logWidget->setMaxLogMessages(value);
}

Ms::Widgets::MLogWidget *Log::Logger::logWidget() const
{
    return m_logWidget;
}

Ms::Log::MLogger *Log::Logger::globalLogger() const
{
    return m_globalLogger;
}

void Log::Logger::setGlobalLogger(Ms::Log::MLogger *logger)
{
    m_globalLogger = logger;
}

const std::string Log::Logger::sessionId() const
{
    return m_sessionId;
}

void Log::Logger::setSessionId(const std::string &sessionId)
{
    m_sessionId = sessionId;
}

bool Log::Logger::operator==(const Log::Logger &other)
{
    return m_sessionId == other.sessionId();
}

void Log::Logger::init()
{
    m_sessionId = "";
    m_logWidget = new Ms::Widgets::MLogWidget();
}
