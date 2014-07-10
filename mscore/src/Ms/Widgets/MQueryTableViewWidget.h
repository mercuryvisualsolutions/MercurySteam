#ifndef QUERYTABLEVIEWWIDGET_H
#define QUERYTABLEVIEWWIDGET_H

#include "MTableViewColumn.h"
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
#include <Wt/Dbo/Dbo>
#include <Wt/WMemoryResource>

#include "../Core/Dbo/MDboManagerBase.h"

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
            void setRowHeight(const Wt::WLength &height);
            const Wt::WLength rowHeight() const;
            void setHeaderHeight(const Wt::WLength &height);
            const Wt::WLength headerHeight() const;
            void setQuery(Wt::Dbo::Query<Wt::Dbo::ptr<T> > &query);
            Wt::Dbo::Query<Wt::Dbo::ptr<T>> &query();
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
            void reload();
            void updateView();
            void saveSelection();
            void loadSelection();
            void selectAll();
            void selectNone();
            void inverseSelection();
            void addColumn(Ms::Widgets::MTableViewColumn column);
            void removeColumn(const Ms::Widgets::MTableViewColumn &column);
            void clearColumns();
            bool dbColumnExist(const std::string &dbColumnName);
            bool columnExist(const std::string &columnDisplayName);
            bool columnIsIgnored(const std::string &columnDisplayName);
            std::string columnName(const std::string &columnDisplayName);
            std::string columnDisplayName(const std::string &dboName);
            void importCSV(const std::string &fileName);
            void setIgnoreNumFilterColumns(int numColumns);
            void ignoredNumFilterColumns();
            std::string generateCSVData();
            void setImportOptionVisible(bool visible);
            void setExportOptionVisible(bool visible);
            std::vector<Wt::Dbo::ptr<T>> selectedItems() const;

            //signals
            Wt::Signal<> &tableSelectionChanged();

        private:
            //signals
            Wt::Signal<> _tableSelectionChanged;

            //slots
            void _mainTableSelectionChanged();
            void _cmbColumnFilterChanged();
            void _lnFilterKeyWentUp();
            void _popMnuSelectAllItemTriggered();
            void _popMnuSelectNoneItemTriggered();
            void _popMnuInverseSelectionItemTriggered();
            void _popMnuIOImportCSVItemTriggered();
            void _popMnuIOExportCSVItemTriggered();
            void _popMnuReloadItemTriggered();

            //functions
            void _setModelUserName(const std::string &name);
            void _refilter();
            void _createMainTable();
            void _updateModel();
            void _updateTable();
            void _prepareView();

            //variables
            std::vector<Ms::Widgets::MTableViewColumn> _columns;
            int _defaultFilterColumnIndex;
            Wt::Dbo::Query<Wt::Dbo::ptr<T>> _query;
            Ms::Core::Dbo::MDboManagerBase *_dboManager;
            std::vector<typename Wt::Dbo::dbo_traits<T>::IdType> _selection;//for saving/loading selection in view
            Wt::WString _filterRegExpression;
            int _ignoreNumFilterColumns = 0;//numbers of columns to ignore when filtering

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
            Wt::WMenuItem *_popMnuReloadItem;
            Ms::Dbo::MDboQueryModel<Wt::Dbo::ptr<T>> *_model;
            Wt::WSortFilterProxyModel *_proxyModel;
            Wt::WToolBar *_tbMain;
            Wt::WComboBox *_cmbColumnFilter;
            Wt::WStandardItemModel *_mdlCmbColumnFilter;
            Wt::WLineEdit *_lnFilter;
            Wt::WTableView *_tblMain;
        };
    }
}

#include "MQueryTableViewWidget_impl.h"

#endif // QUERYTABLEVIEWWIDGET_H
