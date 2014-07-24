#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "logger.h"

#include <Ms/Log/MLogger.h>

namespace Log
{
    class LogManager
    {
    private:
        LogManager();
        LogManager(const LogManager &copy);
        void operator=(const LogManager &copy);

    public:
        static LogManager &instance()
        {
            static LogManager man;
            return man;
        }

        ~LogManager();

        //functions
        Ms::Log::MLogger *getGlobalLogger();
        Logger *getSessionLogger(const std::string &sessionId);

    private:
        //variables
        Ms::Log::MLogger *_globalLogger;
        std::map<std::string,Log::Logger*> _sessionsLoggers;

        //functions
        void _init();

    };
}

#endif // LOGMANAGER_H
