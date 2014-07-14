#ifndef MLOGGER_H
#define MLOGGER_H

#include "MLogGlobals.h"

#include <iostream>

namespace Ms
{
    namespace Log
    {
        class MLogger
        {
        public:
            MLogger();

            //functions

            void log(const std::string &message, Ms::Log::LogMessageType type);

        private:
            //variables

            //functions
        };
    }
}
#endif // MLOGGER_H
