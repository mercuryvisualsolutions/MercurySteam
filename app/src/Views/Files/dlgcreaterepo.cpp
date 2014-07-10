#include "dlgcreaterepo.h"

#include <Wt/WBreak>

#include <Ms/Widgets/MWidgetFactory.h>

Views::DlgCreateRepo::DlgCreateRepo()
{
    _prepareView();
}

const std::string Views::DlgCreateRepo::text() const
{
    return _lnEdit->text().toUTF8();
}

void Views::DlgCreateRepo::_prepareView()
{
    this->setCaption("Create New");
    this->rejectWhenEscapePressed();

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(6);

    this->contents()->setLayout(_layMain);

    _lnEdit = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{2,255}", true);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Name:", _lnEdit));

    _layMain->addWidget(new Wt::WBreak());

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->setDefault(true);
    _btnOk->clicked().connect(this, &Views::DlgCreateRepo::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::DlgCreateRepo::_validate()
{
    if(_lnEdit->validate() != Wt::WValidator::Valid)
        return false;

    return true;
}

void Views::DlgCreateRepo::_btnOkClicked()
{
    if(_validate())
        this->accept();
}
