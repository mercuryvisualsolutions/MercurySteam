#include "viewassets.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewAssets::ViewAssets()
{
    _prepareView();
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectAsset> *Views::ViewAssets::qtvAssets() const
{
    return _qtvAssets;
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
    _importThumbnailsRequested();
}

void Views::ViewAssets::_btnOpenFilesViewClicked()
{
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
        Wt::WPushButton *btn = _qtvAssets->createToolButton("", "icons/Add.png", "Create A New Asset");
        btn->clicked().connect(this, &Views::ViewAssets::_btnCreateAssetClicked);

        _qtvAssets->setImportCSVFeatureEnabled(true);
    }
    else
        _qtvAssets->setImportCSVFeatureEnabled(false);

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //Wt::WPushButton *btn = _qtvAssets->createToolButton("", "icons/Remove.png", "Remove Selected Assets");
        //btn->clicked().connect(this, &Views::ViewAssets::_btnRemoveAssetClicked);
    }

    //requires "view" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        Wt::WPushButton *btn = _qtvAssets->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
        btn->clicked().connect(this, &Views::ViewAssets::_btnImportThumbnailsClicked);
    }

    //requires "CheckIn or CheckOut" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
    {
        Wt::WPushButton *btn = _qtvAssets->createToolButton("", "icons/Files.png", "Files Manager");
        btn->clicked().connect(this, &Views::ViewAssets::_btnOpenFilesViewClicked);
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
