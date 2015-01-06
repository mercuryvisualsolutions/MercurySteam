#ifndef DLGSELECTACTIVITYTEMPLATE_H
#define DLGSELECTACTIVITYTEMPLATE_H

#include "../../Session/sessionmanager.h"
#include "../../Database/dbtables.h"

#include <Ms/Widgets/MWidgetFactory.h>

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

namespace Views
{
    class DlgSelectActivityTemplate : public Wt::WDialog
    {
    public:
        DlgSelectActivityTemplate();

        Wt::Dbo::ptr<Projects::ProjectActivityTemplate> activityTemplate();

    private:
        //variables
        
        //UI
        Wt::WHBoxLayout *m_layMain;
        Wt::WContainerWidget *m_cntLeft;
        Wt::WVBoxLayout *m_layLeft;
        Wt::WComboBox *m_cmbTemplate;
        Wt::WContainerWidget *m_cntCmbTemplate;
        Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectActivityTemplate>> *m_mdlCmbTemplate;
        Wt::WPushButton *m_btnOk;
        Wt::WPushButton *m_btnCancel;

        //functions
        void prepareView();
        void createCmbTemplate();
        bool validate();

        //slots
        void btnOkClicked();
    };
}

#endif // DLGSELECTACTIVITYTEMPLATE_H
