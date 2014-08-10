#ifndef DLGCREATEWORKSTATUSTYPE_H
#define DLGCREATEWORKSTATUSTYPE_H

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WRegExpValidator>
#include <Wt/WComboBox>
#include <Wt/WCheckBox>
#include <Wt/WVBoxLayout>
#include <Wt/WTextArea>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/QueryModel>
#include <Wt/Dbo/Query>

#include "../../Database/dbtables.h"

#include <Ms/Widgets/MWidgetFactory.h>

namespace Views
{
    class DlgCreateWorkStatus : public Wt::WDialog
    {
    public:
        DlgCreateWorkStatus();

        //functions
        std::string type() const;
        Wt::Dbo::ptr<Projects::ProjectWorkStatusType> belongsToType() const;
        bool isActive() const;

    private:
        //variables
        Wt::WVBoxLayout *_layMain;
        Wt::WLineEdit *_txtType;
        Wt::WComboBox *_cmbType;
        Wt::WContainerWidget *_cntCmbType;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectWorkStatusType>> *_mdlCmbType;
        Wt::WComboBox *_cmbActive;
        Wt::WPushButton *_btnOk;
        Wt::WPushButton *_btnCancel;

        //functions
        void _prepareView();
        void _createCmbType();
        bool _validate();

        //slots
        void _btnOkClicked();
    };
}

#endif // DLGCREATEWORKSTATUSTYPE_H
