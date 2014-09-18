#ifndef DLGCREATETASKACTIVITY_H
#define DLGCREATETASKACTIVITY_H

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WSpinBox>
#include <Wt/WCheckBox>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WDateEdit>
#include <Wt/WComboBox>
#include <Wt/WContainerWidget>
#include <Wt/WTextArea>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/QueryModel>
#include <Wt/Dbo/Query>

#include "../../Database/dbtables.h"

#include <Ms/Widgets/MWidgetFactory.h>

namespace Views
{
    class DlgCreateAndEditTaskActivity : public Wt::WDialog
    {
    public:
        DlgCreateAndEditTaskActivity(bool editing = false);

        std::string assetName() const;
        Wt::Dbo::ptr<Projects::ProjectWorkStatus> status() const;
        Wt::Dbo::ptr<Projects::ProjectTaskActivityType> assetType() const;
        Wt::WDate startDate() const;
        Wt::WDate endDate() const;
        int priority() const;
        std::string description() const;
        bool isActive() const;

        bool isEditing();

        bool editedStartDate() const;
        bool editedEndDate() const;
        bool editedPriority() const;
        bool editedType() const;
        bool editedStatus() const;
        bool editedDescription() const;
        bool editedActive() const;

    private:
        //variables
        Wt::WHBoxLayout *_layMain;
        Wt::WContainerWidget *_cntLeft;
        Wt::WVBoxLayout *_layLeft;
        Wt::WContainerWidget *_cntRight;
        Wt::WVBoxLayout *_layRight;
        Wt::WLineEdit *_txtAssetName;
        Wt::WDateEdit *_datStartDate;
        Wt::WDateEdit *_datEndDate;
        Wt::WSpinBox *_spnPriority;
        Wt::WComboBox *_cmbStatus;
        Wt::WContainerWidget *_cntCmbStatus;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectWorkStatus>> *_mdlCmbStatus;
        Wt::WComboBox *_cmbType;
        Wt::WContainerWidget *_cntCmbType;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectTaskActivityType>> *_mdlCmbType;
        Wt::WTextArea *_txtDescription;
        Wt::WComboBox *_cmbActive;
        Wt::WPushButton *_btnOk;
        Wt::WPushButton *_btnCancel;

        bool _editing;

        //functions
        void _prepareView();
        void _createCmbType();
        void _createCmbStatus();
        bool _validate();

        //slots
        void _btnOkClicked();
    };
}

#endif // DLGCREATETASKACTIVITY_H