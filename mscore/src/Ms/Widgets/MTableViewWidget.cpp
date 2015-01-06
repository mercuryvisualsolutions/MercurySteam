#include "MTableViewWidget.h"
#include "../Widgets/Dialogs/MFilesUploadDialog.h"
#include "../IO/MStreamedMemoryResource.h"
#include "../IO/Data/MDataCSV.h"

#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>

Ms::Widgets::MTableViewWidget::MTableViewWidget()
{
    _tableName = "table";
    m_filterRegExpression = "[^$]{0,255}";
    m_ignoreNumFilterColumns = 0;
    m_importCSVFeatureEnabled = true;
    m_exportCSVFeatureEnabled = true;

    prepareView();
}

Ms::Widgets::MTableViewWidget::~MTableViewWidget()
{
    delete m_model;
    delete m_proxyModel;
}

Wt::WTableView *Ms::Widgets::MTableViewWidget::table() const
{
    return m_table;
}

Wt::WStandardItemModel *Ms::Widgets::MTableViewWidget::model() const
{
    return m_model;
}

Wt::WSortFilterProxyModel *Ms::Widgets::MTableViewWidget::proxyModel() const
{
    return m_proxyModel;
}

const std::map<std::string,Ms::Core::MTableViewColumn> &Ms::Widgets::MTableViewWidget::columns() const
{
    return m_columns;
}

const Wt::WToolBar *Ms::Widgets::MTableViewWidget::tbMain() const
{
    return m_tbMain;
}

const Wt::WToolBar *Ms::Widgets::MTableViewWidget::tbGlobal() const
{
    return m_tbGlobal;
}

void Ms::Widgets::MTableViewWidget::selectAll() const
{
    Wt::WModelIndexSet indexSet;

    for(int row = 0; row < m_proxyModel->rowCount(); ++row)
    {
        indexSet.insert(m_proxyModel->index(row,0));
    }

    m_table->setSelectedIndexes(indexSet);
}

void Ms::Widgets::MTableViewWidget::selectNone() const
{
    Wt::WModelIndexSet indexSet;
    m_table->setSelectedIndexes(indexSet);
}

void Ms::Widgets::MTableViewWidget::inverseSelection() const
{
    Wt::WModelIndexSet indexSet;

    for(int row = 0; row < m_proxyModel->rowCount(); ++row)
    {
        if(!m_table->selectionModel()->isSelected(m_proxyModel->index(row,0)))
            indexSet.insert(m_proxyModel->index(row,0));
    }

    m_table->setSelectedIndexes(indexSet);
}

void Ms::Widgets::MTableViewWidget::importCSV(const std::string &/*fileName*/) const
{
//    std::vector<Wt::WStandardItem*> items;

//    Wt::WStandardItem *item = new Wt::WStandardItem("");
//    item->setData(true, Wt::EditRole);
//    items.push_back(item);

//    for(unsigned int i = 1; i < m_model->columnCount(); ++i)
//    {
//        Wt::WStandardItem *item = new Wt::WStandardItem("");
//        item->setFlags(Wt::ItemIsSelectable | Wt::ItemIsEditable);
//        items.push_back(item);
//    }

//    m_mdlAdvancedFilter->appendRow(items);

//    Ms::IO::Data::MDataCSV csvData;
//    if(!Ms::IO::readCsv(fileName, csvData))
//        return;

//    //scan for mandatory columns before import
//    for(auto iter = m_columns.begin(); iter != m_columns.end(); ++iter)
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
//                if(!m_columns[csvData[0][col]].isIgnored())
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

bool Ms::Widgets::MTableViewWidget::columnExists(const std::string &name) const
{
    return m_columns.find(name) != m_columns.end();
}

void Ms::Widgets::MTableViewWidget::addColumn(const Ms::Core::MTableViewColumn &column) const
{
     if(!columnExists(column.name()))
     {
        m_columns[column.name()] = column;
        m_cmbFilterColumn->addItem(column.name());

        m_model->insertColumns(m_columns.size() - 1, 1);
        m_model->setHeaderData(m_columns.size() - 1, Wt::Orientation::Horizontal, column.name());
        m_table->setItemDelegateForColumn(m_columns.size() - 1, column.delegate());
     }
}

void Ms::Widgets::MTableViewWidget::removeColumn(const std::string &name) const
{
    if(columnExists(name))
    {
        m_columns.erase(name);

        for(int i = 0; i < m_model->columnCount(); ++i)
        {
            std::string header = boost::any_cast<std::string>(m_model->headerData(i));
            if(header == name)
            {
                m_model->removeColumn(i);
                m_cmbFilterColumn->removeItem(i);

                break;
            }
        }
    }
}

void Ms::Widgets::MTableViewWidget::clear()
{
    m_columns.clear();
    m_model->clear();

    m_cmbFilterColumn->clear();
}

Wt::WString Ms::Widgets::MTableViewWidget::filterRegExpression() const
{
    return m_filterRegExpression;
}

void Ms::Widgets::MTableViewWidget::setFilterRegExpression(const Wt::WString &exp)
{
    m_filterRegExpression = exp;
    m_proxyModel->setFilterRegExp(exp);
}

bool Ms::Widgets::MTableViewWidget::isImportCSVFeatureEnabled() const
{
    return m_importCSVFeatureEnabled;
}

void Ms::Widgets::MTableViewWidget::setImportCSVFeatureEnabled(bool enabled)
{
    m_importCSVFeatureEnabled = enabled;

    m_popMnuIOImportCSVItem->setHidden(!enabled);
}

bool Ms::Widgets::MTableViewWidget::isExportCSVFeatureEnabled() const
{
    return m_exportCSVFeatureEnabled;
}

void Ms::Widgets::MTableViewWidget::setExportCSVFeatureEnabled(bool enabled)
{
    m_exportCSVFeatureEnabled = enabled;

    m_popMnuIOExportCSVItem->setHidden(!enabled);
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
    m_table = new Wt::WTableView();
    m_table->setColumnResizeEnabled(true);
    m_table->setAlternatingRowColors(true);
    m_table->setRowHeight(25);
    m_table->setHeaderHeight(25);
    m_table->setSelectionMode(Wt::ExtendedSelection);
    m_table->selectionChanged().connect(this, &Ms::Widgets::MTableViewWidget::mainTableSelectionChanged);

    m_model = new Wt::WStandardItemModel();
    m_model->rowsInserted().connect(this, &Ms::Widgets::MTableViewWidget::modelRowsInserted);
    m_model->rowsRemoved().connect(this, &Ms::Widgets::MTableViewWidget::modelRowsRemoved);

    m_proxyModel = new Wt::WSortFilterProxyModel();
    m_proxyModel->setDynamicSortFilter(true);
    m_proxyModel->setFilterRole(Wt::DisplayRole);
    m_proxyModel->setFilterRegExp(m_filterRegExpression);
    m_proxyModel->setSourceModel(m_model);

    m_table->setModel(m_proxyModel);
}

void Ms::Widgets::MTableViewWidget::refilter() const
{
    m_proxyModel->setFilterRegExp(m_lnFilter->text() + m_filterRegExpression);

    updateStatusBar();
}

std::string Ms::Widgets::MTableViewWidget::generateCSVData() const
{
    std::string data = "";
    std::vector<Ms::Core::MTableViewColumn>::size_type i = 0;

    //headers
    for(auto iter = m_columns.begin(); iter != m_columns.end(); ++iter)
    {
        data += (*iter).first;

        if(i++ < m_columns.size() -1)
            data +=",";
    }

    data += "\n";

    //data
    if(m_table->selectedIndexes().size() > 0)//export selected
    {
        Wt::WModelIndexSet::size_type row = 0;
        for(const Wt::WModelIndex &index : m_table->selectedIndexes())
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
            if(row++ < m_table->selectedIndexes().size() - 1)
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

void Ms::Widgets::MTableViewWidget::updateStatusBar() const
{
    m_lblStatus->setText(std::to_string(m_table->selectedIndexes().size()) + " Item(s) selected, " + std::to_string(m_proxyModel->rowCount()) + " Total items.");
}

//Slots
void Ms::Widgets::MTableViewWidget::lnFilterKeyWentUp()
{
    refilter();
}

void Ms::Widgets::MTableViewWidget::cmbFilterColumnChanged()
{
    m_proxyModel->setFilterKeyColumn(m_cmbFilterColumn->currentIndex() + m_ignoreNumFilterColumns);
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
    m_table->setRowHeight(m_sldRowHeight->value());
}

void Ms::Widgets::MTableViewWidget::mainTableSelectionChanged()
{
    updateStatusBar();
}

void Ms::Widgets::MTableViewWidget::modelRowsInserted()
{
    updateStatusBar();
}

void Ms::Widgets::MTableViewWidget::modelRowsRemoved()
{
    updateStatusBar();
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

    //Global Toolbar
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
    m_popMnuSelectAllItem->triggered().connect(this, &Ms::Widgets::MTableViewWidget::popMnuSelectAllItemTriggered);

    m_popMnuSelectNoneItem = m_popMnuSelection->addItem("Select None - Shift+D");
    m_popMnuSelectNoneItem->triggered().connect(this, &Ms::Widgets::MTableViewWidget::popMnuSelectNoneItemTriggered);

    m_popMnuInverseSelectionItem = m_popMnuSelection->addItem("Inverse Selection - Shift+I");
    m_popMnuInverseSelectionItem->triggered().connect(this, &Ms::Widgets::MTableViewWidget::popMnuInverseSelectionItemTriggered);

    m_popMnuIO = new Wt::WPopupMenu();
    m_popMnuTools->addMenu("IO", m_popMnuIO);

    m_popMnuIOImportCSVItem = m_popMnuIO->addItem("Import CSV");
    m_popMnuIOImportCSVItem->triggered().connect(this, &Ms::Widgets::MTableViewWidget::popMnuIOImportCSVItemTriggered);

    m_popMnuIOExportCSVItem = m_popMnuIO->addItem("Export CSV");
    m_popMnuIOExportCSVItem->triggered().connect(this, &Ms::Widgets::MTableViewWidget::popMnuIOExportCSVItemTriggered);

    m_cmbFilterColumn = new Wt::WComboBox();
    m_cmbFilterColumn->setMinimumSize(220, 30);
    m_cmbFilterColumn->setMaximumSize(220, 30);
    m_cmbFilterColumn->changed().connect(this, &Ms::Widgets::MTableViewWidget::cmbFilterColumnChanged);
    m_mdlCmbFilterColumn = new Wt::WStandardItemModel();
    m_cmbFilterColumn->setModel(m_mdlCmbFilterColumn);
    layCntTbGlobal->addWidget(m_cmbFilterColumn);

    m_lnFilter = new Wt::WLineEdit();
    m_lnFilter->setMinimumSize(220, 30);
    m_lnFilter->setMaximumSize(220, 30);
    m_lnFilter->setEmptyText("Filter");
    m_lnFilter->keyWentUp().connect(this, &Ms::Widgets::MTableViewWidget::lnFilterKeyWentUp);

    layCntTbGlobal-> addWidget(m_lnFilter);

    //Main Table View
    Wt::WVBoxLayout *layCntTblMain = new Wt::WVBoxLayout();
    layCntTblMain->setContentsMargins(0,0,0,0);
    layCntTblMain->setSpacing(0);

    Wt::WContainerWidget *cntTblMain = new Wt::WContainerWidget();
    cntTblMain->setLayout(layCntTblMain);
    cntTblMain->setAttributeValue("tabindex", "0");
    cntTblMain->keyWentDown().connect(this, &Ms::Widgets::MTableViewWidget::cntTblMainKeyWentDown);

    createMainTable();

    layCntTblMain->addWidget(m_table, 1);

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
    //m_sldRowHeight->setTickPosition(Wt::WSlider::TicksAbove);
    m_sldRowHeight->valueChanged().connect(this, &Ms::Widgets::MTableViewWidget::sldRowHeightValueChanged);

    layTbMain->addWidget(m_sldRowHeight);

    //Status bar
    m_cntStatusBar = new Wt::WContainerWidget();

    layCntMainView->addWidget(m_cntStatusBar);

    m_layCntStatusBar = new Wt::WHBoxLayout();
    m_layCntStatusBar->setContentsMargins(4,0,4,0);
    m_layCntStatusBar->setSpacing(2);

    m_cntStatusBar->setLayout(m_layCntStatusBar);

    m_lblStatus = new Wt::WLabel("");

    m_layCntStatusBar->addWidget(m_lblStatus);
}
