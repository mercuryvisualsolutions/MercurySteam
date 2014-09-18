#include "MPropertiesPanel.h"

Ms::Widgets::MPropertiesPanel::MPropertiesPanel()
{
    _prepareView();
}

void Ms::Widgets::MPropertiesPanel::showView(const std::string &name)
{
    if(!propertiesViewExists(name))
    {
        Wt::WContainerWidget *view = new Wt::WContainerWidget();
        addPropertiesView(name, view);
    }

    _stkMain->setCurrentWidget(_views[name]);
}

void Ms::Widgets::MPropertiesPanel::addPropertiesView(const std::string &name, Wt::WWidget *widget)
{
    if(!propertiesViewExists(name))
    {
        _views[name] = widget;
        _stkMain->addWidget(widget);
    }
}

void Ms::Widgets::MPropertiesPanel::removePropertiesView(const std::string &name)
{
    if(propertiesViewExists(name))
    {
        _stkMain->removeWidget(_views[name]);
        _views.erase(name);
    }
}

bool Ms::Widgets::MPropertiesPanel::propertiesViewExists(const std::string &name) const
{
    return _views.find(name) != _views.end();
}

Wt::WWidget *Ms::Widgets::MPropertiesPanel::currentShownWidget() const
{
    return _stkMain->currentWidget();
}

std::string Ms::Widgets::MPropertiesPanel::currentShownView() const
{
    for(auto &pair : _views)
    {
        if(pair.second == _stkMain->currentWidget())
            return pair.first;
    }

    return "";
}

void Ms::Widgets::MPropertiesPanel::_prepareView()
{
    setMinimumSize(535, 500);
    setWidth(535);

    setTitle("<b><i>Properties</i></b>");

    Wt::WVBoxLayout *_layMain = dynamic_cast<Wt::WVBoxLayout*>(layout());
    _layMain->setContentsMargins(14,14,14,14);

    _stkMain = new Wt::WStackedWidget();

    _layMain->addWidget(_stkMain, 1);
}
