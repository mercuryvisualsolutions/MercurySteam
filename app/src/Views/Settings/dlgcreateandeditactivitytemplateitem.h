#ifndef DLGCREATEANDEDITACTIVITYTEMPLATEITEM_H
#define DLGCREATEANDEDITACTIVITYTEMPLATEITEM_H

#include "../../Session/sessionmanager.h"
#include "../../Database/dbtables.h"

#include <Ms/Widgets/MWidgetFactory.h>

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

namespace Views
{
    class DlgCreateAndEditActivityTemplateItem : public Wt::WDialog
    {
    public:
        DlgCreateAndEditActivityTemplateItem(bool editing = false);

        Wt::Dbo::ptr<Projects::ProjectWorkStatus> status() const;
        Wt::Dbo::ptr<Projects::ProjectTaskActivityType> type() const;
        int hours() const;
        std::string description() const;
        bool isActive() const;

        bool isEditing();

        bool editedType() const;
        bool editedStatus() const;
        bool editedHours() const;
        bool editedDescription() const;
        bool editedActive() const;

    private:
        //variables
        
        //UI
        Wt::WHBoxLayout *_layMain;
        Wt::WContainerWidget *_cntLeft;
        Wt::WVBoxLayout *_layLeft;
        Wt::WContainerWidget *_cntRight;
        Wt::WVBoxLayout *_layRight;
        Wt::WComboBox *_cmbStatus;
        Wt::WContainerWidget *_cntCmbStatus;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectWorkStatus>> *_mdlCmbStatus;
        Wt::WComboBox *_cmbType;
        Wt::WContainerWidget *_cntCmbType;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectTaskActivityType>> *_mdlCmbType;
        Wt::WSpinBox *_spnHours;
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

#endif // DLGCREATEANDEDITACTIVITYTEMPLATEITEM_H
