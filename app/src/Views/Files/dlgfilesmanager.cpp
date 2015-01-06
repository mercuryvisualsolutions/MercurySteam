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
    m_rootPath(rootPath),
    m_isViewDisabled(true)
{
    m_logger = Session::SessionManager::instance().logger();

    m_absoluteRootPath = Ms::IO::absolutePath(AppSettings::instance().docRoot() + Ms::IO::dirSeparator() + m_rootPath);

    prepareView();
}

void Views::DlgFilesManager::btnCreateClicked()
{
    DlgCreateRepo *dlg = new DlgCreateRepo();
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            std::string path = AppSettings::instance().docRoot() + Ms::IO::dirSeparator() + m_trDirs->treeRoot()->objectName() + Ms::IO::dirSeparator() + dlg->text();

            if(Ms::IO::createPath(path))
            {
                refresh();
            }
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::DlgFilesManager::btnCheckInClicked()
{
    if(m_trDirs->selectedNodes().size() == 0 ||//no selection
            (*m_trDirs->selectedNodes().begin()) == m_trDirs->treeRoot() ||//selection is root node
            (*m_trDirs->selectedNodes().begin())->parentNode() != m_trDirs->treeRoot())//selection parent is not root node
        return;

    std::string path = AppSettings::instance().docRoot() + Ms::IO::dirSeparator() + (*m_trDirs->selectedNodes().begin())->objectName();

    Ms::Widgets::Dialogs::MFilesUploadDialog *dlg = new Ms::Widgets::Dialogs::MFilesUploadDialog(true, true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            std::vector<std::string> delFiles;//holds files for later deletion
            std::string newVerDirName = createNewVersionDir(path);

            for(const std::pair<std::string,std::string> &pair : dlg->uploadedFilesMap())
            {
                std::string localFile = newVerDirName + Ms::IO::dirSeparator() + pair.second;

                Ms::IO::copyFile(pair.first, localFile);//copy and rename the file to the original name

                delFiles.push_back(pair.first);//cache it for later deletion
            }

            for(std::vector<std::string>::size_type i = 0; i <delFiles.size(); ++i)
            {
                Ms::IO::removeFile(delFiles.at(i));//delete tmp files

                m_logger->log(std::string("deleting tmp file ") + delFiles.at(i), Ms::Log::LogMessageType::Info, Log::LogMessageContext::Server);
            }

            refresh();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
}

void Views::DlgFilesManager::btnCheckOutClicked()
{
    std::string url = generateDownloadUrl();
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
        m_downloadedFiles.push_back(file);
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

void Views::DlgFilesManager::btnRefreshClicked()
{
    refresh();
}

void Views::DlgFilesManager::btnViewClicked()
{
    if(m_tblFiles->table()->selectedIndexes().size() > 0)
    {
        viewItem(*m_tblFiles->table()->selectedIndexes().begin());
    }
}

void Views::DlgFilesManager::btnCloseClicked()
{
    if(isDownloadingFiles())
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
                closeDownloadedFiles();
                accept();//close the files manager dialog
            }

            delete dlg;//delete the messagebox dialog
        }));

        dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));;
    }
    else
        this->accept();
}

void Views::DlgFilesManager::trDirsItemSelectionChanged()
{
    if(m_trDirs->selectedNodes().size() != 1)
        return;

    m_tblFiles->clear();

    m_tblFiles->addColumn(Ms::Core::MTableViewColumn("Name"));
    m_tblFiles->addColumn(Ms::Core::MTableViewColumn("Size"));
    m_tblFiles->addColumn(Ms::Core::MTableViewColumn("Last Modified"));

    std::string path = AppSettings::instance().docRoot() + Ms::IO::dirSeparator() + (*m_trDirs->selectedNodes().begin())->objectName();

    std::vector<Ms::IO::MFileInfo> list = Ms::IO::dirFilesInfo(path);

    for(size_t i = 0; i < list.size(); ++i)
    {
        std::vector<Wt::WStandardItem*> items;

        Wt::WStandardItem *item = new Wt::WStandardItem(list.at(i).name());
        item->setIcon("icons/File.png");
        items.push_back(item);

        std::string size = formatSize(list.at(i).size());

        items.push_back(new Wt::WStandardItem(size.c_str()));
        items.push_back(new Wt::WStandardItem(list.at(i).lastModifiedDate().c_str()));

        m_tblFiles->model()->appendRow(items);
    }
}

void Views::DlgFilesManager::tblFilesItemDoubleClicked(Wt::WModelIndex index)
{
    if(!m_isViewDisabled)
        viewItem(index);
}

void Views::DlgFilesManager::viewItem(Wt::WModelIndex index)
{
    std::string dir = (*m_trDirs->selectedNodes().begin())->objectName();//only view first selected item

    std::string fileUrl = dir + Ms::IO::dirSeparator() + m_tblFiles->model()->item(index.row())->text().toUTF8();

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
    m_rootPath = rootPath;

    m_absoluteRootPath = Ms::IO::absolutePath(AppSettings::instance().docRoot() + Ms::IO::dirSeparator() + m_rootPath);
}

bool Views::DlgFilesManager::createDisabled()
{
    return m_btnCreate->isDisabled();
}

void Views::DlgFilesManager::setCreateDisabled(bool disabled)
{
    m_btnCreate->setDisabled(disabled);
}

bool Views::DlgFilesManager::checkInDisabled()
{
    return m_btnCheckIn->isDisabled();
}

void Views::DlgFilesManager::setCheckInDisabled(bool disabled)
{
    m_btnCheckIn->setDisabled(disabled);
}

bool Views::DlgFilesManager::checkOutDisabled()
{
    return m_btnCheckOut->isDisabled();
}

void Views::DlgFilesManager::setCheckOutDisabled(bool disabled)
{
    m_btnCheckOut->setDisabled(disabled);
}

bool Views::DlgFilesManager::viewDisabled()
{
    return m_isViewDisabled;
}

void Views::DlgFilesManager::setViewDisabled(bool disabled)
{
    m_isViewDisabled = disabled;

    m_btnView->setDisabled(disabled);
}

void Views::DlgFilesManager::createDirTree()
{
    m_trDirs = new Wt::WTree();
    m_trDirs->setSelectionMode(Wt::SingleSelection);
    m_trDirs->setMinimumSize(250, 350);

    m_trDirs->itemSelectionChanged().connect(this, &Views::DlgFilesManager::trDirsItemSelectionChanged);

    populateDirTree();
}

void Views::DlgFilesManager::populateDirTree()
{
    if(m_rootPath == "")
        return;

    Wt::WIconPair *folderIcon = new Wt::WIconPair("icons/FolderClosed.png", "icons/FolderOpen.png", false);

    Wt::WTreeNode *rootNode = new Wt::WTreeNode("Root", folderIcon);
    rootNode->setObjectName(m_rootPath);
    m_trDirs->setTreeRoot(rootNode);

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

void Views::DlgFilesManager::createFilesTable()
{
    m_tblFiles = new Ms::Widgets::MTableViewWidget();
    m_tblFiles->setImportCSVFeatureEnabled(false);
    m_tblFiles->table()->doubleClicked().connect(this, &Views::DlgFilesManager::tblFilesItemDoubleClicked);

    m_tblFiles->addColumn(Ms::Core::MTableViewColumn("Name"));
    m_tblFiles->addColumn(Ms::Core::MTableViewColumn("Size"));
    m_tblFiles->addColumn(Ms::Core::MTableViewColumn("Last Modified"));
}

std::string Views::DlgFilesManager::formatSize(u_int64_t size)
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

void Views::DlgFilesManager::refresh()
{
    m_tblFiles->clear();
    populateDirTree();
}

std::string Views::DlgFilesManager::createNewVersionDir(const std::string &path)
{
    int numVersions = Ms::IO::childDirCount(path);

    std::string newVerDirName = std::string(4 - std::to_string(numVersions).size(), '0') + std::to_string(numVersions);
    newVerDirName = path + Ms::IO::dirSeparator() + newVerDirName;

    if(Ms::IO::createDir(newVerDirName))
        return newVerDirName;

    return "";
}

std::string Views::DlgFilesManager::generateDownloadUrl()
{
    if(m_trDirs->selectedNodes().size() == 0 ||//no selection
            (*m_trDirs->selectedNodes().begin()) == m_trDirs->treeRoot() ||//selection is root node
            (*m_trDirs->selectedNodes().begin())->parentNode() == m_trDirs->treeRoot())//selection is a main repo folder
        return "";

    std::string orgDir = AppSettings::instance().docRoot() + Ms::IO::dirSeparator() + (*m_trDirs->selectedNodes().begin())->objectName();
    std::string tmpfilePath = getUniqueTmpFileName() + ".zip";
    std::string command = "zip -j -r \'" + tmpfilePath + "\'";

    if(m_tblFiles->table()->selectedIndexes().size() > 0)
    {
        for(const Wt::WModelIndex index : m_tblFiles->table()->selectedIndexes())
        {
            command = command + " \'" + orgDir + Ms::IO::dirSeparator() + m_tblFiles->model()->item(index.row())->text().toUTF8() + "\'";
        }
    }
    else command = command + " \'" + orgDir + Ms::IO::dirSeparator() + "\'";

    FILE *pipe = popen(command.c_str() , "r");

    if(!pipe)
        return "";

    pclose(pipe);

    return tmpfilePath;
}

std::string Views::DlgFilesManager::getUniqueTmpFileName()
{
    std::string currentUserTmpDir = Users::UsersIO::getUserTempDir(Session::SessionManager::instance().user()->name());

    int numFiles = Ms::IO::dirFilesCount(currentUserTmpDir);

    std::string newFileName = std::string(4 - std::to_string(numFiles).size(), '0') + std::to_string(numFiles);
    newFileName = currentUserTmpDir + Ms::IO::dirSeparator() + newFileName;

    return newFileName;
}

bool Views::DlgFilesManager::isDownloadingFiles()
{
    for(Ms::IO::MStreamedFileResource *file : m_downloadedFiles)
    {
        if(file->isInUse())
            return true;
    }

    return false;
}

void Views::DlgFilesManager::closeDownloadedFiles()
{
    for(Ms::IO::MStreamedFileResource *file : m_downloadedFiles)
    {
        std::string fileName = file->fileName();
        delete file;//delete the file pointer first

        if(file->deleteWhenFinished())
            remove(fileName.c_str());//then delete the actual file
    }

    m_downloadedFiles.clear();
}

void Views::DlgFilesManager::prepareView()
{
    this->setCaption("Files Manager");
    this->rejectWhenEscapePressed();
    this->setResizable(true);
    this->setMinimumSize(1024, 768);

    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);

    this->contents()->setLayout(m_layMain);
    this->contents()->setPadding(0);
    this->contents()->setOverflow(Wt::WContainerWidget::OverflowVisible);

    m_tbMain = new Wt::WToolBar();

    m_layCntTbMain = new Wt::WHBoxLayout();
    m_layCntTbMain->setContentsMargins(0,0,0,0);
    m_layCntTbMain->setSpacing(0);

    m_cntTbMain = new Wt::WContainerWidget();
    m_cntTbMain->setLayout(m_layCntTbMain);

    m_layCntTbMain->addWidget(m_tbMain);

    m_layMain->addWidget(m_cntTbMain);

    m_btnCreate = Ms::Widgets::MWidgetFactory::createButton("", "icons/Add.png", "Create");
    m_btnCreate->clicked().connect(this, &Views::DlgFilesManager::btnCreateClicked);
    m_tbMain->addButton(m_btnCreate);

    m_btnCheckIn = Ms::Widgets::MWidgetFactory::createButton("", "icons/CheckIn.png", "Check In");
    m_btnCheckIn->clicked().connect(this, &Views::DlgFilesManager::btnCheckInClicked);
    m_tbMain->addButton(m_btnCheckIn);

    m_btnCheckOut = Ms::Widgets::MWidgetFactory::createButton("", "icons/CheckOut.png", "Check Out");
    m_btnCheckOut->clicked().connect(this, &Views::DlgFilesManager::btnCheckOutClicked);
    m_tbMain->addButton(m_btnCheckOut);

    m_btnRefresh = Ms::Widgets::MWidgetFactory::createButton("", "icons/Reload.png", "Refresh");
    m_btnRefresh->clicked().connect(this, &Views::DlgFilesManager::btnRefreshClicked);
    m_tbMain->addButton(m_btnRefresh);

    m_btnView = Ms::Widgets::MWidgetFactory::createButton("", "icons/View.png", "View");
    m_btnView->clicked().connect(this, &Views::DlgFilesManager::btnViewClicked);
    m_tbMain->addButton(m_btnView);

    m_layFiles = new Wt::WHBoxLayout();
    m_layFiles->setContentsMargins(0,0,0,0);
    m_layFiles->setSpacing(0);

    m_cntFiles = new Wt::WContainerWidget();
    m_cntFiles->setLayout(m_layFiles);

    m_layMain->addWidget(m_cntFiles, 1);

    createDirTree();
    m_layFiles->addWidget(m_trDirs);

    createFilesTable();
    m_layFiles->addWidget(m_tblFiles, 1);

    m_btnClose = new Wt::WPushButton("Close", this->footer());
    m_btnClose->clicked().connect(this, &Views::DlgFilesManager::btnCloseClicked);
}
