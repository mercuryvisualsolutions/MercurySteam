#include "dlgcreateandeditsequence.h"

#include <Wt/WBreak>
#include <Wt/WCheckBox>

#include "../../Database/dbosession.h"
#include "../../Settings/appsettings.h"

Views::DlgCreateAndEditSequence::DlgCreateAndEditSequence(bool editing) :
  m_editing(editing)
{
    prepareView();
}

std::string Views::DlgCreateAndEditSequence::sequenceName() const
{
    return m_txtSequenceName->text().toUTF8();
}

Wt::WDate Views::DlgCreateAndEditSequence::startDate() const
{
    return m_datStartDate->date();
}

Wt::WDate Views::DlgCreateAndEditSequence::endDate() const
{
    return m_datEndDate->date();
}

int Views::DlgCreateAndEditSequence::duration() const
{
    return m_spnDuration->value();
}

float Views::DlgCreateAndEditSequence::fps() const
{
    return m_spnFPS->value();
}

int Views::DlgCreateAndEditSequence::frameWidth() const
{
    return m_spnWidth->value();
}

int Views::DlgCreateAndEditSequence::frameHeight() const
{
    return m_spnHeight->value();
}

int Views::DlgCreateAndEditSequence::priority() const
{
    return m_spnPriority->value();
}

Wt::Dbo::ptr<Projects::ProjectWorkStatus> Views::DlgCreateAndEditSequence::status() const
{
    return m_mdlCmbStatus->resultRow(m_cmbStatus->currentIndex());
}

std::string Views::DlgCreateAndEditSequence::description() const
{
    return m_txtDescription->text().toUTF8();
}

bool Views::DlgCreateAndEditSequence::isActive() const
{
    return m_cmbActive->currentText() == "Yes" ? true : false;
}

bool Views::DlgCreateAndEditSequence::isEditing()
{
    return m_editing;
}

bool Views::DlgCreateAndEditSequence::editedStartDate() const
{
    if(m_editing)
    {
        if(m_datStartDate->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditSequence::editedEndDate() const
{
    if(m_editing)
    {
        if(m_datEndDate->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditSequence::editedDuration() const
{
    if(m_editing)
    {
        if(m_spnDuration->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditSequence::editedFps() const
{
    if(m_editing)
    {
        if(m_spnFPS->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditSequence::editedFrameWidth() const
{
    if(m_editing)
    {
        if(m_spnWidth->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditSequence::editedFrameHeight() const
{
    if(m_editing)
    {
        if(m_spnHeight->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditSequence::editedPriority() const
{
    if(m_editing)
    {
        if(m_spnPriority->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditSequence::editedStatus() const
{
    if(m_editing)
    {
        if(m_cmbStatus->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditSequence::editedDescription() const
{
    if(m_editing)
    {
        if(m_txtDescription->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Views::DlgCreateAndEditSequence::editedActive() const
{
    if(m_editing)
    {
        if(m_cmbActive->isEnabled())
            return true;
        else
            return false;
    }
    else
        return false;
}

void Views::DlgCreateAndEditSequence::prepareView()
{
    Wt::Dbo::Transaction transaction(Session::SessionManager::instance().dboSession());

    if(!m_editing)
        this->setCaption("Create Sequence");
    else
        this->setCaption("Edit Sequences");

    this->rejectWhenEscapePressed();

    m_layMain = new Wt::WHBoxLayout();
    m_layMain->setContentsMargins(0,0,0,0);
    m_layMain->setSpacing(2);

    this->contents()->setLayout(m_layMain);

    m_layLeft = new Wt::WVBoxLayout();
    m_layLeft->setContentsMargins(0,0,0,0);
    m_layLeft->setSpacing(2);

    m_cntLeft = new Wt::WContainerWidget();
    m_cntLeft->setLayout(m_layLeft);

    m_layMain->addWidget(m_cntLeft);

    m_layRight = new Wt::WVBoxLayout();
    m_layRight->setContentsMargins(16,0,0,0);
    m_layRight->setSpacing(1);

    m_cntRight = new Wt::WContainerWidget();
    m_cntRight->setLayout(m_layRight);

    m_layMain->addWidget(m_cntRight);

    if(!m_editing)
    {
        m_txtSequenceName = Ms::Widgets::MWidgetFactory::createLineEdit("", true, "[A-Za-z0-9 _-]{4,150}");
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Name:", m_txtSequenceName));

        m_layLeft->addWidget(new Wt::WBreak());
    }

    m_datStartDate = Ms::Widgets::MWidgetFactory::createDateEdit();
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Start Date:", m_datStartDate));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Start Date:", m_datStartDate));

    m_layLeft->addWidget(new Wt::WBreak());

    m_datEndDate = Ms::Widgets::MWidgetFactory::createDateEdit();
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("End Date:", m_datEndDate));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("End Date:", m_datEndDate));

    m_layLeft->addWidget(new Wt::WBreak());

    m_spnDuration = Ms::Widgets::MWidgetFactory::createSpinBox(0, INT_MAX, 0);
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Duration In Frames:", m_spnDuration));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Duration In Frames:", m_spnDuration));

    m_layLeft->addWidget(new Wt::WBreak());

    m_spnFPS = Ms::Widgets::MWidgetFactory::createDoubleSpinBox(1.0, DBL_MAX, 25.0);
    m_spnFPS->setValidator(Ms::Widgets::MWidgetFactory::createDoubleValidator(1.0, DBL_MAX));
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("FPS:", m_spnFPS));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("FPS:", m_spnFPS));

    m_layLeft->addWidget(new Wt::WBreak());

    m_spnWidth = Ms::Widgets::MWidgetFactory::createSpinBox(1, INT_MAX, 1920);
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Frame Width:", m_spnWidth));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Frame Width:", m_spnWidth));

    m_layLeft->addWidget(new Wt::WBreak());

    m_spnHeight = Ms::Widgets::MWidgetFactory::createSpinBox(1, INT_MAX, 1080);
    if(m_editing)
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Frame Height:", m_spnHeight));
    else
        m_layLeft->addWidget(Ms::Widgets::MWidgetFactory::createField("Frame Height:", m_spnHeight));

    m_layLeft->addWidget(new Wt::WBreak(), 1);

    m_spnPriority = Ms::Widgets::MWidgetFactory::createSpinBox(0, INT_MAX, 0);
    if(m_editing)
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Priority:", m_spnPriority));
    else
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Priority:", m_spnPriority));

    m_layRight->addWidget(new Wt::WBreak());

    createCmbStatus();
    if(m_editing)
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Status:", m_cntCmbStatus));
    else
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Status:", m_cntCmbStatus));

    m_layRight->addWidget(new Wt::WBreak());

    m_txtDescription = Ms::Widgets::MWidgetFactory::createTextArea("", false);
    if(m_editing)
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Description:", m_txtDescription));
    else
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Description:", m_txtDescription));

    m_layRight->addWidget(new Wt::WBreak());

    m_cmbActive = new Wt::WComboBox();
    m_cmbActive->addItem("Yes");
    m_cmbActive->addItem("No");
    m_cmbActive->setCurrentIndex(0);
    if(m_editing)
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createEditField("Active:", m_cmbActive));
    else
        m_layRight->addWidget(Ms::Widgets::MWidgetFactory::createField("Active:", m_cmbActive));

    m_layRight->addWidget(new Wt::WBreak(), 1);

    m_btnOk = new Wt::WPushButton("Ok", this->footer());
    m_btnOk->clicked().connect(this, &Views::DlgCreateAndEditSequence::btnOkClicked);

    m_btnCancel = new Wt::WPushButton("Cancel", this->footer());
    m_btnCancel->clicked().connect(this, &Wt::WDialog::reject);
    m_btnCancel->setFocus();

    transaction.commit();
}

void Views::DlgCreateAndEditSequence::btnOkClicked()
{
    if(validate())
        this->accept();
}

void Views::DlgCreateAndEditSequence::createCmbStatus()
{
    m_cmbStatus = new Wt::WComboBox();
    m_cmbStatus->setMinimumSize(20, 30);
    m_cntCmbStatus = new Wt::WContainerWidget();
    m_cntCmbStatus->addWidget(m_cmbStatus);

    m_mdlCmbStatus = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<Projects::ProjectWorkStatus>>();

    Wt::Dbo::Query<Wt::Dbo::ptr<Projects::ProjectWorkStatus>> query;
    if(AppSettings::instance().isLoadInactiveData())
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>();
    else
        query = Session::SessionManager::instance().dboSession().find<Projects::ProjectWorkStatus>().where("Active = ?").bind(true);

    m_mdlCmbStatus->setQuery(query);

    m_mdlCmbStatus->reload();

    m_mdlCmbStatus->addColumn("Status", Wt::ItemIsSelectable);

    m_cmbStatus->setModel(m_mdlCmbStatus);

    if(m_mdlCmbStatus->rowCount() > 0)
        m_cmbStatus->setCurrentIndex(0);
}

bool Views::DlgCreateAndEditSequence::validate()
{
    bool result = true;

    if(!m_editing)
    {
        if((m_txtSequenceName->validate() != Wt::WValidator::Valid) ||
                (m_datStartDate->validate() != Wt::WDateValidator::Valid) ||
                (m_datEndDate->validate() != Wt::WDateValidator::Valid) ||
                (m_spnDuration->validate() != Wt::WIntValidator::Valid) ||
                (m_spnFPS->validate() != Wt::WDoubleValidator::Valid) ||
                (m_spnWidth->validate() != Wt::WIntValidator::Valid) ||
                (m_spnHeight->validate() != Wt::WIntValidator::Valid) ||
                (m_spnPriority->validate() != Wt::WIntValidator::Valid) ||
                (m_cmbStatus->currentIndex() == -1))
        {
            result = false;
        }
    }
    else
    {
        if(((m_datStartDate->isEnabled()) && (m_datStartDate->validate() != Wt::WValidator::Valid)) ||
                ((m_datEndDate->isEnabled()) && (m_datEndDate->validate() != Wt::WValidator::Valid)) ||
                ((m_spnDuration->isEnabled()) && (m_spnDuration->validate() != Wt::WValidator::Valid)) ||
                ((m_spnFPS->isEnabled()) && (m_spnFPS->validate() != Wt::WValidator::Valid)) ||
                ((m_spnWidth->isEnabled()) && (m_spnWidth->validate() != Wt::WValidator::Valid)) ||
                ((m_spnHeight->isEnabled()) && (m_spnHeight->validate() != Wt::WValidator::Valid)) ||
                ((m_spnPriority->isEnabled()) && (m_spnPriority->validate() != Wt::WIntValidator::Valid)) ||
                ((m_cmbStatus->isEnabled()) && (m_cmbStatus->currentIndex() == -1)))
        {
            result = false;
        }
    }

    return result;
}
