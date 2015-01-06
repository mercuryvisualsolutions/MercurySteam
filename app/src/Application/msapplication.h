#ifndef MSAPPLICATION_H
#define MSAPPLICATION_H

#include "../Database/dbosession.h"
#include "../Log/logger.h"

#include <Ms/Widgets/MPropertiesPanel.h>

#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/Auth/Login>
#include <Wt/Dbo/SqlConnectionPool>

namespace App
{
    class MSApplication : public Wt::WApplication
    {
    public:
        MSApplication(const Wt::WEnvironment& env, Wt::Dbo::SqlConnectionPool &sqlConnectionPool);
        ~MSApplication();

    public:
        void createGlobalWidgets();
        void destroyGlobalWidgets();

        Database::DboSession &dboSession();
        Ms::Widgets::MPropertiesPanel *propertiesPanel();
        Log::Logger *logger();

    private:
        Database::DboSession m_dboSession;
        Ms::Widgets::MPropertiesPanel *m_propertiesPanel;
        Log::Logger *m_logger;
    };
}

#endif // MSAPPLICATION_H
