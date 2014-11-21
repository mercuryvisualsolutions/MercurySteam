#ifndef VIEWTASKS_H
#define VIEWTASKS_H

#include "../../Session/sessionmanager.h"
#include "../../Database/dbtables.h"
#include "../../Log/logger.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>

namespace Views
{
    class ViewTasks : public Wt::WContainerWidget
    {
    public:
        ViewTasks();

        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *qtvTasks();
        void updateView(const std::vector<Wt::Dbo::ptr<Projects::Project>> &prjVec,
                        const std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>> &seqVec,
                        const std::vector<Wt::Dbo::ptr<Projects::ProjectShot>> &shotVec,
                        const std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>> &assetVec) const;

        bool isCreateOptionHidden();
        void setCreateOptionHidden(bool hidden) const;
        bool isCreateForTemplateOptionHidden();
        void setCreateForTemplateOptionHidden(bool hidden) const;
        bool isRemoveOptionHidden();
        void setRemoveOptionHidden(bool hidden) const;
        bool isEditOptionHidden();
        void setEditOptionHidden(bool hidden) const;
        bool isOpenFilesOptionHidden();
        void setOpenFilesOptionHidden(bool hidden) const;

        void adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user);

        //Signals
        Wt::Signal<> &createTaskRequested();
        Wt::Signal<> &createTasksForTemplateRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTask>>> &removeTasksRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTask>>> &openfilesViewRequested();

    private:
        //Variables

        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *_qtvTasks;
        Log::Logger *_logger;
        //UI
        Wt::WPushButton *_btnCreateTask;
        Wt::WPushButton *_btnCreateTasksForTemplate;
        Wt::WPushButton *_btnRemoveTasks;
        Wt::WPushButton *_btnEditTasks;
        Wt::WPushButton *_btnOpenFilesView;
        Wt::WVBoxLayout *_layMain;

        //Signals
        Wt::Signal<> _createTaskRequested;
        Wt::Signal<> _createTasksForTemplateRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTask>>> _removeTasksRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTask>>> _openfilesViewRequested;

        //Slots
        void _btnCreateTaskClicked();
        void _btnCreateTasksForTemplateClicked();
        void _btnRemoveTasksClicked();
        void _btnEditTasksClicked();
        void _btnOpenFilesViewClicked();

        //Functions
        void _createTasksTableView();
        void _prepareView();
    };
}

#endif // VIEWTASKS_H
