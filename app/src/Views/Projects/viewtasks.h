#ifndef VIEWTASKS_H
#define VIEWTASKS_H

#include "../../Database/dbtables.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>

namespace Views
{
    class ViewTasks : public Wt::WContainerWidget
    {
    public:
        ViewTasks();

        const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *qtvTasks() const;

        //Signals
        Wt::Signal<> &createTaskRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTask>>> &removeTasksRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTask>>> &openfilesViewRequested();

    private:
        //Variables
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *_qtvTasks;
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
