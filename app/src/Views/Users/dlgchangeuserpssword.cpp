#include "dlgchangeuserpssword.h"

#include <Wt/WBreak>
#include <Wt/Auth/UpdatePasswordWidget>

Views::DlgChangeUserPassword::DlgChangeUserPassword()
{
    _prepareView();
}

const std::string Views::DlgChangeUserPassword::password() const
{
    return _txtPass->text().toUTF8();
}

void Views::DlgChangeUserPassword::_prepareView()
{
    this->setCaption("Change Password");
    this->rejectWhenEscapePressed();

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(6);

    this->contents()->setLayout(_layMain);

    _txtPass = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9_-]{6,14}");
    _txtPass->setEchoMode(Wt::WLineEdit::Password);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Password:", _txtPass));

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->setDefault(true);
    _btnOk->clicked().connect(this, &Views::DlgChangeUserPassword::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);

}

bool Views::DlgChangeUserPassword::_validate()
{
    if(_txtPass->validate() != Wt::WValidator::Valid)
        return false;

    return true;
}

void Views::DlgChangeUserPassword::_btnOkClicked()
{
    if(_validate())
        this->accept();
}
