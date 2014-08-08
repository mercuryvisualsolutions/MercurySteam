#include "dlgtaskselectdbo.h"
#include <Wt/WBreak>

#include <Ms/Widgets/MWidgetFactory.h>

Views::DlgTaskSelectDbo::DlgTaskSelectDbo()
{
    _prepareView();
}

std::string Views::DlgTaskSelectDbo::type() const
{
    return _cmbType->currentText().toUTF8();
}

void Views::DlgTaskSelectDbo::_prepareView()
{
    this->setCaption("Select Task Dbo Type");
    this->rejectWhenEscapePressed();

    _layMain = new Wt::WVBoxLayout();
    _layMain->setContentsMargins(0,0,0,0);
    _layMain->setSpacing(6);

    this->contents()->setLayout(_layMain);

    _cmbType = new Wt::WComboBox();
    _cmbType->addItem("Project");
    _cmbType->addItem("Sequence");
    _cmbType->addItem("Shot");
    _cmbType->addItem("Asset");

    _cmbType->setCurrentIndex(0);

    _layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Dbo Type", _cmbType, false));

    _btnOk = new Wt::WPushButton("Ok", this->footer());
    _btnOk->setDefault(true);
    _btnOk->clicked().connect(this, &Views::DlgTaskSelectDbo::_btnOkClicked);

    _btnCancel = new Wt::WPushButton("Cancel", this->footer());
    _btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

void Views::DlgTaskSelectDbo::_btnOkClicked()
{
    this->accept();
}
