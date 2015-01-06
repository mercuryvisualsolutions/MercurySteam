#ifndef VIEWSEQUENCES_H
#define VIEWSEQUENCES_H

#include "../../Session/sessionmanager.h"
#include "../../Database/dbtables.h"
#include "../../Log/logger.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>

namespace Views
{
    class ViewSequences : public Wt::WContainerWidget
    {
    public:
        ViewSequences();

        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectSequence> *qtvSequences() const;
        void updateView(const std::vector<Wt::Dbo::ptr<Projects::Project>> &prjVec) const;

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
        Wt::Signal<> &createSequenceRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> &removeSequencesRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> &editSequencesRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> &openfilesViewRequested();
        Wt::Signal<> &importThumbnailsRequested();

    private:
        //Variables
        
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectSequence> *m_qtvSequences;
        Log::Logger *m_logger;
        //UI
        Wt::WPushButton *m_btnCreateSequence;
        Wt::WPushButton *m_btnRemoveSequences;
        Wt::WPushButton *m_btnEditSequences;
        Wt::WPushButton *m_btnImportThumbnails;
        Wt::WPushButton *m_btnOpenFilesView;
        Wt::WVBoxLayout *m_layMain;

        //Signals
        Wt::Signal<> m_createSequenceRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> m_removeSequencesRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> m_editSequencesRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> m_openfilesViewRequested;
        Wt::Signal<> m_importThumbnailsRequested;

        //Slots
        void btnCreateSequenceClicked();
        void btnRemoveSequencesClicked();
        void btnEditSequencesClicked();
        void btnImportThumbnailsClicked();
        void btnOpenFilesViewClicked();

        //Functions
        void createSequencesTableView();
        void prepareView();
    };
}

#endif // VIEWSEQUENCES_H
