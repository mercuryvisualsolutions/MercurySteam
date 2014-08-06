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
#include "viewprivileges.h"
#include "../Database/viewdbodata.h"
#include "../Database/viewtags.h"
#include "../Database/viewnotes.h"

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

        bool isUsersViewShown();
        bool isGroupsViewShown();

        Ms::Widgets::MQueryTableViewWidget<Users::User> *usersQueryTableView() const;
        Ms::Widgets::MQueryTableViewWidget<Users::Group> *groupsQueryTableView() const;

        //Signals
        Wt::Signal<> &onTabUsersSelected();
        Wt::Signal<> &onTabGroupsSelected();

    private: 
        template<typename T>
        void _addDataToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec);
        template<typename T>
        void _addNoteToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec);
        template<typename T>
        void _addTagToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec);
        template<typename T>
        void _assignTagToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);
        template<typename T>
        void _unAssignTagFromDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);

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
        Wt::WContainerWidget *_cntPropertiesMain;
        Wt::WVBoxLayout *_layCntPropertiesMain;
        Wt::WNavigationBar *_navBarProperties;
        Wt::WMenu *_mnuNavBarProperties;
        Wt::WMenuItem *_mnuNavBarPropertiesDataItem;
        Wt::WMenuItem *_mnuNavBarPropertiesTagsItem;
        Wt::WMenuItem *_mnuNavBarPropertiesNotesItem;
        Wt::WMenuItem *_mnuNavBarPropertiesPrivilegesItem;
        Wt::WStackedWidget *_stkProperties;
        Views::ViewDboData *_viewData;
        Views::ViewTags *_viewTags;
        Views::ViewNotes *_viewNotes;
        Views::ViewPrivileges *_viewPrivileges;

        //slots
        void _mnuNavBarPropertiesDataItemTriggered();
        void _mnuNavBarPropertiesTagsItemTriggered();
        void _mnuNavBarPropertiesNotesItemTriggered();
        void _mnuNavBarPropertiesPrivilegesItemTriggered();

        void _addDataRequested();
        void _removeDataRequested(const std::vector<Wt::Dbo::ptr<Database::DboData>> &dataVec);
        void _createTagRequested();
        void _assignTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);
        void _unassignTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);
        void _filterByTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);
        void _clearTagsFilterRequested();
        void _addNoteRequested();
        void _removeNotesRequested(const std::vector<Wt::Dbo::ptr<Database::Note>> &noteVec);
        void _assignPrivilegesRequested(const std::vector<Wt::Dbo::ptr<Users::Privilege>> &privVec);
        void _unassignPrivilegesRequested(const std::vector<Wt::Dbo::ptr<Users::Privilege>> &privVec);
        void _filterByPrivilegesRequested(const std::vector<Wt::Dbo::ptr<Users::Privilege>> &privVec);
        void _clearFilterPrivilegesFilterRequested();

        //functions
        void _createPropertiesView();

        void _updatePropertiesDataView();
        void _updatePropertiesTagsView();
        void _updatePropertiesAssignedTagsView();
        void _updatePropertiesNotesView();
        void _updatePropertiesPrivilegesView();
        void _updatePropertiesAssignedPrivilegesView();
    };
}

#endif // VIEWUSERS_H
