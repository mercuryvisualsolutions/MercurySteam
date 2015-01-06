#ifndef VIEWMAIN_H
#define VIEWMAIN_H

#include <Wt/WVBoxLayout>
#include <Wt/WStackedWidget>
#include <Wt/WContainerWidget>
#include <Wt/Auth/Login>
#include <Wt/Auth/AuthWidget>

#include "../App/viewapp.h"

namespace Views
{
    class ViewMain : public Wt::WContainerWidget
    {
    public:
        ViewMain();

        //functions
        void showAuthView();
        void authEvent();

    private:
        //variables
        bool m_applicationCreated;
        //UI
        Log::Logger *m_logger;
        Wt::WStackedWidget *m_stkMain;
        Wt::WVBoxLayout *m_layMain;
        //auth view
        Wt::WVBoxLayout *m_layCntAuth;
        Wt::WContainerWidget *m_cntAuth;
        Wt::Auth::AuthWidget *m_authWidget;

        //functions
        void loggedIn();
        void loggedOut();
        void showAppView();
        void prepareView();
        void prepareAppView(Wt::WStackedWidget *widget);

        //variables
        Views::ViewApp *m_viewApp;
    };
}

#endif // VIEWMAIN_H
