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
#include "../Properties/viewproperties.h"

#include <Ms/Widgets/MWidgetFactory.h>
#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

namespace Views
{
    using userData = boost::tuple<Wt::Dbo::ptr<Users::User>, Wt::Dbo::ptr<Users::AuthInfo>, Wt::Dbo::ptr<Users::AuthInfo::AuthIdentityType>>;

    class ViewUsers : public Ms::Widgets::MContainerWidget
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

        /*******************--UI--********************/
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
        Views::ViewProperties *_viewProperties;

        //slots
        void _btnAddPropertiesDataClicked();
        void _btnRemovePropertiesDataClicked(std::vector<Wt::Dbo::ptr<Database::DboData>> dataVec);
        void _btnAddPropertiesTagClicked(std::vector<Wt::Dbo::ptr<Database::Tag>> tagVec);
        void _btnRemovePropertiesTagClicked(std::vector<Wt::Dbo::ptr<Database::Tag>> tagVec);
        void _btnFilterPropertiesTagClicked(std::vector<Wt::Dbo::ptr<Database::Tag>> tagVec);
        void _btnClearFilterPropertiesTagClicked();
        void _btnAddPropertiesNoteClicked();
        void _btnRemovePropertiesNoteClicked(std::vector<Wt::Dbo::ptr<Database::Note>> noteVec);
        void _btnAddPropertiesGroupsPrivilegesClicked(std::vector<Wt::Dbo::ptr<Users::Privilege>> privVec);
        void _btnRemovePropertiesGroupsPrivilegesClicked(std::vector<Wt::Dbo::ptr<Users::Privilege>> privVec);
        void _btnFilterPropertiesGroupsPrivilegesClicked(std::vector<Wt::Dbo::ptr<Users::Privilege>> privVec);
        void _btnClearFilterPropertiesGroupsPrivilegesClicked();
        void _onViewPropertiesSubViewExposed(const std::string &viewName);

        //functions
        void _createPropertiesView();

        void _updatePropertiesDataView();
        void _updatePropertiesAssignedTagsView();
        void _updatePropertiesNotesView();
        void _updatePropertiesGroupsAssignedPrivilegesView();
    };
}

#endif // VIEWUSERS_H
