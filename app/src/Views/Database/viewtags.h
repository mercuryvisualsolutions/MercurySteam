#ifndef VIEWTAGS_H
#define VIEWTAGS_H

#include "../../Database/dbtables.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>

namespace Views
{
    class ViewTags : public Wt::WContainerWidget
    {
    public:
        ViewTags();

        const Ms::Widgets::MQueryTableViewWidget<Database::Tag> *qtvTags() const;
        const Ms::Widgets::MQueryTableViewWidget<Database::Tag> *qtvAssignedTags() const;
        bool isCreateOptionHidden() const;
        void setCreateOptionHidden(bool hidden);

        //Signals
        Wt::Signal<> &createTagRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &assignTagsRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &unassignTagsRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> &filterByTagsRequested();
        Wt::Signal<> &clearTagsFilterRequested();

    private:
        //Variables
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvTags;
        Ms::Widgets::MQueryTableViewWidget<Database::Tag> *_qtvAssignedTags;

        //UI variables
        Wt::WVBoxLayout *_layMain;
        Wt::WContainerWidget *_cntTags;
        Wt::WVBoxLayout *_layCntTags;
        Ms::Widgets::MContainerWidget *_cntAssignedTags;
        Ms::Widgets::MContainerWidget *_cntAvailableTags;
        Wt::WPushButton *_btnCreateTag;

        //Signals
        Wt::Signal<> _createTagRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> _assignTagsRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> _unassignTagsRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::Tag>>> _filterByTagsRequested;
        Wt::Signal<> _clearTagsFilterRequested;

        //Slots
        void _btnCreateTagClicked();
        void _btnAssignTagsClicked();
        void _btnUnassignTagsClicked();
        void _btnTagsFilterClicked();
        void _btnClearTagsFilterClicked();

        //Functions
        void _createTagsTableView();
        void _createAssignedTagsTableView();
        void _prepareView();
    };
}

#endif // VIEWTAGS_H
