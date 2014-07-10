#include "dlgcreatenote.h"
#include <Wt/WBreak>


Views::DlgCreateNote::DlgCreateNote()
{
    _prepareView();
}

std::string Views::DlgCreateNote::content() const
{
    return _txtContent->text().toUTF8();
}

bool Views::DlgCreateNote::isActive() const
{
    return _chkActive->isChecked();
}

void Views::DlgCreateNote::_prepareView()
{
    this->setCaption("Create Note");
    this->rejectWhenEscapePressed();

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(6);

    this->contents()->setLayout(_layMain);

    _txtContent = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[^$]{0,255}");
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Content:", _txtContent));

    _layMain->addWidget(new Wt::WBreak());

    _chkActive = new Wt::WCheckBox("Active");
    _chkActive->setChecked(true);

    _layMain->addWidget(_chkActive);

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->setDefault(true);
    _btnOk->clicked().connect(this, &Views::DlgCreateNote::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::DlgCreateNote::_validate()
{
    if(_txtContent->validate() != Wt::WValidator::Valid)
        return false;

    return true;
}

void Views::DlgCreateNote::_btnOkClicked()
{
    if(_validate())
        this->accept();
}
