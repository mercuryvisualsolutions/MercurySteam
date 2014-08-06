#ifndef VIEWSHOTS_H
#define VIEWSHOTS_H

#include "../../Database/dbtables.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>

namespace Views
{
    class ViewShots : public Wt::WContainerWidget
    {
    public:
        ViewShots();

        const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectShot> *qtvShots() const;

        //Signals
        Wt::Signal<> &createShotRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectShot>>> &removeShotsRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectShot>>> &openfilesViewRequested();
        Wt::Signal<> &importThumbnailsRequested();

    private:
        //Variables
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectShot> *_qtvShots;
        //UI variables
        Wt::WVBoxLayout *_layMain;

        //Signals
        Wt::Signal<> _createShotRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectShot>>> _removeShotsRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectShot>>> _openfilesViewRequested;
        Wt::Signal<> _importThumbnailsRequested;

        //Slots
        void _btnCreateShotClicked();
        void _btnRemoveShotsClicked();
        void _btnImportThumbnailsClicked();
        void _btnOpenFilesViewClicked();

        //Functions
        void _createShotsTableView();
        void _prepareView();
    };
}

#endif // VIEWSHOTS_H
