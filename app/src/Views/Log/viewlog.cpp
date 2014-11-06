#include "viewlog.h"
#include "../../Log/logmanager.h"

#include <Wt/WApplication>

Views::ViewLog::ViewLog()
{
    _prepareView();
}

const Ms::Widgets::MLogWidget *Views::ViewLog::logWidget() const
{
    return Log::LogManager::instance().getSessionLogger(Wt::WApplication::instance()->sessionId())->logWidget();
}

void Views::ViewLog::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _layMain->addWidget(Log::LogManager::instance().getSessionLogger(Wt::WApplication::instance()->sessionId())->logWidget());
}
