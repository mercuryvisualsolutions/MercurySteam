#ifndef DLGCREATETASKACTIVITYTYPE_H
#define DLGCREATETASKACTIVITYTYPE_H

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
    class DlgCreateTaskActivityType : public Wt::WDialog
    {
    public:
        DlgCreateTaskActivityType();

        //functions
        std::string type() const;
        bool isActive() const;

    private:
        //variables
        Wt::WVBoxLayout *_layMain;
        Wt::WLineEdit *_txtType;
        Wt::WComboBox *_cmbActive;
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

#endif // DLGCREATETASKACTIVITYTYPE_H
