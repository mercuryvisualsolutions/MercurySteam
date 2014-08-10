#include "dlgcreatedbodata.h"
#include <Wt/WBreak>


Views::Dialogs::DlgCreateDBOData::DlgCreateDBOData()
{
    _prepareView();
}

std::string Views::Dialogs::DlgCreateDBOData::key() const
{
    return _txtKey->text().toUTF8();
}

std::string Views::Dialogs::DlgCreateDBOData::value() const
{
    return _txtValue->text().toUTF8();
}

bool Views::Dialogs::DlgCreateDBOData::isActive() const
{
    return _cmbActive->currentText() == "Yes" ? true : false;
}

void Views::Dialogs::DlgCreateDBOData::_prepareView()
{
    this->setCaption("Create Data");
    this->rejectWhenEscapePressed();

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(6);

    this->contents()->setLayout(_layMain);

    _txtKey = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[^$]{0,255}");
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Key:", _txtKey));

    _txtValue = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[^$]{0,255}");
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Value:", _txtValue));

    _layMain->addWidget(new Wt::WBreak());

    _cmbActive = new Wt::WComboBox();
    _cmbActive->addItem("Yes");
    _cmbActive->addItem("No");
    _cmbActive->setCurrentIndex(0);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", _cmbActive), 1);

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->setDefault(true);
    _btnOk->clicked().connect(this, &Views::Dialogs::DlgCreateDBOData::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::Dialogs::DlgCreateDBOData::_validate()
{
    if((_txtKey->validate() != Wt::WValidator::Valid) ||
            (_txtValue->validate() != Wt::WValidator::Valid))
        return false;

    return true;
}

void Views::Dialogs::DlgCreateDBOData::_btnOkClicked()
{
    if(_validate())
        this->accept();
}
