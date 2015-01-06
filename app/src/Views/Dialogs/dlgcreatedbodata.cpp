#include "dlgcreatedbodata.h"
#include <Wt/WBreak>


Views::Dialogs::DlgCreateDBOData::DlgCreateDBOData()
{
    prepareView();
}

std::string Views::Dialogs::DlgCreateDBOData::key() const
{
    return m_txtKey->text().toUTF8();
}

std::string Views::Dialogs::DlgCreateDBOData::value() const
{
    return m_txtValue->text().toUTF8();
}

bool Views::Dialogs::DlgCreateDBOData::isActive() const
{
    return m_cmbActive->currentText() == "Yes" ? true : false;
}

void Views::Dialogs::DlgCreateDBOData::prepareView()
{
    this->setCaption("Create Data");
    this->rejectWhenEscapePressed();

    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(2);

    this->contents()->setLayout(m_layMain);

    m_txtKey = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[^$]{0,255}");
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Key:", m_txtKey));

    m_layMain->addWidget(new Wt::WBreak());

    m_txtValue = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[^$]{0,255}");
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Value:", m_txtValue));

    m_layMain->addWidget(new Wt::WBreak());

    m_cmbActive = new Wt::WComboBox();
    m_cmbActive->addItem("Yes");
    m_cmbActive->addItem("No");
    m_cmbActive->setCurrentIndex(0);
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", m_cmbActive), 1);

    m_layMain->addWidget(new Wt::WBreak(), 1);

    m_btnOk = new Wt::WPushButton("Ok", this->footer());
    m_btnOk->clicked().connect(this, &Views::Dialogs::DlgCreateDBOData::btnOkClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::Dialogs::DlgCreateDBOData::validate()
{
    if((m_txtKey->validate() != Wt::WValidator::Valid) ||
            (m_txtValue->validate() != Wt::WValidator::Valid))
        return false;

    return true;
}

void Views::Dialogs::DlgCreateDBOData::btnOkClicked()
{
    if(validate())
        this->accept();
}
