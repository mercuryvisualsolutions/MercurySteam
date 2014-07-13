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

        //variables
        Ms::Log::MLogger *_logger;

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
    };
}

#endif // VIEWUSERS_H
