#include "dlginput.h"

#include <Wt/WBreak>

#include <Ms/Widgets/MWidgetFactory.h>

Views::Dialogs::DlgInput::DlgInput()
{
    prepareView();
}

const std::string Views::Dialogs::DlgInput::text() const
{
    return m_text->text().toUTF8();
}

void Views::Dialogs::DlgInput::prepareView()
{
    this->setCaption("Input");
    this->rejectWhenEscapePressed();

    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(6);

    this->contents()->setLayout(m_layMain);

    m_text = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{2,255}", true);
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Text:", m_text));

    m_layMain->addWidget(new Wt::WBreak());

    m_btnOk = new Wt::WPushButton("Ok", this->footer());
    m_btnOk->clicked().connect(this, &Views::Dialogs::DlgInput::btnOkClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::Dialogs::DlgInput::validate()
{
    if(m_text->validate() != Wt::WValidator::Valid)
        return false;

    return true;
}

void Views::Dialogs::DlgInput::btnOkClicked()
{
    if(validate())
        this->accept();
}
