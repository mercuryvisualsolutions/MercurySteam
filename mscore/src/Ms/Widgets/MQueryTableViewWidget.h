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

#include "../Core/Dbo/MDboManagerBase.h"
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
            MQueryTableViewWidget(Wt::WContainerWidget *parent = 0);
            MQueryTableViewWidget(Ms::Core::Dbo::MDboManagerBase *dboManager, Wt::WContainerWidget *parent = 0);

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
            const Ms::Core::Dbo::MDboManagerBase *dboManager() const;
            void setDboManager(Ms::Core::Dbo::MDboManagerBase *dboManager);
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
            void updateView() const;
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
            Wt::Signal<> _tableSelectionChanged;
            Wt::Signal<Wt::Dbo::ptr<T>> _itemImported;

            //slots
            void _mainTableSelectionChanged();
            void _cmbColumnFilterChanged();
            void _lnFilterKeyWentUp();
            void _cntTblMainKeyWentDown(Wt::WKeyEvent key);
            void _popMnuSelectAllItemTriggered();
            void _popMnuSelectNoneItemTriggered();
            void _popMnuInverseSelectionItemTriggered();
            void _popMnuIOImportCSVItemTriggered();
            void _popMnuIOExportCSVItemTriggered();
            void _popMnuViewAdvancedFilterItemTriggered();
            void _btnAdvancedFilterAddClicked();
            void _btnAdvancedFilterRemoveClicked();
            void _btnAdvancedFilterApplyClicked();
            void _btnAdvancedFilterClearClicked();
            void _popMnuReloadItemTriggered();
            void _sldRowHeightValueChanged();

            //functions
            void _importCSV(const std::string &fileName) const;
            std::string _generateCSVData() const;
            void _setModelUserName(const std::string &name) const;
            void _refilter() const;
            void _createMainTable();
            void _createAdvancedFilterView();
            void _createPropertiesView();
            void _updateModel() const;
            void _updateTable() const;
            void _updateAdvancedFilterTable() const;
            void _toggleAdvancedFilterView() const;
            void _prepareView();

            //variables
            mutable std::vector<Ms::Widgets::MQueryTableViewColumn> _columns;
            mutable Wt::Dbo::Query<Wt::Dbo::ptr<T>> _query;
            mutable Wt::Dbo::Query<Wt::Dbo::ptr<T>> _queryFilter;
            mutable std::vector<typename Wt::Dbo::dbo_traits<T>::IdType> _selection;//for saving/loading selection in view
            Ms::Core::Dbo::MDboManagerBase *_dboManager;
            int _defaultFilterColumnIndex;
            Wt::WString _filterRegExpression;
            int _ignoreNumFilterColumns;//numbers of columns to ignore when filtering
            bool _advancedFilterActive;
            bool _customFilterActive;
            std::string _customFilterString;
            std::string _advancedFilterString;
            bool _importCSVFeatureEnabled;
            bool _exportCSVFeatureEnabled;
            bool _advancedFilterFeatureEnabled;

            //ui variables
            Wt::WPushButton *_btnMnuTools;
            Wt::WPopupMenu *_popMnuTools;
            Wt::WPopupMenu *_popMnuSelection;
            Wt::WMenuItem *_popMnuSelectAllItem;
            Wt::WMenuItem *_popMnuSelectNoneItem;
            Wt::WMenuItem *_popMnuInverseSelectionItem;
            Wt::WPopupMenu *_popMnuIO;
            Wt::WMenuItem *_popMnuIOImportCSVItem;
            Wt::WMenuItem *_popMnuIOExportCSVItem;
            Wt::WPopupMenu *_popMnuView;
            Wt::WMenuItem *_popMnuViewAdvancedFilterItem;
            Wt::WMenuItem *_popMnuReloadItem;
            Ms::Dbo::MDboQueryModel<Wt::Dbo::ptr<T>> *_model;
            Wt::WSortFilterProxyModel *_proxyModel;
            Wt::WToolBar *_tbMain;
            Wt::WToolBar *_tbGlobal;
            Wt::WComboBox *_cmbColumnFilter;
            Wt::WStandardItemModel *_mdlCmbColumnFilter;
            Wt::WLineEdit *_lnFilter;
            Wt::WTableView *_tblMain;
            Wt::WSlider *_sldRowHeight;

            //Advanced filter
            Ms::Widgets::MContainerWidget *_cntAdvancedFilter;
            Wt::WVBoxLayout *_layCntAdvancedFilter;
            Wt::WToolBar *_tbAdvancedFilter;
            Wt::WContainerWidget *_cntTbAdvancedFilter;
            Wt::WVBoxLayout *_layCntTbAdvancedFilter;
            Wt::WTableView *_tblAdvancedFilter;
            Wt::WStandardItemModel *_mdlAdvancedFilter;
        };
    }
}

#include "MQueryTableViewWidget_impl.h"

#endif // QUERYTABLEVIEWWIDGET_H
