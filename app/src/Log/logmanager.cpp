#include "logmanager.h"

Log::LogManager::LogManager()
{
    init();
}

Log::LogManager::~LogManager()
{
    delete m_globalLogger;
}

Ms::Log::MLogger *Log::LogManager::getGlobalLogger()
{
    return m_globalLogger;
}

void Log::LogManager::init()
{
    m_globalLogger = new Ms::Log::MLogger();
}
