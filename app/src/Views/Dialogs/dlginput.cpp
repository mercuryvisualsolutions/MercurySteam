#include "dlginput.h"

#include <Wt/WBreak>

#include <Ms/Widgets/MWidgetFactory.h>

Views::Dialogs::DlgInput::DlgInput()
{
    _prepareView();
}

const std::string Views::Dialogs::DlgInput::text() const
{
    return _text->text().toUTF8();
}

void Views::Dialogs::DlgInput::_prepareView()
{
    this->setCaption("Input");
    this->rejectWhenEscapePressed();

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(6);

    this->contents()->setLayout(_layMain);

    _text = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{2,255}", true);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Text:", _text));

    _layMain->addWidget(new Wt::WBreak());

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->setDefault(true);
    _btnOk->clicked().connect(this, &Views::Dialogs::DlgInput::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::Dialogs::DlgInput::_validate()
{
    if(_text->validate() != Wt::WValidator::Valid)
        return false;

    return true;
}

void Views::Dialogs::DlgInput::_btnOkClicked()
{
    if(_validate())
        this->accept();
}
