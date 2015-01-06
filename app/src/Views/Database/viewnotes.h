#ifndef VIEWNOTES_H
#define VIEWNOTES_H

#include "../../Session/sessionmanager.h"
#include "../../Database/dbtables.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>

namespace Views
{
    class ViewNotes : public Wt::WContainerWidget
    {
    public:
        ViewNotes();

        Ms::Widgets::MQueryTableViewWidget<Database::Note> *qtvNotes();

        //functions
        void adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user);

        //Signals
        Wt::Signal<> &addNoteRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Note>>> &removeNotesRequested();

    private:
        //Variables
        Ms::Widgets::MQueryTableViewWidget<Database::Note> *m_qtvNotes;
        
        //UI variables
        Wt::WVBoxLayout *m_layMain;
        Wt::WPushButton *m_btnCreateNote;

        //Signals
        Wt::Signal<> m_addNoteRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Note>>> m_removeNotesRequested;

        //Slots
        void btnCreateNoteClicked();
        void btnRemoveNotesClicked();

        //Functions
        void createNotesTableView();
        void prepareView();
    };
}

#endif // VIEWNOTES_H
