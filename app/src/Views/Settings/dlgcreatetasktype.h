#ifndef DLGCREATETASKTYPE_H
#define DLGCREATETASKTYPE_H

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WRegExpValidator>
#include <Wt/WCheckBox>
#include <Wt/WVBoxLayout>
#include <Wt/WTextArea>
#include <Wt/WComboBox>

#include <Ms/Widgets/MWidgetFactory.h>

namespace Views
{
    class DlgCreateTaskType : public Wt::WDialog
    {
    public:
        DlgCreateTaskType();

        //functions
        std::string type() const;
        bool isActive() const;

    private:
        //variables
        Wt::WVBoxLayout *_layMain;
        Wt::WLineEdit *_txtType;
        Wt::WCheckBox *_chkActive;
        Wt::WPushButton *_btnOk;
        Wt::WPushButton *_btnCancel;

        //functions
        void _prepareView();
        bool _validate();

        //slots
        void _txtTypeKeyWentUp();
        void _btnOkClicked();
    };
}

#endif // DLGCREATETASKTYPE_H
