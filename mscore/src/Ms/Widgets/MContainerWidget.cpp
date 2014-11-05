#include "MContainerWidget.h"

Ms::Widgets::MContainerWidget::MContainerWidget()
{
    _prepareView();
}

const Wt::WString Ms::Widgets::MContainerWidget::title() const
{
    return _txtViewTitle->text();
}

void Ms::Widgets::MContainerWidget::setTitle(const Wt::WString &title)
{
    _txtViewTitle->setText(title);
}

Wt::Signal<> &Ms::Widgets::MContainerWidget::onExposed()
{
    return _onExposed;
}

Wt::Signal<> &Ms::Widgets::MContainerWidget::onHidden()
{
    return _onHidden;
}

void Ms::Widgets::MContainerWidget::setHidden(bool hidden, const Wt::WAnimation &animation)
{
    if(hidden)
        _onHidden();
    else
        _onExposed();

    Wt::WContainerWidget::setHidden(hidden, animation);
}

void Ms::Widgets::MContainerWidget::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _cntTxtViewTitle = new Wt::WContainerWidget();
    _cntTxtViewTitle->setStyleClass("title-bar");
    _cntTxtViewTitle->setContentAlignment(Wt::AlignCenter);
    _cntTxtViewTitle->setMinimumSize(Wt::WLength::Auto, 25);

    _layMain->addWidget(_cntTxtViewTitle);

    _txtViewTitle = new Wt::WText("<b><i>New View</i></b>");
    //_txtViewTitle->setStyleClass("title-bar-text");

    _cntTxtViewTitle->addWidget(_txtViewTitle);
}
