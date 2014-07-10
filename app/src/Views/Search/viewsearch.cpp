#include "viewsearch.h"

Views::ViewSearch::ViewSearch()
{
    _prepareView();
}

void Views::ViewSearch::_mnuSearchProjectsItemTriggered()
{
    showView("projects");
}

void Views::ViewSearch::_mnuSearchUsersItemTriggered()
{
    showView("users");
}

void Views::ViewSearch::showView(const std::string &view)
{
    if(view == "projects")
    {
        _stkSearch->setCurrentWidget(_cntSearchProjects);
        _mnuSearch->select(_mnuSearchProjectsItem);
    }
    else if(view == "users")
    {
        _stkSearch->setCurrentWidget(_cntSearchUsers);
        _mnuSearch->select(_mnuSearchUsersItem);
    }
}

void Views::ViewSearch::_prepareView()
{
    /*******************--Search--********************/
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _navBarSearch = new Wt::WNavigationBar();
    _navBarSearch->setTitle("Search");
    _cntNavBarSearch = new Wt::WContainerWidget();
    _cntNavBarSearch->addWidget(_navBarSearch);

    //add our navigation bar to the view
    _layMain->addWidget(_cntNavBarSearch);

    _mnuSearch = new Wt::WMenu(Wt::Horizontal);
    _navBarSearch->addMenu(_mnuSearch);

    _mnuSearchProjectsItem = new Wt::WMenuItem("Projects");
    _mnuSearchProjectsItem->triggered().connect(this, &Views::ViewSearch::_mnuSearchProjectsItemTriggered);
    _mnuSearch->addItem(_mnuSearchProjectsItem);

    _mnuSearchUsersItem = new Wt::WMenuItem("Users");
    _mnuSearchUsersItem->triggered().connect(this, &Views::ViewSearch::_mnuSearchUsersItemTriggered);
    _mnuSearch->addItem(_mnuSearchUsersItem);

    _stkSearch = new Wt::WStackedWidget();
    _layMain->addWidget(_stkSearch, 1);//stack Projects view

    //Projects//////////////////////////////////////
    _laySearchProjects = new Wt::WVBoxLayout();
    _laySearchProjects->setContentsMargins(0,0,0,0);
    _laySearchProjects->setSpacing(0);

    _cntSearchProjects = new Wt::WContainerWidget();
    _cntSearchProjects->setStyleClass("toolbar");
    _cntSearchProjects->setLayout(_laySearchProjects);

    //add our Projects view to the Search view
    _stkSearch->addWidget(_cntSearchProjects);


    //users//////////////////////////////////////
    _laySearchUsers = new Wt::WVBoxLayout();
    _laySearchUsers->setContentsMargins(0,0,0,0);
    _laySearchUsers->setSpacing(0);

    _cntSearchUsers = new Wt::WContainerWidget();
    _cntSearchUsers->setStyleClass("toolbar");
    _cntSearchUsers->setLayout(_laySearchUsers);

    //add our Projects view to the Search view
    _stkSearch->addWidget(_cntSearchUsers);
}
