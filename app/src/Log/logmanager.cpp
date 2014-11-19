#include "logmanager.h"

Log::LogManager::LogManager()
{
    _init();
}

Log::LogManager::~LogManager()
{
    delete _globalLogger;
}

Ms::Log::MLogger *Log::LogManager::getGlobalLogger()
{
    return _globalLogger;
}

void Log::LogManager::_init()
{
    _globalLogger = new Ms::Log::MLogger();
}
