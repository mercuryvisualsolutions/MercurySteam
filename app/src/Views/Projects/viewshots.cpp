#include "viewshots.h"

#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/dbosession.h"
#include "../../Projects/projectsio.h"
#include "../Files/dlgfilesmanager.h"
#include "../../Log/logmanager.h"
#include "dlgcreateandeditshot.h"

#include <Wt/WApplication>

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewShots::ViewShots()
{
    m_logger = Session::SessionManager::instance().logger();

    prepareView();
}

Ms::Widgets::MQueryTableViewWidget<Projects::ProjectShot> *Views::ViewShots::qtvShots() const
{
    return m_qtvShots;
}

void Views::ViewShots::updateView(const std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>> &seqVec) const
{
    try
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectShot>> query = Session::SessionManager::instance().dboSession().find<Projects::ProjectShot>();

        if(seqVec.size() > 0)
        {
            std::vector<std::string> sequencesIdValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::ProjectSequence>(seqVec);

            std::string queryStr = "";
            //generate a where clause for projects
            queryStr = "Shot_Sequence_Sequence_Project_Project_Name IN (" + sequencesIdValues.at(1) + ")";

            if(seqVec.size() > 0)
                queryStr += " AND Shot_Sequence_Sequence_Name IN (" + sequencesIdValues.at(0) + ")";

            query.where(queryStr);

            //only load active data if selected in settings
            if(!AppSettings::instance().isLoadInactiveData())
                query.where("Active = ?").bind(true);

            int viewRank = Session::SessionManager::instance().user()->viewRank();
            query.where("View_Rank <= ?").bind(viewRank);
        }
        else
            query.where("Shot_Name = ?").bind("");//clear the view

        m_qtvShots->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        m_qtvShots->clearColumns();

        //add columns
        m_qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(256, 160, "pics/NoPreviewBig.png"), false, true, 256));
        m_qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Shot_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        m_qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Shot_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        m_qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Shot_Sequence_Sequence_Project_Project_Name", "Sequence Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        m_qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Start_Date", "Start Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        m_qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("End_Date", "End Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        m_qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Duration_In_Frames", "Duration In Frames", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        m_qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("FPS", "FPS", flags, new Ms::Widgets::Delegates::MFloatFieldDelegate(editRank)));
        m_qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Frame_Width", "Frame Width", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        m_qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Frame_Height", "Frame Height", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        m_qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
         &Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true),
         "Status", editRank)));
        m_qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));
        m_qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Priority", "Priority", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));

        if(AppSettings::instance().isShowExtraColumns())
            m_qtvShots->addBaseColumns(flags, editRank);

        transaction.commit();

        m_qtvShots->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating shots view" << std::endl;
    }
}

bool Views::ViewShots::isCreateOptionHidden()
{
    if(m_btnCreateShot)
        return m_btnCreateShot->isHidden();

    return false;
}

void Views::ViewShots::setCreateOptionHidden(bool hidden) const
{
    if(m_btnCreateShot)
        m_btnCreateShot->setHidden(hidden);
}

bool Views::ViewShots::isEditOptionHidden()
{
    if(m_btnEditShots)
        return m_btnEditShots->isHidden();

    return false;
}

void Views::ViewShots::setEditOptionHidden(bool hidden) const
{
    if(m_btnEditShots)
        m_btnEditShots->setHidden(hidden);
}

//bool Views::ViewShots::isRemoveOptionHidden()
//{
//    if(_btnRemoveShot)
//        return _btnRemoveShot->isHidden();

//    return false;
//}

//void Views::ViewShots::setRemoveOptionHidden(bool hidden) const
//{
//    if(_btnRemoveShot)
//        _btnRemoveShot->setHidden(hidden);
//}

bool Views::ViewShots::isImportThumbnailsOptionHidden()
{
    if(m_btnImportThumbnails)
        return m_btnImportThumbnails->isHidden();

    return false;
}

void Views::ViewShots::setImportThumbnailsOptionHidden(bool hidden) const
{
    if(m_btnImportThumbnails)
        m_btnImportThumbnails->setHidden(hidden);
}

bool Views::ViewShots::isOpenFilesOptionHidden()
{
    if(m_btnOpenFilesView)
        return m_btnOpenFilesView->isHidden();

    return false;
}

void Views::ViewShots::setOpenFilesOptionHidden(bool hidden) const
{
    if(m_btnOpenFilesView)
        m_btnOpenFilesView->setHidden(hidden);
}

void Views::ViewShots::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasViewFilesPriv = user->hasPrivilege("View Files");
    bool hasEditPriv = user->hasPrivilege("Edit");
    bool hasCreateProjectsPriv = user->hasPrivilege("Create Projects");
    bool hasCheckInPriv = user->hasPrivilege("Check In");
    bool hasCheckOutPriv = user->hasPrivilege("Check Out");
    bool hasCreateRepoPriv = user->hasPrivilege("Create Repositories");

    m_btnCreateShot->setHidden(!hasCreateProjectsPriv);
    m_btnImportThumbnails->setHidden(!hasEditPriv);
    m_btnEditShots->setHidden(!hasEditPriv);

    m_qtvShots->setImportCSVFeatureEnabled(hasCreateProjectsPriv);

    bool showTaskFilesButton = hasViewFilesPriv || hasCheckInPriv || hasCheckOutPriv || hasCreateRepoPriv;//if have any of the privileges
    m_btnOpenFilesView->setHidden(!showTaskFilesButton);
}

Wt::Signal<> &Views::ViewShots::createShotRequested()
{
    return m_createShotRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectShot>>> &Views::ViewShots::removeShotsRequested()
{
    return m_removeShotsRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectShot>>> &Views::ViewShots::openfilesViewRequested()
{
    return m_openfilesViewRequested;
}

Wt::Signal<> &Views::ViewShots::importThumbnailsRequested()
{
    return m_importThumbnailsRequested;
}

void Views::ViewShots::btnCreateShotClicked()
{
    createShotRequested();
}

void Views::ViewShots::btnRemoveShotsClicked()
{
    m_removeShotsRequested(m_qtvShots->selectedItems());
}

void Views::ViewShots::btnEditShotsClicked()
{
    if(m_qtvShots->table()->selectedIndexes().size() == 0)
    {
        m_logger->log("Please select at least one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditShot *dlg = new Views::DlgCreateAndEditShot(true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            for(auto shotPtr : m_qtvShots->selectedItems())
            {
                if(dlg->editedStartDate())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectShot>(shotPtr)->setStartDate(dlg->startDate());

                if(dlg->editedEndDate())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectShot>(shotPtr)->setEndDate(dlg->endDate());

                if(dlg->editedDuration())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectShot>(shotPtr)->setDurationInFrames(dlg->duration());

                if(dlg->editedFps())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectShot>(shotPtr)->setFps(dlg->fps());

                if(dlg->editedFrameWidth())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectShot>(shotPtr)->setFrameWidth(dlg->frameWidth());

                if(dlg->editedFrameHeight())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectShot>(shotPtr)->setFrameHeight(dlg->frameHeight());

                if(dlg->editedPriority())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectShot>(shotPtr)->setPriority(dlg->priority());

                if(dlg->editedStatus())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectShot>(shotPtr)->setStatus(dlg->status());

                if(dlg->editedDescription())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectShot>(shotPtr)->setDescription(dlg->description());

                if(dlg->editedActive())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectShot>(shotPtr)->setActive(dlg->isActive());
            }

            transaction.commit();

            m_qtvShots->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewShots::btnImportThumbnailsClicked()
{
    if(m_qtvShots->model()->rowCount() == 0)
        return;

    Ms::Widgets::Dialogs::MFilesUploadDialog *dlg = new Ms::Widgets::Dialogs::MFilesUploadDialog(true, true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            std::vector<std::string> delFiles;//holds thumbnails files for later deletion

            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            for(const std::pair<std::string,std::string> &pair : dlg->uploadedFilesMap())
            {
                try
                {
                    //get the original raw file name without the extension
                    int lastIndex = pair.second.find_last_of(".");
                    std::string rawFileName = pair.second.substr(0, lastIndex);

                    //match thumbnail by shot name
                    //loop for all shots
                    for(int i = 0; i < m_qtvShots->model()->rowCount(); ++i)
                    {
                        Wt::Dbo::ptr<Projects::ProjectShot> shotPtr = m_qtvShots->model()->resultRow(i);

                        if(shotPtr->name() == rawFileName)//shot has the same name of the thumbnail ?
                        {
                            if(shotPtr->active() == false && (!AppSettings::instance().isLoadInactiveData()))//don't change thumbnail for inactive shot
                                continue;

                            std::string shotDir = Projects::ProjectsIO::getAbsoluteShotDir(shotPtr->projectName(), shotPtr->sequenceName(), shotPtr->name());
                            std::string localFile = shotDir + Ms::IO::dirSeparator() + "thumbnails" + Ms::IO::dirSeparator() + pair.second;

                            if(Ms::IO::fileExists(localFile))//if thumbnail already exist
                                Ms::IO::removeFile(localFile);//delete it

                            if(Ms::IO::copyFile(pair.first, localFile))//copy and rename the file to the original name
                            {
                                Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectShot>(shotPtr)->setThumbnail(
                                            Projects::ProjectsIO::getRelativeShotDir(shotPtr->projectName(), shotPtr->sequenceName(), shotPtr->name()) +
                                        Ms::IO::dirSeparator() + "thumbnails" + Ms::IO::dirSeparator() + pair.second);
                            }

                            break;//shot mtching thumbnail name found, exit loop
                        }
                    }

                }
                catch(Wt::WException e)
                {
                    m_logger->log(std::string("Error occured while trying to import thumbnails to table shots") + e.what(),
                                             Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
                }

                delFiles.push_back(pair.first);//cache it for later deletion
            }

            transaction.commit();

            for(std::vector<std::string>::size_type i = 0; i <delFiles.size(); ++i)
            {
                Ms::IO::removeFile(delFiles.at(i));//after finish processing, delete the uploaded thumbnails

                m_logger->log(std::string("deleting thumbnail file") + delFiles.at(i), Ms::Log::LogMessageType::Info, Log::LogMessageContext::Server);
            }

            m_qtvShots->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));

    m_importThumbnailsRequested();
}

void Views::ViewShots::btnOpenFilesViewClicked()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    if(m_qtvShots->table()->selectedIndexes().size() != 1)
    {
        m_logger->log("Please select only one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Wt::Dbo::ptr<Projects::ProjectShot> shotPtr = m_qtvShots->selectedItems().at(0);

    DlgFilesManager *dlgFiles = new DlgFilesManager(Projects::ProjectsIO::getRelativeShotDir(shotPtr->projectName(), shotPtr->sequenceName(), shotPtr->name()) + Ms::IO::dirSeparator() + "files");
    dlgFiles->finished().connect(std::bind([=]()
    {
        delete dlgFiles;
    }));

    Wt::Dbo::ptr<Users::User> user = Session::SessionManager::instance().user();

    bool hasViewFilesPriv = user->hasPrivilege("View Files");
    bool hasCheckInPriv = user->hasPrivilege("Check In");
    bool hasCheckOutPriv = user->hasPrivilege("Check Out");
    bool hasCreateRepoPriv = user->hasPrivilege("Create Repositories");

    dlgFiles->setViewDisabled(!hasViewFilesPriv);
    dlgFiles->setCreateDisabled(!hasCreateRepoPriv);
    dlgFiles->setCheckInDisabled(!hasCheckInPriv);
    dlgFiles->setCheckOutDisabled(!hasCheckOutPriv);

    transaction.commit();

    dlgFiles->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));

    m_openfilesViewRequested(m_qtvShots->selectedItems());
}

void Views::ViewShots::createShotsTableView()
{
    m_qtvShots = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectShot>(Session::SessionManager::instance().dboSession());
    m_qtvShots->setRowHeight(160);
    m_qtvShots->setDefaultFilterColumnIndex(1);
    m_qtvShots->setIgnoreNumFilterColumns(1);

    m_btnCreateShot = m_qtvShots->createToolButton("", "icons/Add.png", "Create A New Shot");
    m_btnCreateShot->clicked().connect(this, &Views::ViewShots::btnCreateShotClicked);

    //m_btnRemoveShots = m_qtvShots->createToolButton("", "icons/Remove.png", "Remove Selected Shots");
    //m_btnRemoveShots->clicked().connect(this, &Views::ViewShots::_btnRemoveShotClicked);

    m_btnImportThumbnails = m_qtvShots->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
    m_btnImportThumbnails->clicked().connect(this, &Views::ViewShots::btnImportThumbnailsClicked);

    m_btnEditShots = m_qtvShots->createToolButton("", "icons/Edit.png", "Edit Selected Shots");
    m_btnEditShots->clicked().connect(this, &Views::ViewShots::btnEditShotsClicked);

    m_btnOpenFilesView = m_qtvShots->createToolButton("", "icons/Files.png", "Files Manager");
    m_btnOpenFilesView->clicked().connect(this, &Views::ViewShots::btnOpenFilesViewClicked);
}

void Views::ViewShots::prepareView()
{
    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);

    setLayout(m_layMain);

    createShotsTableView();

    m_layMain->addWidget(m_qtvShots);
}
