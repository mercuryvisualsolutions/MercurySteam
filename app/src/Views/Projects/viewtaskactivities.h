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
        
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskActivity> *m_qtvTaskActivities;
        Log::Logger *m_logger;
        //UI
        Wt::WPushButton *m_btnCreateTaskActivity;
        Wt::WPushButton *m_btnCreateTaskActivitiesForTemplate;
        Wt::WPushButton *m_btnRemoveTaskActivities;
        Wt::WPushButton *m_btnEditTaskActivities;
        Wt::WVBoxLayout *m_layMain;

        //Signals
        Wt::Signal<> m_createTaskActivityRequested;
        Wt::Signal<> m_createTaskActivitiesForTemplateRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskActivity>>> m_removeTaskActivitiesRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Projects::ProjectTaskActivity>>> m_openfilesViewRequested;

        //Slots
        void btnCreateTaskActivityClicked();
        void btnCreateTaskActivitiesForTemplateClicked();
        void btnRemoveTaskActivitiesClicked();
        void btnEditTaskActivitiesClicked();

        void taskActivityDataAboutToBeChanged(const Wt::WModelIndex &index, const boost::any &value, int role);

        //Functions
        void createTaskActivitysTableView();
        //requires active transaction
        void sendTaskNotification(Wt::Dbo::ptr<Projects::ProjectTaskActivity> taskActivityPtr, const std::string &property, const std::string &orgValue, const std::string &newValue);
        void prepareView();
    };
}

#endif // VIEWTASKACTIVITIES_H
