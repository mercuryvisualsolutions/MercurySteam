#ifndef DLGCREATEWORKSTATUSTYPE_H
#define DLGCREATEWORKSTATUSTYPE_H

#include "../../Session/sessionmanager.h"
#include "../../Database/dbtables.h"

#include <Ms/Widgets/MWidgetFactory.h>

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WRegExpValidator>
#include <Wt/WComboBox>
#include <Wt/WCheckBox>
#include <Wt/WVBoxLayout>
#include <Wt/WTextArea>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/QueryModel>
#include <Wt/Dbo/Query>

namespace Views
{
    class DlgCreateWorkStatus : public Wt::WDialog
    {
    public:
        DlgCreateWorkStatus();

        //functions
        std::string type() const;
        Wt::Dbo::ptr<Projects::ProjectWorkStatusType> belongsToType() const;
        bool isActive() const;

    private:
        //variables
        
        //UI
        Wt::WVBoxLayout *m_layMain;
        Wt::WLineEdit *m_txtType;
        Wt::WComboBox *m_cmbType;
        Wt::WContainerWidget *m_cntCmbType;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectWorkStatusType>> *m_mdlCmbType;
        Wt::WComboBox *m_cmbActive;
        Wt::WPushButton *m_btnOk;
        Wt::WPushButton *m_btnCancel;

        //functions
        void prepareView();
        void createCmbType();
        bool validate();

        //slots
        void btnOkClicked();
    };
}

#endif // DLGCREATEWORKSTATUSTYPE_H
