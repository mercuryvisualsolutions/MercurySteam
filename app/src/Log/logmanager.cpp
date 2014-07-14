#include "logmanager.h"

Log::LogManager::LogManager()
{
    _init();
}

Log::LogManager::~LogManager()
{
    delete _globalLogger;

    for(auto &pair: _sessionsLoggers)
    {
        delete pair.second;
        pair.second = nullptr;
    }

    _sessionsLoggers.clear();
}

Ms::Log::MLogger *Log::LogManager::getGlobalLogger()
{
    return _globalLogger;
}

Log::Logger *Log::LogManager::getAppSessionLogger(const std::string &sessionId)
{
    if(_sessionsLoggers.find(sessionId) == _sessionsLoggers.end())
    {
        Logger *logger = new Logger(sessionId);
        logger->setGlobalLogger(_globalLogger);

        _sessionsLoggers[sessionId] = logger;
    }

    return _sessionsLoggers[sessionId];
}

void Log::LogManager::_init()
{
    _globalLogger = new Ms::Log::MLogger();
}
