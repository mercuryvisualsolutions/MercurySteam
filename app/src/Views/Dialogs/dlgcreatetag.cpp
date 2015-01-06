#include "dlgcreatetag.h"

#include <Wt/WBreak>

Views::Dialogs::DlgCreateTag::DlgCreateTag()
{
    prepareView();
}

std::string Views::Dialogs::DlgCreateTag::tagName() const
{
    return m_txtTagName->text().toUTF8();
}

std::string Views::Dialogs::DlgCreateTag::tagContent() const
{
    return m_txtTagContent->text().toUTF8();
}

bool Views::Dialogs::DlgCreateTag::isActive() const
{
    return m_cmbActive->currentText() == "Yes" ? true : false;
}

void Views::Dialogs::DlgCreateTag::prepareView()
{
    this->setCaption("Add Tag");
    this->rejectWhenEscapePressed();

    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(2);

    this->contents()->setLayout(m_layMain);

    m_txtTagName = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{2,255}", true);
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Name:", m_txtTagName));

    m_layMain->addWidget(new Wt::WBreak());

    m_txtTagContent = Ms::Widgets::MWidgetFactory::createTextArea("", false);
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Content:", m_txtTagContent));

    m_layMain->addWidget(new Wt::WBreak());

    m_cmbActive = new Wt::WComboBox();
    m_cmbActive->addItem("Yes");
    m_cmbActive->addItem("No");
    m_cmbActive->setCurrentIndex(0);
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", m_cmbActive), 1);

    m_layMain->addWidget(new Wt::WBreak(), 1);

    m_btnOk = new Wt::WPushButton("Ok", this->footer());
    m_btnOk->clicked().connect(this, &Views::Dialogs::DlgCreateTag::btnOkClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::Dialogs::DlgCreateTag::validate()
{
    if((m_txtTagName->validate() != Wt::WValidator::Valid) ||
            (m_txtTagContent->validate() != Wt::WValidator::Valid))
        return false;

    return true;
}

void Views::Dialogs::DlgCreateTag::btnOkClicked()
{
    if(validate())
        this->accept();
}
