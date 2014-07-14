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
        Log::Logger *_logger;
        Wt::WStackedWidget *_stkMain;
        Wt::WVBoxLayout *_layMain;
        //auth view
        Wt::WVBoxLayout *_layCntAuth;
        Wt::WContainerWidget *_cntAuth;
        Wt::Auth::AuthWidget *_authWidget;

        //functions
        void _showAppView();
        void _prepareView();
        void _prepareAppView(Wt::WStackedWidget *widget);

        //variables
        Views::ViewApp *viwApp;
    };
}

#endif // VIEWMAIN_H
