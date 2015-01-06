#include "dlgcreatenote.h"
#include <Wt/WBreak>


Views::Dialogs::DlgCreateNote::DlgCreateNote()
{
    prepareView();
}

std::string Views::Dialogs::DlgCreateNote::content() const
{
    return m_txtContent->text().toUTF8();
}

bool Views::Dialogs::DlgCreateNote::isActive() const
{
    return m_cmbActive->currentText() == "Yes" ? true : false;
}

void Views::Dialogs::DlgCreateNote::prepareView()
{
    this->setCaption("Create Note");
    this->rejectWhenEscapePressed();

    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(2);

    this->contents()->setLayout(m_layMain);

    m_txtContent = Ms::Widgets::MWidgetFactory::createTextArea("", true, "[^$]{0,255}");
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Content:", m_txtContent));

    m_layMain->addWidget(new Wt::WBreak());

    m_cmbActive = new Wt::WComboBox();
    m_cmbActive->addItem("Yes");
    m_cmbActive->addItem("No");
    m_cmbActive->setCurrentIndex(0);
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", m_cmbActive), 1);

    m_layMain->addWidget(new Wt::WBreak(), 1);

    m_btnOk = new Wt::WPushButton("Ok", this->footer());
    m_btnOk->clicked().connect(this, &Views::Dialogs::DlgCreateNote::btnOkClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::Dialogs::DlgCreateNote::validate()
{
    if(m_txtContent->validate() != Wt::WValidator::Valid)
        return false;

    return true;
}

void Views::Dialogs::DlgCreateNote::btnOkClicked()
{
    if(validate())
        this->accept();
}
