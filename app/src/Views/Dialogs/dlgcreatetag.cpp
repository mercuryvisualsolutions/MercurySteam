#include "dlgcreatetag.h"

#include <Wt/WBreak>

Views::Dialogs::DlgCreateTag::DlgCreateTag()
{
    _prepareView();
}

std::string Views::Dialogs::DlgCreateTag::tagName() const
{
    return _txtTagName->text().toUTF8();
}

std::string Views::Dialogs::DlgCreateTag::tagContent() const
{
    return _txtTagContent->text().toUTF8();
}

bool Views::Dialogs::DlgCreateTag::isActive() const
{
    return _cmbActive->currentText() == "Yes" ? true : false;
}

void Views::Dialogs::DlgCreateTag::_prepareView()
{
    this->setCaption("Add Tag");
    this->rejectWhenEscapePressed();

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(2);

    this->contents()->setLayout(_layMain);

    _txtTagName = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{2,255}", true);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Name:", _txtTagName));

    _layMain->addWidget(new Wt::WBreak());

    _txtTagContent = Ms::Widgets::MWidgetFactory::createTextArea("", false);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Content:", _txtTagContent));

    _layMain->addWidget(new Wt::WBreak());

    _cmbActive = new Wt::WComboBox();
    _cmbActive->addItem("Yes");
    _cmbActive->addItem("No");
    _cmbActive->setCurrentIndex(0);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", _cmbActive), 1);

    _layMain->addWidget(new Wt::WBreak(), 1);

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->clicked().connect(this, &Views::Dialogs::DlgCreateTag::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::Dialogs::DlgCreateTag::_validate()
{
    if((_txtTagName->validate() != Wt::WValidator::Valid) ||
            (_txtTagContent->validate() != Wt::WValidator::Valid))
        return false;

    return true;
}

void Views::Dialogs::DlgCreateTag::_btnOkClicked()
{
    if(_validate())
        this->accept();
}
