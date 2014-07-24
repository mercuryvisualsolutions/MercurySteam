#ifndef DLGCREATEDBODATA2_H
#define DLGCREATEDBODATA2_H

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
        class DlgCreateDBOData : public Wt::WDialog
        {
        public:
            DlgCreateDBOData();
            //functions

            std::string key() const;
            std::string value() const;
            bool isActive() const;


        private:
            //variables
            Wt::WVBoxLayout *_layMain;
            Wt::WLineEdit *_txtKey;
            Wt::WLineEdit *_txtValue;
            Wt::WCheckBox *_chkActive;
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

#endif // DLGCREATEDBODATA_H
