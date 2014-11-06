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
#include <Wt/WServer>

#include "Settings/appsettings.h"
#include "Database/databasemanager.h"
#include "Views/Main/viewmain.h"
#include "Users/usersmanager.h"
#include "Projects/projectsmanager.h"
#include "Auth/authmanager.h"
#include "Log/logmanager.h"

Wt::WApplication *createApplication(const Wt::WEnvironment &env)
{
    //load settings with each new session, in case admin changes settings anytime
    AppSettings::instance().loadAppSettings();

    Wt::WApplication *app = new Wt::WApplication(env);

    app->setTitle("Mercury Steam");

    //data checking
    if (app->appRoot().empty())
    {
        Log::LogManager::instance().getGlobalLogger()->log("approot not set!", Ms::Log::LogMessageType::Warning);
    }
    if(app->docRoot().empty())
    {
        Log::LogManager::instance().getGlobalLogger()->log("docroot not set!", Ms::Log::LogMessageType::Warning);
    }

    //theme
    const std::string *themePtr = env.getParameter("theme");
    std::string theme;
    if (!themePtr)
        theme = "bootstrap3";
    else
        theme = *themePtr;

    if(theme == "bootstrap3")
    {
        Wt::WBootstrapTheme *bootstrapTheme = new Wt::WBootstrapTheme(app);
        bootstrapTheme->setVersion(Wt::WBootstrapTheme::Version3);
        bootstrapTheme->setResponsive(true);
        app->setTheme(bootstrapTheme);

        // load the default bootstrap3 (sub-)theme
        app->useStyleSheet("resources/themes/bootstrap/3/bootstrap-theme.min.css");
      }
    else if (theme == "bootstrap2")
    {
        Wt::WBootstrapTheme *bootstrapTheme = new Wt::WBootstrapTheme(app);
        bootstrapTheme->setResponsive(true);
        app->setTheme(bootstrapTheme);
      }
    else
        app->setTheme(new Wt::WCssTheme(theme));

    //main layout
    Wt::WVBoxLayout *layMain = new Wt::WVBoxLayout(app->root());
    layMain->setContentsMargins(0,0,0,0);
    layMain->setSpacing(0);

    Views::ViewMain *viwMain = new Views::ViewMain();

    //add our main view
    layMain->addWidget(viwMain);

    //init session loggers
    Auth::AuthManager::instance().initSessionLogger();
    Database::DatabaseManager::instance().initSessionLogger();
    Projects::ProjectsManager::instance().initSessionLogger();
    Users::UsersManager::instance().initSessionLogger();

    //stylesheet
    app->useStyleSheet("style/focusStyle.css");

    //js required files
    app->require("resources/themes/bootstrap/js/bootstrap.js");
    app->require("resources/themes/bootstrap/js/bootstrap.min.js");
    app->require("resources/themes/bootstrap/js/npm.js");

    //add resources to style WAuthWidget
    app->messageResourceBundle().use("docroot/resources/xml/auth_strings");
    app->messageResourceBundle().use("docroot/resources/xml/bootstrap_theme");

    //handle internalPathChange
    //app->setInternalPath("/auth", true);

    return app;
}

int main(int argc, char *argv[])
{
    //settings
    Log::LogManager::instance().getGlobalLogger()->log("Loading settings..", Ms::Log::LogMessageType::Info);

    AppSettings::instance().setSettingsFileName("Settings.xml");
    if(!boost::filesystem::exists(AppSettings::instance().settingsFileName()))
        AppSettings::instance().initDefaultSettings();
    else
        AppSettings::instance().loadAppSettings();

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
        Log::LogManager::instance().getGlobalLogger()->log("docroot can't be empty, please run the program with argument --docroot <path>", Ms::Log::LogMessageType::Fatal);
        exit(-1);
    }
    if(AppSettings::instance().appRoot() == "")
    {
        Log::LogManager::instance().getGlobalLogger()->log("approot can't be empty, please run the program with argument --approot <path>", Ms::Log::LogMessageType::Fatal);
        exit(-1);
    }

    Log::LogManager::instance().getGlobalLogger()->log("Settings loaded..", Ms::Log::LogMessageType::Info);

    //configure auth service
    Auth::AuthManager::instance().configureAuth();

    //initialize database
    Log::LogManager::instance().getGlobalLogger()->log("Initializing Database..", Ms::Log::LogMessageType::Info);
    Database::DatabaseManager::instance().initDatabase();

    //run the application
    try
    {
        Wt::WServer server(argv[0]);

        server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
        server.addEntryPoint(Wt::Application, createApplication);

        if (server.start())
        {
              Wt::WServer::waitForShutdown();
              server.stop();
        }
    }
    catch (Wt::WServer::Exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << "exception: " << e.what() << std::endl;
    }

    return 0;
}
