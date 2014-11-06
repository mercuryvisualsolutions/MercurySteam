#include "dlgcreategroup.h"

#include <Wt/WBreak>

Views::DlgCreateGroup::DlgCreateGroup()
{
    _prepareView();
}

const std::string Views::DlgCreateGroup::groupName() const
{
    return _txtGroupName->text().toUTF8();
}

int Views::DlgCreateGroup::rank() const
{
    return _spnRank->value();
}

bool Views::DlgCreateGroup::isActive() const
{
    return _cmbActive->currentText() == "Yes" ? true : false;
}

void Views::DlgCreateGroup::_prepareView()
{
    this->setCaption("Create Group");
    this->rejectWhenEscapePressed();

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(2);

    this->contents()->setLayout(_layMain);

    _txtGroupName = Ms::Widgets::MWidgetFactory::createLineEdit();
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Name:", _txtGroupName));

    _layMain->addWidget(new Wt::WBreak());

    _spnRank = Ms::Widgets::MWidgetFactory::createSpinBox();
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Rank:", _spnRank));

    _layMain->addWidget(new Wt::WBreak());

    _cmbActive = new Wt::WComboBox();
    _cmbActive->addItem("Yes");
    _cmbActive->addItem("No");
    _cmbActive->setCurrentIndex(0);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", _cmbActive), 1);

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->setDefault(true);
    _btnOk->clicked().connect(this, &Views::DlgCreateGroup::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::DlgCreateGroup::_validate()
{
    if(_txtGroupName->validate() != Wt::WValidator::Valid ||
            _spnRank->validate() != Wt::WIntValidator::Valid)
        return false;

    return true;
}

void Views::DlgCreateGroup::_btnOkClicked()
{
    if(_validate())
        this->accept();
}
