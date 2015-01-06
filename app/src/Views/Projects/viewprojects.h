#ifndef VIEWPROJECTS_H
#define VIEWPROJECTS_H

#include "../../Session/sessionmanager.h"
#include "projectsdialogs.h"
#include "../../Log/logger.h"
#include "../../Session/sessionmanager.h"
#include "viewsequences.h"
#include "viewshots.h"
#include "viewassets.h"
#include "viewtasks.h"
#include "viewtaskactivities.h"

#include "../Database/viewdbodata.h"
#include "../Database/viewtags.h"
#include "../Database/viewnotes.h"

#include <Ms/Widgets/MWidgetFactory.h>
#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WStackedWidget>
#include <Wt/WMenu>
#include <Wt/WMenuItem>
#include <Wt/WToolBar>
#include <Wt/WNavigationBar>
#include <Wt/WTableView>
#include <Wt/Dbo/QueryModel>
#include <Wt/WSortFilterProxyModel>
#include <Wt/WStandardItemModel>

namespace Views
{
    class ViewProjects : public Ms::Widgets::MContainerWidget
    {
    public:
        ViewProjects();

        //functions
        void updateView();
        void updateProjectsView();
        void updateSequencesView();
        void updateShotsView();
        void updateAssetsView();
        void updateTasksView();
        void updatePropertiesView();

        bool isProjectsViewShown();
        bool isSequencesViewShown();
        bool isShotsViewShown();
        bool isAssetsViewShown();
        bool isTasksViewShown();

        const Ms::Widgets::MQueryTableViewWidget<Projects::Project> *projectsQueryTableView() const;
        const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectSequence> *sequencesQueryTableView() const;
        const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectShot> *shotsQueryTableView() const;
        const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectAsset> *assetsQueryTableView() const;
        const Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *tasksQueryTableView() const;

        void adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user);

        //Signals
        Wt::Signal<> &onTabProjectsSelected();
        Wt::Signal<> &onTabSequencesSelected();
        Wt::Signal<> &onTabShotsSelected();
        Wt::Signal<> &onTabAssetsSelected();
        Wt::Signal<> &onTabTasksSelected();

    private:
        //functions
        template<typename T>
        void addDataToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec);
        template<typename T>
        void addNoteToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec);
        template<typename T>
        void assignTagToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);
        template<typename T>
        void unAssignTagFromDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);

        //Signals
        Wt::Signal<> m_onTabProjectsSelected;
        Wt::Signal<> m_onTabSequencesSelected;
        Wt::Signal<> m_onTabShotsSelected;
        Wt::Signal<> m_onTabAssetsSelected;
        Wt::Signal<> m_onTabTasksSelected;

        //variables
        
        Log::Logger *m_logger;
        Ms::Widgets::MPropertiesPanel *m_propertiesPanel;

        /*******************--Main--********************/
        Wt::WStackedWidget *m_stkMain; //for switching sequences, shots, assets..etc
        Wt::WNavigationBar *m_navBarMain;//project navigation bar
        Wt::WContainerWidget *m_cntNavBarMain;//container for navBarProject
        Wt::WMenu *m_mnuMain;//project menu (holding, shots, assets, sequences..etc)
        Wt::WMenuItem *m_mnuMainProjectsItem;
        Wt::WMenuItem *m_mnuMainSequencesItem;
        Wt::WMenuItem *m_mnuMainShotsItem;
        Wt::WMenuItem *m_mnuMainAssetsItem;
        Wt::WMenuItem *m_mnuMainTasksItem;

        //Projects
        Wt::WVBoxLayout *m_layProjects;//Projects layout
        Wt::WContainerWidget *m_cntProjects;//container for projects view
        Ms::Widgets::MQueryTableViewWidget<Projects::Project> *m_qtvProjects;
        Wt::WPushButton *m_btnCreateProject;
        Wt::WPushButton *m_btnEditProjects;
        Wt::WPushButton *m_btnImportProjectsThumbnails;
        Wt::WPushButton *m_btnProjectsFiles;

        //Sequences
        Wt::WVBoxLayout *m_laySequences;
        Wt::WContainerWidget *m_cntSequences;//container for each sequence view
        Views::ViewSequences *m_viewSequences;

        //shots
        Wt::WVBoxLayout *m_layShots;
        Wt::WContainerWidget *m_cntShots;//container for shots view
        Views::ViewShots *m_viewShots;

        //Assets
        Wt::WVBoxLayout *m_layAssets;
        Wt::WContainerWidget *m_cntAssets;//container for assets view
        Views::ViewAssets *m_viewAssets;

        //Tasks
        Wt::WVBoxLayout *m_layTasks;
        Wt::WContainerWidget *m_cntTasks;//container for task view
        Views::ViewTasks *m_viewTasks;

        //Properties
        Wt::WContainerWidget *m_cntPropertiesMain;
        Wt::WVBoxLayout *m_layCntPropertiesMain;
        Wt::WNavigationBar *m_navBarProperties;
        Wt::WMenu *m_mnuNavBarProperties;
        Wt::WMenuItem *m_mnuNavBarPropertiesDataItem;
        Wt::WMenuItem *m_mnuNavBarPropertiesTagsItem;
        Wt::WMenuItem *m_mnuNavBarPropertiesNotesItem;
        Wt::WMenuItem *m_mnuNavBarPropertiesSequencesItem;
        Wt::WMenuItem *m_mnuNavBarPropertiesShotsItem;
        Wt::WMenuItem *m_mnuNavBarPropertiesAssetsItem;
        Wt::WMenuItem *m_mnuNavBarPropertiesTasksItem;
        Wt::WMenuItem *m_mnuNavBarPropertiesTaskActivitiesItem;
        Wt::WMenuItem *m_mnuNavBarPropertiesActivityTemplatesItem;
        Wt::WMenuItem *m_mnuNavBarPropertiesStatisticsItem;
        Wt::WStackedWidget *m_stkProperties;
        Views::ViewDboData *m_viewPropertiesData;
        Views::ViewTags *m_viewPropertiesTags;
        Views::ViewNotes *m_viewPropertiesNotes;
        Views::ViewSequences *m_viewPropertiesSequences;
        Views::ViewShots *m_viewPropertiesShots;
        Views::ViewAssets *m_viewPropertiesAssets;
        Views::ViewTasks *m_viewPropertiesTasks;
        Views::ViewTaskActivity *m_viewPropertiesTaskActivities;
        Ms::Widgets::MTableViewWidget *m_viewPropertiesStatistics;

        //slots
        /*******************--Main--********************/
        //slots
        void mnuMainProjectsItemTriggered();
        void mnuMainSequencesItemTriggered();
        void mnuMainShotsItemTriggered();
        void mnuMainAssetsItemTriggered();
        void mnuMainTasksItemTriggered();

        //functions
        void prepareView();

        /*******************--Projects--********************/
        //slots
        void btnProjectsCreateClicked();
        void btnProjectsRemoveClicked();
        void btnProjectsEditClicked();
        void btnProjectsFilesClicked();
        void btnProjectsImportThumbnailsClicked();
        void projectImported(Wt::Dbo::ptr<Projects::Project> project);

        //functions
        void createProjectsTableView();

        /*******************--Sequences--********************/
        //slots
        void createSequenceRequested();
        void sequenceImported(Wt::Dbo::ptr<Projects::ProjectSequence> sequence);

        //functions
        void createSequencesTableView();

        /*******************--Shots--********************/
        //slots
        void createShotRequested();
        void shotImported(Wt::Dbo::ptr<Projects::ProjectShot> shot);

        //functions
        void createShotsTableView();

        /*******************--Assets--********************/
        //slots
        void createAssetRequested();
        void assetImported(Wt::Dbo::ptr<Projects::ProjectAsset> asset);

        //functions
        void createAssetsTableView();

        /*******************--Tasks--********************/
        //slots
        void createTasksRequested();
        void createTasksForTemplateRequested();
        void taskImported(Wt::Dbo::ptr<Projects::ProjectTask> task);

        //functions
        void createTasksTableView();

        /*******************--Properties--********************/

        //slots
        void mnuNavBarPropertiesDataItemTriggered();
        void mnuNavBarPropertiesTagsItemTriggered();
        void mnuNavBarPropertiesNotesItemTriggered();
        void mnuNavBarPropertiesSequencesItemTriggered();
        void mnuNavBarPropertiesShotsItemTriggered();
        void mnuNavBarPropertiesAssetsItemTriggered();
        void mnuNavBarPropertiesTasksItemTriggered();
        void mnuNavBarPropertiesTaskActivitiesItemTriggered();
        void mnuNavBarPropertiesStatisticsItemTriggered();

        void addDataRequested();
        void removeDataRequested(const std::vector<Wt::Dbo::ptr<Database::DboData>> &dataVec);
        void createProjectTagRequested();
        void assignTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);
        void unassignTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);
        void filterByTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec, bool exactSelection, bool inverse);
        void clearTagsFilterRequested();
        void addNoteRequested();
        void removeNotesRequested(const std::vector<Wt::Dbo::ptr<Database::Note>> &noteVec);
        void createTaskActivityRequested();
        void createTaskActivitiesForTemplateRequested();
        void removeTaskActivitiesRequested(const std::vector<Wt::Dbo::ptr<Projects::ProjectTaskActivity>> &activityVec);

        //functions
        void createPropertiesView();

        void updatePropertiesDataView();
        void updatePropertiesTagsView();
        void updatePropertiesAssignedTagsView();
        void updatePropertiesNotesView();
        void updatePropertiesSequencesView();
        void updatePropertiesShotsView();
        void updatePropertiesAssetsView();
        void updatePropertiesTasksView();
        void updatePropertiesTaskActivitiesView();
        void updatePropertiesStatisticsView();
    };
}

#endif // VIEWPROJECTS_H
