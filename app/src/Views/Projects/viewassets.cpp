#include "viewassets.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"
#include "../../Projects/projectsio.h"
#include "../Files/dlgfilesmanager.h"
#include "../../Log/logmanager.h"

#include <Wt/WApplication>

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewAssets::ViewAssets()
{
    _logger = Log::LogManager::instance().getSessionLogger(Wt::WApplication::instance()->sessionId());

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
        if(!Database::DatabaseManager::instance().openTransaction())
            return;

        Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectAsset>> query = Database::DatabaseManager::instance().session()->find<Projects::ProjectAsset>();

        if(prjVec.size() > 0)
        {
            std::vector<std::string> projectsIdValues = Database::DatabaseManager::instance().getDboQueryIdValues<Projects::Project>(prjVec);

            //generate the where clause
            query.where("Asset_Project_Project_Name IN (" + projectsIdValues.at(0) + ")");

            //only load active data if selected in settings
            if(!AppSettings::instance().isLoadInactiveData())
                query.where("Active = ?").bind(true);

            int viewRank = Auth::AuthManager::instance().currentUser()->viewRank();
            query.where("View_Rank <= ?").bind(viewRank);
        }
        else
            query.where("Asset_Name = ?").bind("");//clear the view

        _qtvAssets->setQuery(query);

        bool canEdit = Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Auth::AuthManager::instance().currentUser()->editRank();

        _qtvAssets->clearColumns();

        //add columns
        _qtvAssets->addColumn(Ms::Widgets::MTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(256, 160, "pics/NoPreviewBig.png"), false, true, 256));
        _qtvAssets->addColumn(Ms::Widgets::MTableViewColumn("Asset_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvAssets->addColumn(Ms::Widgets::MTableViewColumn("Asset_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        _qtvAssets->addColumn(Ms::Widgets::MTableViewColumn("Asset_Type", "Type", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectAssetType>(
         Database::DatabaseManager::instance().session(),
         AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectAssetType>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectAssetType>().where("Active = ?").bind(true),
         "Type", editRank)));
        _qtvAssets->addColumn(Ms::Widgets::MTableViewColumn("Start_Date", "Start Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvAssets->addColumn(Ms::Widgets::MTableViewColumn("End_Date", "End Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        _qtvAssets->addColumn(Ms::Widgets::MTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
        Database::DatabaseManager::instance().session(),AppSettings::instance().isLoadInactiveData() ? Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>() :
         Database::DatabaseManager::instance().session()->find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true), "Status", editRank)));
        _qtvAssets->addColumn(Ms::Widgets::MTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));
        _qtvAssets->addColumn(Ms::Widgets::MTableViewColumn("Priority", "Priority", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));

        if(AppSettings::instance().isShowExtraColumns())
            _qtvAssets->addBaseColumns(flags, editRank);

        _qtvAssets->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating assets view" << std::endl;
    }
}

bool Views::ViewAssets::isCreateOptionHidden()
{
    return _btnCreateAsset->isHidden();
}

void Views::ViewAssets::setCreateOptionHidden(bool hidden) const
{
    _btnCreateAsset->setHidden(hidden);
}

//bool Views::ViewAssets::isRemoveOptionHidden()
//{
//    return _btnRemoveAssets->isHidden();
//}

//void Views::ViewAssets::setRemoveOptionHidden(bool hidden) const
//{
//    _btnRemoveAssets->setHidden(hidden);
//}

bool Views::ViewAssets::isImportThumbnailsOptionHidden()
{
    return _btnImportThumbnails->isHidden();
}

void Views::ViewAssets::setImportThumbnailsOptionHidden(bool hidden) const
{
    _btnImportThumbnails->setHidden(hidden);
}

bool Views::ViewAssets::isOpenFilesOptionHidden()
{
    return _btnOpenFilesView->isHidden();
}

void Views::ViewAssets::setOpenFilesOptionHidden(bool hidden) const
{
    _btnOpenFilesView->setHidden(hidden);
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

void Views::ViewAssets::_btnImportThumbnailsClicked()
{
    if(_qtvAssets->model()->rowCount() == 0)
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

                //match thumbnail by asset name
                if(!Database::DatabaseManager::instance().openTransaction())
                    return;

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

        for(std::vector<std::string>::size_type i = 0; i < delFiles.size(); ++i)
        {
            Ms::IO::removeFile(delFiles.at(i));//after finish processing, delete the uploaded thumbnails
            _logger->log(std::string("deleting thumbnail file") + delFiles.at(i), Ms::Log::LogMessageType::Info, Log::LogMessageContext::Server);
        }

        _qtvAssets->updateView();

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

    Wt::Dbo::ptr<Projects::ProjectAsset> assetPtr =  _qtvAssets->selectedItems().at(0);

    DlgFilesManager *dlg = new DlgFilesManager(Projects::ProjectsIO::getRelativeAssetDir(assetPtr->projectName(), assetPtr->name()) + Ms::IO::dirSeparator() + "files");
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

    _openfilesViewRequested(_qtvAssets->selectedItems());
}

void Views::ViewAssets::_createAssetsTableView()
{
    _qtvAssets = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectAsset>(&Database::DatabaseManager::instance());
    _qtvAssets->setRowHeight(160);
    _qtvAssets->setDefaultFilterColumnIndex(1);
    _qtvAssets->setIgnoreNumFilterColumns(1);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        _btnCreateAsset = _qtvAssets->createToolButton("", "icons/Add.png", "Create A New Asset");
        _btnCreateAsset->clicked().connect(this, &Views::ViewAssets::_btnCreateAssetClicked);

        _qtvAssets->setImportCSVFeatureEnabled(true);
    }
    else
        _qtvAssets->setImportCSVFeatureEnabled(false);

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //_btnRemoveAssets = _qtvAssets->createToolButton("", "icons/Remove.png", "Remove Selected Assets");
        //_btnRemoveAssets->clicked().connect(this, &Views::ViewAssets::_btnRemoveAssetsClicked);
    }

    //requires "view" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        _btnImportThumbnails = _qtvAssets->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
        _btnImportThumbnails->clicked().connect(this, &Views::ViewAssets::_btnImportThumbnailsClicked);
    }

    //requires "CheckIn or CheckOut" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
    {
        _btnOpenFilesView = _qtvAssets->createToolButton("", "icons/Files.png", "Files Manager");
        _btnOpenFilesView->clicked().connect(this, &Views::ViewAssets::_btnOpenFilesViewClicked);
    }
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
