#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "../Application/msapplication.h"
#include "../Database/dbosession.h"
#include "../Log/logger.h"

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

    public:
        //functions
        Database::DboSession &dboSession();
        Wt::Auth::Login &login();
        Wt::Dbo::ptr<Users::User> user();

        Ms::Widgets::MPropertiesPanel *propertiesPanel();
        Log::Logger *logger();

    private:
        App::MSApplication *getCurrentAppInstance();
    };
}

#endif // SESSIONMANAGER_H
