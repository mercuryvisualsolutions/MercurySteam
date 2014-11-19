#include "viewnotes.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/dbosession.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewNotes::ViewNotes()
{
    _prepareView();

    adjustUIPrivileges();
}

Ms::Widgets::MQueryTableViewWidget<Database::Note> *Views::ViewNotes::qtvNotes()
{
    return _qtvNotes;
}

void Views::ViewNotes::adjustUIPrivileges()
{
    Wt::Dbo::ptr<Users::User> user = Session::SessionManager::instance().user();

    bool hasCreateDboPriv = user->hasPrivilege("Create DBO");

    _btnCreateNote->setHidden(!hasCreateDboPriv);

    _qtvNotes->setImportCSVFeatureEnabled(hasCreateDboPriv);
}

Wt::Signal<> &Views::ViewNotes::addNoteRequested()
{
    return _addNoteRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Note>>> &Views::ViewNotes::removeNotesRequested()
{
    return _removeNotesRequested;
}

void Views::ViewNotes::_btnCreateNoteClicked()
{
    _addNoteRequested();
}

void Views::ViewNotes::_btnRemoveNotesClicked()
{
    _removeNotesRequested(_qtvNotes->selectedItems());
}

void Views::ViewNotes::_createNotesTableView()
{
    _qtvNotes = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Note>(Session::SessionManager::instance().dboSession());

    _btnCreateNote = _qtvNotes->createToolButton("", "icons/Add.png", "Add A New Note");
    _btnCreateNote->clicked().connect(this, &Views::ViewNotes::_btnCreateNoteClicked);

//    Wt::WPushButton *btn = _qtvNotes->createToolButton("", "icons/Remove.png", "Remove Selected Notes");
//    btn->clicked().connect(this, &Views::ViewNotes::_btnRemoveNotesClicked);
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
