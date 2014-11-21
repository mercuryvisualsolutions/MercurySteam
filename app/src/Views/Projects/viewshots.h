#ifndef VIEWSHOTS_H
#define VIEWSHOTS_H

#include "../../Session/sessionmanager.h"
#include "../../Database/dbtables.h"
#include "../../Log/logger.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>

namespace Views
{
    class ViewShots : public Wt::WContainerWidget
    {
    public:
        ViewShots();

        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectShot> *qtvShots() const;
        void updateView(const std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>> &seqVec) const;

        bool isCreateOptionHidden();
        void setCreateOptionHidden(bool hidden) const;
        bool isRemoveOptionHidden();
        void setRemoveOptionHidden(bool hidden) const;
        bool isEditOptionHidden();
        void setEditOptionHidden(bool hidden) const;
        bool isImportThumbnailsOptionHidden();
        void setImportThumbnailsOptionHidden(bool hidden) const;
        bool isOpenFilesOptionHidden();
        void setOpenFilesOptionHidden(bool hidden) const;

        void adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user);

        //Signals
        Wt::Signal<> &createShotRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectShot>>> &removeShotsRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectShot>>> &openfilesViewRequested();
        Wt::Signal<> &importThumbnailsRequested();

    private:
        //Variables
        
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectShot> *_qtvShots;
        Log::Logger *_logger;
        //UI
        Wt::WPushButton *_btnCreateShot;
        Wt::WPushButton *_btnRemoveShots;
        Wt::WPushButton *_btnEditShots;
        Wt::WPushButton *_btnImportThumbnails;
        Wt::WPushButton *_btnOpenFilesView;
        Wt::WVBoxLayout *_layMain;

        //Signals
        Wt::Signal<> _createShotRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectShot>>> _removeShotsRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectShot>>> _openfilesViewRequested;
        Wt::Signal<> _importThumbnailsRequested;

        //Slots
        void _btnCreateShotClicked();
        void _btnRemoveShotsClicked();
        void _btnEditShotsClicked();
        void _btnImportThumbnailsClicked();
        void _btnOpenFilesViewClicked();

        //Functions
        void _createShotsTableView();
        void _prepareView();
    };
}

#endif // VIEWSHOTS_H
