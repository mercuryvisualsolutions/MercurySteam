#include "viewlog.h"
#include "../../Log/logmanager.h"
#include "../../Session/sessionmanager.h"

#include <Wt/WApplication>

Views::ViewLog::ViewLog()
{
    prepareView();
}

const Ms::Widgets::MLogWidget *Views::ViewLog::logWidget() const
{
    return Session::SessionManager::instance().logger()->logWidget();
}

void Views::ViewLog::prepareView()
{
    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);

    setLayout(m_layMain);

    m_layMain->addWidget(Session::SessionManager::instance().logger()->logWidget());
}
