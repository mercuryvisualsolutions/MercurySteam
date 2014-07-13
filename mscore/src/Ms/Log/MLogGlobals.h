#ifndef MLOGGLOBALS_H
#define MLOGGLOBALS_H

namespace Ms
{
    namespace Log
    {
        enum LogMessageType
        {
            Info,
            Warning,
            Error,
            Fatal,
        };

        enum LogMessageContext
        {
            Server = 0x01,
            Client = 0x02,
            ServerAndClient = 0x03,
        };

        inline LogMessageContext operator|(LogMessageContext a, LogMessageContext b)
        {
            return static_cast<LogMessageContext>(static_cast<int>(a) | static_cast<int>(b));
        }
    }
}



#endif // MLOGGLOBALS_H
