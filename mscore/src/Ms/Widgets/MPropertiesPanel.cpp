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
    setMaximumSize(534, Wt::WLength::Auto);
    setMinimumSize(534, Wt::WLength::Auto);

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(14,14,14,14);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _cntTxtPropertiesTitle = new Wt::WContainerWidget();
    _cntTxtPropertiesTitle->setStyleClass("title-bar");
    _cntTxtPropertiesTitle->setContentAlignment(Wt::AlignCenter);
    _cntTxtPropertiesTitle->setMinimumSize(Wt::WLength::Auto, 25);

    _layMain->addWidget(_cntTxtPropertiesTitle);

    _txtPropertiesTitle = new Wt::WText("<b><i>Properties</i></b>");
    _txtPropertiesTitle->setStyleClass("title-bar-text");

    _cntTxtPropertiesTitle->addWidget(_txtPropertiesTitle);

    _stkMain = new Wt::WStackedWidget();

    _layMain->addWidget(_stkMain, 1);
}
