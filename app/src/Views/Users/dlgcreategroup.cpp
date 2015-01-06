#include "dlgcreategroup.h"

#include <Wt/WBreak>

Views::DlgCreateGroup::DlgCreateGroup()
{
    prepareView();
}

const std::string Views::DlgCreateGroup::groupName() const
{
    return m_txtGroupName->text().toUTF8();
}

int Views::DlgCreateGroup::rank() const
{
    return m_spnRank->value();
}

bool Views::DlgCreateGroup::isActive() const
{
    return m_cmbActive->currentText() == "Yes" ? true : false;
}

void Views::DlgCreateGroup::prepareView()
{
    this->setCaption("Create Group");
    this->rejectWhenEscapePressed();

    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(2);

    this->contents()->setLayout(m_layMain);

    m_txtGroupName = Ms::Widgets::MWidgetFactory::createLineEdit();
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Name:", m_txtGroupName));

    m_layMain->addWidget(new Wt::WBreak());

    m_spnRank = Ms::Widgets::MWidgetFactory::createSpinBox();
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Rank:", m_spnRank));

    m_layMain->addWidget(new Wt::WBreak());

    m_cmbActive = new Wt::WComboBox();
    m_cmbActive->addItem("Yes");
    m_cmbActive->addItem("No");
    m_cmbActive->setCurrentIndex(0);
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", m_cmbActive), 1);

    m_btnOk = new Wt::WPushButton("Ok", this->footer());
    m_btnOk->setDefault(true);
    m_btnOk->clicked().connect(this, &Views::DlgCreateGroup::btnOkClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::DlgCreateGroup::validate()
{
    if(m_txtGroupName->validate() != Wt::WValidator::Valid ||
            m_spnRank->validate() != Wt::WIntValidator::Valid)
        return false;

    return true;
}

void Views::DlgCreateGroup::btnOkClicked()
{
    if(validate())
        this->accept();
}
