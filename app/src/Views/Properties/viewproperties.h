#ifndef VIEWPROPERTIES_H
#define VIEWPROPERTIES_H

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

#include "../../Database/databasemanager.h"
#include "../../Users/usersmanager.h"
#include "../../Projects/projectsmanager.h"
#include "../../Settings/appsettings.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>

namespace Views
{
    class ViewProperties : public Wt::WContainerWidget
    {
    public:
        ViewProperties();

        const Ms::Widgets::MQueryTableViewWidget<Database::DboData> *dataQueryTableView() const;
        const Ms::Widgets::MQueryTableViewWidget<Database::Tag> *tagsQueryTableView() const;
        const Ms::Widgets::MQueryTableViewWidget<Database::Tag> *assignedTagsQueryTableView() const;
        const Ms::Widgets::MQueryTableViewWidget<Database::Note> *notesQueryTableView() const;
        const Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *groupsPrivilegesQueryTableView() const;
        const Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *groupsAssignedPrivilegesQueryTableView() const;

        template<typename T>
        void updateDboPropertiesView(const std::vector<Wt::Dbo::ptr<T>> &dboVec) const;

        template<typename T>
        void updateDboDataView(const std::vector<Wt::Dbo::ptr<T>> &dboVec) const;

        template<typename T>
        void updateDboTagsView(const std::vector<Wt::Dbo::ptr<T>> &dboVec) const;

        template<typename T>
        void updateDboNotesView(const std::vector<Wt::Dbo::ptr<T>> &dboVec) const;

        void updateTagsView() const;
        void updateGroupsPrivilegesView() const;
        void updateGroupsAssignedPrivilegesView(std::vector<Wt::Dbo::ptr<Users::Group>> groupVec) const;

        template<typename T>
        void addDataToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec) const;

        template<typename T>
        void addNoteToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec) const;

        template<typename T>
        void addTagsToDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec) const;
        template<typename T>
        void removeTagsFromDbo(const std::vector<Wt::Dbo::ptr<T>> &dboVec, const std::vector<Wt::Dbo::ptr<Database::Tag>> &tagVec) const;

        //Signals
        Wt::Signal<> &onAddData();
        Wt::Signal<> &onRemoveData();
        Wt::Signal<> &onAddTag();
        Wt::Signal<> &onRemoveTag();
        Wt::Signal<> &onAddNote();
        Wt::Signal<> &onRemoveNote();
        Wt::Signal<> &onAddPrivilegeToGroup();
        Wt::Signal<> &onRemovePrivilegeFromGroup();
        Wt::Signal<> &onTabDataSelected();
        Wt::Signal<> &onTabTagsSelected();
        Wt::Signal<> &onTabNotesSelected();
        Wt::Signal<> &onTabGroupsPrivilegesSelected();

        void setSubViewHidden(const std::string &viewName, bool hidden);

    private:
        //variables
        //signals
        Wt::Signal<> _onAddData;
        Wt::Signal<> _onRemoveData;
        Wt::Signal<> _onAddTag;
        Wt::Signal<> _onRemoveTag;
        Wt::Signal<> _onAddNote;
        Wt::Signal<> _onRemoveNote;
        Wt::Signal<> _onAddPrivilegeToGroup;
        Wt::Signal<> _onRemovePrivilegeFromGroup;
        Wt::Signal<> _onTabDataSelected;
        Wt::Signal<> _onTabTagsSelected;
        Wt::Signal<> _onTabNotesSelected;
        Wt::Signal<> _onTabGroupsPrivilegesSelected;

        //UI
        Wt::WText *_txtPropertiesTitle;
        Wt::WContainerWidget *_cntTxtPropertiesTitle;
        Wt::WNavigationBar *_navBarPropertiesMain;
        Wt::WMenu *_mnuPropertiesNavBar;
        Wt::WMenuItem *_mnuPropertiesNavBarDataItem;
        Wt::WMenuItem *_mnuPropertiesNavBarTagsItem;
        Wt::WMenuItem *_mnuPropertiesNavBarNotesItem;
        Wt::WMenuItem *_mnuPropertiesNavBarGroupsPrivilegesItem;
        Wt::WVBoxLayout *_layMain;
        Wt::WStackedWidget *_stkProperties;

        Wt::WContainerWidget *_cntTags;
        Wt::WVBoxLayout *_layCntTags;
        Wt::WContainerWidget *_cntAssignedTags;
        Wt::WVBoxLayout *_layCntAssignedTags;
        Wt::WText *_txtAssignedTagsLabel;
        Wt::WContainerWidget *_cntTxtAssignedTagsLabel;
        Wt::WContainerWidget *_cntAvailableTags;
        Wt::WVBoxLayout *_layCntAvailableTags;
        Wt::WText *_txtAvailableTagsLabel;
        Wt::WContainerWidget *_cntTxtAvailableTagsLabel;

        Wt::WContainerWidget *_cntGroupsPrivileges;
        Wt::WVBoxLayout *_layCntGroupsPrivileges;
        Wt::WContainerWidget *_cntGroupsAssignedPrivileges;
        Wt::WVBoxLayout *_layCntGroupsAssignedPrivileges;
        Wt::WText *_txtGroupsAssignedPrivilegesLabel;
        Wt::WContainerWidget *_cntTxtGroupsAssignedPrivilegesLabel;
        Wt::WContainerWidget *_cntGroupsAvailablePrivileges;
        Wt::WVBoxLayout *_layCntGroupsAvailablePrivileges;
        Wt::WText *_txtGroupsAvailablePrivilegesLabel;
        Wt::WContainerWidget *_cntTxtGroupsAvailablePrivilegesLabel;

        Ms::Widgets::MQueryTableViewWidget<Database::DboData> *_qtvData;
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvTags;
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvAssignedTags;
        Ms::Widgets::MQueryTableViewWidget<Database::Note> *_qtvNotes;
        Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *_qtvGroupsPrivileges;
        Ms::Widgets::MQueryTableViewWidget<Users::Privilege> *_qtvGroupsAssignedPrivileges;

        Wt::WContainerWidget *_cntNoPropertiesView;//used when there are no properties to show

        //functions

        void _createDataTableView();
        void _createTagsTableView();
        void _createAssignedTagsTableView();
        void _createNotesTableView();
        void _createGroupsPrivilegesTableView();
        void _createGroupsAssignedPrivilegesTableView();
        void _mnuPropertiesNavBarDataItemTriggered();
        void _mnuPropertiesNavBarTagsItemTriggered();
        void _mnuPropertiesNavBarNotesItemTriggered();
        void _mnuPropertiesNavBarGroupsPrivilegesItemTriggered();
        void _btnAddDataClicked();
        void _btnRemoveDataClicked();
        void _btnAddTagClicked();
        void _btnRemoveTagClicked();
        void _btnAddNoteClicked();
        void _btnRemoveNoteClicked();
        void _btnAddGroupPrivilegeClicked();
        void _btnRemoveGroupPrivilegeClicked();

        template<typename T>
        void _addExtraColumns(Ms::Widgets::MQueryTableViewWidget<T> *widget, Wt::WFlags<Wt::ItemFlag> flags, int editRank) const;
        void _prepareView();
    };
}

#include "viewproperties_impl.h"

#endif // VIEWPROPERTIES_H
