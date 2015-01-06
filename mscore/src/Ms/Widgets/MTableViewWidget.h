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

            Wt::WTableView *table() const;
            Wt::WStandardItemModel *model() const;
            Wt::WSortFilterProxyModel *proxyModel() const;

            const std::map<std::string,Ms::Core::MTableViewColumn> &columns() const;

            const Wt::WToolBar *tbMain() const;
            const Wt::WToolBar *tbGlobal() const;

            virtual void selectAll() const;
            virtual void selectNone() const;
            virtual void inverseSelection() const;

            virtual void importCSV(const std::string &) const;
            virtual void exportCSV();

            virtual bool columnExists(const std::string &name) const;
            virtual void addColumn(const Ms::Core::MTableViewColumn &column) const;
            virtual void removeColumn(const std::string &name) const;

            virtual void clear();

            virtual Wt::WString filterRegExpression() const;
            virtual void setFilterRegExpression(const Wt::WString &exp);

            virtual bool isImportCSVFeatureEnabled() const;
            virtual void setImportCSVFeatureEnabled(bool enabled);
            virtual bool isExportCSVFeatureEnabled() const;
            virtual void setExportCSVFeatureEnabled(bool enabled);


        protected:
            //UI
            Wt::WPushButton *m_btnMnuTools;
            Wt::WPopupMenu *m_popMnuTools;
            Wt::WPopupMenu *m_popMnuSelection;
            Wt::WMenuItem *m_popMnuSelectAllItem;
            Wt::WMenuItem *m_popMnuSelectNoneItem;
            Wt::WMenuItem *m_popMnuInverseSelectionItem;
            Wt::WPopupMenu *m_popMnuIO;
            Wt::WMenuItem *m_popMnuIOImportCSVItem;
            Wt::WMenuItem *m_popMnuIOExportCSVItem;
            Wt::WTableView *m_table;
            Wt::WStandardItemModel *m_model;
            Wt::WSortFilterProxyModel *m_proxyModel;
            Wt::WComboBox *m_cmbFilterColumn;
            Wt::WStandardItemModel *m_mdlCmbFilterColumn;
            Wt::WLineEdit *m_lnFilter;
            Wt::WToolBar *m_tbMain;
            Wt::WToolBar *m_tbGlobal;
            Wt::WSlider *m_sldRowHeight;
            Wt::WContainerWidget *m_cntStatusBar;
            Wt::WHBoxLayout *m_layCntStatusBar;
            Wt::WLabel *m_lblStatus;

            //General
            int m_defaultFilterColumnIndex;
            Wt::WString m_filterRegExpression;
            int m_ignoreNumFilterColumns;//numbers of columns to ignore when filtering
            mutable std::map<std::string,Ms::Core::MTableViewColumn> m_columns;

            bool m_importCSVFeatureEnabled;
            bool m_exportCSVFeatureEnabled;

            //functions
            virtual std::string tableName() const;
            void setTableName(const std::string &name);
            virtual void createMainTable();

            virtual void refilter() const;
            virtual std::string generateCSVData() const;

            virtual void updateStatusBar() const;

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
            virtual void mainTableSelectionChanged();
            virtual void modelRowsInserted();
            virtual void modelRowsRemoved();

        private:
            void prepareView();

            std::string _tableName;

        };
    }
}
#endif // MTABLEVIEWWIDGET_H
