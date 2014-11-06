#include "dlgcreatenote.h"
#include <Wt/WBreak>


Views::Dialogs::DlgCreateNote::DlgCreateNote()
{
    _prepareView();
}

std::string Views::Dialogs::DlgCreateNote::content() const
{
    return _txtContent->text().toUTF8();
}

bool Views::Dialogs::DlgCreateNote::isActive() const
{
    return _cmbActive->currentText() == "Yes" ? true : false;
}

void Views::Dialogs::DlgCreateNote::_prepareView()
{
    this->setCaption("Create Note");
    this->rejectWhenEscapePressed();

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(2);

    this->contents()->setLayout(_layMain);

    _txtContent = Ms::Widgets::MWidgetFactory::createTextArea("", true, "[^$]{0,255}");
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Content:", _txtContent));

    _layMain->addWidget(new Wt::WBreak());

    _cmbActive = new Wt::WComboBox();
    _cmbActive->addItem("Yes");
    _cmbActive->addItem("No");
    _cmbActive->setCurrentIndex(0);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", _cmbActive), 1);

    _layMain->addWidget(new Wt::WBreak(), 1);

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->clicked().connect(this, &Views::Dialogs::DlgCreateNote::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::Dialogs::DlgCreateNote::_validate()
{
    if(_txtContent->validate() != Wt::WValidator::Valid)
        return false;

    return true;
}

void Views::Dialogs::DlgCreateNote::_btnOkClicked()
{
    if(_validate())
        this->accept();
}
