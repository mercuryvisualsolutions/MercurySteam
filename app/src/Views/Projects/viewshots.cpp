#include "viewshots.h"

#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"
#include "../../Projects/projectsio.h"
#include "../Files/dlgfilesmanager.h"
#include "../../Log/logmanager.h"
#include "dlgcreateandeditshot.h"

#include <Wt/WApplication>

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewShots::ViewShots()
{
    _logger = Log::LogManager::instance().getSessionLogger(Wt::WApplication::instance()->sessionId());

    _prepareView();
}

Ms::Widgets::MQueryTableViewWidget<Projects::ProjectShot> *Views::ViewShots::qtvShots() const
{
    return _qtvShots;
}

void Views::ViewShots::updateView(const std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>> &seqVec) const
{
    try
    {
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectShot>> query = Database::DatabaseManager::instance().session()->find<Projects::ProjectShot>();

        if(seqVec.size() > 0)
        {
            std::vector<std::string> sequencesIdValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::ProjectSequence>(seqVec);

            std::string queryStr = "";
            //generate a where clause for projects
            queryStr = "Shot_Sequence_Sequence_Project_Project_Name IN (" + sequencesIdValues.at(1) + ")";

            if(seqVec.size() > 0)
                queryStr += " AND Shot_Sequence_Sequence_Name IN (" + sequencesIdValues.at(0) + ")";

            query.where(queryStr);

            //only load active data if selected in settings
            if(!AppSettings::instance().isLoadInactiveData())
                query.where("Active = ?").bind(true);

            int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();
            query.where("View_Rank <= ?").bind(viewRank);
        }
        else
            query.where("Shot_Name = ?").bind("");//clear the view

        _qtvShots->setQuery(query);

        bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Auth::AuthManager::instance().currentUser()->editRank();

        _qtvShots->clearColumns();

        //add columns
        _qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(256, 160, "pics/NoPreviewBig.png"), false, true, 256));
        _qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Shot_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Shot_Sequence_Sequence_Name", "Sequence Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Shot_Sequence_Sequence_Project_Project_Name", "Sequence Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Start_Date", "Start Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("End_Date", "End Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Duration_In_Frames", "Duration In Frames", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("FPS", "FPS", flags, new Ms::Widgets::Delegates::MFloatFieldDelegate(editRank)));
        _qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Frame_Width", "Frame Width", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Frame_Height", "Frame Height", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank)));
        _qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
         Database::DatabaseManager::instance().session(),
         AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true),
         "Status", editRank)));
        _qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));
        _qtvShots->addColumn(Ms::Widgets::MQueryTableViewColumn("Priority", "Priority", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));

        if(AppSettings::instance().isShowExtraColumns())
            _qtvShots->addBaseColumns(flags, editRank);

        _qtvShots->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating shots view" << std::endl;
    }
}

bool Views::ViewShots::isCreateOptionHidden()
{
    return _btnCreateShot->isHidden();
}

void Views::ViewShots::setCreateOptionHidden(bool hidden) const
{
    _btnCreateShot->setHidden(hidden);
}

bool Views::ViewShots::isEditOptionHidden()
{
    return _btnEditShots->isHidden();
}

void Views::ViewShots::setEditOptionHidden(bool hidden) const
{
    _btnEditShots->setHidden(hidden);
}

//bool Views::ViewShots::isRemoveOptionHidden()
//{
//    return _btnRemoveShot->isHidden();
//}

//void Views::ViewShots::setRemoveOptionHidden(bool hidden) const
//{
//    _btnRemoveShot->setHidden(hidden);
//}

bool Views::ViewShots::isImportThumbnailsOptionHidden()
{
    return _btnImportThumbnails->isHidden();
}

void Views::ViewShots::setImportThumbnailsOptionHidden(bool hidden) const
{
    _btnImportThumbnails->setHidden(hidden);
}

bool Views::ViewShots::isOpenFilesOptionHidden()
{
    return _btnOpenFilesView->isHidden();
}

void Views::ViewShots::setOpenFilesOptionHidden(bool hidden) const
{
    _btnOpenFilesView->setHidden(hidden);
}

Wt::Signal<> &Views::ViewShots::createShotRequested()
{
    return _createShotRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectShot>>> &Views::ViewShots::removeShotsRequested()
{
    return _removeShotsRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectShot>>> &Views::ViewShots::openfilesViewRequested()
{
    return _openfilesViewRequested;
}

Wt::Signal<> &Views::ViewShots::importThumbnailsRequested()
{
    return _importThumbnailsRequested;
}

void Views::ViewShots::_btnCreateShotClicked()
{
    _createShotRequested();
}

void Views::ViewShots::_btnRemoveShotsClicked()
{
    _removeShotsRequested(_qtvShots->selectedItems());
}

void Views::ViewShots::_btnEditShotsClicked()
{
    if(_qtvShots->table()->selectedIndexes().size() == 0)
    {
        _logger->log("Please select at least one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditShot *dlg = new Views::DlgCreateAndEditShot(true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            for(auto shotPtr : _qtvShots->selectedItems())
            {
                if(dlg->editedStartDate())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectShot>(shotPtr)->setStartDate(dlg->startDate());

                if(dlg->editedEndDate())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectShot>(shotPtr)->setEndDate(dlg->endDate());

                if(dlg->editedDuration())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectShot>(shotPtr)->setDurationInFrames(dlg->duration());

                if(dlg->editedFps())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectShot>(shotPtr)->setFps(dlg->fps());

                if(dlg->editedFrameWidth())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectShot>(shotPtr)->setFrameWidth(dlg->frameWidth());

                if(dlg->editedFrameHeight())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectShot>(shotPtr)->setFrameHeight(dlg->frameHeight());

                if(dlg->editedPriority())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectShot>(shotPtr)->setPriority(dlg->priority());

                if(dlg->editedStatus())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectShot>(shotPtr)->setStatus(dlg->status());

                if(dlg->editedDescription())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectShot>(shotPtr)->setDescription(dlg->description());

                if(dlg->editedActive())
                    Database::DatabaseManager::instance().modifyDbo<Projects::ProjectShot>(shotPtr)->setActive(dlg->isActive());
            }

            _qtvShots->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewShots::_btnImportThumbnailsClicked()
{
    if(_qtvShots->model()->rowCount() == 0)
        return;

    Ms::Widgets::Dialogs::MFilesUploadDialog *dlg = new Ms::Widgets::Dialogs::MFilesUploadDialog(true, true);
    dlg->finished().connect(std::bind([=]()
    {
        std::vector<std::string> delFiles;//holds thumbnails files for later deletion
        for(const std::pair<std::string,std::string> &pair : dlg->uploadedFilesMap())
        {
            try
            {
                //get the original raw file name without the extension
                int lastIndex = pair.second.find_last_of(".");
                std::string rawFileName = pair.second.substr(0, lastIndex);

                //match thumbnail by shot name
                if(!Database::DatabaseManager::instance().openTransaction())
                    return;

                //loop for all shots
                for(int i = 0; i < _qtvShots->model()->rowCount(); ++i)
                {
                    Wt::Dbo::ptr<Projects::ProjectShot> shotPtr = _qtvShots->model()->resultRow(i);

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
                            Database::DatabaseManager::instance().modifyDbo<Projects::ProjectShot>(shotPtr)->setThumbnail(
                                        Projects::ProjectsIO::getRelativeShotDir(shotPtr->projectName(), shotPtr->sequenceName(), shotPtr->name()) +
                                    Ms::IO::dirSeparator() + "thumbnails" + Ms::IO::dirSeparator() + pair.second);
                        }

                        break;//shot mtching thumbnail name found, exit loop
                    }
                }
            }
            catch(Wt::WException e)
            {
                _logger->log(std::string("Error occured while trying to import thumbnails to table shots") + e.what(),
                                         Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
            }

            delFiles.push_back(pair.first);//cache it for later deletion
        }

        for(std::vector<std::string>::size_type i = 0; i <delFiles.size(); ++i)
        {
            Ms::IO::removeFile(delFiles.at(i));//after finish processing, delete the uploaded thumbnails

            _logger->log(std::string("deleting thumbnail file") + delFiles.at(i), Ms::Log::LogMessageType::Info, Log::LogMessageContext::Server);
        }

        _qtvShots->updateView();

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));

    _importThumbnailsRequested();
}

void Views::ViewShots::_btnOpenFilesViewClicked()
{
    if(_qtvShots->table()->selectedIndexes().size() != 1)
    {
        _logger->log("Please select only one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Wt::Dbo::ptr<Projects::ProjectShot> shotPtr = _qtvShots->selectedItems().at(0);

    DlgFilesManager *dlg = new DlgFilesManager(Projects::ProjectsIO::getRelativeShotDir(shotPtr->projectName(), shotPtr->sequenceName(), shotPtr->name()) + Ms::IO::dirSeparator() + "files");
    dlg->finished().connect(std::bind([=]()
    {
        delete dlg;
    }));

    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
        dlg->setCreateDisabled(true);
    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In"))
        dlg->setCheckInDisabled(true);
    if(!Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out"))
        dlg->setCheckOutDisabled(true);

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));

    _openfilesViewRequested(_qtvShots->selectedItems());
}

void Views::ViewShots::_createShotsTableView()
{
    _qtvShots = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectShot>(&Database::DatabaseManager::instance());
    _qtvShots->setRowHeight(160);
    _qtvShots->setDefaultFilterColumnIndex(1);
    _qtvShots->setIgnoreNumFilterColumns(1);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        _btnCreateShot = _qtvShots->createToolButton("", "icons/Add.png", "Create A New Shot");
        _btnCreateShot->clicked().connect(this, &Views::ViewShots::_btnCreateShotClicked);

        _qtvShots->setImportCSVFeatureEnabled(true);
    }
    else
        _qtvShots->setImportCSVFeatureEnabled(false);

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //_btnRemoveShots = _qtvShots->createToolButton("", "icons/Remove.png", "Remove Selected Shots");
        //_btnRemoveShots->clicked().connect(this, &Views::ViewShots::_btnRemoveShotClicked);
    }

    //requires "view" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        _btnImportThumbnails = _qtvShots->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
        _btnImportThumbnails->clicked().connect(this, &Views::ViewShots::_btnImportThumbnailsClicked);
    }

    //requires "view" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        _btnEditShots = _qtvShots->createToolButton("", "icons/Edit.png", "Edit Selected Shots");
        _btnEditShots->clicked().connect(this, &Views::ViewShots::_btnEditShotsClicked);
    }

    //requires "CheckIn or CheckOut" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
    {
        _btnOpenFilesView = _qtvShots->createToolButton("", "icons/Files.png", "Files Manager");
        _btnOpenFilesView->clicked().connect(this, &Views::ViewShots::_btnOpenFilesViewClicked);
    }
}

void Views::ViewShots::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _createShotsTableView();

    _layMain->addWidget(_qtvShots);
}
