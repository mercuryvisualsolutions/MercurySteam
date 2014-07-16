#ifndef MQUERYTABLEVIEWWIDGET_IMPL_H
#define MQUERYTABLEVIEWWIDGET_IMPL_H

#include "MTableViewColumn.h"
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
#include <Wt/Dbo/Dbo>
#include <Wt/WMemoryResource>

namespace Ms
{
    namespace Widgets
    {
        template<typename T>
        Ms::Widgets::MQueryTableViewWidget<T>::MQueryTableViewWidget(Wt::WContainerWidget *parent) :
            Wt::WContainerWidget(parent)
        {
            _defaultFilterColumnIndex = 0;
            _ignoreNumFilterColumns = 0;
            _advancedFilterActive = false;

            _prepareView();
        }

        template<typename T>
        Ms::Widgets::MQueryTableViewWidget<T>::MQueryTableViewWidget(Ms::Core::Dbo::MDboManagerBase *dboManager, Wt::WContainerWidget *parent) :
            Ms::Widgets::MQueryTableViewWidget<T>(parent)
        {
            setDboManager(dboManager);
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
            if(!_dboManager)
                return "";

            return _dboManager->session()->tableName<T>();
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
        void Ms::Widgets::MQueryTableViewWidget<T>::setRowHeight(const Wt::WLength &height)
        {
            _tblMain->setRowHeight(height);
        }

        template<typename T>
        const Wt::WLength Ms::Widgets::MQueryTableViewWidget<T>::rowHeight() const
        {
            return _tblMain->rowHeight();
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
        void Ms::Widgets::MQueryTableViewWidget<T>::setQuery(Wt::Dbo::Query<Wt::Dbo::ptr<T> > &query)
        {
            _query = query;
        }

        template<typename T>
        Wt::Dbo::Query<Wt::Dbo::ptr<T>> &Ms::Widgets::MQueryTableViewWidget<T>::query()
        {
            return _query;
        }

        template<typename T>
        const Ms::Core::Dbo::MDboManagerBase *Ms::Widgets::MQueryTableViewWidget<T>::dboManager() const
        {
            return _dboManager;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setDboManager(Ms::Core::Dbo::MDboManagerBase *dboManager)
        {
            _dboManager = dboManager;

            if(dboManager)
                _setModelUserName(dboManager->userName());
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
        void Ms::Widgets::MQueryTableViewWidget<T>::reload()
        {
            try
            {
                _model->reload();
            }
            catch(...)
            {
              std::cerr << "Exception occured while trying to reload table view" << std::endl;
            }
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::updateView()
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
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::saveSelection()
        {
            //save selection by id
            _selection.clear();

            for(const Wt::WModelIndex &index : _tblMain->selectedIndexes())
                _selection.push_back(_model->resultRow(_proxyModel->mapToSource(index).row()).id());
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::loadSelection()
        {
            //load selection by id
            Wt::WModelIndexSet indexSet;

            for(typename Wt::Dbo::dbo_traits<T>::IdType id : _selection)
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
        void Ms::Widgets::MQueryTableViewWidget<T>::selectAll()
        {
            Wt::WModelIndexSet indexSet;

            for(int row = 0; row < _proxyModel->rowCount(); ++row)
            {
                indexSet.insert(_proxyModel->index(row,0));
            }

            _tblMain->setSelectedIndexes(indexSet);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::selectNone()
        {
            Wt::WModelIndexSet indexSet;
            _tblMain->setSelectedIndexes(indexSet);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::inverseSelection()
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
        void Ms::Widgets::MQueryTableViewWidget<T>::addColumn(Ms::Widgets::MTableViewColumn column)
        {
            _columns.push_back(column);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::removeColumn(const Ms::Widgets::MTableViewColumn &column)
        {
            for(std::vector<Ms::Widgets::MTableViewColumn>::iterator iter = _columns.begin(); iter != _columns.end(); ++iter)
            {
                if((*iter) == column)
                {
                    _columns.erase(iter);

                    break;
                }
            }
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::clearColumns()
        {
            _columns.clear();
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::dbColumnExist(const std::string &dbColumnName)//searches for real column DB name
        {
            return std::find_if(_columns.begin(), _columns.end(), [dbColumnName](const Ms::Widgets::MTableViewColumn &col){
                return col.dbFieldName() == dbColumnName;
            }) != _columns.end();
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::columnExist(const std::string &columnDisplayName)//searches for column display name
        {
            return std::find_if(_columns.begin(), _columns.end(), [columnDisplayName](const Ms::Widgets::MTableViewColumn &col){
                return col.displayName() == columnDisplayName;
            }) != _columns.end();
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::columnIsIgnored(const std::string &columnDisplayName)
        {
            auto iter = std::find_if(_columns.begin(), _columns.end(), [columnDisplayName](const Ms::Widgets::MTableViewColumn &col){
                return col.displayName() == columnDisplayName;
            });

            return (iter != _columns.end()) && (*iter).isIgnored();
        }

        template<typename T>
        std::string Ms::Widgets::MQueryTableViewWidget<T>::columnName(const std::string &columnDisplayName)
        {
            auto iter = std::find_if(_columns.begin(), _columns.end(), [columnDisplayName](const Ms::Widgets::MTableViewColumn &col){
                return col.displayName() == columnDisplayName;
            });

            return iter != _columns.end() ? (*iter).dbFieldName() : "";
        }

        template<typename T>
        std::string Ms::Widgets::MQueryTableViewWidget<T>::columnDisplayName(const std::string &dboName)
        {
            auto iter = std::find_if(_columns.begin(), _columns.end(), [dboName](const Ms::Widgets::MTableViewColumn &col){
                return col.dbFieldName() == dboName;
            });

            return iter != _columns.end() ? (*iter).displayName() : "";
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::importCSV(const std::string &fileName)
        {
            if(!_dboManager->session())
            {
                std::cerr << "Error updating table, DB session is NULL" << std::endl;
                return;
            }

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
                                                   "','" + _dboManager->userName() + "'";

                if(columnsSql != "" && valuesSql != "")//found some columns and values ?
                {
                    //try to import
                    try
                    {
                        if(!_dboManager->openTransaction())
                            return;

                        std::string sqlCommand = "INSERT INTO " + tableName() + " (" + columnsSql + ") VALUES (" + valuesSql + ")";
                        _dboManager->session()->execute(sqlCommand);

                        _dboManager->commitTransaction();
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
        void Ms::Widgets::MQueryTableViewWidget<T>::setIgnoreNumFilterColumns(int numColumns)
        {
            _ignoreNumFilterColumns = numColumns;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::ignoredNumFilterColumns()
        {
            return _ignoreNumFilterColumns;
        }

        template<typename T>
        std::string Ms::Widgets::MQueryTableViewWidget<T>::generateCSVData()
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
                for(const Wt::WModelIndex &index : _tblMain->selectedIndexes())
                {
                    //auto dataMap = _model->itemData(index);

                    for(int col = 0; col < _proxyModel->columnCount(); ++col)
                    {
                        data += _model->text(_proxyModel->mapToSource(_proxyModel->index(index.row(), col)));

                        if(col < _proxyModel->columnCount() -1)
                            data +=",";
                    }
                    if(index.row() < _tblMain->selectedIndexes().size() - 1)
                        data +="\n";
                }
            }
            else//export all, taking in account filtering
            {
                for(int row = 0; row < _proxyModel->rowCount(); ++row)
                {
                    for(int col = 0; col < _proxyModel->columnCount(); ++col)
                    {
                        //auto obj =_model->index(row, col).data();//<-test code line, remove it

                        data += _model->text(_proxyModel->mapToSource(_proxyModel->index(row, col)));

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
        void Ms::Widgets::MQueryTableViewWidget<T>::setImportOptionVisible(bool visible)
        {
            if(visible)
                _popMnuIOImportCSVItem->show();
            else
                _popMnuIOImportCSVItem->hide();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setExportOptionVisible(bool visible)
        {
            if(visible)
                _popMnuIOExportCSVItem->show();
            else
                _popMnuIOExportCSVItem->hide();
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
        Wt::Signal<> &Ms::Widgets::MQueryTableViewWidget<T>::tableSelectionChanged()
        {
            return _tableSelectionChanged;
        }

        //slots
        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_mainTableSelectionChanged()
        {
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
                    importCSV(dlg->spoolFileName());

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

            std::string csvData = generateCSVData();

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
            if(_popMnuViewAdvancedFilterItem->isChecked())
                _updateAdvancedFilterTable();

            _cntAdvancedFilter->setHidden(!_popMnuViewAdvancedFilterItem->isChecked());
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_btnAdvancedFilterAddClicked()
        {
            std::vector<Wt::WStandardItem*> items;
            for(unsigned int i = 0; i < _mdlAdvancedFilter->columnCount(); ++i)
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

            std::string strQuery = "";
            bool openBracket = false;

            for(unsigned int row = 0; row < _mdlAdvancedFilter->rowCount(); ++row)
            {
                std::string colDispName = boost::any_cast<Wt::WString>(_mdlAdvancedFilter->data(row, 0)).toUTF8();
                std::string op1 = boost::any_cast<Wt::WString>(_mdlAdvancedFilter->data(row, 1)).toUTF8();
                std::string value = boost::any_cast<Wt::WString>(_mdlAdvancedFilter->data(row, 2)).toUTF8();
                std::string op2 = boost::any_cast<Wt::WString>(_mdlAdvancedFilter->data(row, 3)).toUTF8();
                std::string combine = boost::any_cast<Wt::WString>(_mdlAdvancedFilter->data(row, 4)).toUTF8();

                if((colDispName == "" || op1 == "" || value == "") ||
                        ((row < _mdlAdvancedFilter->rowCount() - 1) && (op2 == "" || combine == "")))
                    continue;

                std::string subQueryStr = "";

                if((row < _mdlAdvancedFilter->rowCount() - 1) && combine == "YES" && (!openBracket))
                {
                    subQueryStr += "(";
                    openBracket = true;
                }

                subQueryStr += columnName(colDispName);
                subQueryStr += " " + op1;
                subQueryStr += " '" + value + "'";

                if(((row == _mdlAdvancedFilter->rowCount() - 1) || combine == "NO") && openBracket)
                {
                    subQueryStr += ")";
                    openBracket = false;
                }

                if(row < _mdlAdvancedFilter->rowCount() - 1)
                {
                    subQueryStr += " " + op2 + " ";
                }

                strQuery += subQueryStr;
            }

            std::cout << strQuery << std::endl;

            _queryAdvancedFilter = _query;

            if(strQuery != "")
                _queryAdvancedFilter.where(std::string("(") + strQuery + ")");

            _updateModel();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_btnAdvancedFilterClearClicked()
        {
            _advancedFilterActive = false;

            updateView();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_popMnuReloadItemTriggered()
        {
            saveSelection();

            reload();

            loadSelection();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_setModelUserName(const std::string &name)
        {
            if(_model)
                _model->setUserName(name);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_refilter()
        {
            _proxyModel->setFilterRegExp(_lnFilter->text() + _filterRegExpression);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_createMainTable()
        {
            _tblMain = new Wt::WTableView();
            _tblMain->setColumnResizeEnabled(true);
            _tblMain->setAlternatingRowColors(true);
            _tblMain->setRowHeight(160);
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
            _cntAdvancedFilter = new Wt::WContainerWidget();
            _cntAdvancedFilter->setMinimumSize(Wt::WLength::Auto, 300);
            _cntAdvancedFilter->setMaximumSize(Wt::WLength::Auto, 300);

            _layCntAdvancedFilter = new Wt::WVBoxLayout();
            _layCntAdvancedFilter->setContentsMargins(0,0,0,0);
            _layCntAdvancedFilter->setSpacing(0);

            _cntAdvancedFilter->setLayout(_layCntAdvancedFilter);

            _txtAdvancedFilterTitle = new Wt::WText("<b><i>Advanced Filter</i></b>");
            _txtAdvancedFilterTitle->setStyleClass("soft-text");

            _cntTxtAdvancedFilterTitle = new Wt::WContainerWidget();
            _cntTxtAdvancedFilterTitle->setStyleClass("toolbar");
            _cntTxtAdvancedFilterTitle->setContentAlignment(Wt::AlignCenter);
            _cntTxtAdvancedFilterTitle->setMinimumSize(Wt::WLength::Auto, 25);

            _layCntAdvancedFilter->addWidget(_cntTxtAdvancedFilterTitle);

            _cntTxtAdvancedFilterTitle->addWidget(_txtAdvancedFilterTitle);

            _tbAdvancedFilter = new Wt::WToolBar();
            _tbAdvancedFilter->setStyleClass("toolbar");

            _layCntAdvancedFilter->addWidget(_tbAdvancedFilter);

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
            _tblAdvancedFilter->setRowHeight(24);
            _tblAdvancedFilter->setHeaderHeight(24);
            _tblAdvancedFilter->setSelectionMode(Wt::ExtendedSelection);

            _layCntAdvancedFilter->addWidget(_tblAdvancedFilter, 1);

            _mdlAdvancedFilter = new Wt::WStandardItemModel(0,5);

            _tblAdvancedFilter->setModel(_mdlAdvancedFilter);

            _mdlAdvancedFilter->setHeaderData(0, std::string("Column Name"));
            _mdlAdvancedFilter->setHeaderData(1, std::string("Operator"));
            _mdlAdvancedFilter->setHeaderData(2, std::string("Value"));
            _mdlAdvancedFilter->setHeaderData(3, std::string("Operator"));
            _mdlAdvancedFilter->setHeaderData(4, std::string("Combine With Next"));

            std::vector<std::string> delegateColNameItems;
            int i = 0;

            for(auto iter = _columns.begin(); iter != _columns.end(); ++iter)
            {
                if(++i > _ignoreNumFilterColumns)
                    delegateColNameItems.push_back((*iter).displayName());
            }

            Ms::Widgets::Delegates::MComboBoxDelegate *colNameDelegate = new Ms::Widgets::Delegates::MComboBoxDelegate(delegateColNameItems);

            std::vector<std::string> delegateOperator1Items = {"=", "!=", ">", "<"};
            Ms::Widgets::Delegates::MComboBoxDelegate *colOperator1Delegate = new Ms::Widgets::Delegates::MComboBoxDelegate(delegateOperator1Items);

            std::vector<std::string> delegateOperator2Items = {"AND", "OR"};
            Ms::Widgets::Delegates::MComboBoxDelegate *colOperator2Delegate = new Ms::Widgets::Delegates::MComboBoxDelegate(delegateOperator2Items);

            std::vector<std::string> delegateCombineWithNextItems = {"NO", "YES"};
            Ms::Widgets::Delegates::MComboBoxDelegate *colCombineWithNextDelegate = new Ms::Widgets::Delegates::MComboBoxDelegate(delegateCombineWithNextItems);

            _tblAdvancedFilter->setItemDelegateForColumn(0, colNameDelegate);
            _tblAdvancedFilter->setItemDelegateForColumn(1, colOperator1Delegate);
            _tblAdvancedFilter->setItemDelegateForColumn(2, new Ms::Widgets::Delegates::MItemDelegate());
            _tblAdvancedFilter->setItemDelegateForColumn(3, colOperator2Delegate);
            _tblAdvancedFilter->setItemDelegateForColumn(4, colCombineWithNextDelegate);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_updateModel()
        {
            if(!_dboManager->session())
            {
                std::cerr << "Error updating table, session is NULL" << std::endl;
                return;
            }

            try
            {
                if(!_dboManager->openTransaction())
                    return;

                if(_advancedFilterActive)
                    _model->setQuery(_queryAdvancedFilter);
                else
                    _model->setQuery(_query);

                _dboManager->commitTransaction();

                _model->reload();

                //int i;

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
        void Ms::Widgets::MQueryTableViewWidget<T>::_updateTable()
        {
            _tblMain->setModel(_proxyModel);

            int i = 0;
            for(auto iter = _columns.begin(); iter != _columns.end(); ++iter)
            {
                _tblMain->setItemDelegateForColumn(i++, (*iter).delegate());
            }
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_updateAdvancedFilterTable()
        {
            std::vector<std::string> delegateColNameItems;
            int i = 0;

            for(auto iter = _columns.begin(); iter != _columns.end(); ++iter)
            {
                if(++i > _ignoreNumFilterColumns)
                    delegateColNameItems.push_back((*iter).displayName());
            }

            Ms::Widgets::Delegates::MComboBoxDelegate *delegate = dynamic_cast<Ms::Widgets::Delegates::MComboBoxDelegate*>(_tblAdvancedFilter->itemDelegateForColumn(0));
            delegate->setItems(delegateColNameItems); //update delegate columnName column with new columns (if any)
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::_prepareView()
        {
            Wt::WVBoxLayout *layMain = new Wt::WVBoxLayout();
            layMain->setContentsMargins(0,0,0,0);
            layMain->setSpacing(0);

            setLayout(layMain);

            //Main Toolbar
            Wt::WHBoxLayout *layTbMain = new Wt::WHBoxLayout();
            layTbMain->setContentsMargins(0,0,0,0);
            layTbMain->setSpacing(0);

            Wt::WContainerWidget *cntTbMain = new Wt::WContainerWidget();
            cntTbMain->setStyleClass("toolbar");
            cntTbMain->setLayout(layTbMain);

            _tbMain = new Wt::WToolBar();
            layTbMain->addWidget(_tbMain, 1);

            layMain->addWidget(cntTbMain);

            _btnMnuTools = new Wt::WPushButton("");
            _btnMnuTools->setToolTip("Tools");
            Wt::WLink lnkBtnIcon("icons/Tools.png");
            _btnMnuTools->setIcon(lnkBtnIcon);

            layTbMain->addWidget(_btnMnuTools);

            _popMnuTools = new Wt::WPopupMenu();
            _btnMnuTools->setMenu(_popMnuTools);

            _popMnuSelection = new Wt::WPopupMenu();
            _popMnuTools->addMenu("Selection", _popMnuSelection);

            _popMnuSelectAllItem = _popMnuSelection->addItem("Select All");
            _popMnuSelectAllItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_popMnuSelectAllItemTriggered);

            _popMnuSelectNoneItem = _popMnuSelection->addItem("Select None");
            _popMnuSelectNoneItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_popMnuSelectNoneItemTriggered);

            _popMnuInverseSelectionItem = _popMnuSelection->addItem("Inverse Selection");
            _popMnuInverseSelectionItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_popMnuInverseSelectionItemTriggered);

            _popMnuIO = new Wt::WPopupMenu();
            _popMnuTools->addMenu("IO", _popMnuIO);

            _popMnuIOImportCSVItem = _popMnuIO->addItem("Import CSV");
            _popMnuIOImportCSVItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_popMnuIOImportCSVItemTriggered);

            _popMnuIOExportCSVItem = _popMnuIO->addItem("Export CSV");
            _popMnuIOExportCSVItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_popMnuIOExportCSVItemTriggered);

            _popMnuView = new Wt::WPopupMenu();
            _popMnuTools->addMenu("View", _popMnuView);

            _popMnuViewAdvancedFilterItem = _popMnuView->addItem("Advanced Filter");
            _popMnuViewAdvancedFilterItem->setCheckable(true);
            _popMnuViewAdvancedFilterItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_popMnuViewAdvancedFilterItemTriggered);

            _popMnuTools->addSeparator();

            _popMnuReloadItem = _popMnuTools->addItem("Reload");
            _popMnuReloadItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_popMnuReloadItemTriggered);

            _cmbColumnFilter = new Wt::WComboBox();
            _cmbColumnFilter->changed().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_cmbColumnFilterChanged);
            _mdlCmbColumnFilter = new Wt::WStandardItemModel();
            _cmbColumnFilter->setModel(_mdlCmbColumnFilter);
            layTbMain->addWidget(_cmbColumnFilter);

            _lnFilter = new Wt::WLineEdit();
            _lnFilter->setEmptyText("Search");
            //_lnFilter->setStyleClass("search-box");
            _lnFilter->keyWentUp().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::_lnFilterKeyWentUp);

            layTbMain-> addWidget(_lnFilter);

            //Main Table View
            Wt::WVBoxLayout *layTblMain = new Wt::WVBoxLayout();
            layTblMain->setContentsMargins(0,0,0,0);
            layTblMain->setSpacing(0);

            Wt::WContainerWidget *cntTblMain = new Wt::WContainerWidget();
            cntTblMain->setLayout(layTblMain);

            _createMainTable();

            layTblMain->addWidget(_tblMain, 1);

            layMain->addWidget(cntTblMain, 1);

            //advancedFilter
            _createAdvancedFilterView();
            _cntAdvancedFilter->hide();//set advanced filter panel initially hidden
            layMain->addWidget(_cntAdvancedFilter);

        }
    }
}

#endif // MQUERYTABLEVIEWWIDGET_IMPL_H
