#include "msapplication.h"
#include "../Settings/appsettings.h"
#include "../Auth/authmanager.h"
#include "../Views/Main/viewmain.h"

#include <Wt/WVBoxLayout>
#include <Wt/WCssTheme>
#include <Wt/WBootstrapTheme>
#include <Wt/Dbo/Dbo>
#include <Wt/WTableView>
#include <Wt/Dbo/QueryModel>

App::MSApplication::MSApplication(const Wt::WEnvironment &env, Wt::Dbo::SqlConnectionPool &sqlConnectionPool) :
    Wt::WApplication(env),
    _dboSession(sqlConnectionPool)
{
    //load settings with each new session, in case admin changes settings anytime
    AppSettings::instance().loadAppSettings();

    setTitle("Mercury Steam");

    //theme
    const std::string *themePtr = env.getParameter("theme");
    std::string theme;
    if (!themePtr)
        theme = "bootstrap3";
    else
        theme = *themePtr;

    if(theme == "bootstrap3")
    {
        Wt::WBootstrapTheme *bootstrapTheme = new Wt::WBootstrapTheme(this);
        bootstrapTheme->setVersion(Wt::WBootstrapTheme::Version3);
        bootstrapTheme->setResponsive(true);
        setTheme(bootstrapTheme);

        // load the default bootstrap3 (sub-)theme
        useStyleSheet("resources/themes/bootstrap/3/bootstrap-theme.min.css");
      }
    else if (theme == "bootstrap2")
    {
        Wt::WBootstrapTheme *bootstrapTheme = new Wt::WBootstrapTheme(this);
        bootstrapTheme->setResponsive(true);
        setTheme(bootstrapTheme);
      }
    else
        setTheme(new Wt::WCssTheme(theme));

    //main layout
    Wt::WVBoxLayout *layMain = new Wt::WVBoxLayout(root());
    layMain->setContentsMargins(0,0,0,0);
    layMain->setSpacing(0);

    Views::ViewMain *viwMain = new Views::ViewMain();

    //add our main view
    layMain->addWidget(viwMain);

    //stylesheet
    useStyleSheet("style/focusStyle.css");

    //js required files
    require("resources/themes/bootstrap/js/bootstrap.js");
    require("resources/themes/bootstrap/js/bootstrap.min.js");
    require("resources/themes/bootstrap/js/npm.js");

    //add resources to style WAuthWidget
    messageResourceBundle().use("docroot/resources/xml/auth_strings");
    messageResourceBundle().use("docroot/resources/xml/bootstrap_theme");

    //handle internalPathChange
    //setInternalPath("/auth", true);
}

App::MSApplication::~MSApplication()
{
    delete _logger;
}

void App::MSApplication::createGlobalWidgets()
{
    _propertiesPanel = new Ms::Widgets::MPropertiesPanel();
    _logger = new Log::Logger();
}

void App::MSApplication::destroyGlobalWidgets()
{
    delete _propertiesPanel;
    delete _logger;
}

Database::DboSession &App::MSApplication::dboSession()
{
    return _dboSession;
}

Ms::Widgets::MPropertiesPanel *App::MSApplication::propertiesPanel()
{
    return _propertiesPanel;
}

Log::Logger *App::MSApplication::logger()
{
    return _logger;
}
