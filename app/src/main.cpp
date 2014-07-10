#define BOOST_FILESYSTEM_NO_DEPRECATED
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>

#include <iostream>
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WVBoxLayout>
#include <Wt/WCssTheme>
#include <Wt/WBootstrapTheme>
#include <Wt/Dbo/Dbo>
#include <Wt/WTableView>
#include <Wt/Dbo/QueryModel>

#include "Settings/appsettings.h"
#include "Database/databasemanager.h"
#include "Views/Main/viewmain.h"
#include "Users/usersmanager.h"
#include "Projects/projectsmanager.h"
#include "Auth/authmanager.h"

Wt::WApplication *createApplication(const Wt::WEnvironment &env)
{
    //load settings with each new session, in case admin changes settings anytime
    AppSettings::instance().loadAppSettings();

    Wt::WApplication *app = new Wt::WApplication(env);

    app->setTitle("Mercury Steam");

    //data checking
    if (app->appRoot().empty())
    {
        std::cerr << "Warning: approot not set!" << std::endl;
    }
    if(app->docRoot().empty())
    {
        std::cerr << "Warning: docroot not set!" << std::endl;
    }

    //theme
    const std::string *theme = env.getParameter("theme");
    if (theme)
        app->setTheme(new Wt::WCssTheme(*theme));
    else
    {
        Wt::WBootstrapTheme* theme = new Wt::WBootstrapTheme(app);
        theme->setVersion(Wt::WBootstrapTheme::Version2);
        app->setTheme(theme);
    }

    //main layout
    Wt::WVBoxLayout *layMain = new Wt::WVBoxLayout(app->root());
    layMain->setContentsMargins(0,0,0,0);
    layMain->setSpacing(0);

    Views::ViewMain *viwMain = new Views::ViewMain();

    //add our main view
    layMain->addWidget(viwMain);

    //stylesheet
    app->useStyleSheet("style/focusStyle.css");

    app->root()->setStyleClass("toolbar");

    //handle internalPathChange
    //app->setInternalPath("/auth", true);

    return app;
}

int main(int argc, char *argv[])
{
    //settings
    std::cout << "Loading settings.." << std::endl;

    AppSettings::instance().setSettingsFileName("Settings.xml");
    if(!boost::filesystem::exists(AppSettings::instance().settingsFileName()))
    {
        AppSettings::instance().initDefaultSettings();
    }

    if(argc < 0)//check args count
    {

    }

    //parse args to set approot and docroot settings before other initializations
    for(int i = 1; i < argc; ++i)//argv[0] = apppath
    {
        if(i + 1 != argc)//if we don't finish parsing all arguments already
        {
            if(std::string(argv[i]) == "--docroot")
                AppSettings::instance().setDocRoot(argv[i+1]);
            else if(std::string(argv[i]) == "--approot")
                AppSettings::instance().setAppRoot(argv[i+1]);
        }
    }

    if(AppSettings::instance().docRoot() == "")
    {
        std::cerr << "docroot can't be empty, please run the program with argument --docroot <path>" << std::endl;
        exit(0);
    }
    if(AppSettings::instance().appRoot() == "")
    {
        std::cerr << "approot can't be empty, please run the program with argument --approot <path>" << std::endl;
        exit(0);
    }

    std::cout << "Settings loaded.." << std::endl;

    //database
    std::cout << "Establishing database connection.." << std::endl;
    AppSettings::instance().loadAppSettings();

    std::cout << "Connected to \"" << AppSettings::instance().databaseBackEnd() << "\" database using schema \"" << AppSettings::instance().databaseSchema() + "\"" << std::endl;

    //configure auth service
    Auth::AuthManager::instance().configureAuth();

    //initialize database
    Database::DatabaseManager::instance().initDatabase();

    //run the application
    return Wt::WRun(argc, argv, &createApplication);
}
