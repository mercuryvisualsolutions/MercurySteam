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
        Wt::WVBoxLayout *_layMain;
        Wt::WLineEdit *_txtPass;
        Wt::WPushButton *_btnOk;
        Wt::WPushButton *_btnCancel;

        //functions
        void _prepareView();
        bool _validate();

        //slots
        void _btnOkClicked();
    };
}

#endif // DLGCHANGEUSERPASSWORD_H
