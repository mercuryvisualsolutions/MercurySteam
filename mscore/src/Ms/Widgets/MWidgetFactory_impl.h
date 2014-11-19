#ifndef MWIDGETFACTORY_IMPL_H
#define MWIDGETFACTORY_IMPL_H

#include "MQueryTableViewWidget.h"

namespace Ms
{
    namespace Widgets
    {
        template<typename T>
        Ms::Widgets::MQueryTableViewWidget<T> *Ms::Widgets::MWidgetFactory::createQueryTableViewWidget(Core::Dbo::MDboSession &dboSession, Wt::WContainerWidget *parent)
        {
            Ms::Widgets::MQueryTableViewWidget<T> *_qtv = new Ms::Widgets::MQueryTableViewWidget<T>(dboSession, parent);
            _qtv->setHeaderHeight(24);
            _qtv->setRowHeight(24);
            _qtv->setSelectionMode(Wt::ExtendedSelection);
            _qtv->setDynamicSortFilter(true);
            _qtv->setFilterRole(Wt::DisplayRole);
            _qtv->setFilterRegExp("[^$]{0,255}");
            _qtv->setDefaultFilterColumnIndex(0);
            _qtv->setIgnoreNumFilterColumns(0);

            return _qtv;
        }
    }
}

#endif // MWIDGETFACTORY_IMPL_H
