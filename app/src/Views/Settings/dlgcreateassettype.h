#ifndef DLGCREATEASSETTYPE_H
#define DLGCREATEASSETTYPE_H

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
    class DlgCreateAssetType : public Wt::WDialog
    {
    public:
        DlgCreateAssetType();

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
#endif // DLGCREATEASSETTYPE_H
