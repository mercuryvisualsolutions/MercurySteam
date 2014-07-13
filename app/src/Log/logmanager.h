#ifndef LOGMANAGER_H
#define LOGMANAGER_H

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
        Ms::Log::MLogger *getLogger();

    private:
        //variables
        Ms::Log::MLogger *_logger;

        void _init();

    };
}

#endif // LOGMANAGER_H
