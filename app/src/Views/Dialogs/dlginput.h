#ifndef DLGINPUT_H
#define DLGINPUT_H

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WRegExpValidator>
#include <Wt/WSpinBox>
#include <Wt/WCheckBox>
#include <Wt/WVBoxLayout>
#include <Wt/WTextArea>

namespace Views
{
    class DlgInput : public Wt::WDialog
    {
    public:
        DlgInput();
        //functions

        //variables
        const std::string text() const;

    private:
        //variables
        Wt::WVBoxLayout *_layMain;
        Wt::WLineEdit *_text;
        Wt::WPushButton *_btnOk;
        Wt::WPushButton *_btnCancel;

        void _prepareView();
        bool _validate();

        //slots
        void _txtTypeKeyWentUp();
        void _btnOkClicked();
    };
}
#endif // DLGINPUT_H
