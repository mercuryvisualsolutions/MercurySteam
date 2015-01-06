#include "dlgcreatetasktype.h"

#include <Wt/WBreak>

Views::DlgCreateTaskType::DlgCreateTaskType()
{
    prepareView();
}

std::string Views::DlgCreateTaskType::type() const
{
    return m_txtType->text().toUTF8();
}

bool Views::DlgCreateTaskType::isActive() const
{
    return m_cmbActive->currentText() == "Yes" ? true : false;
}

void Views::DlgCreateTaskType::prepareView()
{
    this->setCaption("Create Task Type");
    this->rejectWhenEscapePressed();

    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(2);

    this->contents()->setLayout(m_layMain);

    m_txtType = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{2,255}", true);
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Type:", m_txtType));

    m_layMain->addWidget(new Wt::WBreak());

    m_cmbActive = new Wt::WComboBox();
    m_cmbActive->addItem("Yes");
    m_cmbActive->addItem("No");
    m_cmbActive->setCurrentIndex(0);
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", m_cmbActive), 1);

    m_layMain->addWidget(new Wt::WBreak(), 1);

    m_btnOk = new Wt::WPushButton("Ok", this->footer());
    m_btnOk->setDefault(true);
    m_btnOk->clicked().connect(this, &Views::DlgCreateTaskType::btnOkClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::DlgCreateTaskType::validate()
{
    if(m_txtType->validate() != Wt::WValidator::Valid)
        return false;

    return true;
}

void Views::DlgCreateTaskType::btnOkClicked()
{
    if(validate())
        this->accept();
}
