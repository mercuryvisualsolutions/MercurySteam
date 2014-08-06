#ifndef VIEWSETTINGS_H
#define VIEWSETTINGS_H

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

#include "../../Database/dbtables.h"
#include "../../Log/logger.h"
#include "../../Session/sessionmanager.h"
#include "settingsdialogs.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

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
        void updateTaskTypeView();
        void updateAssetTypeView();
        void updateWorkStatusView();
        void updateUserTitlesView();
        void updateTagsView();

        //Signal
        Wt::Signal<> &onTabGeneralSelected();
        Wt::Signal<> &onTabProjectsSelected();
        Wt::Signal<> &onTabUsersSelected();
        Wt::Signal<> &onTabGlobalSelected();

    private:
        //Signals
        Wt::Signal<> _onTabGeneralSelected;
        Wt::Signal<> _onTabProjectsSelected;
        Wt::Signal<> _onTabUsersSelected;
        Wt::Signal<> _onTabGlobalSelected;

        //variables
        Log::Logger *_logger;
        Ms::Widgets::MPropertiesPanel *_propertiesPanel;

        /*******************--Main--********************/
        Wt::WStackedWidget *_stkSettings; //for switching settings
        Wt::WNavigationBar *_navBarSettings;//settings navigation bar
        Wt::WContainerWidget *_cntNavBarSettings;//container for navBarSettings
        Wt::WMenu *_mnuSettings;//settings menu (holding, General..etc)
        Wt::WMenuItem *_mnuSettingsGeneralItem;
        Wt::WMenuItem *_mnuSettingsProjectsItem;
        Wt::WMenuItem *_mnuSettingsUsersItem;
        Wt::WMenuItem *_mnuSettingsGlobalItem;

        /*******************--General--********************/
        Wt::WVBoxLayout *_laySettingsGeneral;
        Wt::WContainerWidget *_cntGeneralSettings;//container for general settings view
        Wt::WGroupBox *_grpGeneral;//group box for general settings
        Wt::WCheckBox *_chkLoadInactiveData;
        Wt::WCheckBox *_chkShowExtraColumns;
        Wt::WPushButton *_btnSave;//save button
        Wt::WContainerWidget *_cntBtnSave;//save button container

        /*******************--Projects Main--********************/
        Wt::WContainerWidget *_cntProjectsSettings;//container for projects settings view
        Wt::WHBoxLayout *_layCntProjectsSettings;
        Wt::WMenu *_mnuProjectSettings;
        Wt::WContainerWidget *_cntMnuProjectSettings;
        Wt::WVBoxLayout *_layCntMnuProjectSettings;
        Wt::WMenuItem *_mnuProjectSettingsTaskTypeItem;
        Wt::WMenuItem *_mnuProjectSettingsAssetTypeItem;
        Wt::WMenuItem *_mnuProjectSettingsWorkStatusItem;
        Wt::WStackedWidget *_stkProjectProperties;

        /*******************--TaskType--********************/
        Wt::WContainerWidget *_cntTaskType;
        Wt::WVBoxLayout *_layCntTaskType;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectTaskType> *_qtvProjectTaskType;

        //functions
        void _createTaskTypeTableView();

        /*******************--AssetType--********************/
        Wt::WContainerWidget *_cntAssetType;
        Wt::WVBoxLayout *_layCntAssetType;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectAssetType> *_qtvProjectAssetType;

        //functions
        void _createAssetTypeTableView();

        /*******************--WorkStatus--********************/
        Wt::WContainerWidget *_cntWorkStatus;
        Wt::WVBoxLayout *_layCntWorkStatus;
        Ms::Widgets::MQueryTableViewWidget<Projects::ProjectWorkStatus> *_qtvProjectWorkStatus;

        //functions
        void _createWorkStatusTableView();

        /*******************--Users Main--********************/
        Wt::WContainerWidget *_cntUsersSettings;//container for users settings view
        Wt::WHBoxLayout *_layCntUsersSettings;
        Wt::WMenu *_mnuUsersSettings;
        Wt::WContainerWidget *_cntMnuUsersSettings;
        Wt::WVBoxLayout *_layCntMnuUsersSettings;
        Wt::WMenuItem *_mnuUsersSettingsUserTitlesItem;
        Wt::WStackedWidget *_stkUsersProperties;

        /*******************--Titles--********************/
        Wt::WContainerWidget *_cntUserTitles;
        Wt::WVBoxLayout *_layCntUserTitles;
        Ms::Widgets::MQueryTableViewWidget<Users::UserTitle> *_qtvUserTitle;

        //functions
        void _createUsersTitlesTableView();

        /*******************--Global Main--********************/
        Wt::WContainerWidget *_cntGlobalSettings;//container for global settings view
        Wt::WHBoxLayout *_layCntGlobalSettings;
        Wt::WMenu *_mnuGlobalSettings;
        Wt::WContainerWidget *_cntMnuGlobalSettings;
        Wt::WVBoxLayout *_layCntMnuGlobalSettings;
        Wt::WMenuItem *_mnuGlobalSettingsTagsItem;
        Wt::WStackedWidget *_stkGlobalProperties;

        /*******************--Tags--********************/
        Wt::WContainerWidget *_cntTags;
        Wt::WVBoxLayout *_layCntTags;
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvTags;

        /*******************--Properties--********************/
        Wt::WContainerWidget *_cntPropertiesMain;

        //functions
        void _createTagsTableView();

        //slots
        //general
        void _mnuSettingsGeneralItemTriggered();
        void _mnuSettingsProjectsItemTriggered();
        void _mnuSettingsUsersItemTriggered();
        void _mnuSettingsGlobalItemTriggered();
        void _btnSaveClicked();
        //Projects Main
        void _mnuProjectSettingsTaskTypeItemTriggered();
        void _mnuProjectSettingsAssetTypeItemTriggered();
        void _mnuProjectSettingsWorkStatusItemTriggered();
        //TaskType
        void _btnCreateTaskTypeClicked();
        void _btnRemoveTaskTypeClicked();
        //AssetType
        void _btnCreateAssetTypeClicked();
        void _btnRemoveAssetTypeClicked();
        //WorkStatus
        void _btnCreateWorkStatusClicked();
        void _btnRemoveWorkStatusClicked();
        //Users Main
        void _mnuUsersSettingsUserTitlesItemTriggered();
        //Titles
        void _btnCreateUserTitleClicked();
        void _btnRemoveUserTitlesClicked();
        //Global Main
        void _mnuGlobalSettingsTagsItemTriggered();
        //Titles
        void _btnCreateTagClicked();
        void _btnRemoveTagClicked();

        /*******************--Properties--********************/
        void _createPropertiesView();

        void _prepareView();
    };
}

#endif // VIEWSETTINGS_H
