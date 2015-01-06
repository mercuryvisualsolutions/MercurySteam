#ifndef DLGCREATEUSER_H
#define DLGCREATEUSER_H

#include "../../Session/sessionmanager.h"
#include "Database/dbtables.h"

#include <Ms/Widgets/MWidgetFactory.h>

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WRegExpValidator>
#include <Wt/WSpinBox>
#include <Wt/WCheckBox>
#include <Wt/WVBoxLayout>
#include <Wt/WTextArea>
#include <Wt/WComboBox>
#include <Wt/Dbo/QueryModel>

namespace Views
{
/*****************************Users***************************************/
    class DlgCreateUser : public Wt::WDialog
    {
    public:
        DlgCreateUser();
        //functions
        std::string userName() const;
        std::string password() const;
        std::string emailAddress() const;
        std::string phoneNumber() const;
        std::string idNumber() const;
        std::string address() const;
        Wt::Dbo::ptr<Users::Group> group() const;
        Wt::Dbo::ptr<Users::UserTitle> title() const;
        bool isActive() const;


    private:
        //variables
        
        //UI
        Wt::WHBoxLayout *m_layMain;
        Wt::WContainerWidget *m_cntLeft;
        Wt::WVBoxLayout *m_layLeft;
        Wt::WContainerWidget *m_cntRight;
        Wt::WVBoxLayout *m_layRight;
        Wt::WLineEdit *m_txtUserName;
        Wt::WLineEdit *m_txtPassword;
        Wt::WLineEdit *m_txtEmailAddress;
        Wt::WLineEdit *m_txtPhoneNumber;
        Wt::WLineEdit *m_txtIdNumber;
        Wt::WLineEdit *m_txtAddress;
        Wt::WComboBox *m_cmbGroups;
        Wt::WContainerWidget *m_cntCmbGroups;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Users::Group>> *m_mdlCmbGroups;
        Wt::WComboBox *m_cmbTitles;
        Wt::WContainerWidget *m_cntCmbTitles;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Users::UserTitle>> *m_mdlCmbTitles;
        Wt::WComboBox *m_cmbActive;
        Wt::WPushButton *m_btnOk;
        Wt::WPushButton *m_btnCancel;

        //functions
        void prepareView();
        void createCmbGroups();
        void createCmbTitles();
        bool validate();

        //slots
        void btnOkClicked();
    };
}
#endif // DLGCREATEUSER_H
