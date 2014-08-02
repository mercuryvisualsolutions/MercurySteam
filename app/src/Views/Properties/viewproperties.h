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
        void updateTagsView();
        void updateGroupsPrivilegesView();
        const std::string currentView() const;
        void setCurrentView(const std::string &viewName) const;

        //ui
        Ms::Widgets::MQueryTableViewWidget<Database::DboData> *qtvData();
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *qtvTags();
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *qtvAssignedTags();
        Ms::Widgets::MQueryTableViewWidget<Database::Note> *qtvNotes();
        Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *qtvGroupsPrivileges();
        Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *qtvGroupsAssignedPrivileges();

        //singals
        Wt::Signal<std::string> &viewExposed();
        Wt::Signal<> &addDataRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::DboData>>> &removeDataRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &addTagRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &removeTagRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &filterByTagRequested();
        Wt::Signal<> &clearTagFilterRequested();
        Wt::Signal<> &addNoteRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Note>>> &removeNoteRequested();
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

        Ms::Widgets::MQueryTableViewWidget<Database::DboData> *_qtvData;
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvTags;
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvAssignedTags;
        Ms::Widgets::MQueryTableViewWidget<Database::Note> *_qtvNotes;
        Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *_qtvGroupsPrivileges;
        Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *_qtvGroupsAssignedPrivileges;

        //signals
        Wt::Signal<std::string> _viewExposed;
        Wt::Signal<> _addDataRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::DboData>>> _removeDataRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> _addTagRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> _removeTagRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> _filterByTagRequested;
        Wt::Signal<> _clearTagFilterRequested;
        Wt::Signal<> _addNoteRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Note>>> _removeNoteRequested;
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
        void _btnAddTagClicked();
        void _btnRemoveTagClicked();
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

        template<typename T>
        void _addExtraColumns(Ms::Widgets::MQueryTableViewWidget<T> *widget, Wt::WFlags<Wt::ItemFlag> flags, int editRank);

        void _prepareView();
    };
}

#endif // VIEWPROPERTIES_H
