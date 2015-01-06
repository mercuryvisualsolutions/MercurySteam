#ifndef QUERYTABLEVIEWWIDGET_H
#define QUERYTABLEVIEWWIDGET_H

#include "MQueryTableViewColumn.h"
#include "../Dbo/MDboQueryModel.h"
#include "../IO/IO.h"
#include "../IO/MStreamedFileResource.h"
#include "../IO/MStreamedMemoryResource.h"
#include "Dialogs/MFilesUploadDialog.h"

#include <tuple>
#include <utility>
#include <iostream>
#include <algorithm>

#include <Wt/WPopupMenu>
#include <Wt/WPopupMenuItem>
#include <Wt/WSortFilterProxyModel>
#include <Wt/WStandardItemModel>
#include <Wt/Dbo/QueryModel>
#include <Wt/WToolBar>
#include <Wt/WComboBox>
#include <Wt/WTableView>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WToolBar>
#include <Wt/WStackedWidget>
#include <Wt/WNavigationBar>
#include <Wt/Dbo/Dbo>
#include <Wt/WMemoryResource>
#include <Wt/WSlider>

#include "../Core/Dbo/MDboSession.h"
#include "MContainerWidget.h"

namespace Ms
{
    namespace Widgets
    {
        template<typename T>
        class MQueryTableViewWidget : public Wt::WContainerWidget
        {
        public:
            //constructors
            MQueryTableViewWidget(Ms::Core::Dbo::MDboSession &dboSession, Wt::WContainerWidget *parent = 0);

            //destructor
            ~MQueryTableViewWidget();

            //variables
            using item_type = T;

            //functions
            const Ms::Dbo::MDboQueryModel<Wt::Dbo::ptr<T>> *model() const;
            const Wt::WSortFilterProxyModel *proxyModel() const;
            Wt::WTableView *table() const;
            const std::string tableName() const;
            Wt::WPushButton *createToolButton(const std::string &text, const std::string &iconPath = "", const std::string &toolTip = "");
            void setRowHeight(int height);
            int rowHeight() const;
            void setHeaderHeight(const Wt::WLength &height);
            const Wt::WLength headerHeight() const;
            void setQuery(Wt::Dbo::Query<Wt::Dbo::ptr<T>> &query) const;
            Wt::Dbo::Query<Wt::Dbo::ptr<T>> &query() const;
            const Ms::Core::Dbo::MDboSession *dboSession() const;
            void setDboSession(Ms::Core::Dbo::MDboSession &dboSession);
            void setDefaultFilterColumnIndex(int columnIndex);
            int defaultFilterColumnIndex();
            void setSelectionMode(Wt::SelectionMode mode);
            Wt::SelectionMode selectionMode();
            void setDynamicSortFilter(bool enable);
            bool dynamicSortFilter() const;
            void setFilterRole(int role);
            int filterRole();
            void setFilterRegExp(const Wt::WString &regExpression);
            Wt::WString filterRegExp() const;
            void setColumnResizeEnabled(bool enabled);
            bool isColumnResizeEnabled() const;
            void setAlternatingRowColors(bool enable);
            bool alternatingRowColors() const;
            void updateView();
            void saveSelection() const;
            void loadSelection() const;
            void selectAll() const;
            void selectNone() const;
            void inverseSelection() const;
            void addColumn(const Ms::Widgets::MQueryTableViewColumn &column) const;
            void removeColumn(const Ms::Widgets::MQueryTableViewColumn &column) const;
            void clearColumns() const;
            bool dbColumnExist(const std::string &dbColumnName) const;
            bool columnExist(const std::string &columnDisplayName) const;
            bool columnIsIgnored(const std::string &columnDisplayName) const;
            std::string columnName(const std::string &columnDisplayName) const;
            std::string columnDisplayName(const std::string &dboName) const;
            void setIgnoreNumFilterColumns(int numColumns);
            void ignoredNumFilterColumns() const;
            std::vector<Wt::Dbo::ptr<T>> selectedItems() const;
            std::vector<Wt::Dbo::ptr<T>> items() const;
            bool isImportCSVFeatureEnabled() const;
            void setImportCSVFeatureEnabled(bool enabled);
            bool isExportCSVFeatureEnabled() const;
            void setExportCSVFeatureEnabled(bool enabled);
            bool isAdvancedFilterFeatureEnabled() const;
            void setAdvancedFilterFeatureEnabled(bool enabled);
            bool isAdvancedFilterActive() const;
            void setAdvancedFilterActive(bool active);
            std::string advancedFilterString() const;
            bool isCustomFilterActive() const;
            void setCustomFilterActive(bool active);
            std::string customFilterString() const;
            void setCustomFilterString(const std::string &customFilterString);
            void addBaseColumns(Wt::WFlags<Wt::ItemFlag> flags, int editRank) const;

            //signals
            Wt::Signal<> &tableSelectionChanged();
            Wt::Signal<Wt::Dbo::ptr<T>> &itemImported();

        private:
            //signals
            Wt::Signal<> m_tableSelectionChanged;
            Wt::Signal<Wt::Dbo::ptr<T>> m_itemImported;

            //slots
            void mainTableSelectionChanged();
            void cmbColumnFilterChanged();
            void lnFilterKeyWentUp();
            void cntTblMainKeyWentDown(Wt::WKeyEvent key);
            void popMnuSelectAllItemTriggered();
            void popMnuSelectNoneItemTriggered();
            void popMnuInverseSelectionItemTriggered();
            void popMnuIOImportCSVItemTriggered();
            void popMnuIOExportCSVItemTriggered();
            void popMnuViewAdvancedFilterItemTriggered();
            void btnAdvancedFilterAddClicked();
            void btnAdvancedFilterRemoveClicked();
            void btnAdvancedFilterApplyClicked();
            void btnAdvancedFilterClearClicked();
            void popMnuReloadItemTriggered();
            void sldRowHeightValueChanged();

            //functions
            void importCSV(const std::string &fileName);
            std::string generateCSVData() const;
            void setModelUserName(const std::string &name) const;
            void refilter() const;
            void createMainTable();
            void createAdvancedFilterView();
            void createPropertiesView();
            void updateModel();
            void updateTable() const;
            void updateAdvancedFilterTable() const;
            void toggleAdvancedFilterView() const;
            void updateStatusBar() const;
            void prepareView();

            //variables
            mutable std::vector<Ms::Widgets::MQueryTableViewColumn> m_columns;
            mutable Wt::Dbo::Query<Wt::Dbo::ptr<T>> m_query;
            mutable Wt::Dbo::Query<Wt::Dbo::ptr<T>> m_queryFilter;
            mutable std::vector<typename Wt::Dbo::dbo_traits<T>::IdType> m_selection;//for saving/loading selection in view
            Ms::Core::Dbo::MDboSession &m_dboSession;
            int m_defaultFilterColumnIndex;
            Wt::WString m_filterRegExpression;
            int m_ignoreNumFilterColumns;//numbers of columns to ignore when filtering
            bool m_advancedFilterActive;
            bool m_customFilterActive;
            std::string m_customFilterString;
            std::string m_advancedFilterString;
            bool m_importCSVFeatureEnabled;
            bool m_exportCSVFeatureEnabled;
            bool m_advancedFilterFeatureEnabled;

            //ui variables
            Wt::WPushButton *m_btnMnuTools;
            Wt::WPopupMenu *m_popMnuTools;
            Wt::WPopupMenu *m_popMnuSelection;
            Wt::WMenuItem *m_popMnuSelectAllItem;
            Wt::WMenuItem *m_popMnuSelectNoneItem;
            Wt::WMenuItem *m_popMnuInverseSelectionItem;
            Wt::WPopupMenu *m_popMnuIO;
            Wt::WMenuItem *m_popMnuIOImportCSVItem;
            Wt::WMenuItem *m_popMnuIOExportCSVItem;
            Wt::WPopupMenu *m_popMnuView;
            Wt::WMenuItem *m_popMnuViewAdvancedFilterItem;
            Wt::WMenuItem *m_popMnuReloadItem;
            Ms::Dbo::MDboQueryModel<Wt::Dbo::ptr<T>> *m_model;
            Wt::WSortFilterProxyModel *m_proxyModel;
            Wt::WToolBar *m_tbMain;
            Wt::WToolBar *m_tbGlobal;
            Wt::WComboBox *m_cmbColumnFilter;
            Wt::WStandardItemModel *m_mdlCmbColumnFilter;
            Wt::WLineEdit *m_lnFilter;
            Wt::WTableView *m_tblMain;
            Wt::WContainerWidget *m_cntStatusBar;
            Wt::WHBoxLayout *m_layCntStatusBar;
            Wt::WLabel *m_lblStatus;
            Wt::WSlider *m_sldRowHeight;
            Wt::WCheckBox *m_chkCustomFilterActive;
            Wt::WCheckBox *m_chkAdvancedFilterActive;

            //Advanced filter
            Ms::Widgets::MContainerWidget *m_cntAdvancedFilter;
            Wt::WVBoxLayout *m_layCntAdvancedFilter;
            Wt::WToolBar *m_tbAdvancedFilter;
            Wt::WContainerWidget *m_cntTbAdvancedFilter;
            Wt::WVBoxLayout *m_layCntTbAdvancedFilter;
            Wt::WTableView *m_tblAdvancedFilter;
            Wt::WStandardItemModel *m_mdlAdvancedFilter;
        };
    }
}

#include "MQueryTableViewWidget_impl.h"

#endif // QUERYTABLEVIEWWIDGET_H
