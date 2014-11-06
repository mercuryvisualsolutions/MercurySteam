#include "dlgcreateusertitle.h"

#include <Wt/WBreak>

Views::DlgCreateUserTitle::DlgCreateUserTitle()
{
    _prepareView();
}

std::string Views::DlgCreateUserTitle::title() const
{
    return _txtTitle->text().toUTF8();
}

bool Views::DlgCreateUserTitle::isActive() const
{
    return _cmbActive->currentText() == "Yes" ? true : false;
}

void Views::DlgCreateUserTitle::_prepareView()
{
    this->setCaption("Add User Title");
    this->rejectWhenEscapePressed();

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(2);

    this->contents()->setLayout(_layMain);

    _txtTitle = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{2,255}", true);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Title:", _txtTitle));

    _layMain->addWidget(new Wt::WBreak());

    _cmbActive = new Wt::WComboBox();
    _cmbActive->addItem("Yes");
    _cmbActive->addItem("No");
    _cmbActive->setCurrentIndex(0);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", _cmbActive), 1);

    _layMain->addWidget(new Wt::WBreak(), 1);

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->setDefault(true);
    _btnOk->clicked().connect(this, &Views::DlgCreateUserTitle::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::DlgCreateUserTitle::_validate()
{
    if(_txtTitle->validate() != Wt::WValidator::Valid)
        return false;

    return true;
}

void Views::DlgCreateUserTitle::_btnOkClicked()
{
    if(_validate())
        this->accept();
}
