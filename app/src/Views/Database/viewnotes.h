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
        void adjustUIPrivileges();

        //Signals
        Wt::Signal<> &addNoteRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Note>>> &removeNotesRequested();

    private:
        //Variables
        Ms::Widgets::MQueryTableViewWidget<Database::Note> *_qtvNotes;
        
        //UI variables
        Wt::WVBoxLayout *_layMain;
        Wt::WPushButton *_btnCreateNote;

        //Signals
        Wt::Signal<> _addNoteRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Note>>> _removeNotesRequested;

        //Slots
        void _btnCreateNoteClicked();
        void _btnRemoveNotesClicked();

        //Functions
        void _createNotesTableView();
        void _prepareView();
    };
}

#endif // VIEWNOTES_H
