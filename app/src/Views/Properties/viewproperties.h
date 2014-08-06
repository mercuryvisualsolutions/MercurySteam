#ifndef VIEWPROPERTIES_H
#define VIEWPROPERTIES_H

#include "../../Database/dbtables.h"

#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WMenu>
#include <Wt/WMenuItem>
#include <Wt/WStackedWidget>

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

namespace Views
{
    class ViewProperties : public Wt::WContainerWidget
    {
    public:
        ViewProperties();

        //functions
        void setViewHidden(const std::string &viewName, bool hidden) const;
        void updateGroupsPrivilegesView();
        const std::string currentView() const;
        void setCurrentView(const std::string &viewName) const;

        //ui
        //global
        Ms::Widgets::MQueryTableViewWidget<Database::DboData> *qtvData();
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *qtvTags();
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *qtvAssignedTags();
        Ms::Widgets::MQueryTableViewWidget<Database::Note> *qtvNotes();

        //users
        Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *qtvGroupsPrivileges();
        Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *qtvGroupsAssignedPrivileges();

        //singals
        //global
        Wt::Signal<std::string> &viewExposed();
        Wt::Signal<> &addDataRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::DboData>>> &removeDataRequested();
        Wt::Signal<> &createTagRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &assignTagRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &unAssignTagRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &filterByTagRequested();
        Wt::Signal<> &clearTagFilterRequested();
        Wt::Signal<> &addNoteRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Note>>> &removeNoteRequested();

        //users
        Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> &addGroupPrivilegeRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> &removeGroupPrivilegeRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> &filterGroupByPrivilegeRequested();
        Wt::Signal<> &clearGroupPrivilegeFilterRequested();

    private:
        //variables
        //ui
        Wt::WVBoxLayout *_layCntMain;
        Wt::WNavigationBar *_navBarMain;
        Wt::WMenu *_mnuNavBar;
        Wt::WMenuItem *_mnuNavBarDataItem;
        Wt::WMenuItem *_mnuNavBarTagsItem;
        Wt::WMenuItem *_mnuNavBarNotesItem;
        Wt::WMenuItem *_mnuNavBarGroupsPrivilegesItem;
        Wt::WStackedWidget *_stkMain;

        Wt::WContainerWidget *_cntTags;
        Wt::WVBoxLayout *_layCntTags;
        Ms::Widgets::MContainerWidget *_cntAssignedTags;
        Ms::Widgets::MContainerWidget *_cntAvailableTags;

        Wt::WContainerWidget *_cntGroupsPrivileges;
        Wt::WVBoxLayout *_layCntGroupsPrivileges;
        Ms::Widgets::MContainerWidget *_cntGroupsAssignedPrivileges;
        Ms::Widgets::MContainerWidget *_cntGroupsAvailablePrivileges;
        Wt::WContainerWidget *_cntNoProperties;

        //global
        Ms::Widgets::MQueryTableViewWidget<Database::DboData> *_qtvData;
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvTags;
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvAssignedTags;
        Ms::Widgets::MQueryTableViewWidget<Database::Note> *_qtvNotes;

        //users
        Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *_qtvGroupsPrivileges;
        Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *_qtvGroupsAssignedPrivileges;

        //signals
        //global
        Wt::Signal<std::string> _viewExposed;
        Wt::Signal<> _addDataRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::DboData>>> _removeDataRequested;
        Wt::Signal<> _createTagRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> _assignTagRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> _unAssignTagRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> _filterByTagRequested;
        Wt::Signal<> _clearTagFilterRequested;
        Wt::Signal<> _addNoteRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Note>>> _removeNoteRequested;

        //users
        Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> _addGroupPrivilegeRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> _removeGroupPrivilegeRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Users::Privilege>>> _filterGroupByPrivilegeRequested;
        Wt::Signal<> _clearGroupPrivilegeFilterRequested;

        //slots
        void _mnuNavBarDataItemTriggered();
        void _mnuNavBarTagsItemTriggered();
        void _mnuNavBarNotesItemTriggered();
        void _mnuNavBarGroupsPrivilegesItemTriggered();
        void _btnAddDataClicked();
        void _btnRemoveDataClicked();
        void _btnCreateTagClicked();
        void _btnAssignTagClicked();
        void _btnUnAssignTagClicked();
        void _btnFilterTagClicked();
        void _btnClearFilterTagClicked();
        void _btnAddNoteClicked();
        void _btnRemoveNoteClicked();
        void _btnAddGroupsPrivilegesClicked();
        void _btnRemoveGroupsPrivilegesClicked();
        void _btnFilterGroupsPrivilegesClicked();
        void _btnClearFilterGroupsPrivilegesClicked();

        //functions
        void _createDataTableView();
        void _createTagsTableView();
        void _createAssignedTagsTableView();
        void _createNotesTableView();
        void _createGroupsPrivilegesTableView();
        void _createGroupsAssignedPrivilegesTableView();

        void _prepareView();
    };
}

#endif // VIEWPROPERTIES_H
