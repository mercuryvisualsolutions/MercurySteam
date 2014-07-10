#include "dlgcreatetag.h"

#include <Wt/WBreak>

Views::DlgCreateTag::DlgCreateTag()
{
    _prepareView();
}

std::string Views::DlgCreateTag::tagName() const
{
    return _txtTagName->text().toUTF8();
}

std::string Views::DlgCreateTag::tagContent() const
{
    return _txtTagContent->text().toUTF8();
}

bool Views::DlgCreateTag::isActive() const
{
    return _chkActive->isChecked();
}

void Views::DlgCreateTag::_prepareView()
{
    this->setCaption("Add Tag");
    this->rejectWhenEscapePressed();

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(6);

    this->contents()->setLayout(_layMain);

    _txtTagName = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{2,255}", true);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Name:", _txtTagName));

    _layMain->addWidget(new Wt::WBreak());

    _txtTagContent = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{1,255}", true);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Content:", _txtTagContent));

    _layMain->addWidget(new Wt::WBreak());

    _chkActive = new Wt::WCheckBox("Active");
    _chkActive->setChecked(true);

    _layMain->addWidget(_chkActive);

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->setDefault(true);
    _btnOk->clicked().connect(this, &Views::DlgCreateTag::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::DlgCreateTag::_validate()
{
    if(_txtTagName->validate() != Wt::WValidator::Valid &&
            _txtTagContent->validate() != Wt::WValidator::Valid)
        return false;

    return true;
}

void Views::DlgCreateTag::_btnOkClicked()
{
    if(_validate())
        this->accept();
}
