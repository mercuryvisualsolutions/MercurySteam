#ifndef VIEWUSERS_H
#define VIEWUSERS_H

#include "../../Database/dbtables.h"
#include "../../Log/logger.h"
#include "../../Session/sessionmanager.h"
#include "viewprivileges.h"
#include "../Database/viewdbodata.h"
#include "../Database/viewtags.h"
#include "../Database/viewnotes.h"
#include "../../Auth/authmanager.h"

#include <Ms/Widgets/MWidgetFactory.h>
#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

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

namespace Views
{
    //using userData = boost::tuple<Wt::Dbo::ptr<Users::User>, Wt::Dbo::ptr<Auth::AuthInfo>, Wt::Dbo::ptr<Auth::AuthInfo::AuthIdentityType>>;

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

        void adjustUIPrivileges(Wt::Dbo::ptr<Users::User> user);

        //Signals
        Wt::Signal<> &onTabUsersSelected();
        Wt::Signal<> &onTabGroupsSelected();

    private: 
        template<typename T>
        void addDataToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec);
        template<typename T>
        void addNoteToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec);
        template<typename T>
        void assignTagToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);
        template<typename T>
        void unAssignTagFromDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);

        //variables
        
        Log::Logger *m_logger;
        Ms::Widgets::MPropertiesPanel *m_propertiesPanel;

        //Singals
        Wt::Signal<> m_onTabUsersSelected;
        Wt::Signal<> m_onTabGroupsSelected;

        /*******************--UI--********************/
        Wt::WNavigationBar *m_navBarMain;//main navigation bar
        Wt::WStackedWidget *m_stkMain;
        Wt::WMenu *m_mnuMain;
        Wt::WMenuItem *m_mnuMainUsersItem;
        Wt::WMenuItem *m_mnuMainGroupsItem;

        /*******************--Main--********************/
        //functions
        void mnuMainUsersItemTriggered();
        void mnuMainGroupsItemTriggered();
        void prepareView();

        /*******************--Users--********************/
        Ms::Widgets::MQueryTableViewWidget<Users::User> *m_qtvUsers;
        Wt::WPushButton *m_btnCreateUser;
        Wt::WPushButton *m_btnChangeUserPassword;
        Wt::WPushButton *m_btnImportUsersThumbnails;
        //slots
        void btnUsersCreateClicked();
        void btnUsersChangePasswordClicked();
        void btnUsersRemoveClicked();
        void btnUsersImportThumbnailsClicked();

        //functions
        void createUsersTableView();

        /*******************--Groups--********************/
        Ms::Widgets::MQueryTableViewWidget<Users::Group> *m_qtvGroups;
        Wt::WPushButton *m_btnCreateGroup;
        //slots
        void btnGroupsCreateClicked();
        void btnGroupsRemoveClicked();

        //functions
        void createGroupsTableView();

        /*******************--Properties--********************/
        //Properties
        Wt::WContainerWidget *m_cntPropertiesMain;
        Wt::WVBoxLayout *m_layCntPropertiesMain;
        Wt::WNavigationBar *m_navBarProperties;
        Wt::WMenu *m_mnuNavBarProperties;
        Wt::WMenuItem *m_mnuNavBarPropertiesDataItem;
        Wt::WMenuItem *m_mnuNavBarPropertiesTagsItem;
        Wt::WMenuItem *m_mnuNavBarPropertiesNotesItem;
        Wt::WMenuItem *m_mnuNavBarPropertiesPrivilegesItem;
        Wt::WStackedWidget *m_stkProperties;
        Views::ViewDboData *m_viewData;
        Views::ViewTags *m_viewTags;
        Views::ViewNotes *m_viewNotes;
        Views::ViewPrivileges *m_viewPrivileges;

        //slots
        void mnuNavBarPropertiesDataItemTriggered();
        void mnuNavBarPropertiesTagsItemTriggered();
        void mnuNavBarPropertiesNotesItemTriggered();
        void mnuNavBarPropertiesPrivilegesItemTriggered();

        void addDataRequested();
        void removeDataRequested(const std::vector<Wt::Dbo::ptr<Database::DboData>> &dataVec);
        void createTagRequested();
        void assignTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);
        void unassignTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec);
        void filterByTagsRequested(const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec, bool exactSelection, bool inverse);
        void clearTagsFilterRequested();
        void addNoteRequested();
        void removeNotesRequested(const std::vector<Wt::Dbo::ptr<Database::Note>> &noteVec);
        void assignPrivilegesRequested(const std::vector<Wt::Dbo::ptr<Users::Privilege>> &privVec);
        void unassignPrivilegesRequested(const std::vector<Wt::Dbo::ptr<Users::Privilege>> &privVec);
        void filterByPrivilegesRequested(const std::vector<Wt::Dbo::ptr<Users::Privilege>> &privVec, bool exactSelection, bool inverse);
        void clearFilterPrivilegesFilterRequested();

        //functions
        void createPropertiesView();

        void updatePropertiesDataView();
        void updatePropertiesTagsView();
        void updatePropertiesAssignedTagsView();
        void updatePropertiesNotesView();
        void updatePropertiesPrivilegesView();
        void updatePropertiesAssignedPrivilegesView();
    };
}

#endif // VIEWUSERS_H
