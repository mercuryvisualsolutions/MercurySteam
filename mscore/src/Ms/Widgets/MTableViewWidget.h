#ifndef MTABLEVIEWWIDGET_H
#define MTABLEVIEWWIDGET_H

#include "../Core/MTableViewColumn.h"
#include "../Core/MItemModel.h"

#include <Wt/WContainerWidget>
#include <Wt/WTableView>
#include <Wt/WStandardItemModel>
#include <Wt/WSortFilterProxyModel>
#include <Wt/WLineEdit>
#include <Wt/WComboBox>
#include <Wt/WToolBar>
#include <Wt/WPushButton>

namespace Ms
{
    namespace Widgets
    {
        class MTableViewWidget : public Wt::WContainerWidget
        {
        public:
            MTableViewWidget();
            ~MTableViewWidget();

            const Wt::WTableView *table() const;
            const Ms::Core::MItemModel *model() const;
            const Wt::WSortFilterProxyModel *proxyModel() const;

            const std::vector<Ms::Core::MTableViewColumn> columns() const;

            const Wt::WToolBar *tbMain() const;
            const Wt::WToolBar *tbGlobal() const;

            virtual void selectAll() const;
            virtual void selectnone() const;
            virtual void inverseSelection() const;

            virtual void importCSV();
            virtual void exportCSV();


        protected:
            Wt::WTableView *_table;
            Ms::Core::MItemModel *_model;
            Wt::WSortFilterProxyModel *_proxyModel;
            Wt::WLineEdit *_lnFilter;
            Wt::WComboBox *_cmbFilterColumnSelector;
            Wt::WToolBar *_tbMain;
            Wt::WToolBar *_tbGlobal;

            mutable std::vector<Ms::Core::MTableViewColumn> _columns;

        };
    }
}
#endif // MTABLEVIEWWIDGET_H
