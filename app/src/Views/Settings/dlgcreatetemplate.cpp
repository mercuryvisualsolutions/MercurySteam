#include "dlgcreatetemplate.h"
#include <Wt/WBreak>


Views::Dialogs::DlgCreateTemplate::DlgCreateTemplate()
{
    prepareView();
}

std::string Views::Dialogs::DlgCreateTemplate::name() const
{
    return m_txtName->text().toUTF8();
}

bool Views::Dialogs::DlgCreateTemplate::isActive() const
{
    return m_cmbActive->currentText() == "Yes" ? true : false;
}

void Views::Dialogs::DlgCreateTemplate::prepareView()
{
    this->setCaption("Create Template");
    this->rejectWhenEscapePressed();

    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(2);

    this->contents()->setLayout(m_layMain);

    m_txtName = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[^$]{0,255}");
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Name:", m_txtName));

    m_layMain->addWidget(new Wt::WBreak());

    m_cmbActive = new Wt::WComboBox();
    m_cmbActive->addItem("Yes");
    m_cmbActive->addItem("No");
    m_cmbActive->setCurrentIndex(0);
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", m_cmbActive), 1);

    m_layMain->addWidget(new Wt::WBreak(), 1);

    m_btnOk = new Wt::WPushButton("Ok", this->footer());
    m_btnOk->clicked().connect(this, &Views::Dialogs::DlgCreateTemplate::btnOkClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::Dialogs::DlgCreateTemplate::validate()
{
    if((m_txtName->validate() != Wt::WValidator::Valid))
        return false;

    return true;
}

void Views::Dialogs::DlgCreateTemplate::btnOkClicked()
{
    if(validate())
        this->accept();
}
