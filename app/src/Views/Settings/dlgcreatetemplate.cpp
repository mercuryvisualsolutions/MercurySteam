#include "dlgcreatetemplate.h"
#include <Wt/WBreak>


Views::Dialogs::DlgCreateTemplate::DlgCreateTemplate()
{
    _prepareView();
}

std::string Views::Dialogs::DlgCreateTemplate::name() const
{
    return _txtName->text().toUTF8();
}

bool Views::Dialogs::DlgCreateTemplate::isActive() const
{
    return _cmbActive->currentText() == "Yes" ? true : false;
}

void Views::Dialogs::DlgCreateTemplate::_prepareView()
{
    this->setCaption("Create Template");
    this->rejectWhenEscapePressed();

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(6);

    this->contents()->setLayout(_layMain);

    _txtName = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[^$]{0,255}");
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Name:", _txtName));

    _layMain->addWidget(new Wt::WBreak());

    _cmbActive = new Wt::WComboBox();
    _cmbActive->addItem("Yes");
    _cmbActive->addItem("No");
    _cmbActive->setCurrentIndex(0);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", _cmbActive), 1);

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->clicked().connect(this, &Views::Dialogs::DlgCreateTemplate::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::Dialogs::DlgCreateTemplate::_validate()
{
    if((_txtName->validate() != Wt::WValidator::Valid))
        return false;

    return true;
}

void Views::Dialogs::DlgCreateTemplate::_btnOkClicked()
{
    if(_validate())
        this->accept();
}
