#ifndef DLGCREATETAG_H
#define DLGCREATETAG_H

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
    namespace Dialogs
    {
        class DlgCreateTag : public Wt::WDialog
        {
        public:
            DlgCreateTag();

            //functions
            std::string tagName() const;
            std::string tagContent() const;
            bool isActive() const;

        private:
            //variables
            Wt::WVBoxLayout *_layMain;
            Wt::WLineEdit *_txtTagName;
            Wt::WLineEdit *_txtTagContent;
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
#endif // DLGCREATETAG_H
