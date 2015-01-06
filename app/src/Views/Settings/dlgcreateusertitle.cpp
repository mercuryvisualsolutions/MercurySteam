#include "dlgcreateusertitle.h"

#include <Wt/WBreak>

Views::DlgCreateUserTitle::DlgCreateUserTitle()
{
    prepareView();
}

std::string Views::DlgCreateUserTitle::title() const
{
    return m_txtTitle->text().toUTF8();
}

bool Views::DlgCreateUserTitle::isActive() const
{
    return m_cmbActive->currentText() == "Yes" ? true : false;
}

void Views::DlgCreateUserTitle::prepareView()
{
    this->setCaption("Add User Title");
    this->rejectWhenEscapePressed();

    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(2);

    this->contents()->setLayout(m_layMain);

    m_txtTitle = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{2,255}", true);
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Title:", m_txtTitle));

    m_layMain->addWidget(new Wt::WBreak());

    m_cmbActive = new Wt::WComboBox();
    m_cmbActive->addItem("Yes");
    m_cmbActive->addItem("No");
    m_cmbActive->setCurrentIndex(0);
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", m_cmbActive), 1);

    m_layMain->addWidget(new Wt::WBreak(), 1);

    m_btnOk = new Wt::WPushButton("Ok", this->footer());
    m_btnOk->setDefault(true);
    m_btnOk->clicked().connect(this, &Views::DlgCreateUserTitle::btnOkClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::DlgCreateUserTitle::validate()
{
    if(m_txtTitle->validate() != Wt::WValidator::Valid)
        return false;

    return true;
}

void Views::DlgCreateUserTitle::btnOkClicked()
{
    if(validate())
        this->accept();
}
