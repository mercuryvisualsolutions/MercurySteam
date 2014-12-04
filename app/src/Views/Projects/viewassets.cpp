#include "viewassets.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/dbosession.h"
#include "../../Projects/projectsio.h"
#include "../Files/dlgfilesmanager.h"
#include "../../Log/logmanager.h"
#include "dlgcreateandeditasset.h"

#include <Wt/WApplication>

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewAssets::ViewAssets()
{
    _logger = Session::SessionManager::instance().logger();

    _prepareView();
}

Ms::Widgets::MQueryTableViewWidget<Projects::ProjectAsset> *Views::ViewAssets::qtvAssets() const
{
    return _qtvAssets;
}

void Views::ViewAssets::updateView(const std::vector<Wt::Dbo::ptr<Projects::Project>> &prjVec) const
{
    try
    {
        Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectAsset>> query = Session::SessionManager::instance().dboSession().find<Projects::ProjectAsset>();

        if(prjVec.size() > 0)
        {
            std::vector<std::string> projectsIdValues = Session::SessionManager::instance().dboSession().getDboQueryIdValues<Projects::Project>(prjVec);

            //generate the where clause
            query.where("Asset_Project_Project_Name IN (" + projectsIdValues.at(0) + ")");

            //only load active data if selected in settings
            if(!AppSettings::instance().isLoadInactiveData())
                query.where("Active = ?").bind(true);

            int viewRank = Session::SessionManager::instance().user()->viewRank();
            query.where("View_Rank <= ?").bind(viewRank);
        }
        else
            query.where("Asset_Name = ?").bind("");//clear the view

        _qtvAssets->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        _qtvAssets->clearColumns();

        //add columns
        _qtvAssets->addColumn(Ms::Widgets::MQueryTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(256, 160, "pics/NoPreviewBig.png"), false, true, 256));
        _qtvAssets->addColumn(Ms::Widgets::MQueryTableViewColumn("Asset_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvAssets->addColumn(Ms::Widgets::MQueryTableViewColumn("Asset_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvAssets->addColumn(Ms::Widgets::MQueryTableViewColumn("Asset_Type", "Type", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectAssetType>(
         &Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectAssetType>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectAssetType>().where("Active = ?").bind(true),
         "Type", editRank)));
        _qtvAssets->addColumn(Ms::Widgets::MQueryTableViewColumn("Start_Date", "Start Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvAssets->addColumn(Ms::Widgets::MQueryTableViewColumn("End_Date", "End Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvAssets->addColumn(Ms::Widgets::MQueryTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
        &Session::SessionManager::instance().dboSession(),AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true), "Status", editRank)));
        _qtvAssets->addColumn(Ms::Widgets::MQueryTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));
        _qtvAssets->addColumn(Ms::Widgets::MQueryTableViewColumn("Priority", "Priority", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));

        if(AppSettings::instance().isShowExtraColumns())
            _qtvAssets->addBaseColumns(flags, editRank);

        transaction.commit();

        _qtvAssets->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating assets view" << std::endl;
    }
}

bool Views::ViewAssets::isCreateOptionHidden()
{
    if(_btnCreateAsset)
        return _btnCreateAsset->isHidden();

    return false;
}

void Views::ViewAssets::setCreateOptionHidden(bool hidden) const
{
    if(_btnCreateAsset)
        _btnCreateAsset->setHidden(hidden);
}

bool Views::ViewAssets::isEditOptionHidden()
{
    if(_btnEditAssets)
        return _btnEditAssets->isHidden();

    return false;
}

void Views::ViewAssets::setEditOptionHidden(bool hidden) const
{
    if(_btnEditAssets)
        _btnEditAssets->setHidden(hidden);
}

//bool Views::ViewAssets::isRemoveOptionHidden()
//{
//    if(_btnRemoveAssets)
//        return _btnRemoveAssets->isHidden();

//    return false;
//}

//void Views::ViewAssets::setRemoveOptionHidden(bool hidden) const
//{
//    if(_btnRemoveAssets)
//        _btnRemoveAssets->setHidden(hidden);
//}

bool Views::ViewAssets::isImportThumbnailsOptionHidden()
{
    if(_btnImportThumbnails)
        return _btnImportThumbnails->isHidden();

    return false;
}

void Views::ViewAssets::setImportThumbnailsOptionHidden(bool hidden) const
{
    if(_btnImportThumbnails)
        _btnImportThumbnails->setHidden(hidden);
}

bool Views::ViewAssets::isOpenFilesOptionHidden()
{
    if(_btnOpenFilesView)
        return _btnOpenFilesView->isHidden();

    return false;
}

void Views::ViewAssets::setOpenFilesOptionHidden(bool hidden) const
{
    if(_btnOpenFilesView)
        _btnOpenFilesView->setHidden(hidden);
}

void Views::ViewAssets::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasViewFilesPriv = user->hasPrivilege("View Files");
    bool hasEditPriv = user->hasPrivilege("Edit");
    bool hasCreateProjectsPriv = user->hasPrivilege("Create Projects");
    bool hasCheckInPriv = user->hasPrivilege("Check In");
    bool hasCheckOutPriv = user->hasPrivilege("Check Out");
    bool hasCreateRepoPriv = user->hasPrivilege("Create Repositories");

    _btnCreateAsset->setHidden(!hasCreateProjectsPriv);
    _btnImportThumbnails->setHidden(!hasEditPriv);
    _btnEditAssets->setHidden(!hasEditPriv);

    _qtvAssets->setImportCSVFeatureEnabled(hasCreateProjectsPriv);

    bool showTaskFilesButton = hasViewFilesPriv || hasCheckInPriv || hasCheckOutPriv || hasCreateRepoPriv;//if have any of the privileges
    _btnOpenFilesView->setHidden(!showTaskFilesButton);
}

Wt::Signal<> &Views::ViewAssets::createAssetRequested()
{
    return _createAssetRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>>> &Views::ViewAssets::removeAssetsRequested()
{
    return _removeAssetsRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>>> &Views::ViewAssets::openfilesViewRequested()
{
    return _openfilesViewRequested;
}

Wt::Signal<> &Views::ViewAssets::importThumbnailsRequested()
{
    return _importThumbnailsRequested;
}

void Views::ViewAssets::_btnCreateAssetClicked()
{
    _createAssetRequested();
}

void Views::ViewAssets::_btnRemoveAssetsClicked()
{
    _removeAssetsRequested(_qtvAssets->selectedItems());
}

void Views::ViewAssets::_btnEditAssetsClicked()
{
    if(_qtvAssets->table()->selectedIndexes().size() == 0)
    {
        _logger->log("Please select at least one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditAsset *dlg = new Views::DlgCreateAndEditAsset(true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            for(auto assetPtr : _qtvAssets->selectedItems())
            {
                if(dlg->editedStartDate())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectAsset>(assetPtr)->setStartDate(dlg->startDate());

                if(dlg->editedEndDate())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectAsset>(assetPtr)->setEndDate(dlg->endDate());

                if(dlg->editedType())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectAsset>(assetPtr)->setType(dlg->assetType());

                if(dlg->editedPriority())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectAsset>(assetPtr)->setPriority(dlg->priority());

                if(dlg->editedStatus())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectAsset>(assetPtr)->setStatus(dlg->status());

                if(dlg->editedDescription())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectAsset>(assetPtr)->setDescription(dlg->description());

                if(dlg->editedActive())
                    Session::SessionManager::instance().dboSession().modifyDbo<Projects::ProjectAsset>(assetPtr)->setActive(dlg->isActive());
            }

            transaction.commit();

            _qtvAssets->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewAssets::_btnImportThumbnailsClicked()
{
    if(_qtvAssets->model()->rowCount() == 0)
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

                    //match thumbnail by asset name
                    //loop for all assets
                    for(int i = 0; i < _qtvAssets->model()->rowCount(); ++i)
                    {
                        Wt::Dbo::ptr<Projects::ProjectAsset> assetPtr = _qtvAssets->model()->resultRow(i);

                        if(assetPtr->name() == rawFileName)//asset has the same name of the thumbnail ?
                        {
                            if(assetPtr->active() == false && (!AppSettings::instance().isLoadInactiveData()))//don't change thumbnail for inactive asset
                                continue;

                            std::string assetDir = Projects::ProjectsIO::getAbsoluteAssetDir(assetPtr->projectName(), assetPtr->name());
                            std::string localFile = assetDir + Ms::IO::dirSeparator() + "thumbnails" + Ms::IO::dirSeparator() + pair.second;

                            if(Ms::IO::fileExists(localFile))//if thumbnail already exist
                                Ms::IO::removeFile(localFile);//delete it

                            if(Ms::IO::copyFile(pair.first, localFile))//copy and rename the file to the original name
                            {
                                assetPtr.modify()->setThumbnail(Projects::ProjectsIO::getRelativeAssetDir(assetPtr->projectName(), assetPtr->name()) +
                                        Ms::IO::dirSeparator() + "thumbnails" + Ms::IO::dirSeparator() + pair.second);
                            }

                            break;//asset mtching thumbnail name found, exit loop
                        }
                    }
                }
                catch(Wt::WException e)
                {
                    _logger->log(std::string("Error occured while trying to import thumbnails to table assets") + e.what(),
                                             Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
                }

                delFiles.push_back(pair.first);//cache it for later deletion
            }

            transaction.commit();

            for(std::vector<std::string>::size_type i = 0; i < delFiles.size(); ++i)
            {
                Ms::IO::removeFile(delFiles.at(i));//after finish processing, delete the uploaded thumbnails
                _logger->log(std::string("deleting thumbnail file") + delFiles.at(i), Ms::Log::LogMessageType::Info, Log::LogMessageContext::Server);
            }

            _qtvAssets->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));

    _importThumbnailsRequested();
}

void Views::ViewAssets::_btnOpenFilesViewClicked()
{
    if(_qtvAssets->table()->selectedIndexes().size() != 1)
    {
        _logger->log("Please select only one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    Wt::Dbo::ptr<Projects::ProjectAsset> assetPtr =  _qtvAssets->selectedItems().at(0);

    DlgFilesManager *dlgFiles = new DlgFilesManager(Projects::ProjectsIO::getRelativeAssetDir(assetPtr->projectName(), assetPtr->name()) + Ms::IO::dirSeparator() + "files");
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

    _openfilesViewRequested(_qtvAssets->selectedItems());
}

void Views::ViewAssets::_createAssetsTableView()
{
    _qtvAssets = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectAsset>(Session::SessionManager::instance().dboSession());
    _qtvAssets->setRowHeight(160);
    _qtvAssets->setDefaultFilterColumnIndex(1);
    _qtvAssets->setIgnoreNumFilterColumns(1);

    _btnCreateAsset = _qtvAssets->createToolButton("", "icons/Add.png", "Create A New Asset");
    _btnCreateAsset->clicked().connect(this, &Views::ViewAssets::_btnCreateAssetClicked);

    _qtvAssets->setImportCSVFeatureEnabled(true);

//    _btnRemoveAssets = _qtvAssets->createToolButton("", "icons/Remove.png", "Remove Selected Assets");
//    _btnRemoveAssets->clicked().connect(this, &Views::ViewAssets::_btnRemoveAssetsClicked);

    _btnImportThumbnails = _qtvAssets->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
    _btnImportThumbnails->clicked().connect(this, &Views::ViewAssets::_btnImportThumbnailsClicked);

    _btnEditAssets = _qtvAssets->createToolButton("", "icons/Edit.png", "Edit Selected Assets");
    _btnEditAssets->clicked().connect(this, &Views::ViewAssets::_btnEditAssetsClicked);

    _btnOpenFilesView = _qtvAssets->createToolButton("", "icons/Files.png", "Files Manager");
    _btnOpenFilesView->clicked().connect(this, &Views::ViewAssets::_btnOpenFilesViewClicked);
}

void Views::ViewAssets::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _createAssetsTableView();

    _layMain->addWidget(_qtvAssets);
}
