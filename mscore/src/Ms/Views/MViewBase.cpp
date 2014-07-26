#include "MViewBase.h"

Ms::Views::MViewBase::MViewBase()
{
}

Wt::Signal<> &Ms::Views::MViewBase::onExposed()
{
    return _onExposed;
}

Wt::Signal<> &Ms::Views::MViewBase::onHidden()
{
    return _onHidden;
}

void Ms::Views::MViewBase::setHidden(bool hidden, const Wt::WAnimation &animation)
{
    Wt::WContainerWidget::setHidden(hidden, animation);

    if(hidden)
        _onHidden();
    else
        _onExposed();
}
