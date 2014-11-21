#ifndef VIEWTASKACTIVITIES_H
#define VIEWTASKACTIVITIES_H

#include "../../Session/sessionmanager.h"
#include "../../Database/dbtables.h"
#include "../../Log/logger.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>

namespace Views
{
    class ViewTaskActivity : public Wt::WContainerWidget
    {
    public:
        ViewTaskActivity();

        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskActivity> *qtvTaskActivities() const;
        void updateView(const std::vector<Wt::Dbo::ptr<Projects::ProjectTask> > &taskVec) const;

        bool isCreateOptionHidden();
        void setCreateOptionHidden(bool hidden) const;
        bool isCreateForTemplateOptionHidden();
        void setCreateForTemplateOptionHidden(bool hidden) const;
        bool isRemoveOptionHidden();
        void setRemoveOptionHidden(bool hidden) const;
        bool isEditOptionHidden();
        void setEditOptionHidden(bool hidden) const;

        void adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user);

        //Signals
        Wt::Signal<> &createTaskActivityRequested();
        Wt::Signal<> &createTaskActivitiesForTemplateRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskActivity>>> &removeTaskActivitiesRequested();

    private:
        //Variables
        
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskActivity> *_qtvTaskActivities;
        Log::Logger *_logger;
        //UI
        Wt::WPushButton *_btnCreateTaskActivity;
        Wt::WPushButton *_btnCreateTaskActivitiesForTemplate;
        Wt::WPushButton *_btnRemoveTaskActivities;
        Wt::WPushButton *_btnEditTaskActivities;
        Wt::WVBoxLayout *_layMain;

        //Signals
        Wt::Signal<> _createTaskActivityRequested;
        Wt::Signal<> _createTaskActivitiesForTemplateRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskActivity>>> _removeTaskActivitiesRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskActivity>>> _openfilesViewRequested;

        //Slots
        void _btnCreateTaskActivityClicked();
        void _btnCreateTaskActivitiesForTemplateClicked();
        void _btnRemoveTaskActivitiesClicked();
        void _btnEditTaskActivitiesClicked();

        //Functions
        void _createTaskActivitysTableView();
        void _prepareView();
    };
}

#endif // VIEWTASKACTIVITIES_H
