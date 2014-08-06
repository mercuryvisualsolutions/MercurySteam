#ifndef VIEWDBODATA_H
#define VIEWDBODATA_H

#include "../../Database/dbtables.h"

#include <Ms/Widgets/MQueryTableViewWidget.h>
#include <Ms/Widgets/MContainerWidget.h>

#include <Wt/WContainerWidget>

namespace Views
{
    class ViewDboData : public Wt::WContainerWidget
    {
    public:
        ViewDboData();

        const Ms::Widgets::MQueryTableViewWidget<Database::DboData> *qtvData() const;

        //Signals
        Wt::Signal<> &addDataRequested();
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::DboData>>> &removeDataRequested();

    private:
        //Variables
        Ms::Widgets::MQueryTableViewWidget<Database::DboData> *_qtvData;
        //UI variables
        Wt::WVBoxLayout *_layMain;

        //Signals
        Wt::Signal<> _addDataRequested;
        Wt::Signal<std::vector<Wt::Dbo::ptr<Database::DboData>>> _removeDataRequested;

        //Slots
        void _btnAddDataClicked();
        void _btnRemoveDataClicked();

        //Functions
        void _createDataTableView();
        void _prepareView();
    };
}

#endif // VIEWDBODATA_H
