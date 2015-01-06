#include "dlgchangeuserpssword.h"

#include <Wt/WBreak>
#include <Wt/Auth/UpdatePasswordWidget>

Views::DlgChangeUserPassword::DlgChangeUserPassword()
{
    prepareView();
}

const std::string Views::DlgChangeUserPassword::password() const
{
    return m_txtPass->text().toUTF8();
}

void Views::DlgChangeUserPassword::prepareView()
{
    this->setCaption("Change Password");
    this->rejectWhenEscapePressed();

    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(6);

    this->contents()->setLayout(m_layMain);

    m_txtPass = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9_-]{6,14}");
    m_txtPass->setEchoMode(Wt::WLineEdit::Password);
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Password:", m_txtPass));

    m_btnOk = new Wt::WPushButton("Ok", this->footer());
    m_btnOk->setDefault(true);
    m_btnOk->clicked().connect(this, &Views::DlgChangeUserPassword::btnOkClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);

}

bool Views::DlgChangeUserPassword::validate()
{
    if(m_txtPass->validate() != Wt::WValidator::Valid)
        return false;

    return true;
}

void Views::DlgChangeUserPassword::btnOkClicked()
{
    if(validate())
        this->accept();
}
