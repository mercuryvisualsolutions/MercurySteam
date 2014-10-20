#ifndef MTABLEVIEWWIDGET_H
#define MTABLEVIEWWIDGET_H

#include "../Core/MTableViewColumn.h"

#include <Wt/WContainerWidget>
#include <Wt/WTableView>
#include <Wt/WStandardItemModel>
#include <Wt/WSortFilterProxyModel>
#include <Wt/WLineEdit>
#include <Wt/WComboBox>
#include <Wt/WToolBar>
#include <Wt/WPushButton>
#include <Wt/WSlider>
#include <Wt/WPopupMenu>
#include <Wt/WPopupMenuItem>

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
            const Wt::WAbstractItemModel *model() const;
            const Wt::WSortFilterProxyModel *proxyModel() const;

            const std::map<std::string,Ms::Core::MTableViewColumn> &columns() const;

            const Wt::WToolBar *tbMain() const;
            const Wt::WToolBar *tbGlobal() const;

            virtual void selectAll() const;
            virtual void selectNone() const;
            virtual void inverseSelection() const;

            virtual void importCSV(const std::string &) const;
            virtual void exportCSV();

            virtual bool columnExists(const std::string &name) const;
            virtual void addColumn(const Core::MTableViewColumn &column) const;
            virtual void removeColumn(const std::string &name) const;

            virtual void clear();


        protected:
            //UI
            Wt::WPushButton *_btnMnuTools;
            Wt::WPopupMenu *_popMnuTools;
            Wt::WPopupMenu *_popMnuSelection;
            Wt::WMenuItem *_popMnuSelectAllItem;
            Wt::WMenuItem *_popMnuSelectNoneItem;
            Wt::WMenuItem *_popMnuInverseSelectionItem;
            Wt::WPopupMenu *_popMnuIO;
            Wt::WMenuItem *_popMnuIOImportCSVItem;
            Wt::WMenuItem *_popMnuIOExportCSVItem;
            Wt::WTableView *_table;
            Wt::WStandardItemModel *_model;
            Wt::WSortFilterProxyModel *_proxyModel;
            Wt::WComboBox *_cmbFilterColumn;
            Wt::WStandardItemModel *_mdlCmbFilterColumn;
            Wt::WLineEdit *_lnFilter;
            Wt::WToolBar *_tbMain;
            Wt::WToolBar *_tbGlobal;
            Wt::WSlider *_sldRowHeight;

            //General
            int _defaultFilterColumnIndex;
            Wt::WString _filterRegExpression;
            int _ignoreNumFilterColumns;//numbers of columns to ignore when filtering
            mutable std::map<std::string,Ms::Core::MTableViewColumn> _columns;

            //functions
            virtual std::string tableName() const;
            virtual void createMainTable();

            virtual void refilter() const;
            virtual std::string generateCSVData() const;

            //slots
            virtual void lnFilterKeyWentUp();
            virtual void cmbFilterColumnChanged();
            virtual void cntTblMainKeyWentDown(Wt::WKeyEvent key);
            virtual void popMnuSelectAllItemTriggered();
            virtual void popMnuSelectNoneItemTriggered();
            virtual void popMnuInverseSelectionItemTriggered();
            virtual void popMnuIOImportCSVItemTriggered();
            virtual void popMnuIOExportCSVItemTriggered();
            virtual void sldRowHeightValueChanged();

        private:
            void prepareView();

        };
    }
}
#endif // MTABLEVIEWWIDGET_H
