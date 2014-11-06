#include "MTableViewWidget.h"
#include "../Widgets/Dialogs/MFilesUploadDialog.h"
#include "../IO/MStreamedMemoryResource.h"
#include "../IO/Data/MDataCSV.h"

#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>

Ms::Widgets::MTableViewWidget::MTableViewWidget()
{
    _tableName = "table";
    _filterRegExpression = "[^$]{0,255}";
    _ignoreNumFilterColumns = 0;
    _importCSVFeatureEnabled = true;
    _exportCSVFeatureEnabled = true;
    _rowHeightChanged = false;

    prepareView();
}

Ms::Widgets::MTableViewWidget::~MTableViewWidget()
{
}

Wt::WTableView *Ms::Widgets::MTableViewWidget::table() const
{
    return _table;
}

Wt::WStandardItemModel *Ms::Widgets::MTableViewWidget::model() const
{
    return _model;
}

Wt::WSortFilterProxyModel *Ms::Widgets::MTableViewWidget::proxyModel() const
{
    return _proxyModel;
}

const std::map<std::string,Ms::Core::MTableViewColumn> &Ms::Widgets::MTableViewWidget::columns() const
{
    return _columns;
}

const Wt::WToolBar *Ms::Widgets::MTableViewWidget::tbMain() const
{
    return _tbMain;
}

const Wt::WToolBar *Ms::Widgets::MTableViewWidget::tbGlobal() const
{
    return _tbGlobal;
}

void Ms::Widgets::MTableViewWidget::selectAll() const
{
    Wt::WModelIndexSet indexSet;

    for(int row = 0; row < _proxyModel->rowCount(); ++row)
    {
        indexSet.insert(_proxyModel->index(row,0));
    }

    _table->setSelectedIndexes(indexSet);
}

void Ms::Widgets::MTableViewWidget::selectNone() const
{
    Wt::WModelIndexSet indexSet;
    _table->setSelectedIndexes(indexSet);
}

void Ms::Widgets::MTableViewWidget::inverseSelection() const
{
    Wt::WModelIndexSet indexSet;

    for(int row = 0; row < _proxyModel->rowCount(); ++row)
    {
        if(!_table->selectionModel()->isSelected(_proxyModel->index(row,0)))
            indexSet.insert(_proxyModel->index(row,0));
    }

    _table->setSelectedIndexes(indexSet);
}

void Ms::Widgets::MTableViewWidget::importCSV(const std::string &/*fileName*/) const
{
//    std::vector<Wt::WStandardItem*> items;

//    Wt::WStandardItem *item = new Wt::WStandardItem("");
//    item->setData(true, Wt::EditRole);
//    items.push_back(item);

//    for(unsigned int i = 1; i < _model->columnCount(); ++i)
//    {
//        Wt::WStandardItem *item = new Wt::WStandardItem("");
//        item->setFlags(Wt::ItemIsSelectable | Wt::ItemIsEditable);
//        items.push_back(item);
//    }

//    _mdlAdvancedFilter->appendRow(items);

//    Ms::IO::Data::MDataCSV csvData;
//    if(!Ms::IO::readCsv(fileName, csvData))
//        return;

//    //scan for mandatory columns before import
//    for(auto iter = _columns.begin(); iter != _columns.end(); ++iter)
//    {
//        if((*iter).second.isMandatory())
//        {
//            bool found = false;
//            for(auto &col : csvData[0])//only the first data row csvData[0] is sufficient
//            {
//                if(col == (*iter).second.name())
//                {
//                    found = true;
//                    break;
//                }
//            }
//            if(!found)
//            {
//                std::cerr << "Error while importing CSV, mandatory column " << "\"" << (*iter).second.name() << "\"" << " was not found" << std::endl;
//                return;
//            }
//        }
//    }

//    //all mandatory columns found, import data
//    for(std::size_t row = 1; row < csvData.numRows(); ++row)
//    {
//        std::string columnsSql = "";
//        std::string valuesSql = "";

//        for(std::size_t col = 0; col < csvData[0].size(); ++col)
//        {
//            if(columnExists(csvData[0][col]))//only import columns that exist in table and not ignored
//            {
//                if(!_columns[csvData[0][col]].isIgnored())
//                {
//                    //generate columns and values sql
//                    if(csvData[row][col] != "")//if the column has data
//                    {
//                        if(columnsSql != "")
//                            columnsSql += ",";

//                        if(valuesSql != "")
//                            valuesSql += ",";

//                        columnsSql += columnName(csvData[0][col]);
//                        valuesSql = valuesSql + "'" + csvData[row][col] + "'";
//                    }
//                }
//            }
//        }
//    }
}

void Ms::Widgets::MTableViewWidget::exportCSV()
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

bool Ms::Widgets::MTableViewWidget::columnExists(const std::string &name) const
{
    return _columns.find(name) != _columns.end();
}

void Ms::Widgets::MTableViewWidget::addColumn(const Ms::Core::MTableViewColumn &column) const
{
     if(!columnExists(column.name()))
     {
        _columns[column.name()] = column;
        _cmbFilterColumn->addItem(column.name());

        _model->insertColumns(_columns.size() - 1, 1);
        _model->setHeaderData(_columns.size() - 1, Wt::Orientation::Horizontal, column.name());
        _table->setItemDelegateForColumn(_columns.size() - 1, column.delegate());
     }
}

void Ms::Widgets::MTableViewWidget::removeColumn(const std::string &name) const
{
    if(columnExists(name))
    {
        _columns.erase(name);

        for(int i = 0; i < _model->columnCount(); ++i)
        {
            std::string header = boost::any_cast<std::string>(_model->headerData(i));
            if(header == name)
            {
                _model->removeColumn(i);
                _cmbFilterColumn->removeItem(i);

                break;
            }
        }
    }
}

void Ms::Widgets::MTableViewWidget::clear()
{
    _columns.clear();
    _model->clear();

    _cmbFilterColumn->clear();
}

Wt::WString Ms::Widgets::MTableViewWidget::filterRegExpression() const
{
    return _filterRegExpression;
}

void Ms::Widgets::MTableViewWidget::setFilterRegExpression(const Wt::WString &exp)
{
    _filterRegExpression = exp;
    _proxyModel->setFilterRegExp(exp);
}

bool Ms::Widgets::MTableViewWidget::isImportCSVFeatureEnabled() const
{
    return _importCSVFeatureEnabled;
}

void Ms::Widgets::MTableViewWidget::setImportCSVFeatureEnabled(bool enabled)
{
    _importCSVFeatureEnabled = enabled;

    _popMnuIOImportCSVItem->setHidden(!enabled);
}

bool Ms::Widgets::MTableViewWidget::isExportCSVFeatureEnabled() const
{
    return _exportCSVFeatureEnabled;
}

void Ms::Widgets::MTableViewWidget::setExportCSVFeatureEnabled(bool enabled)
{
    _exportCSVFeatureEnabled = enabled;

    _popMnuIOExportCSVItem->setHidden(!enabled);
}

std::string Ms::Widgets::MTableViewWidget::tableName() const
{
    return _tableName;
}

void Ms::Widgets::MTableViewWidget::setTableName(const std::string &name)
{
    _tableName = name;
}

void Ms::Widgets::MTableViewWidget::createMainTable()
{
    _table = new Wt::WTableView();
    _table->setColumnResizeEnabled(true);
    _table->setAlternatingRowColors(true);
    _table->setRowHeight(25);
    _table->setHeaderHeight(25);
    _table->setSelectionMode(Wt::ExtendedSelection);

    _model = new Wt::WStandardItemModel();

    _proxyModel = new Wt::WSortFilterProxyModel();
    _proxyModel->setDynamicSortFilter(true);
    _proxyModel->setFilterRole(Wt::DisplayRole);
    _proxyModel->setFilterRegExp(_filterRegExpression);
    _proxyModel->setSourceModel(_model);

    _table->setModel(_proxyModel);
}

void Ms::Widgets::MTableViewWidget::refilter() const
{
    _proxyModel->setFilterRegExp(_lnFilter->text() + _filterRegExpression);
}

std::string Ms::Widgets::MTableViewWidget::generateCSVData() const
{
    std::string data = "";
    std::vector<Ms::Core::MTableViewColumn>::size_type i = 0;

    //headers
    for(auto iter = _columns.begin(); iter != _columns.end(); ++iter)
    {
        data += (*iter).first;

        if(i++ < _columns.size() -1)
            data +=",";
    }

    data += "\n";

    //data
    if(_table->selectedIndexes().size() > 0)//export selected
    {
        Wt::WModelIndexSet::size_type row = 0;
        for(const Wt::WModelIndex &index : _table->selectedIndexes())
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
            if(row++ < _table->selectedIndexes().size() - 1)
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

//Slots
void Ms::Widgets::MTableViewWidget::lnFilterKeyWentUp()
{
    refilter();
}

void Ms::Widgets::MTableViewWidget::cmbFilterColumnChanged()
{
    _proxyModel->setFilterKeyColumn(_cmbFilterColumn->currentIndex() + _ignoreNumFilterColumns);
    refilter();
}

void Ms::Widgets::MTableViewWidget::cntTblMainKeyWentDown(Wt::WKeyEvent key)
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
}

void Ms::Widgets::MTableViewWidget::popMnuSelectAllItemTriggered()
{
    selectAll();
}

void Ms::Widgets::MTableViewWidget::popMnuSelectNoneItemTriggered()
{
    selectNone();
}

void Ms::Widgets::MTableViewWidget::popMnuInverseSelectionItemTriggered()
{
    inverseSelection();
}

void Ms::Widgets::MTableViewWidget::popMnuIOImportCSVItemTriggered()
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

void Ms::Widgets::MTableViewWidget::popMnuIOExportCSVItemTriggered()
{
    exportCSV();
}

void Ms::Widgets::MTableViewWidget::sldRowHeightValueChanged()
{
    _table->setRowHeight(_sldRowHeight->value());

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
    _table->setColumnWidth(_model->columnCount() -1, _table->columnWidth(_model->columnCount() -1).value() + resizeValue);
    //////////////////////////////////////////////////////////////////////////////
}

void Ms::Widgets::MTableViewWidget::prepareView()
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
    _popMnuSelectAllItem->triggered().connect(this, &Ms::Widgets::MTableViewWidget::popMnuSelectAllItemTriggered);

    _popMnuSelectNoneItem = _popMnuSelection->addItem("Select None - Shift+D");
    _popMnuSelectNoneItem->triggered().connect(this, &Ms::Widgets::MTableViewWidget::popMnuSelectNoneItemTriggered);

    _popMnuInverseSelectionItem = _popMnuSelection->addItem("Inverse Selection - Shift+I");
    _popMnuInverseSelectionItem->triggered().connect(this, &Ms::Widgets::MTableViewWidget::popMnuInverseSelectionItemTriggered);

    _popMnuIO = new Wt::WPopupMenu();
    _popMnuTools->addMenu("IO", _popMnuIO);

    _popMnuIOImportCSVItem = _popMnuIO->addItem("Import CSV");
    _popMnuIOImportCSVItem->triggered().connect(this, &Ms::Widgets::MTableViewWidget::popMnuIOImportCSVItemTriggered);

    _popMnuIOExportCSVItem = _popMnuIO->addItem("Export CSV");
    _popMnuIOExportCSVItem->triggered().connect(this, &Ms::Widgets::MTableViewWidget::popMnuIOExportCSVItemTriggered);

    _cmbFilterColumn = new Wt::WComboBox();
    _cmbFilterColumn->setMinimumSize(220, 30);
    _cmbFilterColumn->setMaximumSize(220, 30);
    _cmbFilterColumn->changed().connect(this, &Ms::Widgets::MTableViewWidget::cmbFilterColumnChanged);
    _mdlCmbFilterColumn = new Wt::WStandardItemModel();
    _cmbFilterColumn->setModel(_mdlCmbFilterColumn);
    layCntTbGlobal->addWidget(_cmbFilterColumn);

    _lnFilter = new Wt::WLineEdit();
    _lnFilter->setMinimumSize(220, 30);
    _lnFilter->setMaximumSize(220, 30);
    _lnFilter->setEmptyText("Filter");
    _lnFilter->keyWentUp().connect(this, &Ms::Widgets::MTableViewWidget::lnFilterKeyWentUp);

    layCntTbGlobal-> addWidget(_lnFilter);

    //Main Table View
    Wt::WVBoxLayout *layCntTblMain = new Wt::WVBoxLayout();
    layCntTblMain->setContentsMargins(0,0,0,0);
    layCntTblMain->setSpacing(0);

    Wt::WContainerWidget *cntTblMain = new Wt::WContainerWidget();
    cntTblMain->setLayout(layCntTblMain);
    cntTblMain->setAttributeValue("tabindex", "0");
    cntTblMain->keyWentDown().connect(this, &Ms::Widgets::MTableViewWidget::cntTblMainKeyWentDown);

    createMainTable();

    layCntTblMain->addWidget(_table, 1);

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
    //_sldRowHeight->setTickPosition(Wt::WSlider::TicksAbove);
    _sldRowHeight->valueChanged().connect(this, &Ms::Widgets::MTableViewWidget::sldRowHeightValueChanged);

    layTbMain->addWidget(_sldRowHeight);
}
