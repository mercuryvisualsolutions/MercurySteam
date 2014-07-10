#ifndef DLGCREATEUSER_H
#define DLGCREATEUSER_H

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

#include "Database/dbtables.h"

#include <Ms/Widgets/MWidgetFactory.h>

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
        Wt::WHBoxLayout *_layMain;
        Wt::WContainerWidget *_cntLeft;
        Wt::WVBoxLayout *_layLeft;
        Wt::WContainerWidget *_cntRight;
        Wt::WVBoxLayout *_layRight;
        Wt::WLineEdit *_txtUserName;
        Wt::WLineEdit *_txtPassword;
        Wt::WLineEdit *_txtEmailAddress;
        Wt::WLineEdit *_txtPhoneNumber;
        Wt::WLineEdit *_txtIdNumber;
        Wt::WLineEdit *_txtAddress;
        Wt::WComboBox *_cmbGroups;
        Wt::WContainerWidget *_cntCmbGroups;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Users::Group>> *_mdlCmbGroups;
        Wt::WComboBox *_cmbTitles;
        Wt::WContainerWidget *_cntCmbTitles;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Users::UserTitle>> *_mdlCmbTitles;
        Wt::WCheckBox *_chkActive;
        Wt::WPushButton *_btnOk;
        Wt::WPushButton *_btnCancel;

        //functions
        void _prepareView();
        void _createCmbGroups();
        void _createCmbTitles();
        bool _validate();

        //slots
        void _btnOkClicked();
    };
}
#endif // DLGCREATEUSER_H
