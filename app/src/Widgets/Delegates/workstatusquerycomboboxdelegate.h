#ifndef WORKSTATUSQUERYCOMBOBOXDELEGATE_H
#define WORKSTATUSQUERYCOMBOBOXDELEGATE_H

#include <Wt/WContainerWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WComboBox>

#include <Ms/Widgets/Delegates/MQueryComboBoxDelegate.h>

namespace Widgets
{
    namespace Delegates
    {
        template<typename T>
        class WorkStatusQueryComboBoxDelegate : public Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>
        {
        public:
            WorkStatusQueryComboBoxDelegate();
            WorkStatusQueryComboBoxDelegate(Wt::Dbo::Session *session, Wt::Dbo::Query<Wt::Dbo::ptr<T>> comboBoxQuery,
                                       const std::string &displayColumn, int editRank = 0);

            Wt::WWidget *update(Wt::WWidget *widget, const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) override;
        };
    }
}

#include "workstatusquerycomboboxdelegate_impl.h"

#endif // WORKSTATUSQUERYCOMBOBOXDELEGATE_H
