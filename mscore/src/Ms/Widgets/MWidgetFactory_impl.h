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
            Ms::Widgets::MQueryTableViewWidget<T> *qtv = new Ms::Widgets::MQueryTableViewWidget<T>(dboSession, parent);
            qtv->setHeaderHeight(24);
            qtv->setRowHeight(24);
            qtv->setSelectionMode(Wt::ExtendedSelection);
            qtv->setDynamicSortFilter(true);
            qtv->setFilterRole(Wt::DisplayRole);
            qtv->setFilterRegExp("[^$]{0,255}");
            qtv->setDefaultFilterColumnIndex(0);
            qtv->setIgnoreNumFilterColumns(0);

            return qtv;
        }
    }
}

#endif // MWIDGETFACTORY_IMPL_H
