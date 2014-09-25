#ifndef DLGSELECTTASKTEMPLATE_H
#define DLGSELECTTASKTEMPLATE_H

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WComboBox>
#include <Wt/WContainerWidget>
#include <Wt/WTextArea>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/QueryModel>
#include <Wt/Dbo/Query>

#include "../../Database/dbtables.h"

#include <Ms/Widgets/MWidgetFactory.h>

namespace Views
{
    class DlgSelectTaskTemplate : public Wt::WDialog
    {
    public:
        DlgSelectTaskTemplate();

        Wt::Dbo::ptr<Projects::ProjectTaskTemplate> taskTemplate() const;

    private:
        //variables
        Wt::WHBoxLayout *_layMain;
        Wt::WContainerWidget *_cntLeft;
        Wt::WVBoxLayout *_layLeft;
        Wt::WComboBox *_cmbTemplate;
        Wt::WContainerWidget *_cntCmbTemplate;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectTaskTemplate>> *_mdlCmbTemplate;
        Wt::WPushButton *_btnOk;
        Wt::WPushButton *_btnCancel;

        //functions
        void _prepareView();
        void _createCmbTemplate();
        bool _validate();

        //slots
        void _btnOkClicked();
    };
}

#endif // DLGSELECTTASKTEMPLATE_H
