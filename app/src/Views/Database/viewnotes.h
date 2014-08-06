#ifndef VIEWNOTES_H
#define VIEWNOTES_H

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

        const Ms::Widgets::MQueryTableViewWidget<Database::Note> *qtvNotes() const;

        //Signals
        Wt::Signal<> &addNoteRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Note>>> &removeNotesRequested();

    private:
        //Variables
        Ms::Widgets::MQueryTableViewWidget<Database::Note> *_qtvNotes;
        //UI variables
        Wt::WVBoxLayout *_layMain;

        //Signals
        Wt::Signal<> _addNoteRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Note>>> _removeNotesRequested;

        //Slots
        void _btnAddNoteClicked();
        void _btnRemoveNotesClicked();

        //Functions
        void _createNotesTableView();
        void _prepareView();
    };
}

#endif // VIEWNOTES_H
