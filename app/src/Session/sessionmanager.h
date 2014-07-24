#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <Ms/Log/MLogger.h>
#include <Ms/Widgets/MPropertiesPanel.h>

namespace Session
{
    class SessionManager
    {
    private:
        SessionManager();
        SessionManager(const SessionManager &copy);
        void operator=(const SessionManager &copy);

    public:
        static SessionManager &instance()
        {
            static SessionManager man;
            return man;
        }

        ~SessionManager();

        //functions
        Ms::Widgets::MPropertiesPanel *getSessionPropertiesPanel(const std::string &sessionId);

    private:
        //variables
        std::map<std::string,Ms::Widgets::MPropertiesPanel*> _sessionsPropertiesPanels;

        //functions
        void _init();
    };
}

#endif // SESSIONMANAGER_H
