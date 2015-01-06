#ifndef DLGCHANGEUSERPASSWORD_H
#define DLGCHANGEUSERPASSWORD_H

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WRegExpValidator>
#include <Wt/WCheckBox>
#include <Wt/WVBoxLayout>
#include <Wt/WTextArea>

#include <Ms/Widgets/MWidgetFactory.h>

namespace Views
{
    class DlgChangeUserPassword : public Wt::WDialog
    {
    public:
        DlgChangeUserPassword();
        //functions
        const std::string password() const;

    private:
        //variables
        Wt::WVBoxLayout *m_layMain;
        Wt::WLineEdit *m_txtPass;
        Wt::WPushButton *m_btnOk;
        Wt::WPushButton *m_btnCancel;

        //functions
        void prepareView();
        bool validate();

        //slots
        void btnOkClicked();
    };
}

#endif // DLGCHANGEUSERPASSWORD_H
