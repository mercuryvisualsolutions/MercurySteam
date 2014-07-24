#ifndef LOGGER_H
#define LOGGER_H

#include "logglobals.h"

#include <Ms/Log/MLogger.h>
#include <Ms/Widgets/MLogWidget.h>

namespace Log
{
    class Logger
    {
    public:
        Logger();
        Logger(const std::string &sessionId);

        ~Logger();

        //functions
        void log(const std::string &message, Ms::Log::LogMessageType type, Log::LogMessageContext context = Log::LogMessageContext::ServerAndClient) const;
        unsigned int maxOnScreenLogMessages() const;
        void setMaxOnScreenLogMessages(unsigned int value);
        Ms::Widgets::MLogWidget *logWidget() const;
        Ms::Log::MLogger *globalLogger() const;
        void setGlobalLogger(Ms::Log::MLogger *logger);
        const std::string sessionId() const;
        void setSessionId(const std::string &sessionId);

        bool operator==(const Logger &other);

    private:
        //variables
        Ms::Log::MLogger *_globalLogger;
        Ms::Widgets::MLogWidget *_logWidget;
        std::string _sessionId;

        //functions
        void _init();
    };
}

#endif // LOGGER_H
