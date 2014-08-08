#ifndef VIEWSEQUENCES_H
#define VIEWSEQUENCES_H

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
        bool isImportThumbnailsOptionHidden();
        void setImportThumbnailsOptionHidden(bool hidden) const;
        bool isOpenFilesOptionHidden();
        void setOpenFilesOptionHidden(bool hidden) const;

        //Signals
        Wt::Signal<> &createSequenceRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> &removeSequencesRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>>> &openfilesViewRequested();
        Wt::Signal<> &importThumbnailsRequested();

    private:
        //Variables
        //UI
        Wt::WPushButton *_btnCreateSequence;
        Wt::WPushButton *_btnRemoveSequences;
        Wt::WPushButton *_btnImportThumbnails;
        Wt::WPushButton *_btnOpenFilesView;

        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectSequence> *_qtvSequences;
        Log::Logger *_logger;
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
