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
        
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectShot> *m_qtvShots;
        Log::Logger *m_logger;
        //UI
        Wt::WPushButton *m_btnCreateShot;
        Wt::WPushButton *m_btnRemoveShots;
        Wt::WPushButton *m_btnEditShots;
        Wt::WPushButton *m_btnImportThumbnails;
        Wt::WPushButton *m_btnOpenFilesView;
        Wt::WVBoxLayout *m_layMain;

        //Signals
        Wt::Signal<> m_createShotRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectShot>>> m_removeShotsRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectShot>>> m_openfilesViewRequested;
        Wt::Signal<> m_importThumbnailsRequested;

        //Slots
        void btnCreateShotClicked();
        void btnRemoveShotsClicked();
        void btnEditShotsClicked();
        void btnImportThumbnailsClicked();
        void btnOpenFilesViewClicked();

        void shotDataAboutToBeChanged(const Wt::WModelIndex &index, const boost::any &value, int role);

        //Functions
        void createShotsTableView();
        //requires active transaction
        void sendTaskNotification(Wt::Dbo::ptr<Projects::ProjectShot> shotPtr, const std::string &property, const std::string &orgValue, const std::string &newValue);
        void prepareView();
    };
}

#endif // VIEWSHOTS_H
