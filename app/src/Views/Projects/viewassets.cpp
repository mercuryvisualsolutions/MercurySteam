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
    m_logger = Session::SessionManager::instance().logger();

    prepareView();
}

Ms::Widgets::MQueryTableViewWidget<Projects::ProjectAsset> *Views::ViewAssets::qtvAssets() const
{
    return m_qtvAssets;
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

        m_qtvAssets->setQuery(query);

        bool canEdit = Session::SessionManager::instance().user()->hasPrivilege("Edit");
        Wt::WFlags<Wt::ItemFlag> flags;
        if(canEdit)
            flags = Wt::ItemIsSelectable | Wt::ItemIsEditable;
        else
            flags = Wt::ItemIsSelectable;

        int editRank = Session::SessionManager::instance().user()->editRank();

        m_qtvAssets->clearColumns();

        //add columns
        m_qtvAssets->addColumn(Ms::Widgets::MQueryTableViewColumn("Thumbnail", "Thumbnail", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MThumbnailDelegate(256, 160, "pics/NoPreviewBig.png"), false, true, 256));
        m_qtvAssets->addColumn(Ms::Widgets::MQueryTableViewColumn("Asset_Name", "Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        m_qtvAssets->addColumn(Ms::Widgets::MQueryTableViewColumn("Asset_Project_Project_Name", "Project Name", Wt::ItemIsSelectable, new Ms::Widgets::Delegates::MItemDelegate(editRank), true));
        m_qtvAssets->addColumn(Ms::Widgets::MQueryTableViewColumn("Asset_Type", "Type", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectAssetType>(
         &Session::SessionManager::instance().dboSession(),
         AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectAssetType>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectAssetType>().where("Active = ?").bind(true),
         "Type", editRank)));
        m_qtvAssets->addColumn(Ms::Widgets::MQueryTableViewColumn("Start_Date", "Start Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        m_qtvAssets->addColumn(Ms::Widgets::MQueryTableViewColumn("End_Date", "End Date", flags, new Ms::Widgets::Delegates::MDateDelegate(editRank)));
        m_qtvAssets->addColumn(Ms::Widgets::MQueryTableViewColumn("Current_Status", "Status", flags, new Ms::Widgets::Delegates::MQueryComboBoxDelegate<Projects::ProjectWorkStatus>(
        &Session::SessionManager::instance().dboSession(),AppSettings::instance().isLoadInactiveData() ? Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>() :
         Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true), "Status", editRank)));
        m_qtvAssets->addColumn(Ms::Widgets::MQueryTableViewColumn("Description", "Description", flags, new Ms::Widgets::Delegates::MItemDelegate(editRank)));
        m_qtvAssets->addColumn(Ms::Widgets::MQueryTableViewColumn("Priority", "Priority", flags, new Ms::Widgets::Delegates::MIntFieldDelegate(editRank), false));

        if(AppSettings::instance().isShowExtraColumns())
            m_qtvAssets->addBaseColumns(flags, editRank);

        transaction.commit();

        m_qtvAssets->updateView();
    }
    catch(...)
    {
        std::cerr << "Exception occured while updating assets view" << std::endl;
    }
}

bool Views::ViewAssets::isCreateOptionHidden()
{
    if(m_btnCreateAsset)
        return m_btnCreateAsset->isHidden();

    return false;
}

void Views::ViewAssets::setCreateOptionHidden(bool hidden) const
{
    if(m_btnCreateAsset)
        m_btnCreateAsset->setHidden(hidden);
}

bool Views::ViewAssets::isEditOptionHidden()
{
    if(m_btnEditAssets)
        return m_btnEditAssets->isHidden();

    return false;
}

void Views::ViewAssets::setEditOptionHidden(bool hidden) const
{
    if(m_btnEditAssets)
        m_btnEditAssets->setHidden(hidden);
}

//bool Views::ViewAssets::isRemoveOptionHidden()
//{
//    if(m_btnRemoveAssets)
//        return m_btnRemoveAssets->isHidden();

//    return false;
//}

//void Views::ViewAssets::setRemoveOptionHidden(bool hidden) const
//{
//    if(m_btnRemoveAssets)
//        m_btnRemoveAssets->setHidden(hidden);
//}

bool Views::ViewAssets::isImportThumbnailsOptionHidden()
{
    if(m_btnImportThumbnails)
        return m_btnImportThumbnails->isHidden();

    return false;
}

void Views::ViewAssets::setImportThumbnailsOptionHidden(bool hidden) const
{
    if(m_btnImportThumbnails)
        m_btnImportThumbnails->setHidden(hidden);
}

bool Views::ViewAssets::isOpenFilesOptionHidden()
{
    if(m_btnOpenFilesView)
        return m_btnOpenFilesView->isHidden();

    return false;
}

void Views::ViewAssets::setOpenFilesOptionHidden(bool hidden) const
{
    if(m_btnOpenFilesView)
        m_btnOpenFilesView->setHidden(hidden);
}

void Views::ViewAssets::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasViewFilesPriv = user->hasPrivilege("View Files");
    bool hasEditPriv = user->hasPrivilege("Edit");
    bool hasCreateProjectsPriv = user->hasPrivilege("Create Projects");
    bool hasCheckInPriv = user->hasPrivilege("Check In");
    bool hasCheckOutPriv = user->hasPrivilege("Check Out");
    bool hasCreateRepoPriv = user->hasPrivilege("Create Repositories");

    m_btnCreateAsset->setHidden(!hasCreateProjectsPriv);
    m_btnImportThumbnails->setHidden(!hasEditPriv);
    m_btnEditAssets->setHidden(!hasEditPriv);

    m_qtvAssets->setImportCSVFeatureEnabled(hasCreateProjectsPriv);

    bool showTaskFilesButton = hasViewFilesPriv || hasCheckInPriv || hasCheckOutPriv || hasCreateRepoPriv;//if have any of the privileges
    m_btnOpenFilesView->setHidden(!showTaskFilesButton);
}

Wt::Signal<> &Views::ViewAssets::createAssetRequested()
{
    return m_createAssetRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>>> &Views::ViewAssets::removeAssetsRequested()
{
    return m_removeAssetsRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>>> &Views::ViewAssets::openfilesViewRequested()
{
    return m_openfilesViewRequested;
}

Wt::Signal<> &Views::ViewAssets::importThumbnailsRequested()
{
    return m_importThumbnailsRequested;
}

void Views::ViewAssets::btnCreateAssetClicked()
{
    createAssetRequested();
}

void Views::ViewAssets::btnRemoveAssetsClicked()
{
    m_removeAssetsRequested(m_qtvAssets->selectedItems());
}

void Views::ViewAssets::btnEditAssetsClicked()
{
    if(m_qtvAssets->table()->selectedIndexes().size() == 0)
    {
        m_logger->log("Please select at least one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Views::DlgCreateAndEditAsset *dlg = new Views::DlgCreateAndEditAsset(true);
    dlg->finished().connect(std::bind([=]()
    {
        if(dlg->result() == Wt::WDialog::Accepted)
        {
            Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

            for(auto assetPtr : m_qtvAssets->selectedItems())
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

            m_qtvAssets->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));
}

void Views::ViewAssets::btnImportThumbnailsClicked()
{
    if(m_qtvAssets->model()->rowCount() == 0)
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
                    for(int i = 0; i < m_qtvAssets->model()->rowCount(); ++i)
                    {
                        Wt::Dbo::ptr<Projects::ProjectAsset> assetPtr = m_qtvAssets->model()->resultRow(i);

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
                    m_logger->log(std::string("Error occured while trying to import thumbnails to table assets") + e.what(),
                                             Ms::Log::LogMessageType::Error, Log::LogMessageContext::ServerAndClient);
                }

                delFiles.push_back(pair.first);//cache it for later deletion
            }

            transaction.commit();

            for(std::vector<std::string>::size_type i = 0; i < delFiles.size(); ++i)
            {
                Ms::IO::removeFile(delFiles.at(i));//after finish processing, delete the uploaded thumbnails
                m_logger->log(std::string("deleting thumbnail file") + delFiles.at(i), Ms::Log::LogMessageType::Info, Log::LogMessageContext::Server);
            }

            m_qtvAssets->updateView();
        }

        delete dlg;
    }));

    dlg->animateShow(Wt::WAnimation(Wt::WAnimation::AnimationEffect::Pop, Wt::WAnimation::TimingFunction::EaseInOut));

    m_importThumbnailsRequested();
}

void Views::ViewAssets::btnOpenFilesViewClicked()
{
    if(m_qtvAssets->table()->selectedIndexes().size() != 1)
    {
        m_logger->log("Please select only one item.", Ms::Log::LogMessageType::Warning);

        return;
    }

    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    Wt::Dbo::ptr<Projects::ProjectAsset> assetPtr =  m_qtvAssets->selectedItems().at(0);

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

    m_openfilesViewRequested(m_qtvAssets->selectedItems());
}

void Views::ViewAssets::createAssetsTableView()
{
    m_qtvAssets = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectAsset>(Session::SessionManager::instance().dboSession());
    m_qtvAssets->setRowHeight(160);
    m_qtvAssets->setDefaultFilterColumnIndex(1);
    m_qtvAssets->setIgnoreNumFilterColumns(1);

    m_btnCreateAsset = m_qtvAssets->createToolButton("", "icons/Add.png", "Create A New Asset");
    m_btnCreateAsset->clicked().connect(this, &Views::ViewAssets::btnCreateAssetClicked);

    m_qtvAssets->setImportCSVFeatureEnabled(true);

//    m_btnRemoveAssets = m_qtvAssets->createToolButton("", "icons/Remove.png", "Remove Selected Assets");
//    m_btnRemoveAssets->clicked().connect(this, &Views::ViewAssets::btnRemoveAssetsClicked);

    m_btnImportThumbnails = m_qtvAssets->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
    m_btnImportThumbnails->clicked().connect(this, &Views::ViewAssets::btnImportThumbnailsClicked);

    m_btnEditAssets = m_qtvAssets->createToolButton("", "icons/Edit.png", "Edit Selected Assets");
    m_btnEditAssets->clicked().connect(this, &Views::ViewAssets::btnEditAssetsClicked);

    m_btnOpenFilesView = m_qtvAssets->createToolButton("", "icons/Files.png", "Files Manager");
    m_btnOpenFilesView->clicked().connect(this, &Views::ViewAssets::btnOpenFilesViewClicked);
}

void Views::ViewAssets::prepareView()
{
    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);

    setLayout(m_layMain);

    createAssetsTableView();

    m_layMain->addWidget(m_qtvAssets);
}
