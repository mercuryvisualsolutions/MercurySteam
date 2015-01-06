#include "viewnotes.h"
#include "../../Auth/authmanager.h"
#include "../../Settings/appsettings.h"
#include "../../Database/dbosession.h"

#include <Ms/Widgets/MWidgetFactory.h>

Views::ViewNotes::ViewNotes()
{
    prepareView();
}

Ms::Widgets::MQueryTableViewWidget<Database::Note> *Views::ViewNotes::qtvNotes()
{
    return m_qtvNotes;
}

void Views::ViewNotes::adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user)
{
    bool hasCreateDboPriv = user->hasPrivilege("Create DBO");

    m_btnCreateNote->setHidden(!hasCreateDboPriv);

    m_qtvNotes->setImportCSVFeatureEnabled(hasCreateDboPriv);
}

Wt::Signal<> &Views::ViewNotes::addNoteRequested()
{
    return m_addNoteRequested;
}

Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Note>>> &Views::ViewNotes::removeNotesRequested()
{
    return m_removeNotesRequested;
}

void Views::ViewNotes::btnCreateNoteClicked()
{
    addNoteRequested();
}

void Views::ViewNotes::btnRemoveNotesClicked()
{
    m_removeNotesRequested(m_qtvNotes->selectedItems());
}

void Views::ViewNotes::createNotesTableView()
{
    m_qtvNotes = Ms::Widgets::MWidgetFactory::createQueryTableViewWidget<Database::Note>(Session::SessionManager::instance().dboSession());

    m_btnCreateNote = m_qtvNotes->createToolButton("", "icons/Add.png", "Add A New Note");
    m_btnCreateNote->clicked().connect(this, &Views::ViewNotes::btnCreateNoteClicked);

//    Wt::WPushButton *btn = m_qtvNotes->createToolButton("", "icons/Remove.png", "Remove Selected Notes");
//    btn->clicked().connect(this, &Views::ViewNotes::btnRemoveNotesClicked);
}

void Views::ViewNotes::prepareView()
{
    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(0);

    setLayout(m_layMain);

    createNotesTableView();

    m_layMain->addWidget(m_qtvNotes);
}
