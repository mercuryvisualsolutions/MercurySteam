#include "dlgcreateassettype.h"
#include <Wt/WBreak>

#include <Ms/Widgets/MWidgetFactory.h>

Views::DlgCreateAssetType::DlgCreateAssetType()
{
    _prepareView();
}

std::string Views::DlgCreateAssetType::type() const
{
    return _txtType->text().toUTF8();
}

bool Views::DlgCreateAssetType::isActive() const
{
    return _cmbActive->currentText() == "Yes" ? true : false;
}

void Views::DlgCreateAssetType::_prepareView()
{
    this->setCaption("Create Asset Type");
    this->rejectWhenEscapePressed();

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(6);

    this->contents()->setLayout(_layMain);

    _txtType = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{2,255}", true);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Type:", _txtType));

    _layMain->addWidget(new Wt::WBreak());

    _cmbActive = new Wt::WComboBox();
    _cmbActive->addItem("Yes");
    _cmbActive->addItem("No");
    _cmbActive->setCurrentIndex(0);
    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", _cmbActive), 1);

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->setDefault(true);
    _btnOk->clicked().connect(this, &Views::DlgCreateAssetType::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::DlgCreateAssetType::_validate()
{
    if(_txtType->validate() != Wt::WValidator::Valid)
        return false;

    return true;
}

void Views::DlgCreateAssetType::_btnOkClicked()
{
    if(_validate())
        this->accept();
}
