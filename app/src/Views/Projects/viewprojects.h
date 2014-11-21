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
        void _addDataToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec);
        template<typename T>
        void _addNoteToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec);
        template<typename T>
        void _assignTagToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);
        template<typename T>
        void _unAssignTagFromDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);

        //Signals
        Wt::Signal<> _onTabProjectsSelected;
        Wt::Signal<> _onTabSequencesSelected;
        Wt::Signal<> _onTabShotsSelected;
        Wt::Signal<> _onTabAssetsSelected;
        Wt::Signal<> _onTabTasksSelected;

        //variables
        
        Log::Logger *_logger;
        Ms::Widgets::MPropertiesPanel *_propertiesPanel;

        /*******************--Main--********************/
        Wt::WContainerWidget *_cntProjectsAndData;
        Wt::WHBoxLayout *_layCntProjectsAndData;
        Wt::WStackedWidget *_stkMain; //for switching sequences, shots, assets..etc
        Wt::WNavigationBar *_navBarMain;//project navigation bar
        Wt::WContainerWidget *_cntNavBarMain;//container for navBarProject
        Wt::WMenu *_mnuMain;//project menu (holding, shots, assets, sequences..etc)
        Wt::WMenuItem *_mnuMainProjectsItem;
        Wt::WMenuItem *_mnuMainSequencesItem;
        Wt::WMenuItem *_mnuMainShotsItem;
        Wt::WMenuItem *_mnuMainAssetsItem;
        Wt::WMenuItem *_mnuMainTasksItem;

        //Projects
        Wt::WVBoxLayout *_layProjects;//Projects layout
        Wt::WContainerWidget *_cntProjects;//container for projects view
        Ms::Widgets::MQueryTableViewWidget<Projects::Project> *_qtvProjects;
        Wt::WPushButton *_btnCreateProject;
        Wt::WPushButton *_btnEditProjects;
        Wt::WPushButton *_btnImportProjectsThumbnails;
        Wt::WPushButton *_btnProjectsFiles;

        //Sequences
        Wt::WVBoxLayout *_laySequences;
        Wt::WContainerWidget *_cntSequences;//container for each sequence view
        Views::ViewSequences *_viewSequences;

        //shots
        Wt::WVBoxLayout *_layShots;
        Wt::WContainerWidget *_cntShots;//container for shots view
        Views::ViewShots *_viewShots;

        //Assets
        Wt::WVBoxLayout *_layAssets;
        Wt::WContainerWidget *_cntAssets;//container for assets view
        Views::ViewAssets *_viewAssets;

        //Tasks
        Wt::WVBoxLayout *_layTasks;
        Wt::WContainerWidget *_cntTasks;//container for task view
        Views::ViewTasks *_viewTasks;

        //Properties
        Wt::WContainerWidget *_cntPropertiesMain;
        Wt::WVBoxLayout *_layCntPropertiesMain;
        Wt::WNavigationBar *_navBarProperties;
        Wt::WMenu *_mnuNavBarProperties;
        Wt::WMenuItem *_mnuNavBarPropertiesDataItem;
        Wt::WMenuItem *_mnuNavBarPropertiesTagsItem;
        Wt::WMenuItem *_mnuNavBarPropertiesNotesItem;
        Wt::WMenuItem *_mnuNavBarPropertiesSequencesItem;
        Wt::WMenuItem *_mnuNavBarPropertiesShotsItem;
        Wt::WMenuItem *_mnuNavBarPropertiesAssetsItem;
        Wt::WMenuItem *_mnuNavBarPropertiesTasksItem;
        Wt::WMenuItem *_mnuNavBarPropertiesTaskActivitiesItem;
        Wt::WMenuItem *_mnuNavBarPropertiesActivityTemplatesItem;
        Wt::WMenuItem *_mnuNavBarPropertiesStatisticsItem;
        Wt::WStackedWidget *_stkProperties;
        Views::ViewDboData *_viewPropertiesData;
        Views::ViewTags *_viewPropertiesTags;
        Views::ViewNotes *_viewPropertiesNotes;
        Views::ViewSequences *_viewPropertiesSequences;
        Views::ViewShots *_viewPropertiesShots;
        Views::ViewAssets *_viewPropertiesAssets;
        Views::ViewTasks *_viewPropertiesTasks;
        Views::ViewTaskActivity *_viewPropertiesTaskActivities;
        Ms::Widgets::MTableViewWidget *_viewPropertiesStatistics;

        //slots
        /*******************--Main--********************/
        //slots
        void _mnuMainProjectsItemTriggered();
        void _mnuMainSequencesItemTriggered();
        void _mnuMainShotsItemTriggered();
        void _mnuMainAssetsItemTriggered();
        void _mnuMainTasksItemTriggered();

        //functions
        void _prepareView();

        /*******************--Projects--********************/
        //slots
        void _btnProjectsCreateClicked();
        void _btnProjectsRemoveClicked();
        void _btnProjectsEditClicked();
        void _btnProjectsFilesClicked();
        void _btnProjectsImportThumbnailsClicked();
        void _projectImported(Wt::Dbo::ptr<Projects::Project> project);

        //functions
        void _createProjectsTableView();

        /*******************--Sequences--********************/
        //slots
        void _createSequenceRequested();
        void _sequenceImported(Wt::Dbo::ptr<Projects::ProjectSequence> sequence);

        //functions
        void _createSequencesTableView();

        /*******************--Shots--********************/
        //slots
        void _createShotRequested();
        void _shotImported(Wt::Dbo::ptr<Projects::ProjectShot> shot);

        //functions
        void _createShotsTableView();

        /*******************--Assets--********************/
        //slots
        void _createAssetRequested();
        void _assetImported(Wt::Dbo::ptr<Projects::ProjectAsset> asset);

        //functions
        void _createAssetsTableView();

        /*******************--Tasks--********************/
        //slots
        void _createTasksRequested();
        void _createTasksForTemplateRequested();
        void _taskImported(Wt::Dbo::ptr<Projects::ProjectTask> task);

        //functions
        void _createTasksTableView();

        /*******************--Properties--********************/

        //slots
        void _mnuNavBarPropertiesDataItemTriggered();
        void _mnuNavBarPropertiesTagsItemTriggered();
        void _mnuNavBarPropertiesNotesItemTriggered();
        void _mnuNavBarPropertiesSequencesItemTriggered();
        void _mnuNavBarPropertiesShotsItemTriggered();
        void _mnuNavBarPropertiesAssetsItemTriggered();
        void _mnuNavBarPropertiesTasksItemTriggered();
        void _mnuNavBarPropertiesTaskActivitiesItemTriggered();
        void _mnuNavBarPropertiesStatisticsItemTriggered();

        void _addDataRequested();
        void _removeDataRequested(const std::vector<Wt::Dbo::ptr<Database::DboData>> &dataVec);
        void _createProjectTagRequested();
        void _assignTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);
        void _unassignTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);
        void _filterByTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec, bool exactSelection, bool inverse);
        void _clearTagsFilterRequested();
        void _addNoteRequested();
        void _removeNotesRequested(const std::vector<Wt::Dbo::ptr<Database::Note>> &noteVec);
        void _createTaskActivityRequested();
        void _createTaskActivitiesForTemplateRequested();
        void _removeTaskActivitiesRequested(const std::vector<Wt::Dbo::ptr<Projects::ProjectTaskActivity>> &activityVec);

        //functions
        void _createPropertiesView();

        void _updatePropertiesDataView();
        void _updatePropertiesTagsView();
        void _updatePropertiesAssignedTagsView();
        void _updatePropertiesNotesView();
        void _updatePropertiesSequencesView();
        void _updatePropertiesShotsView();
        void _updatePropertiesAssetsView();
        void _updatePropertiesTasksView();
        void _updatePropertiesTaskActivitiesView();
        void _updatePropertiesStatisticsView();
    };
}

#endif // VIEWPROJECTS_H
