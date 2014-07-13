#include "dlgfilesmanager.h"
#include "dlgcreaterepo.h"
#include "../../Users/usersmanager.h"
#include "../../Log/logmanager.h"

#include <Wt/WIconPair>
#include <Wt/WStandardItem>

#include <stack>

#include <Ms/IO/IO.h>
#include <Ms/Widgets/Dialogs/MFilesUploadDialog.h>

Views::DlgFilesManager::DlgFilesManager()
{
    _logger = Log::LogManager::instance().getLogger();

    _prepareView();
}

Views::DlgFilesManager::DlgFilesManager(const std::string &rootPath)
{
    setRootpath(rootPath);
    _prepareView();
}

Views::DlgFilesManager::~DlgFilesManager()
{
}

void Views::DlgFilesManager::_btnCreateClicked()
{
    DlgCreateRepo *dlg = new DlgCreateRepo();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            std::string path = _trDirs->treeRoot()->objectName() + Ms::IO::dirSeparator() + dlg->text();

            if(Ms::IO::createPath(path))
            {
                _refresh();
            }
        }

        delete dlg;
    }));

    dlg->show();
}

void Views::DlgFilesManager::_btnCheckInClicked()
{
    if(_trDirs->selectedNodes().size() == 0 ||//no selection
            (*_trDirs->selectedNodes().begin()) == _trDirs->treeRoot() ||//selection is root node
            (*_trDirs->selectedNodes().begin())->parentNode() != _trDirs->treeRoot())//selection parent is not root node
        return;

    std::string path = (*_trDirs->selectedNodes().begin())->objectName();

    Ms::Widgets::Dialogs::MFilesUploadDialog *dlg = new Ms::Widgets::Dialogs::MFilesUploadDialog(true, true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            std::vector<std::string> delFiles;//holds files for later deletion
            std::string newVerDirName = _createNewVersionDir(path);

            for(const std::pair<std::string,std::string> &pair : dlg->uploadedFilesMap())
            {
                std::string localFile = newVerDirName + Ms::IO::dirSeparator() + pair.second;

                Ms::IO::copyFile(pair.first, localFile);//copy and rename the file to the original name

                delFiles.push_back(pair.first);//cache it for later deletion
            }

            for(std::vector<std::string>::size_type i = 0; i <delFiles.size(); ++i)
            {
                Ms::IO::removeFile(delFiles.at(i));//delete tmp files

                _logger->log(std::string("deleting tmp file ") + delFiles.at(i), Ms::Log::LogMessageType::Info);
            }

            _refresh();
        }

        delete dlg;
    }));

    dlg->show();
}

//FIXME::fix the memory leak by deleting the "file" pointer
void Views::DlgFilesManager::_btnCheckOutClicked()
{
    std::string url = _generateDownloadUrl();
    if(url == "")
        return;

    bool linkClicked = false;

    Ms::IO::MStreamedFileResource *file = new Ms::IO::MStreamedFileResource(url, true, this);
    file->setMimeType("application/octet-stream");
    file->suggestFileName("download.zip");
    file->setDispositionType(Wt::WFileResource::Attachment);

    Wt::WDialog *dlg = new Wt::WDialog("Download Link");
    dlg->setClosable(true);

    Wt::WAnchor *anchor = new Wt::WAnchor(Wt::WLink(file), "Download");
    anchor->clicked().connect(std::bind([this, dlg, file, &linkClicked]()
    {
        linkClicked = true;
        _downloadedFiles.push_back(file);
        dlg->accept();
    }));

    dlg->contents()->addWidget(anchor);
    dlg->finished().connect(std::bind([=,&linkClicked]()
    {
        if(!linkClicked)
        {
            if(file->deleteWhenFinished())
                remove(url.c_str());

            delete file;
        }

        delete dlg;
    }));

    dlg->show();
}

void Views::DlgFilesManager::_btnRefreshClicked()
{
    _refresh();
}

void Views::DlgFilesManager::_btnCloseClicked()
{
    if(_isDownloadingFiles())
    {
        Wt::WDialog *dlg = new Wt::WDialog("Confirmation");
        dlg->rejectWhenEscapePressed();

        Wt::WLabel *lbl = new Wt::WLabel("Some downloads are still in progress, if you close the files manager dialog,\n"
                                           "while download still in progress you will probably end up downloading incomplete files.\n"
                                           "do you want to continue?");
        dlg->contents()->addWidget(lbl);

        Wt::WPushButton *btnOk = new Wt::WPushButton("OK", dlg->footer());
        btnOk->setDefault(true);
        btnOk->clicked().connect(dlg, &Wt::WDialog::accept);

        Wt::WPushButton *btnCancel = new Wt::WPushButton("Cancel", dlg->footer());
        btnCancel->clicked().connect(dlg, &Wt::WDialog::reject);

        dlg->finished().connect(std::bind([this, dlg]()
        {
            if(dlg->result() == Wt::WDialog::Accepted)
            {
                _closeDownloadedFiles();
                accept();//close the files manager dialog
            }

            delete dlg;//delete the messagebox dialog
        }));

        dlg->show();
    }
    else
        this->accept();
}

void Views::DlgFilesManager::_trDirsItemSelectionChanged()
{
    if(_trDirs->selectedNodes().size() != 1)
        return;

    _mdlTblFiles->clear();

    std::string path = (*_trDirs->selectedNodes().begin())->objectName();

    std::vector<Ms::IO::MFileInfo> list = Ms::IO::dirFilesInfo(path);

    for(size_t i = 0; i < list.size(); ++i)
    {
        Wt::WStandardItem *item = new Wt::WStandardItem(list.at(i).name());
        item->setIcon("icons/File.png");
        _mdlTblFiles->setItem(i, 0, item);

        std::string size = _formatSize(list.at(i).size());

        Wt::WStandardItem *itemSize = new Wt::WStandardItem(size.c_str());
        _mdlTblFiles->setItem(i, 1, itemSize);

        Wt::WStandardItem *itemLastModified = new Wt::WStandardItem(list.at(i).lastModifiedDate().c_str());
        _mdlTblFiles->setItem(i, 2, itemLastModified);
    }

    if(_mdlTblFiles->rowCount() > 0)
    {
        _mdlTblFiles->setHeaderData(0, Wt::Horizontal, Wt::WString("Name"), Wt::DisplayRole);
        _mdlTblFiles->setHeaderData(1, Wt::Horizontal, Wt::WString("Size"), Wt::DisplayRole);
        _mdlTblFiles->setHeaderData(2, Wt::Horizontal, Wt::WString("Last Modified"), Wt::DisplayRole);
    }
}

void Views::DlgFilesManager::setRootpath(const std::string &rootPath)
{
    _rootPath = rootPath;
}

bool Views::DlgFilesManager::createDisabled()
{
    return _btnCreate->isDisabled();
}

void Views::DlgFilesManager::setCreateDisabled(bool disabled)
{
    _btnCreate->setDisabled(disabled);
}

bool Views::DlgFilesManager::checkInDisabled()
{
    return _btnCheckIn->isDisabled();
}

void Views::DlgFilesManager::setCheckInDisabled(bool disabled)
{
    _btnCheckIn->setDisabled(disabled);
}

bool Views::DlgFilesManager::checkOutDisabled()
{
    return _btnCheckOut->isDisabled();
}

void Views::DlgFilesManager::setCheckOutDisabled(bool disabled)
{
    _btnCheckOut->setDisabled(disabled);
}

void Views::DlgFilesManager::_createDirTree()
{
    _trDirs = new Wt::WTree();
    _trDirs->setSelectionMode(Wt::SingleSelection);
    _trDirs->setMinimumSize(250, 350);

    _trDirs->itemSelectionChanged().connect(this, &Views::DlgFilesManager::_trDirsItemSelectionChanged);

    _populateDirTree();
}

void Views::DlgFilesManager::_populateDirTree()
{
    if(_rootPath == "")
        return;

    Wt::WIconPair *folderIcon = new Wt::WIconPair("icons/FolderClosed.png", "icons/FolderOpen.png", false);

    Wt::WTreeNode *rootNode = new Wt::WTreeNode("Root", folderIcon);
    rootNode->setObjectName(_rootPath);
    _trDirs->setTreeRoot(rootNode);

    std::stack<Wt::WTreeNode*> nodesStack;
    nodesStack.push(rootNode);

    while(nodesStack.size() > 0)
    {
        Wt::WTreeNode *currentNode = nodesStack.top();
        nodesStack.pop();

        std::vector<std::string> dirs = Ms::IO::dirChildren(currentNode->objectName());

        for(std::string &dir : dirs)
        {
            Wt::WIconPair *icon = new Wt::WIconPair("icons/FolderClosed.png", "icons/FolderOpen.png", false);
            Wt::WTreeNode *childNode = new Wt::WTreeNode(dir, icon);
            childNode->setObjectName(currentNode->objectName() + Ms::IO::dirSeparator() + dir);

            currentNode->addChildNode(childNode);

            nodesStack.push(childNode);
        }
    }

    rootNode->expand();
}

void Views::DlgFilesManager::_createFilesTable()
{
    _tblFiles = new Wt::WTableView();
    _tblFiles->setColumnResizeEnabled(true);
    _tblFiles->setAlternatingRowColors(true);
    _tblFiles->setRowHeight(24);
    _tblFiles->setHeaderHeight(24);
    _tblFiles->setSelectionMode(Wt::ExtendedSelection);

    _mdlTblFiles = new Wt::WStandardItemModel();
    _tblFiles->setModel(_mdlTblFiles);
}

std::string Views::DlgFilesManager::_formatSize(u_int64_t size)
{
    std::string strSize = "";

    if(size < 1024)//less than kilobyte
        strSize = std::to_string(size) + " B";
    else if(size < 1048576)//less than megabyte
        strSize = std::to_string(static_cast<float>(size / 1024.0f)) + " KiB";
    else if(size < 1073741824)//less than gigabyte
        strSize = std::to_string(static_cast<float>(size / 1048576.0f)) + " MiB";
    else//gigabyte
        strSize = std::to_string(static_cast<float>(size / 1073741824.0f)) + " GiB";

    return strSize;
}

void Views::DlgFilesManager::_refresh()
{
    _mdlTblFiles->clear();
    _populateDirTree();
}

std::string Views::DlgFilesManager::_createNewVersionDir(const std::string &path)
{
    int numVersions = Ms::IO::childDirCount(path);

    std::string newVerDirName = std::string(4 - std::to_string(numVersions).size(), '0') + std::to_string(numVersions);
    newVerDirName = path + Ms::IO::dirSeparator() + newVerDirName;

    if(Ms::IO::createDir(newVerDirName))
        return newVerDirName;

    return "";
}

std::string Views::DlgFilesManager::_generateDownloadUrl()
{
    if(_trDirs->selectedNodes().size() == 0 ||//no selection
            (*_trDirs->selectedNodes().begin()) == _trDirs->treeRoot() ||//selection is root node
            (*_trDirs->selectedNodes().begin())->parentNode() == _trDirs->treeRoot())//selection is a main repo folder
        return "";

    std::string orgDir = (*_trDirs->selectedNodes().begin())->objectName();
    std::string tmpfilePath = _getUniqueTmpFileName() + ".zip";
    std::string command = "zip -j -r \'" + tmpfilePath + "\'";

    if(_tblFiles->selectedIndexes().size() > 0)
    {
        for(const Wt::WModelIndex index : _tblFiles->selectedIndexes())
        {
            command = command + " \'" + orgDir + Ms::IO::dirSeparator() + _mdlTblFiles->item(index.row())->text().toUTF8() + "\'";
        }
    }
    else command = command + " \'" + orgDir + Ms::IO::dirSeparator() + "\'";

    FILE *pipe = popen(command.c_str() , "r");

    if(!pipe)
        return "";

    pclose(pipe);

    return tmpfilePath;
}

std::string Views::DlgFilesManager::_getUniqueTmpFileName()
{
    std::string currentUserTmpDir = Users::UsersManager::instance().getCurrentUserTmpDir();

    int numFiles = Ms::IO::dirFilesCount(currentUserTmpDir);

    std::string newFileName = std::string(4 - std::to_string(numFiles).size(), '0') + std::to_string(numFiles);
    newFileName = currentUserTmpDir + Ms::IO::dirSeparator() + newFileName;

    return newFileName;
}

bool Views::DlgFilesManager::_isDownloadingFiles()
{
    for(Ms::IO::MStreamedFileResource *file : _downloadedFiles)
    {
        if(file->isInUse())
            return true;
    }

    return false;
}

void Views::DlgFilesManager::_closeDownloadedFiles()
{
    for(Ms::IO::MStreamedFileResource *file : _downloadedFiles)
    {
        std::string fileName = file->fileName();
        delete file;//delete the file pointer first

        if(file->deleteWhenFinished())
            remove(fileName.c_str());//then delete the actual file
    }

    _downloadedFiles.clear();
}

void Views::DlgFilesManager::_prepareView()
{
    this->setCaption("Files Manager");
    this->rejectWhenEscapePressed();
    this->setResizable(true);
    this->setMinimumSize(800, 600);

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    this->contents()->setLayout(_layMain);
    this->contents()->setPadding(0);
    this->contents()->setOverflow(Wt::WContainerWidget::OverflowVisible);

    _tbMain = new Wt::WToolBar();

    _layCntTbMain = new Wt::WHBoxLayout();
    _layCntTbMain->setContentsMargins(0,0,0,0);
    _layCntTbMain->setSpacing(0);

    _cntTbMain = new Wt::WContainerWidget();
    _cntTbMain->setStyleClass("toolbar");
    _cntTbMain->setLayout(_layCntTbMain);

    _layCntTbMain->addWidget(_tbMain);

    _layMain->addWidget(_cntTbMain);

    _btnCreate = Ms::Widgets::MWidgetFactory::createButton("", "icons/Add.png", "Create");
    _btnCreate->clicked().connect(this, &Views::DlgFilesManager::_btnCreateClicked);
    _tbMain->addButton(_btnCreate);

    _btnCheckIn = Ms::Widgets::MWidgetFactory::createButton("", "icons/CheckIn.png", "Check In");
    _btnCheckIn->clicked().connect(this, &Views::DlgFilesManager::_btnCheckInClicked);
    _tbMain->addButton(_btnCheckIn);

    _btnCheckOut = Ms::Widgets::MWidgetFactory::createButton("", "icons/CheckOut.png", "Check Out");
    _btnCheckOut->clicked().connect(this, &Views::DlgFilesManager::_btnCheckOutClicked);
    _tbMain->addButton(_btnCheckOut);

    _btnRefresh = Ms::Widgets::MWidgetFactory::createButton("", "icons/Reload.png", "Refresh");
    _btnRefresh->clicked().connect(this, &Views::DlgFilesManager::_btnRefreshClicked);
    _tbMain->addButton(_btnRefresh);

    _layFiles = new Wt::WHBoxLayout();
    _layFiles->setContentsMargins(0,0,0,0);
    _layFiles->setSpacing(0);

    _cntFiles = new Wt::WContainerWidget();
    _cntFiles->setLayout(_layFiles);

    _layMain->addWidget(_cntFiles, 1);

    _createDirTree();
    _layFiles->addWidget(_trDirs);

    _createFilesTable();
    _layFiles->addWidget(_tblFiles, 1);

    _btnClose = new Wt::WPushButton("Close", this->footer());
    _btnClose->clicked().connect(this, &Views::DlgFilesManager::_btnCloseClicked);
}
