#include "MLogWidget.h"
#include "MWidgetFactory.h"

#include <Wt/WText>

Ms::Widgets::MLogWidget::MLogWidget()
{
    m_maxLogMessages = 2000;
    prepareView();
}

Ms::Widgets::MLogWidget::~MLogWidget()
{
    m_logMsgs.clear();
}

unsigned int Ms::Widgets::MLogWidget::maxLogMessages()
{
    return m_maxLogMessages;
}

void Ms::Widgets::MLogWidget::setMaxLogMessages(unsigned int value)
{
    m_maxLogMessages = value;
}

void Ms::Widgets::MLogWidget::clearLog()
{
    for(auto iter = m_logMsgs.begin(); iter != m_logMsgs.end(); ++iter)
        m_layCntLogArea->removeWidget((*iter));

    m_logMsgs.clear();
}

void Ms::Widgets::MLogWidget::log(const std::string &message, Ms::Log::LogMessageType type)
{
    checkMaxLogMessages();

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
    m_layCntLogArea->addWidget(txt);

    m_logMsgs.push_back(txt);
}

void Ms::Widgets::MLogWidget::onBtnClearLogClicked()
{
    clearLog();
}

void Ms::Widgets::MLogWidget::checkMaxLogMessages()
{
    if(m_logMsgs.size() == m_maxLogMessages)
    {
        m_layCntLogArea->removeWidget(m_logMsgs.at(0));
        m_logMsgs.erase(m_logMsgs.begin());
    }
}

void Ms::Widgets::MLogWidget::prepareView()
{
    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(4,4,4,4);
    m_layMain->setSpacing(0);

    setLayout(m_layMain);
    setLineHeight(15);
    setMinimumSize(Wt::WLength::Auto, 100);

    m_layCntTbMain = new Wt::WVBoxLayout();
    m_layCntTbMain->setContentsMargins(0,0,0,4);
    m_layCntTbMain->setSpacing(0);

    m_cntTbMain = new Wt::WContainerWidget();
    m_cntTbMain->setLayout(m_layCntTbMain);

    m_layMain->addWidget(m_cntTbMain);

    m_tbMain = new Wt::WToolBar();
    m_tbMain->setMaximumSize(Wt::WLength::Auto, 30);
    m_layCntTbMain->addWidget(m_tbMain);

    m_btnClearLog = Ms::Widgets::MWidgetFactory::createButton("", "icons/Clear.png", "Clear log");
    m_btnClearLog->clicked().connect(this, &Ms::Widgets::MLogWidget::onBtnClearLogClicked);
    m_tbMain->addButton(m_btnClearLog);

    m_cntLogArea = new Wt::WContainerWidget();

    m_layCntLogArea = new Wt::WVBoxLayout();
    m_layCntLogArea->setContentsMargins(0,0,0,0);
    m_layCntLogArea->setSpacing(0);
    m_cntLogArea->setLayout(m_layCntLogArea);

    m_scrLog = new Wt::WScrollArea();
    m_scrLog->setScrollBarPolicy(Wt::WScrollArea::ScrollBarAsNeeded);
    m_scrLog->setWidget(m_cntLogArea);

    m_layMain->addWidget(m_scrLog, 1);
}
