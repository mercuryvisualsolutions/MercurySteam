#ifndef VIEWSEQUENCES_H
#define VIEWSEQUENCES_H

#include "../../Database/dbtables.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>

namespace Views
{
    class ViewSequences : public Wt::WContainerWidget
    {
    public:
        ViewSequences();

        const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectSequence> *qtvSequences() const;

        //Signals
        Wt::Signal<> &createSequenceRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> &removeSequencesRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> &openfilesViewRequested();
        Wt::Signal<> &importThumbnailsRequested();

    private:
        //Variables
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectSequence> *_qtvSequences;
        //UI variables
        Wt::WVBoxLayout *_layMain;

        //Signals
        Wt::Signal<> _createSequenceRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> _removeSequencesRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> _openfilesViewRequested;
        Wt::Signal<> _importThumbnailsRequested;

        //Slots
        void _btnCreateSequenceClicked();
        void _btnRemoveSequencesClicked();
        void _btnImportThumbnailsClicked();
        void _btnOpenFilesViewClicked();

        //Functions
        void _createSequencesTableView();
        void _prepareView();
    };
}

#endif // VIEWSEQUENCES_H
