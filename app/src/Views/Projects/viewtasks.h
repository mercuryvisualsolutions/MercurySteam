#ifndef VIEWTASKS_H
#define VIEWTASKS_H

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

        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *qtvTasks() const;
        void updateView(const std::vector<Wt::Dbo::ptr<Projects::Project>> &prjVec,
                        const std::vector<Wt::Dbo::ptr<Projects::ProjectSequence>> &seqVec,
                        const std::vector<Wt::Dbo::ptr<Projects::ProjectShot>> &shotVec,
                        const std::vector<Wt::Dbo::ptr<Projects::ProjectAsset>> &assetVec) const;

        bool isCreateOptionHidden();
        void setCreateOptionHidden(bool hidden) const;
//        bool isRemoveOptionHidden();
//        void setRemoveOptionHidden(bool hidden) const;
        bool isOpenFilesOptionHidden();
        void setOpenFilesOptionHidden(bool hidden) const;

        //Signals
        Wt::Signal<> &createTaskRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTask>>> &removeTasksRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTask>>> &openfilesViewRequested();

    private:
        //Variables
        //UI
        Wt::WPushButton *_btnCreateTask;
        Wt::WPushButton *_btnRemoveTasks;
        Wt::WPushButton *_btnOpenFilesView;

        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *_qtvTasks;
        Log::Logger *_logger;
        //UI variables
        Wt::WVBoxLayout *_layMain;

        //Signals
        Wt::Signal<> _createTaskRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTask>>> _removeTasksRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTask>>> _openfilesViewRequested;

        //Slots
        void _btnCreateTaskClicked();
        void _btnRemoveTasksClicked();
        void _btnOpenFilesViewClicked();

        //Functions
        void _createTasksTableView();
        void _prepareView();
    };
}

#endif // VIEWTASKS_H
