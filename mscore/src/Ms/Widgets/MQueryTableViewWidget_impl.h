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
            m_dboSession(dboSession)
        {
            m_defaultFilterColumnIndex = 0;
            m_ignoreNumFilterColumns = 0;
            m_advancedFilterActive = false;
            m_customFilterActive = false;
            m_customFilterString = "";
            m_advancedFilterString = "";
            m_filterRegExpression = "[^$]{0,255}";

            //features
            m_importCSVFeatureEnabled = true;
            m_exportCSVFeatureEnabled = true;
            m_advancedFilterFeatureEnabled = true;

            prepareView();

            setModelUserName(dboSession.userName());
        }

        template<typename T>
        Ms::Widgets::MQueryTableViewWidget<T>::~MQueryTableViewWidget()
        {
            delete m_model;
            delete m_proxyModel;
        }

        //functions
        template<typename T>
        const Ms::Dbo::MDboQueryModel<Wt::Dbo::ptr<T>> *Ms::Widgets::MQueryTableViewWidget<T>::model() const
        {
            return m_model;
        }

        template<typename T>
        const Wt::WSortFilterProxyModel *Ms::Widgets::MQueryTableViewWidget<T>::proxyModel() const
        {
            return m_proxyModel;
        }

        template<typename T>
        Wt::WTableView *Ms::Widgets::MQueryTableViewWidget<T>::table() const
        {
            return m_tblMain;
        }

        template<typename T>
        const std::string Ms::Widgets::MQueryTableViewWidget<T>::tableName() const
        {
            return m_dboSession.tableName<T>();
        }

        template<typename T>
        Wt::WPushButton *Ms::Widgets::MQueryTableViewWidget<T>::createToolButton(const std::string &text, const std::string &iconPath, const std::string &toolTip)
        {
            Wt::WPushButton *btn = new Wt::WPushButton(text);
            btn->setToolTip(toolTip);
            btn->setIcon(Wt::WLink(iconPath));

            m_tbMain->addButton(btn);

            return btn;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setRowHeight(int height)
        {
            m_sldRowHeight->setValue(height);
            m_tblMain->setRowHeight(height);
        }

        template<typename T>
        int Ms::Widgets::MQueryTableViewWidget<T>::rowHeight() const
        {
            return m_tblMain->rowHeight().value();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setHeaderHeight(const Wt::WLength &height)
        {
            m_tblMain->setHeaderHeight(height);
        }

        template<typename T>
        const Wt::WLength Ms::Widgets::MQueryTableViewWidget<T>::headerHeight() const
        {
            return m_tblMain->headerHeight();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setQuery(Wt::Dbo::Query<Wt::Dbo::ptr<T> > &query) const
        {
            m_query = query;
        }

        template<typename T>
        Wt::Dbo::Query<Wt::Dbo::ptr<T>> &Ms::Widgets::MQueryTableViewWidget<T>::query() const
        {
            return m_query;
        }

        template<typename T>
        const Ms::Core::Dbo::MDboSession *Ms::Widgets::MQueryTableViewWidget<T>::dboSession() const
        {
            return m_dboSession;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setDboSession(Ms::Core::Dbo::MDboSession &dboSession)
        {
            m_dboSession = dboSession;

            setModelUserName(dboSession.userName());
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setDefaultFilterColumnIndex(int columnIndex)
        {
            m_defaultFilterColumnIndex = columnIndex;
            m_proxyModel->setFilterKeyColumn(columnIndex);
        }

        template<typename T>
        int Ms::Widgets::MQueryTableViewWidget<T>::defaultFilterColumnIndex()
        {
            return m_defaultFilterColumnIndex;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setSelectionMode(Wt::SelectionMode mode)
        {
            m_tblMain->setSelectionMode(mode);
        }

        template<typename T>
        Wt::SelectionMode Ms::Widgets::MQueryTableViewWidget<T>::selectionMode()
        {
            return m_tblMain->selectionMode();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setDynamicSortFilter(bool enable)
        {
            m_proxyModel->setDynamicSortFilter(enable);
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::dynamicSortFilter() const
        {
            return m_proxyModel->dynamicSortFilter();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setFilterRole(int role)
        {
            m_proxyModel->setFilterRole(role);
        }

        template<typename T>
        int Ms::Widgets::MQueryTableViewWidget<T>::filterRole()
        {
            return m_proxyModel->filterRole();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setFilterRegExp(const Wt::WString &regExpression)
        {
            m_filterRegExpression = regExpression;
            m_proxyModel->setFilterRegExp(regExpression);
        }

        template<typename T>
        Wt::WString Ms::Widgets::MQueryTableViewWidget<T>::filterRegExp() const
        {
            return m_filterRegExpression;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setColumnResizeEnabled(bool enabled)
        {
            m_tblMain->setColumnResizeEnabled(enabled);
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::isColumnResizeEnabled() const
        {
            return m_tblMain->isColumnResizeEnabled();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setAlternatingRowColors(bool enable)
        {
            m_tblMain->setAlternatingRowColors(enable);
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::alternatingRowColors() const
        {
            return m_tblMain->alternatingRowColors();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::updateView()
        {
            try
            {
                if(m_tblMain->selectionMode() != Wt::ExtendedSelection)
                {
                    m_popMnuSelectAllItem->setDisabled(true);
                    m_popMnuSelectNoneItem->setDisabled(true);
                    m_popMnuInverseSelectionItem->setDisabled(true);
                }

                saveSelection();

                updateModel();
                updateTable();

                if(!m_cntAdvancedFilter->isHidden())
                    updateAdvancedFilterTable();

                loadSelection();

                updateStatusBar();
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
            m_selection.clear();

            for(const Wt::WModelIndex &index : m_tblMain->selectedIndexes())
                m_selection.push_back(m_model->resultRow(m_proxyModel->mapToSource(index).row()).id());
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::loadSelection() const
        {
            //load selection by id
            Wt::WModelIndexSet indexSet;

            for(const typename Wt::Dbo::dbo_traits<T>::IdType &id : m_selection)
            {
                for(int row = 0; row < m_proxyModel->rowCount(); ++row)
                {
                    if((m_model->resultRow(m_proxyModel->mapToSource(m_proxyModel->index(row,0)).row()).id() == id))
                    {
                        indexSet.insert(m_proxyModel->index(row,0));
                        //m_tblMain->select(m_proxyModel->index(row,0));
                    }
                }
            }

            m_tblMain->setSelectedIndexes(indexSet);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::selectAll() const
        {
            Wt::WModelIndexSet indexSet;

            for(int row = 0; row < m_proxyModel->rowCount(); ++row)
            {
                indexSet.insert(m_proxyModel->index(row,0));
            }

            m_tblMain->setSelectedIndexes(indexSet);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::selectNone() const
        {
            Wt::WModelIndexSet indexSet;
            m_tblMain->setSelectedIndexes(indexSet);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::inverseSelection() const
        {
            Wt::WModelIndexSet indexSet;

            for(int row = 0; row < m_proxyModel->rowCount(); ++row)
            {
                if(!m_tblMain->selectionModel()->isSelected(m_proxyModel->index(row,0)))
                    indexSet.insert(m_proxyModel->index(row,0));
            }

            m_tblMain->setSelectedIndexes(indexSet);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::addColumn(const Ms::Widgets::MQueryTableViewColumn &column) const
        {
            m_columns.push_back(column);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::removeColumn(const Ms::Widgets::MQueryTableViewColumn &column) const
        {
            for(std::vector<Ms::Widgets::MQueryTableViewColumn>::iterator iter = m_columns.begin(); iter != m_columns.end(); ++iter)
            {
                if((*iter) == column)
                {
                    m_columns.erase(iter);

                    break;
                }
            }
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::clearColumns() const
        {
            m_columns.clear();
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::dbColumnExist(const std::string &dbColumnName) const//searches for real column DB name
        {
            return std::find_if(m_columns.begin(), m_columns.end(), [dbColumnName](const Ms::Widgets::MQueryTableViewColumn &col){
                return col.dbFieldName() == dbColumnName;
            }) != m_columns.end();
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::columnExist(const std::string &columnDisplayName) const//searches for column display name
        {
            return std::find_if(m_columns.begin(), m_columns.end(), [columnDisplayName](const Ms::Widgets::MQueryTableViewColumn &col){
                return col.displayName() == columnDisplayName;
            }) != m_columns.end();
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::columnIsIgnored(const std::string &columnDisplayName) const
        {
            auto iter = std::find_if(m_columns.begin(), m_columns.end(), [columnDisplayName](const Ms::Widgets::MQueryTableViewColumn &col){
                return col.displayName() == columnDisplayName;
            });

            return (iter != m_columns.end()) && (*iter).isIgnored();
        }

        template<typename T>
        std::string Ms::Widgets::MQueryTableViewWidget<T>::columnName(const std::string &columnDisplayName) const
        {
            auto iter = std::find_if(m_columns.begin(), m_columns.end(), [columnDisplayName](const Ms::Widgets::MQueryTableViewColumn &col){
                return col.displayName() == columnDisplayName;
            });

            return iter != m_columns.end() ? (*iter).dbFieldName() : "";
        }

        template<typename T>
        std::string Ms::Widgets::MQueryTableViewWidget<T>::columnDisplayName(const std::string &dboName) const
        {
            auto iter = std::find_if(m_columns.begin(), m_columns.end(), [dboName](const Ms::Widgets::MQueryTableViewColumn &col){
                return col.dbFieldName() == dboName;
            });

            return iter != m_columns.end() ? (*iter).displayName() : "";
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setIgnoreNumFilterColumns(int numColumns)
        {
            m_ignoreNumFilterColumns = numColumns;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::ignoredNumFilterColumns() const
        {
            return m_ignoreNumFilterColumns;
        }

        template<typename T>
        std::vector<Wt::Dbo::ptr<T>> Ms::Widgets::MQueryTableViewWidget<T>::selectedItems() const
        {
            std::vector<Wt::Dbo::ptr<T>> vec;

            for(const Wt::WModelIndex &index : m_tblMain->selectedIndexes())
            {
                Wt::Dbo::ptr<T> dboPtr = m_model->resultRow(m_proxyModel->mapToSource(index).row());

                vec.push_back(dboPtr);
            }

            return vec;
        }

        template<typename T>
        std::vector<Wt::Dbo::ptr<T>> Ms::Widgets::MQueryTableViewWidget<T>::items() const
        {
            std::vector<Wt::Dbo::ptr<T>> vec;

            for(int row = 0; row < m_proxyModel->rowCount(); ++row)
            {
                const Wt::WModelIndex index = m_proxyModel->mapToSource(m_proxyModel->index(row, 0));

                Wt::Dbo::ptr<T> dboPtr = m_model->resultRow(index.row());

                vec.push_back(dboPtr);
            }

            return vec;
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::isImportCSVFeatureEnabled() const
        {
            return m_importCSVFeatureEnabled;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setImportCSVFeatureEnabled(bool enabled)
        {
            m_importCSVFeatureEnabled = enabled;

            m_popMnuIOImportCSVItem->setHidden(!enabled);
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::isExportCSVFeatureEnabled() const
        {
            return m_exportCSVFeatureEnabled;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setExportCSVFeatureEnabled(bool enabled)
        {
            m_exportCSVFeatureEnabled = enabled;

            m_popMnuIOExportCSVItem->setHidden(!enabled);
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::isAdvancedFilterFeatureEnabled() const
        {
            return m_advancedFilterFeatureEnabled;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setAdvancedFilterFeatureEnabled(bool enabled)
        {
            m_advancedFilterFeatureEnabled = enabled;

            m_popMnuViewAdvancedFilterItem->setHidden(!enabled);

            if(enabled)
            {
                if(m_popMnuViewAdvancedFilterItem->isChecked())
                    m_cntAdvancedFilter->show();
                else
                    m_cntAdvancedFilter->hide();
            }
            else
                m_cntAdvancedFilter->hide();
        }

        template<typename T>
        std::string Ms::Widgets::MQueryTableViewWidget<T>::advancedFilterString() const
        {
            return m_advancedFilterString;
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::isAdvancedFilterActive() const
        {
            return m_advancedFilterActive;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setAdvancedFilterActive(bool active)
        {
            m_advancedFilterActive = active;

            m_chkAdvancedFilterActive->setChecked(active);

            updateModel();
        }

        template<typename T>
        bool Ms::Widgets::MQueryTableViewWidget<T>::isCustomFilterActive() const
        {
            return m_customFilterActive;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setCustomFilterActive(bool active)
        {
            m_customFilterActive = active;

            m_chkCustomFilterActive->setChecked(active);

            updateModel();
        }

        template<typename T>
        std::string Ms::Widgets::MQueryTableViewWidget<T>::customFilterString() const
        {
            return m_customFilterString;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setCustomFilterString(const std::string &customFilterString)
        {
            m_customFilterString = customFilterString;
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

        template<typename T>
        Wt::Dbo::ptr<T> Ms::Widgets::MQueryTableViewWidget<T>::itemForModelIndex(const Wt::WModelIndex &index) const
        {
            return m_model->resultRow(index.row());
        }

        template<typename T>
        Wt::Dbo::ptr<T> Ms::Widgets::MQueryTableViewWidget<T>::itemForProxyModelIndex(const Wt::WModelIndex &proxyIndex) const
        {
            return m_model->resultRow(m_proxyModel->mapToSource(proxyIndex).row());
        }

        //Signals
        template<typename T>
        Wt::Signal<> &Ms::Widgets::MQueryTableViewWidget<T>::tableSelectionChanged()
        {
            return m_tableSelectionChanged;
        }

        template<typename T>
        Wt::Signal<Wt::Dbo::ptr<T>> &Ms::Widgets::MQueryTableViewWidget<T>::itemImported()
        {
            return m_itemImported;
        }

        //slots
        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::mainTableSelectionChanged()
        {
            updateStatusBar();

            m_tableSelectionChanged();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::cmbColumnFilterChanged()
        {
            m_proxyModel->setFilterKeyColumn(m_cmbColumnFilter->currentIndex() + m_ignoreNumFilterColumns);
            refilter();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::lnFilterKeyWentUp()
        {
            refilter();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::cntTblMainKeyWentDown(Wt::WKeyEvent key)
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
                m_popMnuViewAdvancedFilterItem->setChecked(!m_popMnuViewAdvancedFilterItem->isChecked());
                toggleAdvancedFilterView();
            }
            else if(((key.modifiers() & Wt::KeyboardModifier::ShiftModifier) == Wt::KeyboardModifier::ShiftModifier) && key.key() == Wt::Key::Key_R)
            {
                updateView();
            }
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::popMnuSelectAllItemTriggered()
        {
            selectAll();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::popMnuSelectNoneItemTriggered()
        {
            selectNone();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::popMnuInverseSelectionItemTriggered()
        {
            inverseSelection();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::popMnuIOImportCSVItemTriggered()
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
        void Ms::Widgets::MQueryTableViewWidget<T>::popMnuIOExportCSVItemTriggered()
        {
            if(m_model->rowCount() == 0)
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
        void Ms::Widgets::MQueryTableViewWidget<T>::popMnuViewAdvancedFilterItemTriggered()
        {
            toggleAdvancedFilterView();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::btnAdvancedFilterAddClicked()
        {
            std::vector<Wt::WStandardItem*> items;

            Wt::WStandardItem *item = new Wt::WStandardItem("");
            item->setData(true, Wt::EditRole);
            items.push_back(item);

            for(int i = 1; i < m_mdlAdvancedFilter->columnCount(); ++i)
            {
                Wt::WStandardItem *item = new Wt::WStandardItem("");
                item->setFlags(Wt::ItemIsSelectable | Wt::ItemIsEditable);
                items.push_back(item);
            }

            m_mdlAdvancedFilter->appendRow(items);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::btnAdvancedFilterRemoveClicked()
        {
            while(m_tblAdvancedFilter->selectedIndexes().size() > 0)
            {
                m_mdlAdvancedFilter->removeRows((*m_tblAdvancedFilter->selectedIndexes().cbegin()).row(), 1);
            }
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::btnAdvancedFilterApplyClicked()
        {
            m_advancedFilterActive = true;

            m_advancedFilterString = "";

            bool openBracket = false;

            std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string>> vecValidItems;

            //gather valid columns
            for(int row = 0; row < m_mdlAdvancedFilter->rowCount(); ++row)
            {
                bool colActive = boost::any_cast<bool>(m_mdlAdvancedFilter->data(row, 0, Wt::EditRole));

                if(!colActive)
                    continue;

                std::string colDispName = boost::any_cast<Wt::WString>(m_mdlAdvancedFilter->data(row, 1)).toUTF8();
                std::string op1 = boost::any_cast<Wt::WString>(m_mdlAdvancedFilter->data(row, 2)).toUTF8();
                std::string value = boost::any_cast<Wt::WString>(m_mdlAdvancedFilter->data(row, 3)).toUTF8();
                std::string op2 = boost::any_cast<Wt::WString>(m_mdlAdvancedFilter->data(row, 4)).toUTF8();
                std::string combine = boost::any_cast<Wt::WString>(m_mdlAdvancedFilter->data(row, 5)).toUTF8();

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

                m_advancedFilterString += subQueryStr;
            }

            std::cout << m_advancedFilterString << std::endl;

            setAdvancedFilterActive(true);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::btnAdvancedFilterClearClicked()
        {
            setAdvancedFilterActive(false);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::popMnuReloadItemTriggered()
        {
            updateView();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::sldRowHeightValueChanged()
        {
            m_tblMain->setRowHeight(m_sldRowHeight->value());
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::importCSV(const std::string &fileName)
        {
            Ms::IO::Data::MDataCSV csvData;
            if(!Ms::IO::readCsv(fileName, csvData))
                return;

            //scan for mandatory columns before import
            for(auto iter = m_columns.begin(); iter != m_columns.end(); ++iter)
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
                        //don't seg fault on rows with different length than header (yes the file is corrupted and we shouldn't read it, but anyways!)
                        if(col > csvData[row].size() - 1)
                            continue;

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
                                                   "','" + m_dboSession.userName() + "'";

                if(columnsSql != "" && valuesSql != "")//found some columns and values ?
                {
                    //try to import
                    try
                    {
                        Wt::Dbo::Transaction transaction(m_dboSession);

                        std::string sqlCommand = "INSERT INTO " + tableName() + " (" + columnsSql + ") VALUES (" + valuesSql + ")";

                        m_dboSession.execute(sqlCommand);

                        transaction.commit();

                        //m_itemImported(dboPtr);//emit the itemImported signal
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
        std::string Ms::Widgets::MQueryTableViewWidget<T>::generateCSVData() const
        {
            std::string data = "";
            unsigned int i = 0;

            //headers
            for(auto iter = m_columns.begin(); iter != m_columns.end(); ++iter)
            {
                data += (*iter).displayName();

                if(i++ < m_columns.size() -1)
                    data +=",";
            }

            data += "\n";

            //data
            if(m_tblMain->selectedIndexes().size() > 0)//export selected
            {
                unsigned int row = 0;
                for(const Wt::WModelIndex &index : m_tblMain->selectedIndexes())
                {
                    for(int col = 0; col < m_proxyModel->columnCount(); ++col)
                    {
                        boost::any indexData = m_proxyModel->mapToSource(m_proxyModel->index(index.row(), col)).data(Wt::DisplayRole);

                        if(indexData.type() == typeid(bool))//format bool to 0 and 1 to be able to import exported files to database
                            boost::any_cast<bool>(indexData) ? data += "1" : data += "0";
                        else
                            data += Wt::asString(indexData).toUTF8();

                        if(col < m_proxyModel->columnCount() -1)
                            data +=",";
                    }
                    if(row++ < m_tblMain->selectedIndexes().size() - 1)
                        data +="\n";
                }
            }
            else//export all, taking in account filtering
            {
                for(int row = 0; row < m_proxyModel->rowCount(); ++row)
                {
                    for(int col = 0; col < m_proxyModel->columnCount(); ++col)
                    {
                        boost::any indexData = m_proxyModel->mapToSource(m_proxyModel->index(row, col)).data(Wt::DisplayRole);

                        if(indexData.type() == typeid(bool))//format bool to 0 and 1 to be able to import exported files to database
                            boost::any_cast<bool>(indexData) ? data += "1" : data += "0";
                        else
                            data += Wt::asString(indexData).toUTF8();

                        if(col < m_proxyModel->columnCount() -1)
                            data +=",";
                    }
                    if(row < m_proxyModel->rowCount() -1)
                        data +="\n";
                }
            }

            return data;
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::setModelUserName(const std::string &name) const
        {
            if(m_model)
                m_model->setUserName(name);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::refilter() const
        {
            m_proxyModel->setFilterRegExp(m_filterRegExpression + "(?i)" + m_lnFilter->text() + "(?-i)" + m_filterRegExpression);

            updateStatusBar();
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::createMainTable()
        {
            m_tblMain = new Wt::WTableView();
            m_tblMain->setColumnResizeEnabled(true);
            m_tblMain->setAlternatingRowColors(true);
            m_tblMain->setRowHeight(30);
            m_tblMain->setHeaderHeight(30);
            m_tblMain->setSelectionMode(Wt::ExtendedSelection);
            m_tblMain->selectionChanged().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::mainTableSelectionChanged);

            m_model = new Ms::Dbo::MDboQueryModel<Wt::Dbo::ptr<T>>();

            m_proxyModel = new Wt::WSortFilterProxyModel();
            m_proxyModel->setDynamicSortFilter(true);
            m_proxyModel->setFilterRole(Wt::DisplayRole);
            m_proxyModel->setFilterRegExp(m_filterRegExpression);
            m_proxyModel->setSourceModel(m_model);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::createAdvancedFilterView()
        {
            m_cntAdvancedFilter = new Ms::Widgets::MContainerWidget();
            m_cntAdvancedFilter->setTitle("<b><i>Advanced Filter</i></b>");
            m_cntAdvancedFilter->setMinimumSize(Wt::WLength::Auto, 300);
            m_cntAdvancedFilter->setMaximumSize(Wt::WLength::Auto, 300);

            m_layCntAdvancedFilter = dynamic_cast<Wt::WVBoxLayout*>(m_cntAdvancedFilter->layout());
            m_layCntAdvancedFilter->setContentsMargins(14,0,14,14);

            m_cntTbAdvancedFilter = new Wt::WContainerWidget();

            m_layCntAdvancedFilter->addWidget(m_cntTbAdvancedFilter);

            m_layCntTbAdvancedFilter = new Wt::WVBoxLayout();
            m_layCntTbAdvancedFilter->setContentsMargins(0,10,0,10);
            m_layCntTbAdvancedFilter->setSpacing(0);

            m_cntTbAdvancedFilter->setLayout(m_layCntTbAdvancedFilter);

            m_tbAdvancedFilter = new Wt::WToolBar();

            m_layCntTbAdvancedFilter->addWidget(m_tbAdvancedFilter);

            Wt::WPushButton *btnFilterAdd = new Wt::WPushButton("");
            btnFilterAdd->setToolTip("Add Filter");
            btnFilterAdd->setIcon(Wt::WLink("icons/Add.png"));
            btnFilterAdd->clicked().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::btnAdvancedFilterAddClicked);

            m_tbAdvancedFilter->addButton(btnFilterAdd);

            Wt::WPushButton *btnFilterRemove = new Wt::WPushButton("");
            btnFilterRemove->setToolTip("Remove Filter");
            btnFilterRemove->setIcon(Wt::WLink("icons/Remove.png"));
            btnFilterRemove->clicked().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::btnAdvancedFilterRemoveClicked);

            m_tbAdvancedFilter->addButton(btnFilterRemove);

            Wt::WPushButton *btnFilterApply = new Wt::WPushButton("");
            btnFilterApply->setToolTip("Apply Filter");
            btnFilterApply->setIcon(Wt::WLink("icons/Apply.png"));
            btnFilterApply->clicked().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::btnAdvancedFilterApplyClicked);

            m_tbAdvancedFilter->addButton(btnFilterApply);

            Wt::WPushButton *btnFilterClear = new Wt::WPushButton("");
            btnFilterClear->setToolTip("Clear Filter");
            btnFilterClear->setIcon(Wt::WLink("icons/Clear.png"));
            btnFilterClear->clicked().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::btnAdvancedFilterClearClicked);

            m_tbAdvancedFilter->addButton(btnFilterClear);


            m_tblAdvancedFilter = new Wt::WTableView();
            m_tblAdvancedFilter->setColumnResizeEnabled(true);
            m_tblAdvancedFilter->setAlternatingRowColors(true);
            m_tblAdvancedFilter->setRowHeight(25);
            m_tblAdvancedFilter->setHeaderHeight(25);
            m_tblAdvancedFilter->setSelectionMode(Wt::ExtendedSelection);

            m_layCntAdvancedFilter->addWidget(m_tblAdvancedFilter, 1);

            m_mdlAdvancedFilter = new Wt::WStandardItemModel(0,6);

            m_tblAdvancedFilter->setModel(m_mdlAdvancedFilter);

            m_mdlAdvancedFilter->setHeaderData(0, std::string("Active"));
            m_mdlAdvancedFilter->setHeaderData(1, std::string("Column Name"));
            m_mdlAdvancedFilter->setHeaderData(2, std::string("Operator"));
            m_mdlAdvancedFilter->setHeaderData(3, std::string("Value"));
            m_mdlAdvancedFilter->setHeaderData(4, std::string("Operator"));
            m_mdlAdvancedFilter->setHeaderData(5, std::string("Combine With Next"));

            std::vector<std::string> delegateColNameItems;
            int i = 0;

            Ms::Widgets::Delegates::MCheckBoxDelegate *colActiveDelegate = new Ms::Widgets::Delegates::MCheckBoxDelegate();

            for(auto iter = m_columns.begin(); iter != m_columns.end(); ++iter)
            {
                if(++i > m_ignoreNumFilterColumns)
                    delegateColNameItems.push_back((*iter).displayName());
            }

            Ms::Widgets::Delegates::MComboBoxDelegate *colNameDelegate = new Ms::Widgets::Delegates::MComboBoxDelegate(delegateColNameItems);

            std::vector<std::string> delegateOperator1Items = {"=", "!=", ">", ">=", "<", "<="};
            Ms::Widgets::Delegates::MComboBoxDelegate *colOperator1Delegate = new Ms::Widgets::Delegates::MComboBoxDelegate(delegateOperator1Items);

            std::vector<std::string> delegateOperator2Items = {"AND", "OR"};
            Ms::Widgets::Delegates::MComboBoxDelegate *colOperator2Delegate = new Ms::Widgets::Delegates::MComboBoxDelegate(delegateOperator2Items);

            std::vector<std::string> delegateCombineWithNextItems = {"NO", "YES"};
            Ms::Widgets::Delegates::MComboBoxDelegate *colCombineWithNextDelegate = new Ms::Widgets::Delegates::MComboBoxDelegate(delegateCombineWithNextItems);


            m_tblAdvancedFilter->setItemDelegateForColumn(0, colActiveDelegate);
            m_tblAdvancedFilter->setItemDelegateForColumn(1, colNameDelegate);
            m_tblAdvancedFilter->setItemDelegateForColumn(2, colOperator1Delegate);
            m_tblAdvancedFilter->setItemDelegateForColumn(3, new Ms::Widgets::Delegates::MItemDelegate());
            m_tblAdvancedFilter->setItemDelegateForColumn(4, colOperator2Delegate);
            m_tblAdvancedFilter->setItemDelegateForColumn(5, colCombineWithNextDelegate);
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::updateModel()
        {
            try
            {
                Wt::Dbo::Transaction transaction(m_dboSession);

                m_queryFilter = m_query;

                if(m_customFilterActive)
                {
                    if(m_customFilterString != "")
                        m_queryFilter.where(std::string("(") + m_customFilterString + ")");
                }

                if(m_advancedFilterActive)
                {
                    if(m_advancedFilterString != "")
                        m_queryFilter.where(std::string("(") + m_advancedFilterString + ")");
                }

                m_model->setQuery(m_queryFilter);

                transaction.commit();

                m_model->reload();

                //int i = 0;

                for(auto iter = m_columns.begin(); iter != m_columns.end(); ++iter)
                {
                    m_model->addColumn((*iter).dbFieldName(), (*iter).displayName(), (*iter).flags());
                    //m_tblMain->setColumnWidth(i++, (*iter).width());
                }

                m_cmbColumnFilter->clear();

                int i = 0;
                //populate m_cmbColumnFilter
                for(auto iter = m_columns.begin(); iter != m_columns.end(); ++iter)
                {
                    if(++i > m_ignoreNumFilterColumns)
                        m_cmbColumnFilter->addItem((*iter).displayName());
                }

                if(m_model->columnCount() >= m_defaultFilterColumnIndex)
                    m_proxyModel->setFilterKeyColumn(m_defaultFilterColumnIndex);//reset the model filter column
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
        void Ms::Widgets::MQueryTableViewWidget<T>::updateTable() const
        {
            m_tblMain->setModel(m_proxyModel);

            int i = 0;
            for(auto iter = m_columns.begin(); iter != m_columns.end(); ++iter)
            {
                m_tblMain->setItemDelegateForColumn(i++, (*iter).delegate());
            }
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::updateAdvancedFilterTable() const
        {
            std::vector<std::string> delegateColNameItems;
            int i = 0;

            for(auto iter = m_columns.begin(); iter != m_columns.end(); ++iter)
            {
                if(++i > m_ignoreNumFilterColumns)
                    delegateColNameItems.push_back((*iter).displayName());
            }

            Ms::Widgets::Delegates::MComboBoxDelegate *delegate = dynamic_cast<Ms::Widgets::Delegates::MComboBoxDelegate*>(m_tblAdvancedFilter->itemDelegateForColumn(1));
            delegate->setItems(delegateColNameItems); //update delegate columnName column with new columns (if any)
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::toggleAdvancedFilterView() const
        {
            if(m_popMnuViewAdvancedFilterItem->isChecked())
                updateAdvancedFilterTable();

            m_cntAdvancedFilter->setHidden(!m_popMnuViewAdvancedFilterItem->isChecked());
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::updateStatusBar() const
        {
            m_lblStatus->setText(std::to_string(m_tblMain->selectedIndexes().size()) + " Item(s) selected, " + std::to_string(m_proxyModel->rowCount()) + " Total items.");
        }

        template<typename T>
        void Ms::Widgets::MQueryTableViewWidget<T>::prepareView()
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

            m_tbGlobal = new Wt::WToolBar();
            layCntTbGlobal->addWidget(m_tbGlobal, 1);

            m_btnMnuTools = new Wt::WPushButton("");
            m_btnMnuTools->setToolTip("Tools");
            Wt::WLink lnkBtnIcon("icons/Tools.png");
            m_btnMnuTools->setIcon(lnkBtnIcon);

            layCntTbGlobal->addWidget(m_btnMnuTools);

            m_popMnuTools = new Wt::WPopupMenu();
            m_btnMnuTools->setMenu(m_popMnuTools);

            m_popMnuSelection = new Wt::WPopupMenu();
            m_popMnuTools->addMenu("Selection", m_popMnuSelection);

            m_popMnuSelectAllItem = m_popMnuSelection->addItem("Select All - Shift+A");
            m_popMnuSelectAllItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::popMnuSelectAllItemTriggered);

            m_popMnuSelectNoneItem = m_popMnuSelection->addItem("Select None - Shift+D");
            m_popMnuSelectNoneItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::popMnuSelectNoneItemTriggered);

            m_popMnuInverseSelectionItem = m_popMnuSelection->addItem("Inverse Selection - Shift+I");
            m_popMnuInverseSelectionItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::popMnuInverseSelectionItemTriggered);

            m_popMnuIO = new Wt::WPopupMenu();
            m_popMnuTools->addMenu("IO", m_popMnuIO);

            m_popMnuIOImportCSVItem = m_popMnuIO->addItem("Import CSV");
            m_popMnuIOImportCSVItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::popMnuIOImportCSVItemTriggered);

            m_popMnuIOExportCSVItem = m_popMnuIO->addItem("Export CSV");
            m_popMnuIOExportCSVItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::popMnuIOExportCSVItemTriggered);

            m_popMnuView = new Wt::WPopupMenu();
            m_popMnuTools->addMenu("View", m_popMnuView);

            m_popMnuViewAdvancedFilterItem = m_popMnuView->addItem("Advanced Filter - Shift+F");
            m_popMnuViewAdvancedFilterItem->setCheckable(true);
            m_popMnuViewAdvancedFilterItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::popMnuViewAdvancedFilterItemTriggered);

            m_popMnuTools->addSeparator();

            m_popMnuReloadItem = m_popMnuTools->addItem("Reload - Shift+R");
            m_popMnuReloadItem->triggered().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::popMnuReloadItemTriggered);

            m_cmbColumnFilter = new Wt::WComboBox();
            m_cmbColumnFilter->setMinimumSize(220, 30);
            m_cmbColumnFilter->setMaximumSize(220, 30);
            m_cmbColumnFilter->changed().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::cmbColumnFilterChanged);
            m_mdlCmbColumnFilter = new Wt::WStandardItemModel();
            m_cmbColumnFilter->setModel(m_mdlCmbColumnFilter);
            layCntTbGlobal->addWidget(m_cmbColumnFilter);

            m_lnFilter = new Wt::WLineEdit();
            m_lnFilter->setMinimumSize(220, 30);
            m_lnFilter->setMaximumSize(220, 30);
            m_lnFilter->setEmptyText("Filter");
            m_lnFilter->keyWentUp().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::lnFilterKeyWentUp);

            layCntTbGlobal-> addWidget(m_lnFilter);

            //Main Table View
            Wt::WVBoxLayout *layCntTblMain = new Wt::WVBoxLayout();
            layCntTblMain->setContentsMargins(0,0,0,0);
            layCntTblMain->setSpacing(0);

            Wt::WContainerWidget *cntTblMain = new Wt::WContainerWidget();
            cntTblMain->setLayout(layCntTblMain);
            cntTblMain->setAttributeValue("tabindex", "0");
            cntTblMain->keyWentDown().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::cntTblMainKeyWentDown);

            createMainTable();

            layCntTblMain->addWidget(m_tblMain, 1);

            layCntMainView->addWidget(cntTblMain, 1);

            //Main toolbar
            Wt::WHBoxLayout *layTbMain = new Wt::WHBoxLayout();
            layTbMain->setContentsMargins(0,8,0,8);
            layTbMain->setSpacing(0);

            Wt::WContainerWidget *cntTbMain = new Wt::WContainerWidget();
            cntTbMain->setLayout(layTbMain);

            layCntMainView->addWidget(cntTbMain);

            m_tbMain = new Wt::WToolBar();
            layTbMain->addWidget(m_tbMain, 1);

            m_sldRowHeight = new Wt::WSlider(Wt::Horizontal);
            m_sldRowHeight->resize(100, 20);
            m_sldRowHeight->setRange(25,300);
            m_sldRowHeight->setValue(25);
            m_sldRowHeight->setTickInterval(10);
            m_sldRowHeight->setHandleWidth(10);
            m_sldRowHeight->setTickPosition(Wt::WSlider::TicksAbove);
            m_sldRowHeight->valueChanged().connect(this, &Ms::Widgets::MQueryTableViewWidget<T>::sldRowHeightValueChanged);

            layTbMain->addWidget(m_sldRowHeight);

            //Status bar
            m_cntStatusBar = new Wt::WContainerWidget();

            layCntMainView->addWidget(m_cntStatusBar);

            m_layCntStatusBar = new Wt::WHBoxLayout();
            m_layCntStatusBar->setContentsMargins(4,0,4,0);
            m_layCntStatusBar->setSpacing(2);

            m_cntStatusBar->setLayout(m_layCntStatusBar);

            m_lblStatus = new Wt::WLabel("");

            m_layCntStatusBar->addWidget(m_lblStatus, 1);

            Wt::WHBoxLayout *layCntActiveFilters = new Wt::WHBoxLayout();
            layCntActiveFilters->setContentsMargins(8,0,8,0);
            layCntActiveFilters->setSpacing(6);

            Wt::WContainerWidget *cntActiveFilters = new Wt::WContainerWidget();
            cntActiveFilters->setLayout(layCntActiveFilters);

            m_layCntStatusBar->addWidget(cntActiveFilters);

            m_chkCustomFilterActive = new Wt::WCheckBox("CF");
            m_chkCustomFilterActive->setInline(true);
            m_chkCustomFilterActive->setToolTip("Custom Filter");
            m_chkCustomFilterActive->setDisabled(true);

            layCntActiveFilters->addWidget(m_chkCustomFilterActive);

            m_chkAdvancedFilterActive = new Wt::WCheckBox("AF");
            m_chkAdvancedFilterActive->setInline(true);
            m_chkAdvancedFilterActive->setToolTip("Advanced Filter");
            m_chkAdvancedFilterActive->setDisabled(true);

            layCntActiveFilters->addWidget(m_chkAdvancedFilterActive);

            //advancedFilter
            createAdvancedFilterView();
            m_cntAdvancedFilter->hide();//set advanced filter panel initially hidden
            layCntMainView->addWidget(m_cntAdvancedFilter);
        }
    }
}

#endif // MQUERYTABLEVIEWWIDGET_IMPL_H
