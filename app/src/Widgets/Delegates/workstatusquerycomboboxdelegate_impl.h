#ifndef WORKSTATUSQUERYCOMBOBOXDELEGATE_IMPL_H
#define WORKSTATUSQUERYCOMBOBOXDELEGATE_IMPL_H

#include <Ms/Widgets/Delegates/MQueryComboBoxDelegate.h>

namespace Widgets
{
    namespace Delegates
    {
        template<typename T>
        Widgets::Delegates::WorkStatusQueryComboBoxDelegate<T>::WorkStatusQueryComboBoxDelegate() :
            Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>()
        {
        }

        template<typename T>
        Widgets::Delegates::WorkStatusQueryComboBoxDelegate<T>::WorkStatusQueryComboBoxDelegate(Wt::Dbo::Session *session, Wt::Dbo::Query<Wt::Dbo::ptr<T> > comboBoxQuery, const std::string &displayColumn, int editRank) :
           Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>(session, comboBoxQuery, displayColumn, editRank)
        {
        }

        template<typename T>
        Wt::WWidget *Widgets::Delegates::WorkStatusQueryComboBoxDelegate<T>::update(Wt::WWidget *widget, const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags)
        {
//            if(widget)
//                widget->setStyleClass("work-status-in-progress");

            return Ms::Widgets::Delegates::MItemDelegate::update(widget, index, flags);
        }
    }
}

#endif // WORKSTATUSQUERYCOMBOBOXDELEGATE_IMPL_H
