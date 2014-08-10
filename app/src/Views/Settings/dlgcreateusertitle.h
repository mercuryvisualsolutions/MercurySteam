#ifndef DLGCREATETITLE_H
#define DLGCREATETITLE_H

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WRegExpValidator>
#include <Wt/WSpinBox>
#include <Wt/WCheckBox>
#include <Wt/WVBoxLayout>
#include <Wt/WTextArea>

#include <Ms/Widgets/MWidgetFactory.h>

namespace Views
{
    class DlgCreateUserTitle : public Wt::WDialog
    {
    public:
        DlgCreateUserTitle();

        //functions
        std::string title() const;
        bool isActive() const;

    private:
        //variables
        Wt::WVBoxLayout *_layMain;
        Wt::WLineEdit *_txtTitle;
        Wt::WComboBox *_cmbActive;
        Wt::WPushButton *_btnOk;
        Wt::WPushButton *_btnCancel;

        //functions
        void _prepareView();
        bool _validate();

        //slots
        void _txtTitleKeyWentUp();
        void _btnOkClicked();
    };
}
#endif // DLGCREATETITLE_H
