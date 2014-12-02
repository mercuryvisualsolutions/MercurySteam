#ifndef MQUERYTABLEVIEWWIDGET_IMPL_H
#define MQUERYTABLEVIEWWIDGET_IMPL_H

#include "MQueryTableViewColumn.h"
#include "../Dbo/MDboQueryModel.h"
#include "../IO/IO.h"
#include "../IO/MStreamedFileResource.h"
#include "../IO/MStreamedMemoryResource.h"
#include "Dialogs/MFilesUploadDialog.h"

#include <vector>
#include <tuple>
#include <utility>
#include <iostream>
#include <algorithm>

#include <Wt/WPopupMenu>
#include <Wt/WPopupMenuItem>
#include <Wt/WSortFilterProxyModel>
#include <Wt/WStandardItemModel>
#include <Wt/WStandardItem>
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
#include <Wt/WBoostAny>

namespace Ms
{
    namespace Widgets
    {
        template<typename T>
        Ms::Widgets::MQueryTableViewWidget<T>::MQueryTableViewWidget(Ms::Core::Dbo::MDboSession &dboSession, Wt::WContainerWidget *parent) :
            Wt::WContainerWidget(parent),
            _dboSession(dboSession)
        {
            _defaultFilterColumnIndex = 0;
            _ignoreNumFilterColumns = 0;
            _advancedFilterActive = false;
            _customFilterActive = false;
            _customFilterString = "";
            _advancedFilterString = "";

            _rowHeightChanged = false;

            //features
            _importCSVFeatureEnabled = true;
            _exportCSVFeatureEnabled = true;
            _advancedFilterFeatureEnabled = true;

            _prepareView();

            _setModelUserName(dboSession.userName());
        }

        template<typename T>
        Ms::Widgets::MQueryTableViewWidget<T>::~MQueryTableViewWidget()
        {
            delete _model;
            delete _proxyModel;
        }

        //functions
        template<typename T>
        const Ms::Dbo::MDboQueryModel<Wt::Dbo::ptr<T>> *Ms::Widgets::MQueryTableViewWidget<T>::model() const
        {
            return _model;
        }

        template<typename T>
        const Wt::WSortFilterProxyModel *Ms::Widgets::MQueryTableViewWidget<T>::proxyModel() const
        {
            return _proxyModel;
        }

        template<typename T>
        Wt::WTableView *Ms::Widgets::MQueryTableViewWidget<T>::table() const
        {
            return _tblMain;
        }

        template<typename T>
        const std::string Ms::Widgets::MQueryTableViewWidget<T>::tableName() const
        {
            return _dboSession.tableName<T>();
        }

        template<typename T>
        Wt::WPushButton *Ms::Widgets::MQueryTableViewWidget<T>::createToolButton(const std::string &text, const std::string &iconPath, const std::string &toolTip)
        {
            Wt::WPushButton *btn = new Wt::WPushButton(text);
            btn->setToolTip(toolTip);
            btn->setIcon(Wt::WLink(iconPath));

            _tbMain->addButton(btn);

            return btn;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setRowHeight(int height)
        {
            _sldRowHeight->setValue(height);
            _tblMain->setRowHeight(height);
        }

        template<typename T>
        int Ms::Widgets::MQueryTableViewWidget<T>::rowHeight() const
        {
            return _tblMain->rowHeight().value();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setHeaderHeight(const Wt::WLength &height)
        {
            _tblMain->setHeaderHeight(height);
        }

        template<typename T>
        const Wt::WLength Ms::Widgets::MQueryTableViewWidget<T>::headerHeight() const
        {
            return _tblMain->headerHeight();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setQuery(Wt::Dbo::Query<Wt::Dbo::ptr<T> > &query) const
        {
            _query = query;
        }

        template<typename T>
        Wt::Dbo::Query<Wt::Dbo::ptr<T>> &Ms::Widgets::MQueryTableViewWidget<T>::query() const
        {
            return _query;
        }

        template<typename T>
        const Ms::Core::Dbo::MDboSession *Ms::Widgets::MQueryTableViewWidget<T>::dboSession() const
        {
            return _dboSession;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setDboSession(Ms::Core::Dbo::MDboSession &dboSession)
        {
            _dboSession = dboSession;

            _setModelUserName(dboSession.userName());
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setDefaultFilterColumnIndex(int columnIndex)
        {
            _defaultFilterColumnIndex = columnIndex;
            _proxyModel->setFilterKeyColumn(columnIndex);
        }

        template<typename T>
        int Ms::Widgets::MQueryTableViewWidget<T>::defaultFilterColumnIndex()
        {
            return _defaultFilterColumnIndex;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setSelectionMode(Wt::SelectionMode mode)
        {
            _tblMain->setSelectionMode(mode);
        }

        template<typename T>
        Wt::SelectionMode Ms::Widgets::MQueryTableViewWidget<T>::selectionMode()
        {
            return _tblMain->selectionMode();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setDynamicSortFilter(bool enable)
        {
            _proxyModel->setDynamicSortFilter(enable);
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::dynamicSortFilter() const
        {
            return _proxyModel->dynamicSortFilter();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setFilterRole(int role)
        {
            _proxyModel->setFilterRole(role);
        }

        template<typename T>
        int Ms::Widgets::MQueryTableViewWidget<T>::filterRole()
        {
            return _proxyModel->filterRole();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setFilterRegExp(const Wt::WString &regExpression)
        {
            _filterRegExpression = regExpression;
            _proxyModel->setFilterRegExp(regExpression);
        }

        template<typename T>
        Wt::WString Ms::Widgets::MQueryTableViewWidget<T>::filterRegExp() const
        {
            return _filterRegExpression;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setColumnResizeEnabled(bool enabled)
        {
            _tblMain->setColumnResizeEnabled(enabled);
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::isColumnResizeEnabled() const
        {
            return _tblMain->isColumnResizeEnabled();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setAlternatingRowColors(bool enable)
        {
            _tblMain->setAlternatingRowColors(enable);
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::alternatingRowColors() const
        {
            return _tblMain->alternatingRowColors();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::updateView()
        {
            try
            {
                if(_tblMain->selectionMode() != Wt::ExtendedSelection)
                {
                    _popMnuSelectAllItem->setDisabled(true);
                    _popMnuSelectNoneItem->setDisabled(true);
                    _popMnuInverseSelectionItem->setDisabled(true);
                }

                saveSelection();

                _updateModel();
                _updateTable();

                if(!_cntAdvancedFilter->isHidden())
                    _updateAdvancedFilterTable();

                loadSelection();

                _updateStatusBar();
            }
            catch(Wt::Dbo::Exception ex)
            {
                std::cout << ex.what() << std::endl;
            }
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::saveSelection() const
        {
            //save selection by id
            _selection.clear();

            for(const Wt::WModelIndex &index : _tblMain->selectedIndexes())
                _selection.push_back(_model->resultRow(_proxyModel->mapToSource(index).row()).id());
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::loadSelection() const
        {
            //load selection by id
            Wt::WModelIndexSet indexSet;

            for(const typename Wt::Dbo::dbo_traits<T>::IdType &id : _selection)
            {
                for(int row = 0; row < _proxyModel->rowCount(); ++row)
                {
                    if((_model->resultRow(_proxyModel->mapToSource(_proxyModel->index(row,0)).row()).id() == id))
                    {
                        indexSet.insert(_proxyModel->index(row,0));
                        //_tblMain->select(_proxyModel->index(row,0));
                    }
                }
            }

            _tblMain->setSelectedIndexes(indexSet);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::selectAll() const
        {
            Wt::WModelIndexSet indexSet;

            for(int row = 0; row < _proxyModel->rowCount(); ++row)
            {
                indexSet.insert(_proxyModel->index(row,0));
            }

            _tblMain->setSelectedIndexes(indexSet);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::selectNone() const
        {
            Wt::WModelIndexSet indexSet;
            _tblMain->setSelectedIndexes(indexSet);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::inverseSelection() const
        {
            Wt::WModelIndexSet indexSet;

            for(int row = 0; row < _proxyModel->rowCount(); ++row)
            {
                if(!_tblMain->selectionModel()->isSelected(_proxyModel->index(row,0)))
                    indexSet.insert(_proxyModel->index(row,0));
            }

            _tblMain->setSelectedIndexes(indexSet);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::addColumn(const Ms::Widgets::MQueryTableViewColumn &column) const
        {
            _columns.push_back(column);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::removeColumn(const Ms::Widgets::MQueryTableViewColumn &column) const
        {
            for(std::vector<Ms::Widgets::MQueryTableViewColumn>::iterator iter = _columns.begin(); iter != _columns.end(); ++iter)
            {
                if((*iter) == column)
                {
                    _columns.erase(iter);

                    break;
                }
            }
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::clearColumns() const
        {
            _columns.clear();
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::dbColumnExist(const std::string &dbColumnName) const//searches for real column DB name
        {
            return std::find_if(_columns.begin(), _columns.end(), [dbColumnName](const Ms::Widgets::MQueryTableViewColumn &col){
                return col.dbFieldName() == dbColumnName;
            }) != _columns.end();
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::columnExist(const std::string &columnDisplayName) const//searches for column display name
        {
            return std::find_if(_columns.begin(), _columns.end(), [columnDisplayName](const Ms::Widgets::MQueryTableViewColumn &col){
                return col.displayName() == columnDisplayName;
            }) != _columns.end();
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::columnIsIgnored(const std::string &columnDisplayName) const
        {
            auto iter = std::find_if(_columns.begin(), _columns.end(), [columnDisplayName](const Ms::Widgets::MQueryTableViewColumn &col){
                return col.displayName() == columnDisplayName;
            });

            return (iter != _columns.end()) && (*iter).isIgnored();
        }

        template<typename T>
        std::string Ms::Widgets::MQueryTableViewWidget<T>::columnName(const std::string &columnDisplayName) const
        {
            auto iter = std::find_if(_columns.begin(), _columns.end(), [columnDisplayName](const Ms::Widgets::MQueryTableViewColumn &col){
                return col.displayName() == columnDisplayName;
            });

            return iter != _columns.end() ? (*iter).dbFieldName() : "";
        }

        template<typename T>
        std::string Ms::Widgets::MQueryTableViewWidget<T>::columnDisplayName(const std::string &dboName) const
        {
            auto iter = std::find_if(_columns.begin(), _columns.end(), [dboName](const Ms::Widgets::MQueryTableViewColumn &col){
                return col.dbFieldName() == dboName;
            });

            return iter != _columns.end() ? (*iter).displayName() : "";
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setIgnoreNumFilterColumns(int numColumns)
        {
            _ignoreNumFilterColumns = numColumns;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::ignoredNumFilterColumns() const
        {
            return _ignoreNumFilterColumns;
        }

        template<typename T>
        std::vector<Wt::Dbo::ptr<T>> Ms::Widgets::MQueryTableViewWidget<T>::selectedItems() const
        {
            std::vector<Wt::Dbo::ptr<T>> vec;

            for(const Wt::WModelIndex &index : _tblMain->selectedIndexes())
            {
                Wt::Dbo::ptr<T> dboPtr = _model->resultRow(_proxyModel->mapToSource(index).row());

                vec.push_back(dboPtr);
            }

            return vec;
        }

        template<typename T>
        std::vector<Wt::Dbo::ptr<T>> Ms::Widgets::MQueryTableViewWidget<T>::items() const
        {
            std::vector<Wt::Dbo::ptr<T>> vec;

            for(int row = 0; row < _proxyModel->rowCount(); ++row)
            {
                const Wt::WModelIndex index = _proxyModel->mapToSource(_proxyModel->index(row, 0));

                Wt::Dbo::ptr<T> dboPtr = _model->resultRow(index.row());

                vec.push_back(dboPtr);
            }

            return vec;
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::isImportCSVFeatureEnabled() const
        {
            return _importCSVFeatureEnabled;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setImportCSVFeatureEnabled(bool enabled)
        {
            _importCSVFeatureEnabled = enabled;

            _popMnuIOImportCSVItem->setHidden(!enabled);
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::isExportCSVFeatureEnabled() const
        {
            return _exportCSVFeatureEnabled;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setExportCSVFeatureEnabled(bool enabled)
        {
            _exportCSVFeatureEnabled = enabled;

            _popMnuIOExportCSVItem->setHidden(!enabled);
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::isAdvancedFilterFeatureEnabled() const
        {
            return _advancedFilterFeatureEnabled;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setAdvancedFilterFeatureEnabled(bool enabled)
        {
            _advancedFilterFeatureEnabled = enabled;

            _popMnuViewAdvancedFilterItem->setHidden(!enabled);

            if(enabled)
            {
                if(_popMnuViewAdvancedFilterItem->isChecked())
                    _cntAdvancedFilter->show();
                else
                    _cntAdvancedFilter->hide();
            }
            else
                _cntAdvancedFilter->hide();
        }

        template<typename T>
        std::string Ms::Widgets::MQueryTableViewWidget<T>::advancedFilterString() const
        {
            return _advancedFilterString;
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::isAdvancedFilterActive() const
        {
            return _advancedFilterActive;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setAdvancedFilterActive(bool active)
        {
            _advancedFilterActive = active;

            _chkAdvancedFilterActive->setChecked(active);

            _updateModel();
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::isCustomFilterActive() const
        {
            return _customFilterActive;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setCustomFilterActive(bool active)
        {
            _customFilterActive = active;

            _chkCustomFilterActive->setChecked(active);

            _updateModel();
        }

        template<typename T>
        std::string Ms::Widgets::MQueryTableViewWidget<T>::customFilterString() const
        {
            return _customFilterString;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setCustomFilterString(const std::string &customFilterString)
        {
            _customFilterString = customFilterString;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::addBaseColumns(Wt::WFlags<Wt::ItemFlag> flags, int editRank) const
        {
            addColumn(Ms::Widgets::MQueryTableViewColumn("View_Rank", "View Rank", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false, true));
            addColumn(Ms::Widgets::MQueryTableViewColumn("Edit_Rank", "Edit Rank", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false, true));
            addColumn(Ms::Widgets::MQueryTableViewColumn("Remove_Rank", "Remove Rank", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false, true));
            addColumn(Ms::Widgets::MQueryTableViewColumn("Date_Created", "Date Created", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
            addColumn(Ms::Widgets::MQueryTableViewColumn("Created_By", "Created By", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
            addColumn(Ms::Widgets::MQueryTableViewColumn("Last_Modified_Date", "Last Modified Date", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
            addColumn(Ms::Widgets::MQueryTableViewColumn("Last_Modified_By", "Last Modified By", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(), false, true));
            addColumn(Ms::Widgets::MQueryTableViewColumn("Active", "Active", Wt::ItemIsSelectable | Wt::ItemIsUserCheckable, new Ms::Widgets::Delegates::MCheckBoxDelegate(editRank)));
        }

        //Signals
        template<typename T>
        Wt::Signal<> &Ms::Widgets::MQueryTableViewWidget<T>::tableSelectionChanged()
        {
            return _tableSelectionChanged;
        }

        template<typename T>
        Wt::Signal<Wt::Dbo::ptr<T>> &Ms::Widgets::MQueryTableViewWidget<T>::itemImported()
        {
            return _itemImported;
        }

        //slots
        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_mainTableSelectionChanged()
        {
            _updateStatusBar();

            _tableSelectionChanged();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_cmbColumnFilterChanged()
        {
            _proxyModel->setFilterKeyColumn(_cmbColumnFilter->currentIndex() + _ignoreNumFilterColumns);
            _refilter();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_lnFilterKeyWentUp()
        {
            _refilter();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_cntTblMainKeyWentDown(Wt::WKeyEvent key)
        {
            if(((key.modifiers() & Wt::KeyboardModifier::ShiftModifier) == Wt::KeyboardModifier::ShiftModifier) && key.key() == Wt::Key::Key_A)
            {
                selectAll();
            }
            else if(((key.modifiers() & Wt::KeyboardModifier::ShiftModifier) == Wt::KeyboardModifier::ShiftModifier) && key.key() == Wt::Key::Key_D)
            {
                selectNone();
            }
            else if(((key.modifiers() & Wt::KeyboardModifier::ShiftModifier) == Wt::KeyboardModifier::ShiftModifier) && key.key() == Wt::Key::Key_I)
            {
                inverseSelection();
            }
            else if(((key.modifiers() & Wt::KeyboardModifier::ShiftModifier) == Wt::KeyboardModifier::ShiftModifier) && key.key() == Wt::Key::Key_F)
            {
                _popMnuViewAdvancedFilterItem->setChecked(!_popMnuViewAdvancedFilterItem->isChecked());
                _toggleAdvancedFilterView();
            }
            else if(((key.modifiers() & Wt::KeyboardModifier::ShiftModifier) == Wt::KeyboardModifier::ShiftModifier) && key.key() == Wt::Key::Key_R)
            {
                updateView();
            }
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_popMnuSelectAllItemTriggered()
        {
            selectAll();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_popMnuSelectNoneItemTriggered()
        {
            selectNone();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_popMnuInverseSelectionItemTriggered()
        {
            inverseSelection();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_popMnuIOImportCSVItemTriggered()
        {
            Ms::Widgets::Dialogs::MFilesUploadDialog *dlg = new Ms::Widgets::Dialogs::MFilesUploadDialog(false, true);
            dlg->finished().connect(std::bind([=]()
            {
                if(dlg->result() == Wt::WDialog::Accepted)
                    _importCSV(dlg->spoolFileName());

                remove(dlg->spoolFileName().c_str());//delete the temp csv file
                delete dlg;
            }));

            dlg->show();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_popMnuIOExportCSVItemTriggered()
        {
            if(_model->rowCount() == 0)
                return;

            bool linkClicked = false;

            std::string csvData = _generateCSVData();

            Ms::IO::MStreamedMemoryResource *csvResource = new Ms::IO::MStreamedMemoryResource(std::vector<unsigned char>(csvData.begin(), csvData.end()), this);
            csvResource->setMimeType("text/plain");
            csvResource->suggestFileName(tableName() + ".csv");
            csvResource->setDispositionType(Wt::WMemoryResource::Attachment);

            Wt::WDialog *dlg = new Wt::WDialog("Download Link");
            dlg->setClosable(true);

            Wt::WAnchor *anchor = new Wt::WAnchor(Wt::WLink(csvResource), "Download");
            anchor->clicked().connect(std::bind([dlg,&linkClicked]()
            {
                linkClicked = true;
                dlg->accept();
            }));

            dlg->contents()->addWidget(anchor);
            dlg->finished().connect(std::bind([=]()
            {
                if(!linkClicked)
                    delete csvResource;

                delete dlg;
            }));

            dlg->show();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_popMnuViewAdvancedFilterItemTriggered()
        {
            _toggleAdvancedFilterView();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_btnAdvancedFilterAddClicked()
        {
            std::vector<Wt::WStandardItem*> items;

            Wt::WStandardItem *item = new Wt::WStandardItem("");
            item->setData(true, Wt::EditRole);
            items.push_back(item);

            for(unsigned int i = 1; i < _mdlAdvancedFilter->columnCount(); ++i)
            {
                Wt::WStandardItem *item = new Wt::WStandardItem("");
                item->setFlags(Wt::ItemIsSelectable | Wt::ItemIsEditable);
                items.push_back(item);
            }

            _mdlAdvancedFilter->appendRow(items);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_btnAdvancedFilterRemoveClicked()
        {
            while(_tblAdvancedFilter->selectedIndexes().size() > 0)
            {
                _mdlAdvancedFilter->removeRows((*_tblAdvancedFilter->selectedIndexes().cbegin()).row(), 1);
            }
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_btnAdvancedFilterApplyClicked()
        {
            _advancedFilterActive = true;

            _advancedFilterString = "";

            bool openBracket = false;

            std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string>> vecValidItems;

            //gather valid columns
            for(unsigned int row = 0; row < _mdlAdvancedFilter->rowCount(); ++row)
            {
                bool colActive = boost::any_cast<bool>(_mdlAdvancedFilter->data(row, 0, Wt::EditRole));

                if(!colActive)
                    continue;

                std::string colDispName = boost::any_cast<Wt::WString>(_mdlAdvancedFilter->data(row, 1)).toUTF8();
                std::string op1 = boost::any_cast<Wt::WString>(_mdlAdvancedFilter->data(row, 2)).toUTF8();
                std::string value = boost::any_cast<Wt::WString>(_mdlAdvancedFilter->data(row, 3)).toUTF8();
                std::string op2 = boost::any_cast<Wt::WString>(_mdlAdvancedFilter->data(row, 4)).toUTF8();
                std::string combine = boost::any_cast<Wt::WString>(_mdlAdvancedFilter->data(row, 5)).toUTF8();

                if((colDispName == "" || op1 == "" || value == ""))
                    continue;

                vecValidItems.push_back(std::make_tuple(colDispName, op1, value, op2, combine));
            }

            //generate sql
            for(unsigned int row = 0; row < vecValidItems.size(); ++row)
            {
                std::string colDispName = std::get<0>(vecValidItems.at(row));
                std::string op1 = std::get<1>(vecValidItems.at(row));
                std::string value = std::get<2>(vecValidItems.at(row));
                std::string op2 = std::get<3>(vecValidItems.at(row));
                std::string combine = std::get<4>(vecValidItems.at(row));

                bool hasMoreRows = row < (vecValidItems.size() - 1);
                bool isLastRow = row == (vecValidItems.size() - 1);

                if((hasMoreRows) && (op2 == "" || combine == ""))
                    continue;

                std::string subQueryStr = "";

                if(hasMoreRows && combine == "YES" && (!openBracket))
                {
                    subQueryStr += "(";
                    openBracket = true;
                }

                subQueryStr += columnName(colDispName);
                subQueryStr += " " + op1;
                subQueryStr += " '" + value + "'";

                if(((isLastRow) || combine == "NO") && openBracket)
                {
                    subQueryStr += ")";
                    openBracket = false;
                }

                if(hasMoreRows)
                    subQueryStr += " " + op2 + " ";

                _advancedFilterString += subQueryStr;
            }

            std::cout << _advancedFilterString << std::endl;

            setAdvancedFilterActive(true);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_btnAdvancedFilterClearClicked()
        {
            setAdvancedFilterActive(false);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_popMnuReloadItemTriggered()
        {
            updateView();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_sldRowHeightValueChanged()
        {
            _tblMain->setRowHeight(_sldRowHeight->value());
            //resize the last column of the table to fix a bug in Wt where table horizontal scroll bar gets hidden
            //when we chage rowHeight at runtime.
            //////////////////////////////////////////////////////////////////////////////
            int resizeValue = 0;
            if(_rowHeightChanged)
            {
                resizeValue = 1;
                _rowHeightChanged = false;
            }
            else
            {
                resizeValue = -1;
                _rowHeightChanged = true;
            }
            _tblMain->setColumnWidth(_model->columnCount() -1, _tblMain->columnWidth(_model->columnCount() -1).value() + resizeValue);
            //////////////////////////////////////////////////////////////////////////////
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_importCSV(const std::string &fileName)
        {
            Ms::IO::Data::MDataCSV csvData;
            if(!Ms::IO::readCsv(fileName, csvData))
                return;

            //scan for mandatory columns before import
            for(auto iter = _columns.begin(); iter != _columns.end(); ++iter)
            {
                if((*iter).isMandatory())
                {
                    bool found = false;
                    for(auto &col : csvData[0])//only the first data row csvData[0] is sufficient
                    {
                        if(col == (*iter).displayName())
                        {
                            found = true;
                            break;
                        }
                    }
                    if(!found)
                    {
                        std::cerr << "Error while importing CSV, mandatory column " << "\"" << (*iter).displayName() << "\"" << " was not found" << std::endl;
                        return;
                    }
                }
            }

            //all mandatory columns found, import data
            for(std::size_t row = 1; row < csvData.numRows(); ++row)
            {
                std::string columnsSql = "";
                std::string valuesSql = "";

                for(std::size_t col = 0; col < csvData[0].size(); ++col)
                {
                    if(columnExist(csvData[0][col]) && (!columnIsIgnored(csvData[0][col])))//only import columns that exist in table and not ignored
                    {
                        //generate columns and values sql
                        if(csvData[row][col] != "")//if the column has data
                        {
                            if(columnsSql != "")
                                columnsSql += ",";

                            if(valuesSql != "")
                                valuesSql += ",";

                            columnsSql += columnName(csvData[0][col]);
                            valuesSql = valuesSql + "'" + csvData[row][col] + "'";
                        }
                    }
                }

                //add extra columns
                columnsSql += ",Date_Created,Created_By";
                valuesSql += ",'" + std::to_string(Wt::WDateTime::currentDateTime().date().year()) + "-" +
                        std::to_string(Wt::WDateTime::currentDateTime().date().month()) + "-" +
                        std::to_string(Wt::WDateTime::currentDateTime().date().day()) + " " +
                        std::to_string(Wt::WDateTime::currentDateTime().time().hour()) + ":" +
                        std::to_string(Wt::WDateTime::currentDateTime().time().minute()) + ":" +
                        std::to_string(Wt::WDateTime::currentDateTime().time().second()) +
                                                   "','" + _dboSession.userName() + "'";

                if(columnsSql != "" && valuesSql != "")//found some columns and values ?
                {
                    //try to import
                    try
                    {
                        Wt::Dbo::Transaction transaction(_dboSession);

                        std::string sqlCommand = "INSERT INTO " + tableName() + " (" + columnsSql + ") VALUES (" + valuesSql + ")";

                        _dboSession.execute(sqlCommand);

                        transaction.commit();

                        //_itemImported(dboPtr);//emit the itemImported signal
                    }
                    catch(Wt::Dbo::NoUniqueResultException e)
                    {
                        std::cerr << "error while importing CSV" << std::endl << e.what() << std::endl;
                    }
                    catch(Wt::Dbo::Exception e)
                    {
                        std::cerr << "error while importing CSV" << std::endl << e.what() << std::endl;
                    }
                    catch(...)
                    {
                        std::cerr << "Unknown error while importing CSV" << std::endl;
                    }
                }
            }

            updateView();
        }

        template<typename T>
        std::string Ms::Widgets::MQueryTableViewWidget<T>::_generateCSVData() const
        {
            std::string data = "";
            int i = 0;

            //headers
            for(auto iter = _columns.begin(); iter != _columns.end(); ++iter)
            {
                data += (*iter).displayName();

                if(i++ < _columns.size() -1)
                    data +=",";
            }

            data += "\n";

            //data
            if(_tblMain->selectedIndexes().size() > 0)//export selected
            {
                int row = 0;
                for(const Wt::WModelIndex &index : _tblMain->selectedIndexes())
                {
                    for(int col = 0; col < _proxyModel->columnCount(); ++col)
                    {
                        boost::any indexData = _proxyModel->mapToSource(_proxyModel->index(index.row(), col)).data(Wt::DisplayRole);

                        if(indexData.type() == typeid(bool))//format bool to 0 and 1 to be able to import exported files to database
                            boost::any_cast<bool>(indexData) ? data += "1" : data += "0";
                        else
                            data += Wt::asString(indexData).toUTF8();

                        if(col < _proxyModel->columnCount() -1)
                            data +=",";
                    }
                    if(row++ < _tblMain->selectedIndexes().size() - 1)
                        data +="\n";
                }
            }
            else//export all, taking in account filtering
            {
                for(int row = 0; row < _proxyModel->rowCount(); ++row)
                {
                    for(int col = 0; col < _proxyModel->columnCount(); ++col)
                    {
                        boost::any indexData = _proxyModel->mapToSource(_proxyModel->index(row, col)).data(Wt::DisplayRole);

                        if(indexData.type() == typeid(bool))//format bool to 0 and 1 to be able to import exported files to database
                            boost::any_cast<bool>(indexData) ? data += "1" : data += "0";
                        else
                            data += Wt::asString(indexData).toUTF8();

                        if(col < _proxyModel->columnCount() -1)
                            data +=",";
                    }
                    if(row < _proxyModel->rowCount() -1)
                        data +="\n";
                }
            }

            return data;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_setModelUserName(const std::string &name) const
        {
            if(_model)
                _model->setUserName(name);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_refilter() const
        {
            _proxyModel->setFilterRegExp(_lnFilter->text() + _filterRegExpression);

            _updateStatusBar();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_createMainTable()
        {
            _tblMain = new Wt::WTableView();
            _tblMain->setColumnResizeEnabled(true);
            _tblMain->setAlternatingRowColors(true);
            _tblMain->setRowHeight(30);
            _tblMain->setHeaderHeight(30);
            _tblMain->setSelectionMode(Wt::ExtendedSelection);
            _tblMain->selectionChanged().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_mainTableSelectionChanged);

            _model = new Ms::Dbo::MDboQueryModel<Wt::Dbo::ptr<T>>();

            _proxyModel = new Wt::WSortFilterProxyModel();
            _proxyModel->setDynamicSortFilter(true);
            _proxyModel->setFilterRole(Wt::DisplayRole);
            _proxyModel->setFilterRegExp(_filterRegExpression);
            _proxyModel->setSourceModel(_model);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_createAdvancedFilterView()
        {
            _cntAdvancedFilter = new Ms::Widgets::MContainerWidget();
            _cntAdvancedFilter->setTitle("<b><i>Advanced Filter</i></b>");
            _cntAdvancedFilter->setMinimumSize(Wt::WLength::Auto, 300);
            _cntAdvancedFilter->setMaximumSize(Wt::WLength::Auto, 300);

            _layCntAdvancedFilter = dynamic_cast<Wt::WVBoxLayout*>(_cntAdvancedFilter->layout());
            _layCntAdvancedFilter->setContentsMargins(14,0,14,14);

            _cntTbAdvancedFilter = new Wt::WContainerWidget();

            _layCntAdvancedFilter->addWidget(_cntTbAdvancedFilter);

            _layCntTbAdvancedFilter = new Wt::WVBoxLayout();
            _layCntTbAdvancedFilter->setContentsMargins(0,10,0,10);
            _layCntTbAdvancedFilter->setSpacing(0);

            _cntTbAdvancedFilter->setLayout(_layCntTbAdvancedFilter);

            _tbAdvancedFilter = new Wt::WToolBar();

            _layCntTbAdvancedFilter->addWidget(_tbAdvancedFilter);

            Wt::WPushButton *btnFilterAdd = new Wt::WPushButton("");
            btnFilterAdd->setToolTip("Add Filter");
            btnFilterAdd->setIcon(Wt::WLink("icons/Add.png"));
            btnFilterAdd->clicked().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_btnAdvancedFilterAddClicked);

            _tbAdvancedFilter->addButton(btnFilterAdd);

            Wt::WPushButton *btnFilterRemove = new Wt::WPushButton("");
            btnFilterRemove->setToolTip("Remove Filter");
            btnFilterRemove->setIcon(Wt::WLink("icons/Remove.png"));
            btnFilterRemove->clicked().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_btnAdvancedFilterRemoveClicked);

            _tbAdvancedFilter->addButton(btnFilterRemove);

            Wt::WPushButton *btnFilterApply = new Wt::WPushButton("");
            btnFilterApply->setToolTip("Apply Filter");
            btnFilterApply->setIcon(Wt::WLink("icons/Apply.png"));
            btnFilterApply->clicked().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_btnAdvancedFilterApplyClicked);

            _tbAdvancedFilter->addButton(btnFilterApply);

            Wt::WPushButton *btnFilterClear = new Wt::WPushButton("");
            btnFilterClear->setToolTip("Clear Filter");
            btnFilterClear->setIcon(Wt::WLink("icons/Clear.png"));
            btnFilterClear->clicked().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_btnAdvancedFilterClearClicked);

            _tbAdvancedFilter->addButton(btnFilterClear);


            _tblAdvancedFilter = new Wt::WTableView();
            _tblAdvancedFilter->setColumnResizeEnabled(true);
            _tblAdvancedFilter->setAlternatingRowColors(true);
            _tblAdvancedFilter->setRowHeight(25);
            _tblAdvancedFilter->setHeaderHeight(25);
            _tblAdvancedFilter->setSelectionMode(Wt::ExtendedSelection);

            _layCntAdvancedFilter->addWidget(_tblAdvancedFilter, 1);

            _mdlAdvancedFilter = new Wt::WStandardItemModel(0,6);

            _tblAdvancedFilter->setModel(_mdlAdvancedFilter);

            _mdlAdvancedFilter->setHeaderData(0, std::string("Active"));
            _mdlAdvancedFilter->setHeaderData(1, std::string("Column Name"));
            _mdlAdvancedFilter->setHeaderData(2, std::string("Operator"));
            _mdlAdvancedFilter->setHeaderData(3, std::string("Value"));
            _mdlAdvancedFilter->setHeaderData(4, std::string("Operator"));
            _mdlAdvancedFilter->setHeaderData(5, std::string("Combine With Next"));

            std::vector<std::string> delegateColNameItems;
            int i = 0;

            Ms::Widgets::Delegates::MCheckBoxDelegate *colActiveDelegate = new Ms::Widgets::Delegates::MCheckBoxDelegate();

            for(auto iter = _columns.begin(); iter != _columns.end(); ++iter)
            {
                if(++i > _ignoreNumFilterColumns)
                    delegateColNameItems.push_back((*iter).displayName());
            }

            Ms::Widgets::Delegates::MComboBoxDelegate *colNameDelegate = new Ms::Widgets::Delegates::MComboBoxDelegate(delegateColNameItems);

            std::vector<std::string> delegateOperator1Items = {"=", "!=", ">", ">=", "<", "<="};
            Ms::Widgets::Delegates::MComboBoxDelegate *colOperator1Delegate = new Ms::Widgets::Delegates::MComboBoxDelegate(delegateOperator1Items);

            std::vector<std::string> delegateOperator2Items = {"AND", "OR"};
            Ms::Widgets::Delegates::MComboBoxDelegate *colOperator2Delegate = new Ms::Widgets::Delegates::MComboBoxDelegate(delegateOperator2Items);

            std::vector<std::string> delegateCombineWithNextItems = {"NO", "YES"};
            Ms::Widgets::Delegates::MComboBoxDelegate *colCombineWithNextDelegate = new Ms::Widgets::Delegates::MComboBoxDelegate(delegateCombineWithNextItems);


            _tblAdvancedFilter->setItemDelegateForColumn(0, colActiveDelegate);
            _tblAdvancedFilter->setItemDelegateForColumn(1, colNameDelegate);
            _tblAdvancedFilter->setItemDelegateForColumn(2, colOperator1Delegate);
            _tblAdvancedFilter->setItemDelegateForColumn(3, new Ms::Widgets::Delegates::MItemDelegate());
            _tblAdvancedFilter->setItemDelegateForColumn(4, colOperator2Delegate);
            _tblAdvancedFilter->setItemDelegateForColumn(5, colCombineWithNextDelegate);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_updateModel()
        {
            try
            {
                Wt::Dbo::Transaction transaction(_dboSession);

                _queryFilter = _query;

                if(_customFilterActive)
                {
                    if(_customFilterString != "")
                        _queryFilter.where(std::string("(") + _customFilterString + ")");
                }

                if(_advancedFilterActive)
                {
                    if(_advancedFilterString != "")
                        _queryFilter.where(std::string("(") + _advancedFilterString + ")");
                }

                _model->setQuery(_queryFilter);

                transaction.commit();

                _model->reload();

                //int i = 0;

                for(auto iter = _columns.begin(); iter != _columns.end(); ++iter)
                {
                    _model->addColumn((*iter).dbFieldName(), (*iter).displayName(), (*iter).flags());
                    //_tblMain->setColumnWidth(i++, (*iter).width());
                }

                _cmbColumnFilter->clear();

                int i = 0;
                //populate _cmbColumnFilter
                for(auto iter = _columns.begin(); iter != _columns.end(); ++iter)
                {
                    if(++i > _ignoreNumFilterColumns)
                        _cmbColumnFilter->addItem((*iter).displayName());
                }

                if(_model->columnCount() >= _defaultFilterColumnIndex)
                    _proxyModel->setFilterKeyColumn(_defaultFilterColumnIndex);//reset the model filter column
            }
            catch(Wt::Dbo::Exception ex)
            {
                std::cerr << "Error occured while trying to update query model for view \""  + tableName() + "\"" << std::endl << ex.what() << std::endl;
            }
            catch(...)
            {
                std::cerr << "Error occured while trying to update query model for view \""  + tableName() + "\"" << std::endl;
            }
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_updateTable() const
        {
            _tblMain->setModel(_proxyModel);

            int i = 0;
            for(auto iter = _columns.begin(); iter != _columns.end(); ++iter)
            {
                _tblMain->setItemDelegateForColumn(i++, (*iter).delegate());
            }
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_updateAdvancedFilterTable() const
        {
            std::vector<std::string> delegateColNameItems;
            int i = 0;

            for(auto iter = _columns.begin(); iter != _columns.end(); ++iter)
            {
                if(++i > _ignoreNumFilterColumns)
                    delegateColNameItems.push_back((*iter).displayName());
            }

            Ms::Widgets::Delegates::MComboBoxDelegate *delegate = dynamic_cast<Ms::Widgets::Delegates::MComboBoxDelegate*>(_tblAdvancedFilter->itemDelegateForColumn(1));
            delegate->setItems(delegateColNameItems); //update delegate columnName column with new columns (if any)
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_toggleAdvancedFilterView() const
        {
            if(_popMnuViewAdvancedFilterItem->isChecked())
                _updateAdvancedFilterTable();

            _cntAdvancedFilter->setHidden(!_popMnuViewAdvancedFilterItem->isChecked());
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_updateStatusBar() const
        {
            _lblStatus->setText(std::to_string(_tblMain->selectedIndexes().size()) + " Item(s) selected, " + std::to_string(_proxyModel->rowCount()) + " Total items.");
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_prepareView()
        {
            Wt::WHBoxLayout *layMain = new Wt::WHBoxLayout();
            layMain->setContentsMargins(0,0,0,0);
            layMain->setSpacing(0);

            setLayout(layMain);
            //setStyleClass("container-box");

            Wt::WContainerWidget *cntMainView = new Wt::WContainerWidget();

            layMain->addWidget(cntMainView);

            Wt::WVBoxLayout *layCntMainView = new Wt::WVBoxLayout();
            layCntMainView->setContentsMargins(8,0,8,0);
            layCntMainView->setSpacing(0);

            cntMainView->setLayout(layCntMainView);

            //Main Toolbar
            Wt::WHBoxLayout *layCntTbGlobal = new Wt::WHBoxLayout();
            layCntTbGlobal->setContentsMargins(0,10,0,10);
            layCntTbGlobal->setSpacing(4);

            Wt::WContainerWidget *cntTbGlobal = new Wt::WContainerWidget();
            cntTbGlobal->setLayout(layCntTbGlobal);
            cntTbGlobal->setStyleClass("form-inline");

            layCntMainView->addWidget(cntTbGlobal);

            _tbGlobal = new Wt::WToolBar();
            layCntTbGlobal->addWidget(_tbGlobal, 1);

            _btnMnuTools = new Wt::WPushButton("");
            _btnMnuTools->setToolTip("Tools");
            Wt::WLink lnkBtnIcon("icons/Tools.png");
            _btnMnuTools->setIcon(lnkBtnIcon);

            layCntTbGlobal->addWidget(_btnMnuTools);

            _popMnuTools = new Wt::WPopupMenu();
            _btnMnuTools->setMenu(_popMnuTools);

            _popMnuSelection = new Wt::WPopupMenu();
            _popMnuTools->addMenu("Selection", _popMnuSelection);

            _popMnuSelectAllItem = _popMnuSelection->addItem("Select All - Shift+A");
            _popMnuSelectAllItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_popMnuSelectAllItemTriggered);

            _popMnuSelectNoneItem = _popMnuSelection->addItem("Select None - Shift+D");
            _popMnuSelectNoneItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_popMnuSelectNoneItemTriggered);

            _popMnuInverseSelectionItem = _popMnuSelection->addItem("Inverse Selection - Shift+I");
            _popMnuInverseSelectionItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_popMnuInverseSelectionItemTriggered);

            _popMnuIO = new Wt::WPopupMenu();
            _popMnuTools->addMenu("IO", _popMnuIO);

            _popMnuIOImportCSVItem = _popMnuIO->addItem("Import CSV");
            _popMnuIOImportCSVItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_popMnuIOImportCSVItemTriggered);

            _popMnuIOExportCSVItem = _popMnuIO->addItem("Export CSV");
            _popMnuIOExportCSVItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_popMnuIOExportCSVItemTriggered);

            _popMnuView = new Wt::WPopupMenu();
            _popMnuTools->addMenu("View", _popMnuView);

            _popMnuViewAdvancedFilterItem = _popMnuView->addItem("Advanced Filter - Shift+F");
            _popMnuViewAdvancedFilterItem->setCheckable(true);
            _popMnuViewAdvancedFilterItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_popMnuViewAdvancedFilterItemTriggered);

            _popMnuTools->addSeparator();

            _popMnuReloadItem = _popMnuTools->addItem("Reload - Shift+R");
            _popMnuReloadItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_popMnuReloadItemTriggered);

            _cmbColumnFilter = new Wt::WComboBox();
            _cmbColumnFilter->setMinimumSize(220, 30);
            _cmbColumnFilter->setMaximumSize(220, 30);
            _cmbColumnFilter->changed().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_cmbColumnFilterChanged);
            _mdlCmbColumnFilter = new Wt::WStandardItemModel();
            _cmbColumnFilter->setModel(_mdlCmbColumnFilter);
            layCntTbGlobal->addWidget(_cmbColumnFilter);

            _lnFilter = new Wt::WLineEdit();
            _lnFilter->setMinimumSize(220, 30);
            _lnFilter->setMaximumSize(220, 30);
            _lnFilter->setEmptyText("Filter");
            _lnFilter->keyWentUp().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_lnFilterKeyWentUp);

            layCntTbGlobal-> addWidget(_lnFilter);

            //Main Table View
            Wt::WVBoxLayout *layCntTblMain = new Wt::WVBoxLayout();
            layCntTblMain->setContentsMargins(0,0,0,0);
            layCntTblMain->setSpacing(0);

            Wt::WContainerWidget *cntTblMain = new Wt::WContainerWidget();
            cntTblMain->setLayout(layCntTblMain);
            cntTblMain->setAttributeValue("tabindex", "0");
            cntTblMain->keyWentDown().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_cntTblMainKeyWentDown);

            _createMainTable();

            layCntTblMain->addWidget(_tblMain, 1);

            layCntMainView->addWidget(cntTblMain, 1);

            //Main toolbar
            Wt::WHBoxLayout *layTbMain = new Wt::WHBoxLayout();
            layTbMain->setContentsMargins(0,8,0,8);
            layTbMain->setSpacing(0);

            Wt::WContainerWidget *cntTbMain = new Wt::WContainerWidget();
            cntTbMain->setLayout(layTbMain);

            layCntMainView->addWidget(cntTbMain);

            _tbMain = new Wt::WToolBar();
            layTbMain->addWidget(_tbMain, 1);

            _sldRowHeight = new Wt::WSlider(Wt::Horizontal);
            _sldRowHeight->resize(100, 20);
            _sldRowHeight->setRange(25,300);
            _sldRowHeight->setValue(25);
            _sldRowHeight->setTickInterval(10);
            _sldRowHeight->setHandleWidth(10);
            _sldRowHeight->setTickPosition(Wt::WSlider::TicksAbove);
            _sldRowHeight->valueChanged().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_sldRowHeightValueChanged);

            layTbMain->addWidget(_sldRowHeight);

            //Status bar
            _cntStatusBar = new Wt::WContainerWidget();

            layCntMainView->addWidget(_cntStatusBar);

            _layCntStatusBar = new Wt::WHBoxLayout();
            _layCntStatusBar->setContentsMargins(4,0,4,0);
            _layCntStatusBar->setSpacing(2);

            _cntStatusBar->setLayout(_layCntStatusBar);

            _lblStatus = new Wt::WLabel("");

            _layCntStatusBar->addWidget(_lblStatus, 1);

            Wt::WHBoxLayout *layCntActiveFilters = new Wt::WHBoxLayout();
            layCntActiveFilters->setContentsMargins(8,0,8,0);
            layCntActiveFilters->setSpacing(6);

            Wt::WContainerWidget *cntActiveFilters = new Wt::WContainerWidget();
            cntActiveFilters->setLayout(layCntActiveFilters);

            _layCntStatusBar->addWidget(cntActiveFilters);

            _chkCustomFilterActive = new Wt::WCheckBox("CF");
            _chkCustomFilterActive->setInline(true);
            _chkCustomFilterActive->setToolTip("Custom Filter");
            _chkCustomFilterActive->setDisabled(true);

            layCntActiveFilters->addWidget(_chkCustomFilterActive);

            _chkAdvancedFilterActive = new Wt::WCheckBox("AF");
            _chkAdvancedFilterActive->setInline(true);
            _chkAdvancedFilterActive->setToolTip("Advanced Filter");
            _chkAdvancedFilterActive->setDisabled(true);

            layCntActiveFilters->addWidget(_chkAdvancedFilterActive);

            //advancedFilter
            _createAdvancedFilterView();
            _cntAdvancedFilter->hide();//set advanced filter panel initially hidden
            layCntMainView->addWidget(_cntAdvancedFilter);
        }
    }
}

#endif // MQUERYTABLEVIEWWIDGET_IMPL_H
