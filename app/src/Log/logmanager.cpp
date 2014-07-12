#include "logmanager.h"

Log::LogManager::LogManager()
{
    _init();
}

Log::LogManager::~LogManager()
{
    delete _logWidget;
}

Ms::Widgets::MLogWidget *Log::LogManager::getLogWidget()
{
    return _logWidget;
}

void Log::LogManager::_init()
{
    _logWidget = new Ms::Widgets::MLogWidget();
}
