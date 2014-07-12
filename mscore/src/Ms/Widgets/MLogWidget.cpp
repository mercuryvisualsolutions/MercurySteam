#include "MLogWidget.h"
#include "MWidgetFactory.h"

#include <Wt/WText>

Ms::Widgets::MLogWidget::MLogWidget()
{
    _maxLogMessages = 2000;
    _prepareView();
}

Ms::Widgets::MLogWidget::~MLogWidget()
{
    _logMsgs.clear();
}

unsigned int Ms::Widgets::MLogWidget::maxLogMessages()
{
    return _maxLogMessages;
}

void Ms::Widgets::MLogWidget::setMaxLogMessages(unsigned int value)
{
    _maxLogMessages = value;
}

void Ms::Widgets::MLogWidget::clearLog()
{
    for(auto iter = _logMsgs.begin(); iter != _logMsgs.end(); ++iter)
        _layCntLogArea->removeWidget((*iter));

    _logMsgs.clear();
}

void Ms::Widgets::MLogWidget::info(const std::string &message)
{
    _checkMaxLogMessages();

    std::string str = "[Info]";
    str += "[" + Wt::WDateTime::currentDateTime().toString().toUTF8() + "]->" + message;

    Wt::WText *txt = new Wt::WText(str);
    txt->setStyleClass("inf-log");

    _layCntLogArea->addWidget(txt);

    _logMsgs.push_back(txt);
}

void Ms::Widgets::MLogWidget::warning(const std::string &message)
{
    _checkMaxLogMessages();

    std::string str = "[Warning]";
    str += "[" + Wt::WDateTime::currentDateTime().toString().toUTF8() + "]->" + message;

    Wt::WText *txt = new Wt::WText(str);
    txt->setStyleClass("wrn-log");

    _layCntLogArea->addWidget(txt);

    _logMsgs.push_back(txt);
}

void Ms::Widgets::MLogWidget::error(const std::string &message)
{
    _checkMaxLogMessages();

    std::string str = "[Error]";
    str += "[" + Wt::WDateTime::currentDateTime().toString().toUTF8() + "]->" + message;

    Wt::WText *txt = new Wt::WText(str);
    txt->setStyleClass("err-log");

    _layCntLogArea->addWidget(txt);

    _logMsgs.push_back(txt);
}

void Ms::Widgets::MLogWidget::fatal(const std::string &message)
{
    _checkMaxLogMessages();

    std::string str = "[Fatal]";
    str += "[" + Wt::WDateTime::currentDateTime().toString().toUTF8() + "]->" + message;

    Wt::WText *txt = new Wt::WText(str);
    txt->setStyleClass("fatal-log");

    _layCntLogArea->addWidget(txt);

    _logMsgs.push_back(txt);
}

void Ms::Widgets::MLogWidget::_onBtnClearLogClicked()
{
    clearLog();
}

void Ms::Widgets::MLogWidget::_checkMaxLogMessages()
{
    if(_logMsgs.size() == _maxLogMessages)
    {
        _layCntLogArea->removeWidget(_logMsgs.at(0));
        _logMsgs.erase(_logMsgs.begin());
    }
}

void Ms::Widgets::MLogWidget::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);
    setLineHeight(15);

    _tbMain = new Wt::WToolBar();
    _tbMain->setMaximumSize(Wt::WLength::Auto, 30);
    _tbMain->setStyleClass("toolbar");
    _layMain->addWidget(_tbMain);

    _btnClearLog = Ms::Widgets::MWidgetFactory::createButton("", "icons/Clear.png", "Clear log");
    _btnClearLog->clicked().connect(this, &Ms::Widgets::MLogWidget::_onBtnClearLogClicked);
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

    _layMain->addWidget(_scrLog, 1);
}
