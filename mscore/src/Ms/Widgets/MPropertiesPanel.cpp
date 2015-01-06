#include "MPropertiesPanel.h"

Ms::Widgets::MPropertiesPanel::MPropertiesPanel()
{
    prepareView();
}

void Ms::Widgets::MPropertiesPanel::showView(const std::string &name)
{
    if(!propertiesViewExists(name))
    {
        Wt::WContainerWidget *view = new Wt::WContainerWidget();
        addPropertiesView(name, view);
    }

    m_stkMain->setCurrentWidget(m_views[name]);
}

void Ms::Widgets::MPropertiesPanel::addPropertiesView(const std::string &name, Wt::WWidget *widget)
{
    if(!propertiesViewExists(name))
    {
        m_views[name] = widget;
        m_stkMain->addWidget(widget);
    }
}

void Ms::Widgets::MPropertiesPanel::removePropertiesView(const std::string &name)
{
    if(propertiesViewExists(name))
    {
        m_stkMain->removeWidget(m_views[name]);
        m_views.erase(name);
    }
}

bool Ms::Widgets::MPropertiesPanel::propertiesViewExists(const std::string &name) const
{
    return m_views.find(name) != m_views.end();
}

Wt::WWidget *Ms::Widgets::MPropertiesPanel::currentShownWidget() const
{
    return m_stkMain->currentWidget();
}

std::string Ms::Widgets::MPropertiesPanel::currentShownView() const
{
    for(auto &pair : m_views)
    {
        if(pair.second == m_stkMain->currentWidget())
            return pair.first;
    }

    return "";
}

void Ms::Widgets::MPropertiesPanel::prepareView()
{
    setMinimumSize(300, 500);
    setWidth(600);

    setTitle("<b><i>Properties</i></b>");

    Wt::WVBoxLayout *m_layMain = dynamic_cast<Wt::WVBoxLayout*>(layout());
    m_layMain->setContentsMargins(14,0,14,14);

    m_stkMain = new Wt::WStackedWidget();

    m_layMain->addWidget(m_stkMain, 1);
}
