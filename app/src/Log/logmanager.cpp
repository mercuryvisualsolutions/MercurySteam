#include "logmanager.h"

Log::LogManager::LogManager()
{
    _init();
}

Log::LogManager::~LogManager()
{
    delete _logger;
}

Ms::Log::MLogger *Log::LogManager::getLogger()
{
    return _logger;
}

void Log::LogManager::_init()
{
    _logger = new Ms::Log::MLogger();
}
