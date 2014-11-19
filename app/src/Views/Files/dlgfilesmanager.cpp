#include "dlgfilesmanager.h"
#include "dlgcreaterepo.h"
#include "../../Users/usersio.h"
#include "../../Log/logmanager.h"
#include "../../Settings/appsettings.h"

#include <Wt/WIconPair>
#include <Wt/WStandardItem>
#include <Wt/WApplication>
#include <Wt/WImage>
#include <Wt/WPainter>
#include <Wt/WMediaPlayer>
#include <Wt/WScrollArea>

#include <stack>

#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/jpeg_io.hpp>
#include <boost/gil/extension/io/png_io.hpp>

#include <Ms/IO/IO.h>
#include <Ms/Widgets/Dialogs/MFilesUploadDialog.h>

Views::DlgFilesManager::DlgFilesManager(const std::string &rootPath) :
    _rootPath(rootPath),
    _isViewDisabled(true)
{
    _logger = Session::SessionManager::instance().logger();

    _absoluteRootPath = Ms::IO::absolutePath(AppSettings::instance().docRoot() + Ms::IO::dirSeparator() + _rootPath);

    _prepareView();
}

void Views::DlgFilesManager::_btnCreateClicked()
{
    DlgCreateRepo *dlg = new DlgCreateRepo();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            std::string path = AppSettings::instance().docRoot() + Ms::IO::dirSeparator() + _trDirs->treeRoot()->objectName() + Ms::IO::dirSeparator() + dlg->text();

            if(Ms::IO::createPath(path))
            {
                _refresh();
            }
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::DlgFilesManager::_btnCheckInClicked()
{
    if(_trDirs->selectedNodes().size() == 0 ||//no selection
            (*_trDirs->selectedNodes().begin()) == _trDirs->treeRoot() ||//selection is root node
            (*_trDirs->selectedNodes().begin())->parentNode() != _trDirs->treeRoot())//selection parent is not root node
        return;

    std::string path = AppSettings::instance().docRoot() + Ms::IO::dirSeparator() + (*_trDirs->selectedNodes().begin())->objectName();

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

                _logger->log(std::string("deleting tmp file ") + delFiles.at(i), Ms::Log::LogMessageType::Info, Log::LogMessageContext::Server);
            }

            _refresh();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

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

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::DlgFilesManager::_btnRefreshClicked()
{
    _refresh();
}

void Views::DlgFilesManager::_btnViewClicked()
{
    if(_tblFiles->table()->selectedIndexes().size() > 0)
    {
        viewItem(*_tblFiles->table()->selectedIndexes().begin());
    }
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

        dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
    }
    else
        this->accept();
}

void Views::DlgFilesManager::_trDirsItemSelectionChanged()
{
    if(_trDirs->selectedNodes().size() != 1)
        return;

    _tblFiles->clear();

    _tblFiles->addColumn(Ms::Core::MTableViewColumn("Name"));
    _tblFiles->addColumn(Ms::Core::MTableViewColumn("Size"));
    _tblFiles->addColumn(Ms::Core::MTableViewColumn("Last Modified"));

    std::string path = AppSettings::instance().docRoot() + Ms::IO::dirSeparator() + (*_trDirs->selectedNodes().begin())->objectName();

    std::vector<Ms::IO::MFileInfo> list = Ms::IO::dirFilesInfo(path);

    for(size_t i = 0; i < list.size(); ++i)
    {
        std::vector<Wt::WStandardItem*> items;

        Wt::WStandardItem *item = new Wt::WStandardItem(list.at(i).name());
        item->setIcon("icons/File.png");
        items.push_back(item);

        std::string size = _formatSize(list.at(i).size());

        items.push_back(new Wt::WStandardItem(size.c_str()));
        items.push_back(new Wt::WStandardItem(list.at(i).lastModifiedDate().c_str()));

        _tblFiles->model()->appendRow(items);
    }
}

void Views::DlgFilesManager::tblFilesItemDoubleClicked(Wt::WModelIndex index)
{
    if(!_isViewDisabled)
        viewItem(index);
}

void Views::DlgFilesManager::viewItem(Wt::WModelIndex index)
{
    std::string dir = (*_trDirs->selectedNodes().begin())->objectName();//only view first selected item

    std::string fileUrl = dir + Ms::IO::dirSeparator() + _tblFiles->model()->item(index.row())->text().toUTF8();

    Ms::IO::MFileInfo file(fileUrl);

    Wt::WDialog *dlg = new Wt::WDialog("View");
    Wt::WVBoxLayout *layDlg = new Wt::WVBoxLayout();
    layDlg->setContentsMargins(0,0,0,0);
    dlg->contents()->setLayout(layDlg);
    dlg->contents()->setOverflow(Wt::WContainerWidget::OverflowVisible);
    dlg->contents()->setContentAlignment(Wt::AlignCenter);
    dlg->rejectWhenEscapePressed(true);
    dlg->finished().connect(std::bind([=]()
    {
        delete dlg;
    }));

    if(file.extension() == "jpg" || file.extension() == "png")//image
    {
        boost::gil::point2<std::ptrdiff_t> dim;

        if(file.extension() == "jpg")
            dim = boost::gil::jpeg_read_dimensions(AppSettings::instance().docRoot() + Ms::IO::dirSeparator() + file.fullName());
        else if(file.extension() == "png")
            dim = boost::gil::png_read_dimensions(AppSettings::instance().docRoot() + Ms::IO::dirSeparator() + file.fullName());

        Wt::WImage *img = new Wt::WImage(std::string(fileUrl));

        //scale the image to screen
        float scaleFactor = 1.0f;

        if(dim.y > dim.x)
        {
            if(dim.y > 800)
                scaleFactor = (800.0f / static_cast<float>(dim.y));
        }
        else
        {
            if(dim.x > 800)
                scaleFactor = (800.0f / static_cast<float>(dim.x));
        }

        img->setWidth(dim.x * scaleFactor);
        img->setHeight(dim.y * scaleFactor);

        dlg->contents()->setWidth((dim.x * scaleFactor) + 50);
        dlg->contents()->setHeight((dim.y * scaleFactor) + 50);

        layDlg->addWidget(img, 1);
    }
    else if(file.extension() == "mp3" || file.extension() == "wav")//audio
    {
        Wt::WMediaPlayer *player = new Wt::WMediaPlayer(Wt::WMediaPlayer::Audio);
        player->setTitle(file.name());

        if(file.extension() == "mp3")
            player->addSource(Wt::WMediaPlayer::MP3, Wt::WLink(fileUrl));
        else
            player->addSource(Wt::WMediaPlayer::WAV, Wt::WLink(fileUrl));

        layDlg->addWidget(player);

        player->play();
    }
    else if(file.extension() == "mp4" || file.extension() == "flv")//video
    {
        Wt::WMediaPlayer *player = new Wt::WMediaPlayer(Wt::WMediaPlayer::Video);
        player->setTitle(file.name());

        if(file.extension() == "mp4")
            player->addSource(Wt::WMediaPlayer::M4V, Wt::WLink(fileUrl));
        else
            player->addSource(Wt::WMediaPlayer::FLV, Wt::WLink(fileUrl));

        layDlg->addWidget(player);

        player->play();
    }

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Fade, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::DlgFilesManager::setRootpath(const std::string &rootPath)
{
    _rootPath = rootPath;

    _absoluteRootPath = Ms::IO::absolutePath(AppSettings::instance().docRoot() + Ms::IO::dirSeparator() + _rootPath);
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

bool Views::DlgFilesManager::viewDisabled()
{
    return _isViewDisabled;
}

void Views::DlgFilesManager::setViewDisabled(bool disabled)
{
    _isViewDisabled = disabled;

    _btnView->setDisabled(disabled);
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

        std::vector<std::string> dirs = Ms::IO::dirChildren(AppSettings::instance().docRoot() + Ms::IO::dirSeparator() + currentNode->objectName());

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
    _tblFiles = new Ms::Widgets::MTableViewWidget();
    _tblFiles->setImportCSVFeatureEnabled(false);
    _tblFiles->table()->doubleClicked().connect(this, &Views::DlgFilesManager::tblFilesItemDoubleClicked);

    _tblFiles->addColumn(Ms::Core::MTableViewColumn("Name"));
    _tblFiles->addColumn(Ms::Core::MTableViewColumn("Size"));
    _tblFiles->addColumn(Ms::Core::MTableViewColumn("Last Modified"));
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
    _tblFiles->clear();
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

    std::string orgDir = AppSettings::instance().docRoot() + Ms::IO::dirSeparator() + (*_trDirs->selectedNodes().begin())->objectName();
    std::string tmpfilePath = _getUniqueTmpFileName() + ".zip";
    std::string command = "zip -j -r \'" + tmpfilePath + "\'";

    if(_tblFiles->table()->selectedIndexes().size() > 0)
    {
        for(const Wt::WModelIndex index : _tblFiles->table()->selectedIndexes())
        {
            command = command + " \'" + orgDir + Ms::IO::dirSeparator() + _tblFiles->model()->item(index.row())->text().toUTF8() + "\'";
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
    std::string currentUserTmpDir = Users::UsersIO::getUserTempDir(Session::SessionManager::instance().user()->name());

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
    this->setMinimumSize(1024, 768);

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

    _btnView = Ms::Widgets::MWidgetFactory::createButton("", "icons/View.png", "View");
    _btnView->clicked().connect(this, &Views::DlgFilesManager::_btnViewClicked);
    _tbMain->addButton(_btnView);

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
