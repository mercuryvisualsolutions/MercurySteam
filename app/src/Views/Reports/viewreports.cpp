#include "viewreports.h"

Views::ViewReports::ViewReports()
{
    prepareView();
}

void Views::ViewReports::mnuReportsProjectsItemTriggered()
{
    showView("projects");
}

void Views::ViewReports::mnuReportsUsersItemTriggered()
{
    showView("users");
}

void Views::ViewReports::showView(const std::string &view)
{
    if(view == "projects")
    {
        m_stkReports->setCurrentWidget(m_cntReportsProjects);
        m_mnuReports->select(m_mnuReportsProjectsItem);
    }
    else if(view == "users")
    {
        m_stkReports->setCurrentWidget(m_cntReportsUsers);
        m_mnuReports->select(m_mnuReportsUsersItem);
    }
}

void Views::ViewReports::prepareView()
{
    /*******************--Reports--********************/
    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);

    setLayout(m_layMain);

    m_navBarReports = new Wt::WNavigationBar();
    m_navBarReports->setTitle("Reports");
    m_cntNavBarReports = new Wt::WContainerWidget();
    m_cntNavBarReports->addWidget(m_navBarReports);

    //add our navigation bar to the view
    m_layMain->addWidget(m_cntNavBarReports);

    m_mnuReports = new Wt::WMenu(Wt::Horizontal);
    m_navBarReports->addMenu(m_mnuReports);

    m_mnuReportsProjectsItem = new Wt::WMenuItem("Projects");
    m_mnuReportsProjectsItem->triggered().connect(this, &Views::ViewReports::mnuReportsProjectsItemTriggered);
    m_mnuReports->addItem(m_mnuReportsProjectsItem);

    m_mnuReportsUsersItem = new Wt::WMenuItem("Users");
    m_mnuReportsUsersItem->triggered().connect(this, &Views::ViewReports::mnuReportsUsersItemTriggered);
    m_mnuReports->addItem(m_mnuReportsUsersItem);

    m_stkReports = new Wt::WStackedWidget();
    m_layMain->addWidget(m_stkReports, 1);//stack Projects view

    //Projects//////////////////////////////////////
    m_layReportsProjects = new Wt::WVBoxLayout();
    m_layReportsProjects->setContentsMargins(0,0,0,0);
    m_layReportsProjects->setSpacing(0);

    m_cntReportsProjects = new Wt::WContainerWidget();
    m_cntReportsProjects->setLayout(m_layReportsProjects);

    //add our Projects view to the Reports view
    m_stkReports->addWidget(m_cntReportsProjects);


    //users//////////////////////////////////////
    m_layReportsUsers = new Wt::WVBoxLayout();
    m_layReportsUsers->setContentsMargins(0,0,0,0);
    m_layReportsUsers->setSpacing(0);

    m_cntReportsUsers = new Wt::WContainerWidget();
    m_cntReportsUsers->setLayout(m_layReportsUsers);

    //add our Projects view to the Reports view
    m_stkReports->addWidget(m_cntReportsUsers);
}
