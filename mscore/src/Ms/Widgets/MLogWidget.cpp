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

void Ms::Widgets::MLogWidget::log(const std::string &message, Ms::Log::LogMessageType type)
{
    _checkMaxLogMessages();

    std::string str = "";

    Wt::WText *txt = new Wt::WText(str);

    if(type == Ms::Log::LogMessageType::Info)
    {
        str = "[Info]";
        txt->setStyleClass("inf-log");
    }
    else if(type == Ms::Log::LogMessageType::Warning)
    {
        str = "[Warn]";
        txt->setStyleClass("wrn-log");
    }
    else if(type == Ms::Log::LogMessageType::Error)
    {
        str = "[Error]";
        txt->setStyleClass("err-log");
    }
    else if(type == Ms::Log::LogMessageType::Fatal)
    {
        str = "[Fatal]";
        txt->setStyleClass("fatal-log");
    }

    str += "[" + Wt::WDateTime::currentDateTime().toString().toUTF8() + "] -> " + message;

    txt->setText(str.c_str());
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
