#ifndef MLOGGER_H
#define MLOGGER_H

#include "Widgets/MLogWidget.h"

namespace Ms
{
    namespace Log
    {
        class MLogger
        {
        public:
            MLogger();
            ~MLogger();

            //functions
            unsigned int maxClientLogMessages();
            void setMaxClientLogMessages(unsigned int value);
            Ms::Log::Widgets::MLogWidget *creteClientLoggerWidget();
            void log(const std::string &message, Ms::Log::LogMessageType type, Ms::Log::LogMessageContext context = Ms::Log::LogMessageContext::Server);

            Ms::Log::Widgets::MLogWidget *logWidget();

        private:
            //variables
            unsigned int _maxClientLogMessages;
            Ms::Log::Widgets::MLogWidget *_logWidget;

            //functions
            void _init();
        };
    }
}
#endif // MLOGGER_H
