#include "viewshots.h"

#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewShots::ViewShots()
{
    _prepareView();
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectShot> *Views::ViewShots::qtvShots() const
{
    return _qtvShots;
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

void Views::ViewShots::_btnImportThumbnailsClicked()
{
    _importThumbnailsRequested();
}

void Views::ViewShots::_btnOpenFilesViewClicked()
{
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
        Wt::WPushButton *btn = _qtvShots->createToolButton("", "icons/Add.png", "Create A New Shot");
        btn->clicked().connect(this, &Views::ViewShots::_btnCreateShotClicked);

        _qtvShots->setImportCSVFeatureEnabled(true);
    }
    else
        _qtvShots->setImportCSVFeatureEnabled(false);

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //Wt::WPushButton *btn = _qtvShots->createToolButton("", "icons/Remove.png", "Remove Selected Shots");
        //btn->clicked().connect(this, &Views::ViewShots::_btnRemoveShotClicked);
    }

    //requires "view" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        Wt::WPushButton *btn = _qtvShots->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
        btn->clicked().connect(this, &Views::ViewShots::_btnImportThumbnailsClicked);
    }

    //requires "CheckIn or CheckOut" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
    {
        Wt::WPushButton *btn = _qtvShots->createToolButton("", "icons/Files.png", "Files Manager");
        btn->clicked().connect(this, &Views::ViewShots::_btnOpenFilesViewClicked);
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
