#include "viewlog.h"

#include <Ms/Widgets/MWidgetFactory.h>

#include <Wt/WText>

Views::ViewLog::ViewLog()
{
    _maxLogMessages = 10000;
    _prepareView();
}

Views::ViewLog::~ViewLog()
{
    _logMsgs.clear();
}

unsigned int Views::ViewLog::maxLogMessages()
{
    return _maxLogMessages;
}

void Views::ViewLog::setMaxLogMessages(unsigned int value)
{
    _maxLogMessages = value;
}

void Views::ViewLog::clearLog()
{
    for(auto iter = _logMsgs.begin(); iter != _logMsgs.end(); ++iter)
        _layCntLogArea->removeWidget((*iter));

    _logMsgs.clear();
}

void Views::ViewLog::info(const std::string &message)
{
    _checkMaxLogMessages();

    std::string str = "[Info]";
    str += "[" + Wt::WDateTime::currentDateTime().toString().toUTF8() + "]->" + message;

    Wt::WText *txt = new Wt::WText(str);
    txt->setStyleClass("inf-log");

    _layCntLogArea->addWidget(txt);

    _logMsgs.push_back(txt);
}

void Views::ViewLog::warning(const std::string &message)
{
    _checkMaxLogMessages();

    std::string str = "[Warning]";
    str += "[" + Wt::WDateTime::currentDateTime().toString().toUTF8() + "]->" + message;

    Wt::WText *txt = new Wt::WText(str);
    txt->setStyleClass("wrn-log");

    _layCntLogArea->addWidget(txt);

    _logMsgs.push_back(txt);
}

void Views::ViewLog::error(const std::string &message)
{
    _checkMaxLogMessages();

    std::string str = "[Error]";
    str += "[" + Wt::WDateTime::currentDateTime().toString().toUTF8() + "]->" + message;

    Wt::WText *txt = new Wt::WText(str);
    txt->setStyleClass("err-log");

    _layCntLogArea->addWidget(txt);

    _logMsgs.push_back(txt);
}

void Views::ViewLog::fatal(const std::string &message)
{
    _checkMaxLogMessages();

    std::string str = "[Fatal]";
    str += "[" + Wt::WDateTime::currentDateTime().toString().toUTF8() + "]->" + message;

    Wt::WText *txt = new Wt::WText(str);
    txt->setStyleClass("fatal-log");

    _layCntLogArea->addWidget(txt);

    _logMsgs.push_back(txt);
}

void Views::ViewLog::_onBtnClearLogClicked()
{
    clearLog();
}

void Views::ViewLog::_checkMaxLogMessages()
{
    if(_logMsgs.size() == _maxLogMessages)
    {
        _layCntLogArea->removeWidget(_logMsgs.at(0));
        _logMsgs.erase(_logMsgs.begin());
    }
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

    _cntLogWidget = new Wt::WContainerWidget();
    _cntLogWidget->setMaximumSize(Wt::WLength::Auto, 230);

    _layCntLogWidget = new Wt::WVBoxLayout();
    _layCntLogWidget->setContentsMargins(0,0,0,0);
    _layCntLogWidget->setSpacing(0);

    _cntLogWidget->setLayout(_layCntLogWidget);
    _cntLogWidget->setLineHeight(15);

    _panLog->setCentralWidget(_cntLogWidget);

    _tbMain = new Wt::WToolBar();
    _tbMain->setMaximumSize(Wt::WLength::Auto, 30);
    _tbMain->setStyleClass("toolbar");
    _layCntLogWidget->addWidget(_tbMain);

    _btnClearLog = Ms::Widgets::MWidgetFactory::createButton("", "icons/Clear.png", "Clear log");
    _btnClearLog->clicked().connect(this, &Views::ViewLog::_onBtnClearLogClicked);
    _tbMain->addButton(_btnClearLog);

    _cntLogArea = new Wt::WContainerWidget();

    _layCntLogArea = new Wt::WVBoxLayout();
    _layCntLogArea->setContentsMargins(0,0,0,0);
    _layCntLogArea->setSpacing(0);
    _cntLogArea->setLayout(_layCntLogArea);

    _scrLog = new Wt::WScrollArea();
    _scrLog->setScrollBarPolicy(Wt::WScrollArea::ScrollBarAsNeeded);
    _scrLog->setWidget(_cntLogArea);
    _scrLog->setMinimumSize(Wt::WLength::Auto, 200);
    _scrLog->setMaximumSize(Wt::WLength::Auto, 200);

    _layCntLogWidget->addWidget(_scrLog, 1);
}
