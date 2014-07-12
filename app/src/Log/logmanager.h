#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <Ms/Widgets/MLogWidget.h>

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
        Ms::Widgets::MLogWidget *getLogWidget();

    private:
        //variables
        Ms::Widgets::MLogWidget *_logWidget;

        void _init();

    };
}

#endif // LOGMANAGER_H
