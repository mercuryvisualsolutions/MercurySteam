#ifndef DLGCREATETEMPLATE_H
#define DLGCREATETEMPLATE_H

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
    namespace Dialogs
    {
        class DlgCreateTemplate : public Wt::WDialog
        {
        public:
            DlgCreateTemplate();
            //functions

            std::string name() const;
            bool isActive() const;


        private:
            //variables
            Wt::WVBoxLayout *_layMain;
            Wt::WLineEdit *_txtName;
            Wt::WComboBox *_cmbActive;
            Wt::WPushButton *_btnOk;
            Wt::WPushButton *_btnCancel;

            //functions
            void _prepareView();
            bool _validate();

            //slots
            void _btnOkClicked();
        };
    }
}

#endif // DLGCREATETEMPLATE_H
