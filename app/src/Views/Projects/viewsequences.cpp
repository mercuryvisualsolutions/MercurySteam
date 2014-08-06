#include "viewsequences.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewSequences::ViewSequences()
{
    _prepareView();
}

const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectSequence> *Views::ViewSequences::qtvSequences() const
{
    return _qtvSequences;
}

Wt::Signal<> &Views::ViewSequences::createSequenceRequested()
{
    return _createSequenceRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> &Views::ViewSequences::removeSequencesRequested()
{
    return _removeSequencesRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> &Views::ViewSequences::openfilesViewRequested()
{
    return _openfilesViewRequested;
}

Wt::Signal<> &Views::ViewSequences::importThumbnailsRequested()
{
    return _importThumbnailsRequested;
}

void Views::ViewSequences::_btnCreateSequenceClicked()
{
    _createSequenceRequested();
}

void Views::ViewSequences::_btnRemoveSequencesClicked()
{
    _removeSequencesRequested(_qtvSequences->selectedItems());
}

void Views::ViewSequences::_btnImportThumbnailsClicked()
{
    _importThumbnailsRequested();
}

void Views::ViewSequences::_btnOpenFilesViewClicked()
{
    _openfilesViewRequested(_qtvSequences->selectedItems());
}

void Views::ViewSequences::_createSequencesTableView()
{
    _qtvSequences = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Projects::ProjectSequence>(&Database::DatabaseManager::instance());
    _qtvSequences->setRowHeight(160);
    _qtvSequences->setDefaultFilterColumnIndex(1);
    _qtvSequences->setIgnoreNumFilterColumns(1);

    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvSequences->createToolButton("", "icons/Add.png", "Create A New Sequence");
        btn->clicked().connect(this, &Views::ViewSequences::_btnCreateSequenceClicked);

        _qtvSequences->setImportCSVFeatureEnabled(true);
    }
    else
        _qtvSequences->setImportCSVFeatureEnabled(false);

    //requires "remove" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
    {
        //Wt::WPushButton *btn = _qtvSequences->createToolButton("", "icons/Remove.png", "Remove Selected Sequences");
        //btn->clicked().connect(this, &Views::ViewSequences::_btnRemoveSequenceClicked);
    }

    //requires "view" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Edit"))
    {
        Wt::WPushButton *btn = _qtvSequences->createToolButton("", "icons/Thumbnail.png", "Import Thumbnails");
        btn->clicked().connect(this, &Views::ViewSequences::_btnImportThumbnailsClicked);
    }

    //requires "CheckIn or CheckOut" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Check In") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Check Out") ||
            Auth::AuthManager::instance().currentUser()->hasPrivilege("Create Repositories"))
    {
        Wt::WPushButton *btn = _qtvSequences->createToolButton("", "icons/Files.png", "Files Manager");
        btn->clicked().connect(this, &Views::ViewSequences::_btnOpenFilesViewClicked);
    }
}

void Views::ViewSequences::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _createSequencesTableView();

    _layMain->addWidget(_qtvSequences);
}
