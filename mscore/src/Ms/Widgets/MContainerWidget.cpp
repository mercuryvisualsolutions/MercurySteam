#include "MContainerWidget.h"

Ms::Widgets::MContainerWidget::MContainerWidget()
{
    prepareView();
}

const Wt::WString Ms::Widgets::MContainerWidget::title() const
{
    return m_txtViewTitle->text();
}

void Ms::Widgets::MContainerWidget::setTitle(const Wt::WString &title)
{
    m_txtViewTitle->setText(title);
}

Wt::Signal<> &Ms::Widgets::MContainerWidget::onExposed()
{
    return m_onExposed;
}

Wt::Signal<> &Ms::Widgets::MContainerWidget::onHidden()
{
    return m_onHidden;
}

void Ms::Widgets::MContainerWidget::setHidden(bool hidden, const Wt::WAnimation &animation)
{
    if(hidden)
        m_onHidden();
    else
        m_onExposed();

    Wt::WContainerWidget::setHidden(hidden, animation);
}

void Ms::Widgets::MContainerWidget::prepareView()
{
    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);

    setLayout(m_layMain);

    m_cntTxtViewTitle = new Wt::WContainerWidget();
    m_cntTxtViewTitle->setStyleClass("title-bar");
    m_cntTxtViewTitle->setContentAlignment(Wt::AlignCenter);
    m_cntTxtViewTitle->setMinimumSize(Wt::WLength::Auto, 25);

    m_layMain->addWidget(m_cntTxtViewTitle);

    m_txtViewTitle = new Wt::WText("<b><i>New View</i></b>");
    //m_txtViewTitle->setStyleClass("title-bar-text");

    m_cntTxtViewTitle->addWidget(m_txtViewTitle);
}
