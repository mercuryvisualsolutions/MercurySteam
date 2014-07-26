#include "viewreports.h"

Views::ViewReports::ViewReports()
{
    _prepareView();
}

void Views::ViewReports::_mnuReportsProjectsItemTriggered()
{
    showView("projects");
}

void Views::ViewReports::_mnuReportsUsersItemTriggered()
{
    showView("users");
}

void Views::ViewReports::showView(const std::string &view)
{
    if(view == "projects")
    {
        _stkReports->setCurrentWidget(_cntReportsProjects);
        _mnuReports->select(_mnuReportsProjectsItem);
    }
    else if(view == "users")
    {
        _stkReports->setCurrentWidget(_cntReportsUsers);
        _mnuReports->select(_mnuReportsUsersItem);
    }
}

void Views::ViewReports::_prepareView()
{
    /*******************--Reports--********************/
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _navBarReports = new Wt::WNavigationBar();
    _navBarReports->setTitle("Reports");
    _cntNavBarReports = new Wt::WContainerWidget();
    _cntNavBarReports->addWidget(_navBarReports);

    //add our navigation bar to the view
    _layMain->addWidget(_cntNavBarReports);

    _mnuReports = new Wt::WMenu(Wt::Horizontal);
    _navBarReports->addMenu(_mnuReports);

    _mnuReportsProjectsItem = new Wt::WMenuItem("Projects");
    _mnuReportsProjectsItem->triggered().connect(this, &Views::ViewReports::_mnuReportsProjectsItemTriggered);
    _mnuReports->addItem(_mnuReportsProjectsItem);

    _mnuReportsUsersItem = new Wt::WMenuItem("Users");
    _mnuReportsUsersItem->triggered().connect(this, &Views::ViewReports::_mnuReportsUsersItemTriggered);
    _mnuReports->addItem(_mnuReportsUsersItem);

    _stkReports = new Wt::WStackedWidget();
    _layMain->addWidget(_stkReports, 1);//stack Projects view

    //Projects//////////////////////////////////////
    _layReportsProjects = new Wt::WVBoxLayout();
    _layReportsProjects->setContentsMargins(0,0,0,0);
    _layReportsProjects->setSpacing(0);

    _cntReportsProjects = new Wt::WContainerWidget();
    _cntReportsProjects->setLayout(_layReportsProjects);

    //add our Projects view to the Reports view
    _stkReports->addWidget(_cntReportsProjects);


    //users//////////////////////////////////////
    _layReportsUsers = new Wt::WVBoxLayout();
    _layReportsUsers->setContentsMargins(0,0,0,0);
    _layReportsUsers->setSpacing(0);

    _cntReportsUsers = new Wt::WContainerWidget();
    _cntReportsUsers->setLayout(_layReportsUsers);

    //add our Projects view to the Reports view
    _stkReports->addWidget(_cntReportsUsers);
}
