#include "viewlog.h"

Views::ViewLog::ViewLog()
{
    _prepareView();
}

const Ms::Log::Widgets::MLogWidget *Views::ViewLog::logWidget() const
{
    return Log::LogManager::instance().getLogger()->logWidget();
}

void Views::ViewLog::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _panLog = new Wt::WPanel();
    _panLog->setTitleBar(true);
    _panLog->setTitle("Log");
    _panLog->setCollapsible(true);

    //Wt::WAnimation panLogAnimation(Wt::WAnimation::SlideInFromTop, Wt::WAnimation::EaseOut, 100);
    //_panLog->setAnimation(panLogAnimation);
    _panLog->setStyleClass("toolbar");
    _panLog->setCollapsed(true);
    _panLog->setLineHeight(15);
    _layMain->addWidget(_panLog, 1);

    _panLog->setCentralWidget(Log::LogManager::instance().getLogger()->logWidget());
}
