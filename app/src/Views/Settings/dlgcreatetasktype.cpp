#include "dlgcreatetasktype.h"

#include <Wt/WBreak>

Views::DlgCreateTaskType::DlgCreateTaskType()
{
    _prepareView();
}

std::string Views::DlgCreateTaskType::type() const
{
    return _txtType->text().toUTF8();
}

bool Views::DlgCreateTaskType::isActive() const
{
    return _chkActive->isChecked();
}

void Views::DlgCreateTaskType::_prepareView()
{
    this->setCaption("Create Task Type");
    this->rejectWhenEscapePressed();

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(6);

    this->contents()->setLayout(_layMain);

    _txtType = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{2,255}", true);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Type:", _txtType));

    _layMain->addWidget(new Wt::WBreak());

    _chkActive = new Wt::WCheckBox("Active");
    _chkActive->setChecked(true);

    _layMain->addWidget(_chkActive);

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->setDefault(true);
    _btnOk->clicked().connect(this, &Views::DlgCreateTaskType::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::DlgCreateTaskType::_validate()
{
    if(_txtType->validate() != Wt::WValidator::Valid)
        return false;

    return true;
}

void Views::DlgCreateTaskType::_btnOkClicked()
{
    if(_validate())
        this->accept();
}
