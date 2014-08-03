#ifndef VIEWPROJECTS_H
#define VIEWPROJECTS_H

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

#include "projectsdialogs.h"
#include "../../Log/logger.h"
#include "../../Session/sessionmanager.h"
#include "../Properties/viewproperties.h"

#include <Ms/Widgets/MWidgetFactory.h>
#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

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

        Ms::Widgets::MQueryTableViewWidget<Projects::Project> *projectsQueryTableView() const;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectSequence> *sequencesQueryTableView() const;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectShot> *shotsQueryTableView() const;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectAsset> *assetsQueryTableView() const;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *tasksQueryTableView() const;

        //Signals
        Wt::Signal<> &onTabProjectsSelected();
        Wt::Signal<> &onTabSequencesSelected();
        Wt::Signal<> &onTabShotsSelected();
        Wt::Signal<> &onTabAssetsSelected();
        Wt::Signal<> &onTabTasksSelected();

    private:
        //functions
        template<typename T>
        void _addExtraColumns(Ms::Widgets::MQueryTableViewWidget<T> *widget, Wt::WFlags<Wt::ItemFlag> flags, int editRank);

        template<typename T>
        void _addDataToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec);
        template<typename T>
        void _addNoteToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec);
        template<typename T>
        void _addTagsToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);
        template<typename T>
        void _removeTagsFromDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);

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

        //Sequences
        Wt::WVBoxLayout *_laySequences;
        Wt::WContainerWidget *_cntSequences;//container for each sequence view
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectSequence> *_qtvSequences;

        //shots
        Wt::WVBoxLayout *_layShots;
        Wt::WContainerWidget *_cntShots;//container for shots view
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectShot> *_qtvShots;

        //Assets
        Wt::WVBoxLayout *_layAssets;
        Wt::WContainerWidget *_cntAssets;//container for assets view
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectAsset> *_qtvAssets;

        //Tasks
        Wt::WVBoxLayout *_layTasks;
        Wt::WContainerWidget *_cntTasks;//container for task view
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTask> *_qtvTasks;

        //Properties
        Views::ViewProperties *_viewProperties;

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
        void _btnProjectsFilesClicked();
        void _btnProjectsImportThumbnailsClicked();

        //functions
        void _createProjectsTableView();

        /*******************--Sequences--********************/
        //slots
        void _btnSequencesCreateClicked();
        void _btnSequencesRemoveClicked();
        void _btnSequencesFilesClicked();
        void _btnSequencesImportThumbnailsClicked();

        //functions
        void _createSequencesTableView();

        /*******************--Shots--********************/
        //slots
        void _btnShotsCreateClicked();
        void _btnShotsRemoveClicked();
        void _btnShotsFilesClicked();
        void _btnShotsImportThumbnailsClicked();

        //functions
        void _createShotsTableView();

        /*******************--Assets--********************/
        //slots
        void _btnAssetsCreateClicked();
        void _btnAssetsRemoveClicked();
        void _btnAssetsFilesClicked();
        void _btnAssetsImportThumbnailsClicked();

        //functions
        void _createAssetsTableView();

        /*******************--Tasks--********************/
        //slots
        void _btnTasksCreateClicked();
        void _btnTasksRemoveClicked();
        void _btnTasksFilesClicked();

        //functions
        void _createTasksTableView();

        /*******************--Properties--********************/

        //slots
        void _btnAddPropertiesDataClicked();
        void _btnRemovePropertiesDataClicked(const std::vector<Wt::Dbo::ptr<Database::DboData>> &dataVec);
        void _btnAddPropertiesTagClicked(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);
        void _btnRemovePropertiesTagClicked(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);
        void _btnFilterPropertiesTagClicked(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);
        void _btnClearFilterPropertiesTagClicked();
        void _btnAddPropertiesNoteClicked();
        void _btnRemovePropertiesNoteClicked(const std::vector<Wt::Dbo::ptr<Database::Note>> &noteVec);
        void _onViewPropertiesSubViewExposed(const std::string &viewName);

        //functions
        void _createPropertiesView();

        void _updatePropertiesDataView();
        void _updatePropertiesTagsView();
        void _updatePropertiesAssignedTagsView();
        void _updatePropertiesNotesView();
    };
}

#endif // VIEWPROJECTS_H
