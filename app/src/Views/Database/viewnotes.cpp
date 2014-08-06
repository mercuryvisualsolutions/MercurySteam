#include "viewnotes.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/databasemanager.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewNotes::ViewNotes()
{
    _prepareView();
}

const Ms::Widgets::MQueryTableViewWidget<Database::Note> *Views::ViewNotes::qtvNotes() const
{
    return _qtvNotes;
}

Wt::Signal<> &Views::ViewNotes::addNoteRequested()
{
    return _addNoteRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Note>>> &Views::ViewNotes::removeNotesRequested()
{
    return _removeNotesRequested;
}

void Views::ViewNotes::_btnAddNoteClicked()
{
    _addNoteRequested();
}

void Views::ViewNotes::_btnRemoveNotesClicked()
{
    _removeNotesRequested(_qtvNotes->selectedItems());
}

void Views::ViewNotes::_createNotesTableView()
{
    _qtvNotes = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Note>(&Database::DatabaseManager::instance());
    //requires "create" privilege
    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Create"))
    {
        Wt::WPushButton *btn = _qtvNotes->createToolButton("", "icons/Add.png", "Add A New Note");
        btn->clicked().connect(this, &Views::ViewNotes::_btnAddNoteClicked);
    }
    else
        _qtvNotes->setImportCSVFeatureEnabled(false);

    //requires "remove" privilege
//    if(Auth::AuthManager::instance().currentUser()->hasPrivilege("Remove"))
//    {
//        Wt::WPushButton *btn = _qtvNotes->createToolButton("", "icons/Remove.png", "Remove Selected Notes");
//        btn->clicked().connect(this, &Views::ViewNotes::_btnRemoveNotesClicked);
//    }
}

void Views::ViewNotes::_prepareView()
{
    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(0);

    setLayout(_layMain);

    _createNotesTableView();

    _layMain->addWidget(_qtvNotes);
}
