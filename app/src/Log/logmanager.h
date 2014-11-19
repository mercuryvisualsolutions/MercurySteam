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

    private:
        //variables
        Ms::Log::MLogger *_globalLogger;

        //functions
        void _init();

    };
}

#endif // LOGMANAGER_H
