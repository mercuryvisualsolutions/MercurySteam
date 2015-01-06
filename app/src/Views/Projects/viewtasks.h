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

        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *m_qtvTasks;
        Log::Logger *m_logger;
        //UI
        Wt::WPushButton *m_btnCreateTask;
        Wt::WPushButton *m_btnCreateTasksForTemplate;
        Wt::WPushButton *m_btnRemoveTasks;
        Wt::WPushButton *m_btnEditTasks;
        Wt::WPushButton *m_btnOpenFilesView;
        Wt::WVBoxLayout *m_layMain;

        //Signals
        Wt::Signal<> m_createTaskRequested;
        Wt::Signal<> m_createTasksForTemplateRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTask>>> m_removeTasksRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTask>>> m_openfilesViewRequested;

        //Slots
        void btnCreateTaskClicked();
        void btnCreateTasksForTemplateClicked();
        void btnRemoveTasksClicked();
        void btnEditTasksClicked();
        void btnOpenFilesViewClicked();

        //Functions
        void createTasksTableView();
        void prepareView();
    };
}

#endif // VIEWTASKS_H
