#include "dlgcreaterepo.h"

#include <Wt/WBreak>

#include <Ms/Widgets/MWidgetFactory.h>

Views::DlgCreateRepo::DlgCreateRepo()
{
    prepareView();
}

const std::string Views::DlgCreateRepo::text() const
{
    return m_lnEdit->text().toUTF8();
}

void Views::DlgCreateRepo::prepareView()
{
    this->setCaption("Create New");
    this->rejectWhenEscapePressed();

    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(6);

    this->contents()->setLayout(m_layMain);

    m_lnEdit = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{2,255}", true);
    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Name:", m_lnEdit));

    m_layMain->addWidget(new Wt::WBreak());

    m_btnOk = new Wt::WPushButton("Ok", this->footer());
    m_btnOk->setDefault(true);
    m_btnOk->clicked().connect(this, &Views::DlgCreateRepo::btnOkClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

bool Views::DlgCreateRepo::validate()
{
    if(m_lnEdit->validate() != Wt::WValidator::Valid)
        return false;

    return true;
}

void Views::DlgCreateRepo::btnOkClicked()
{
    if(validate())
        this->accept();
}
