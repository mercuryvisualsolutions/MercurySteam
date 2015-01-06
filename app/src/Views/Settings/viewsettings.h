#ifndef VIEWSETTINGS_H
#define VIEWSETTINGS_H

#include "../../Database/dbtables.h"
#include "../../Log/logger.h"
#include "../../Session/sessionmanager.h"
#include "viewtasktemplate.h"
#include "viewactivitytemplate.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WStackedWidget>
#include <Wt/WNavigationBar>
#include <Wt/WMenu>
#include <Wt/WMenuItem>
#include <Wt/WApplication>
#include <Wt/WCheckBox>
#include <Wt/WGroupBox>
#include <Wt/WToolBar>
#include <Wt/WPushButton>
#include <Wt/WTableView>
#include <Wt/Dbo/QueryModel>
#include <Wt/WStandardItemModel>
#include <Wt/WSortFilterProxyModel>

namespace Views
{
    class ViewSettings : public Ms::Widgets::MContainerWidget
    {
    public:
        ViewSettings();

        //functions
        void showView(const std::string &view);
        void updateView();
        void updateGeneralSettingsView();
        void updateProjectsSettingsView();
        void updateUsersSettingsView();
        void updateGlobalSettingsView();
        void updateTaskActivityTypeView();
        void updateTaskTypeView();
        void updateAssetTypeView();
        void updateWorkStatusView();
        void updateTaskTemplatesView();
        void updateActivityTemplatesView();
        void updateUserTitlesView();
        void updateTagsView();

        void adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user);

        //Signal
        Wt::Signal<> &onTabGeneralSelected();
        Wt::Signal<> &onTabProjectsSelected();
        Wt::Signal<> &onTabUsersSelected();
        Wt::Signal<> &onTabGlobalSelected();

    private:
        //Signals
        Wt::Signal<> m_onTabGeneralSelected;
        Wt::Signal<> m_onTabProjectsSelected;
        Wt::Signal<> m_onTabUsersSelected;
        Wt::Signal<> m_onTabGlobalSelected;

        //variables
        
        Log::Logger *m_logger;
        Ms::Widgets::MPropertiesPanel *m_propertiesPanel;

        /*******************--Main--********************/
        Wt::WStackedWidget *m_stkSettings; //for switching settings
        Wt::WNavigationBar *m_navBarSettings;//settings navigation bar
        Wt::WContainerWidget *m_cntNavBarSettings;//container for navBarSettings
        Wt::WMenu *m_mnuSettings;//settings menu (holding, General..etc)
        Wt::WMenuItem *m_mnuSettingsGeneralItem;
        Wt::WMenuItem *m_mnuSettingsProjectsItem;
        Wt::WMenuItem *m_mnuSettingsUsersItem;
        Wt::WMenuItem *m_mnuSettingsGlobalItem;

        /*******************--General--********************/
        Wt::WVBoxLayout *m_laySettingsGeneral;
        Wt::WContainerWidget *m_cntGeneralSettings;//container for general settings view
        Wt::WGroupBox *m_grpGeneral;//group box for general settings
        Wt::WCheckBox *m_chkLoadInactiveData;
        Wt::WCheckBox *m_chkShowExtraColumns;
        Wt::WPushButton *m_btnSave;//save button
        Wt::WContainerWidget *m_cntBtnSave;//save button container

        /*******************--Projects Main--********************/
        Wt::WContainerWidget *m_cntProjectsSettings;//container for projects settings view
        Wt::WHBoxLayout *m_layCntProjectsSettings;
        Wt::WMenu *m_mnuProjectSettings;
        Wt::WContainerWidget *m_cntMnuProjectSettings;
        Wt::WVBoxLayout *m_layCntMnuProjectSettings;
        Wt::WMenuItem *m_mnuProjectSettingsTaskActivityTypeItem;
        Wt::WMenuItem *m_mnuProjectSettingsTaskTypeItem;
        Wt::WMenuItem *m_mnuProjectSettingsAssetTypeItem;
        Wt::WMenuItem *m_mnuProjectSettingsWorkStatusItem;
        Wt::WMenuItem *m_mnuProjectSettingsTaskTemplatesItem;
        Wt::WMenuItem *m_mnuProjectSettingsActivityTemplatesItem;
        Wt::WStackedWidget *m_stkProjectProperties;

        /*******************--TaskActivityType--********************/
        Wt::WContainerWidget *m_cntTaskActivityType;
        Wt::WVBoxLayout *m_layCntTaskActivityType;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskActivityType> *m_qtvProjectTaskActivityType;
        Wt::WPushButton *m_btnCreateTaskActivityType;

        //functions
        void createTaskActivityTypeTableView();

        /*******************--TaskType--********************/
        Wt::WContainerWidget *m_cntTaskType;
        Wt::WVBoxLayout *m_layCntTaskType;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskType> *m_qtvProjectTaskType;
        Wt::WPushButton *m_btnCreateTaskType;

        //functions
        void createTaskTypeTableView();

        /*******************--AssetType--********************/
        Wt::WContainerWidget *m_cntAssetType;
        Wt::WVBoxLayout *m_layCntAssetType;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectAssetType> *m_qtvProjectAssetType;
        Wt::WPushButton *m_btnCreateAssetType;

        //functions
        void createAssetTypeTableView();

        /*******************--WorkStatus--********************/
        Wt::WContainerWidget *m_cntWorkStatus;
        Wt::WVBoxLayout *m_layCntWorkStatus;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectWorkStatus> *m_qtvProjectWorkStatus;
        Wt::WPushButton *m_btnCreateWorkStatus;

        //functions
        void createWorkStatusTableView();

        /*******************--Task Template--********************/
        Views::ViewTaskTemplates *m_viewTaskTemplates;

        /*******************--Activity Template--********************/
        Views::ViewActivityTemplates *m_viewActivityTemplates;

        /*******************--Users Main--********************/
        Wt::WContainerWidget *m_cntUsersSettings;//container for users settings view
        Wt::WHBoxLayout *m_layCntUsersSettings;
        Wt::WMenu *m_mnuUsersSettings;
        Wt::WContainerWidget *m_cntMnuUsersSettings;
        Wt::WVBoxLayout *m_layCntMnuUsersSettings;
        Wt::WMenuItem *m_mnuUsersSettingsUserTitlesItem;
        Wt::WStackedWidget *m_stkUsersProperties;

        /*******************--Titles--********************/
        Wt::WContainerWidget *m_cntUserTitles;
        Wt::WVBoxLayout *m_layCntUserTitles;
        Ms::Widgets::MQueryTableViewWidget<Users::UserTitle> *m_qtvUserTitle;
        Wt::WPushButton *m_btnCreateUserTitle;

        //functions
        void createUsersTitlesTableView();

        /*******************--Global Main--********************/
        Wt::WContainerWidget *m_cntGlobalSettings;//container for global settings view
        Wt::WHBoxLayout *m_layCntGlobalSettings;
        Wt::WMenu *m_mnuGlobalSettings;
        Wt::WContainerWidget *m_cntMnuGlobalSettings;
        Wt::WVBoxLayout *m_layCntMnuGlobalSettings;
        Wt::WMenuItem *m_mnuGlobalSettingsTagsItem;
        Wt::WStackedWidget *m_stkGlobalProperties;

        /*******************--Tags--********************/
        Wt::WContainerWidget *m_cntTags;
        Wt::WVBoxLayout *m_layCntTags;
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *m_qtvTags;
        Wt::WPushButton *m_btnCreateTag;

        /*******************--Properties--********************/
        Wt::WContainerWidget *m_cntPropertiesMain;

        //functions
        void createTagsTableView();

        //slots
        //general
        void mnuSettingsGeneralItemTriggered();
        void mnuSettingsProjectsItemTriggered();
        void mnuSettingsUsersItemTriggered();
        void mnuSettingsGlobalItemTriggered();
        void btnSaveClicked();
        //Projects Main
        void mnuProjectSettingsTaskActivityTypeItemTriggered();
        void mnuProjectSettingsTaskTypeItemTriggered();
        void mnuProjectSettingsAssetTypeItemTriggered();
        void mnuProjectSettingsWorkStatusItemTriggered();
        void mnuProjectSettingsTaskTemplatesItemTriggered();
        void mnuProjectSettingsActivityTemplatesItemTriggered();
        //TaskActivityType
        void btnCreateTaskActivityTypeClicked();
        void btnRemoveTaskActivityTypeClicked();
        //TaskType
        void btnCreateTaskTypeClicked();
        void btnRemoveTaskTypeClicked();
        //AssetType
        void btnCreateAssetTypeClicked();
        void btnRemoveAssetTypeClicked();
        //WorkStatus
        void btnCreateWorkStatusClicked();
        void btnRemoveWorkStatusClicked();
        //Users Main
        void mnuUsersSettingsUserTitlesItemTriggered();
        //Titles
        void btnCreateUserTitleClicked();
        void btnRemoveUserTitlesClicked();
        //Global Main
        void mnuGlobalSettingsTagsItemTriggered();
        //Titles
        void btnCreateTagClicked();
        void btnRemoveTagClicked();

        /*******************--Properties--********************/
        void createPropertiesView();

        void prepareView();
    };
}

#endif // VIEWSETTINGS_H
