#ifndef DLGCREATEANDEDITTASKTEMPLATEITEM_H
#define DLGCREATEANDEDITTASKTEMPLATEITEM_H

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
    class DlgCreateAndEditTaskTemplateItem : public Wt::WDialog
    {
    public:
        DlgCreateAndEditTaskTemplateItem(bool editing = false);

        Wt::Dbo::ptr<Projects::ProjectWorkStatus> status() const;
        Wt::Dbo::ptr<Projects::ProjectTaskType> type() const;
        std::string description() const;
        bool isActive() const;

        bool isEditing();

        bool editedType() const;
        bool editedStatus() const;
        bool editedDescription() const;
        bool editedActive() const;

    private:
        //variables
        
        //UI
        Wt::WHBoxLayout *m_layMain;
        Wt::WContainerWidget *m_cntLeft;
        Wt::WVBoxLayout *m_layLeft;
        Wt::WContainerWidget *m_cntRight;
        Wt::WVBoxLayout *m_layRight;
        Wt::WComboBox *m_cmbStatus;
        Wt::WContainerWidget *m_cntCmbStatus;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectWorkStatus>> *m_mdlCmbStatus;
        Wt::WComboBox *m_cmbType;
        Wt::WContainerWidget *m_cntCmbType;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectTaskType>> *m_mdlCmbType;
        Wt::WTextArea *m_txtDescription;
        Wt::WComboBox *m_cmbActive;
        Wt::WPushButton *m_btnOk;
        Wt::WPushButton *m_btnCancel;

        bool m_editing;

        //functions
        void prepareView();
        void createCmbType();
        void createCmbStatus();
        bool validate();

        //slots
        void btnOkClicked();
    };
}

#endif // DLGCREATEANDEDITTASKTEMPLATEITEM_H
