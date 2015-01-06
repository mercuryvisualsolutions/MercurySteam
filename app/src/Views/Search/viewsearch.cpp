#include "viewsearch.h"

Views::ViewSearch::ViewSearch()
{
    prepareView();
}

void Views::ViewSearch::mnuSearchProjectsItemTriggered()
{
    showView("projects");
}

void Views::ViewSearch::mnuSearchUsersItemTriggered()
{
    showView("users");
}

void Views::ViewSearch::showView(const std::string &view)
{
    if(view == "projects")
    {
        m_stkSearch->setCurrentWidget(m_cntSearchProjects);
        m_mnuSearch->select(m_mnuSearchProjectsItem);
    }
    else if(view == "users")
    {
        m_stkSearch->setCurrentWidget(m_cntSearchUsers);
        m_mnuSearch->select(m_mnuSearchUsersItem);
    }
}

void Views::ViewSearch::prepareView()
{
    /*******************--Search--********************/
    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);

    setLayout(m_layMain);

    m_navBarSearch = new Wt::WNavigationBar();
    m_navBarSearch->setTitle("Search");
    m_cntNavBarSearch = new Wt::WContainerWidget();
    m_cntNavBarSearch->addWidget(m_navBarSearch);

    //add our navigation bar to the view
    m_layMain->addWidget(m_cntNavBarSearch);

    m_mnuSearch = new Wt::WMenu(Wt::Horizontal);
    m_navBarSearch->addMenu(m_mnuSearch);

    m_mnuSearchProjectsItem = new Wt::WMenuItem("Projects");
    m_mnuSearchProjectsItem->triggered().connect(this, &Views::ViewSearch::mnuSearchProjectsItemTriggered);
    m_mnuSearch->addItem(m_mnuSearchProjectsItem);

    m_mnuSearchUsersItem = new Wt::WMenuItem("Users");
    m_mnuSearchUsersItem->triggered().connect(this, &Views::ViewSearch::mnuSearchUsersItemTriggered);
    m_mnuSearch->addItem(m_mnuSearchUsersItem);

    m_stkSearch = new Wt::WStackedWidget();
    m_layMain->addWidget(m_stkSearch, 1);//stack Projects view

    //Projects//////////////////////////////////////
    m_laySearchProjects = new Wt::WVBoxLayout();
    m_laySearchProjects->setContentsMargins(0,0,0,0);
    m_laySearchProjects->setSpacing(0);

    m_cntSearchProjects = new Wt::WContainerWidget();
    m_cntSearchProjects->setLayout(m_laySearchProjects);

    //add our Projects view to the Search view
    m_stkSearch->addWidget(m_cntSearchProjects);


    //users//////////////////////////////////////
    m_laySearchUsers = new Wt::WVBoxLayout();
    m_laySearchUsers->setContentsMargins(0,0,0,0);
    m_laySearchUsers->setSpacing(0);

    m_cntSearchUsers = new Wt::WContainerWidget();
    m_cntSearchUsers->setLayout(m_laySearchUsers);

    //add our Projects view to the Search view
    m_stkSearch->addWidget(m_cntSearchUsers);
}
