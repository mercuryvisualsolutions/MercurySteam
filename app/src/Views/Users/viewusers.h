#ifndef VIEWUSERS_H
#define VIEWUSERS_H

#include <Wt/WContainerWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WNavigationBar>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WToolBar>
#include <Wt/WTableView>
#include <Wt/Dbo/QueryModel>
#include <Wt/WStandardItemModel>
#include <Wt/WStandardItem>
#include <Wt/WNavigationBar>
#include <Wt/WMenu>
#include <Wt/WMenuItem>
#include <Wt/WStackedWidget>
#include <Wt/WSortFilterProxyModel>
#include <Wt/Auth/Dbo/AuthInfo>

#include <utility>

#include "../../Database/dbtables.h"
#include "../../Log/logger.h"
#include "../../Session/sessionmanager.h"

#include <Ms/Widgets/MWidgetFactory.h>
#include <Ms/Widgets/MQueryTableViewWidget.h>

namespace Views
{
    using userData = boost::tuple<Wt::Dbo::ptr<Users::User>, Wt::Dbo::ptr<Users::AuthInfo>, Wt::Dbo::ptr<Users::AuthInfo::AuthIdentityType>>;

    class ViewUsers : public Wt::WContainerWidget
    {
    public:
        ViewUsers();
        //functions

        void updateView();
        void updateUsersView();
        void updateGroupsView();

        void updatePropertiesView();

        void showPropertiesView();

        bool isUsersViewShown();
        bool isGroupsViewShown();

        Ms::Widgets::MQueryTableViewWidget<Users::User> *usersQueryTableView() const;
        Ms::Widgets::MQueryTableViewWidget<Users::Group> *groupsQueryTableView() const;

        //Signals
        Wt::Signal<> &onTabUsersSelected();
        Wt::Signal<> &onTabGroupsSelected();

    private: 
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

        //variables
        Log::Logger *_logger;
        Ms::Widgets::MPropertiesPanel *_propertiesPanel;

        //Singals
        Wt::Signal<> _onTabUsersSelected;
        Wt::Signal<> _onTabGroupsSelected;

        /*******************--Layouts--********************/
        Wt::WVBoxLayout *_layMain;//main layout

        Wt::WText *_txtUsersPanelTitle;
        Wt::WContainerWidget *_cntTxtUsersPanelTitle;

        Wt::WNavigationBar *_navBarMain;//main navigation bar
        Wt::WStackedWidget *_stkMain;
        Wt::WMenu *_mnuMain;
        Wt::WMenuItem *_mnuMainUsersItem;
        Wt::WMenuItem *_mnuMainGroupsItem;

        /*******************--Main--********************/
        //functions
        void _mnuMainUsersItemTriggered();
        void _mnuMainGroupsItemTriggered();
        void _prepareView();

        /*******************--Users--********************/
        Ms::Widgets::MQueryTableViewWidget<Users::User> *_qtvUsers;
        //slots
        void _btnUsersCreateClicked();
        void _btnUsersChangePasswordClicked();
        void _btnUsersRemoveClicked();
        void _btnUsersImportThumbnailsClicked();

        //functions
        void _createUsersTableView();

        /*******************--Groups--********************/
        Ms::Widgets::MQueryTableViewWidget<Users::Group> *_qtvGroups;
        //slots
        void _btnGroupsCreateClicked();
        void _btnGroupsRemoveClicked();

        //functions
        void _createGroupsTableView();

        /*******************--Properties--********************/
        //Properties
        Wt::WContainerWidget *_cntPropertiesMain;
        Wt::WVBoxLayout *_layCntPropertiesMain;
        Wt::WNavigationBar *_navBarPropertiesMain;
        Wt::WMenu *_mnuPropertiesNavBar;
        Wt::WMenuItem *_mnuPropertiesNavBarDataItem;
        Wt::WMenuItem *_mnuPropertiesNavBarTagsItem;
        Wt::WMenuItem *_mnuPropertiesNavBarNotesItem;
        Wt::WMenuItem *_mnuPropertiesNavBarGroupsPrivilegesItem;
        Wt::WStackedWidget *_stkProperties;

        Wt::WContainerWidget *_cntPropertiesTags;
        Wt::WVBoxLayout *_layCntPropertiesTags;
        Wt::WContainerWidget *_cntPropertiesAssignedTags;
        Wt::WVBoxLayout *_layCntPropertiesAssignedTags;
        Wt::WText *_txtPropertiesAssignedTagsLabel;
        Wt::WContainerWidget *_cntTxtPropertiesAssignedTagsLabel;
        Wt::WContainerWidget *_cntPropertiesAvailableTags;
        Wt::WVBoxLayout *_layCntPropertiesAvailableTags;
        Wt::WText *_txtPropertiesAvailableTagsLabel;
        Wt::WContainerWidget *_cntTxtPropertiesAvailableTagsLabel;

        Wt::WContainerWidget *_cntPropertiesGroupsPrivileges;
        Wt::WVBoxLayout *_layCntPropertiesGroupsPrivileges;
        Wt::WContainerWidget *_cntPropertiesGroupsAssignedPrivileges;
        Wt::WVBoxLayout *_layCntPropertiesGroupsAssignedPrivileges;
        Wt::WText *_txtPropertiesGroupsAssignedPrivilegesLabel;
        Wt::WContainerWidget *_cntTxtPropertiesGroupsAssignedPrivilegesLabel;
        Wt::WContainerWidget *_cntPropertiesGroupsAvailablePrivileges;
        Wt::WVBoxLayout *_layCntPropertiesGroupsAvailablePrivileges;
        Wt::WText *_txtPropertiesGroupsAvailablePrivilegesLabel;
        Wt::WContainerWidget *_cntTxtPropertiesGroupsAvailablePrivilegesLabel;

        Ms::Widgets::MQueryTableViewWidget<Database::DboData> *_qtvPropertiesData;
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvPropertiesTags;
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvPropertiesAssignedTags;
        Ms::Widgets::MQueryTableViewWidget<Database::Note> *_qtvPropertiesNotes;
        Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *_qtvPropertiesGroupsPrivileges;
        Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *_qtvPropertiesGroupsAssignedPrivileges;

        //slots
        void _mnuPropertiesNavBarDataItemTriggered();
        void _mnuPropertiesNavBarTagsItemTriggered();
        void _mnuPropertiesNavBarNotesItemTriggered();
        void _mnuPropertiesNavBarGroupsPrivilegesItemTriggered();
        void _btnAddPropertiesDataClicked();
        void _btnRemovePropertiesDataClicked();
        void _btnAddPropertiesTagClicked();
        void _btnRemovePropertiesTagClicked();
        void _btnFilterPropertiesTagClicked();
        void _btnClearFilterPropertiesTagClicked();
        void _btnAddPropertiesNoteClicked();
        void _btnRemovePropertiesNoteClicked();
        void _btnAddPropertiesGroupsPrivilegesClicked();
        void _btnRemovePropertiesGroupsPrivilegesClicked();
        void _btnFilterPropertiesGroupsPrivilegesClicked();
        void _btnClearFilterPropertiesGroupsPrivilegesClicked();

        //functions
        void _createPropertiesView();

        void _createPropertiesDataTableView();
        void _createPropertiesTagsTableView();
        void _createPropertiesAssignedTagsTableView();
        void _createPropertiesNotesTableView();
        void _createPropertiesGroupsPrivilegesTableView();
        void _createPropertiesGroupsAssignedPrivilegesTableView();

        void _updatePropertiesDataView();
        void _updatePropertiesTagsView();
        void _updatePropertiesAssignedTagsView();
        void _updatePropertiesNotesView();
        void _updatePropertiesGroupsPrivilegesView();
        void _updatePropertiesGroupsAssignedPrivilegesView();
    };
}

#endif // VIEWUSERS_H
