#include "dlgtaskselectdbo.h"
#include <Wt/WBreak>

#include <Ms/Widgets/MWidgetFactory.h>

Views::DlgTaskSelectDbo::DlgTaskSelectDbo()
{
    prepareView();
}

std::string Views::DlgTaskSelectDbo::type() const
{
    return m_cmbType->currentText().toUTF8();
}

void Views::DlgTaskSelectDbo::prepareView()
{
    this->setCaption("Select Task Dbo Type");
    this->rejectWhenEscapePressed();

    m_layMain = new Wt::WVBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(6);

    this->contents()->setLayout(m_layMain);

    m_cmbType = new Wt::WComboBox();
    m_cmbType->addItem("Project");
    m_cmbType->addItem("Sequence");
    m_cmbType->addItem("Shot");
    m_cmbType->addItem("Asset");

    m_cmbType->setCurrentIndex(0);

    m_layMain->addWidget(Ms::Widgets::MWidgetFactory::createField("Dbo Type", m_cmbType, false));

    m_btnOk = new Wt::WPushButton("Ok", this->footer());
    m_btnOk->setDefault(true);
    m_btnOk->clicked().connect(this, &Views::DlgTaskSelectDbo::btnOkClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);
}

void Views::DlgTaskSelectDbo::btnOkClicked()
{
    this->accept();
}
