#include "MWidgetFactory.h"

Ms::Widgets::MWidgetFactory::MWidgetFactory()
{
}

void Ms::Widgets::MWidgetFactory::shotInfoMessage(const Wt::WString &title, const Wt::WString &message)
{
    Wt::WMessageBox *msg = new Wt::WMessageBox(title, message, Wt::Information, Wt::Ok);
    msg->buttonClicked().connect(std::bind([=]()
    {
        delete msg;
    }));
    msg->show();
}

void Ms::Widgets::MWidgetFactory::shotWarningMessage(const Wt::WString &title, const Wt::WString &message)
{
    Wt::WMessageBox *msg = new Wt::WMessageBox(title, message, Wt::Warning, Wt::Ok);
    msg->buttonClicked().connect(std::bind([=]()
    {
        delete msg;
    }));
    msg->show();
}

Wt::WPushButton *Ms::Widgets::MWidgetFactory::createButton(const std::string &text, const std::string &iconPath, const std::string &toolTip)
{
    Wt::WPushButton *btn = new Wt::WPushButton(text);
    btn->setToolTip(toolTip);
    if(!iconPath.empty())
    {
        Wt::WLink lnkBtnIcon(iconPath);
        btn->setIcon(lnkBtnIcon);
    }

    return btn;
}

Wt::WSpinBox *Ms::Widgets::MWidgetFactory::createSpinBox(int min, int max, int defaultValue, bool createValidator, bool validatorIsMandatory)
{
    Wt::WSpinBox *spn = new Wt::WSpinBox();
    spn->setMinimumSize(20, 30);
    spn->setMinimum(min);
    spn->setMaximum(max);
    spn->setValue(defaultValue);
    if(createValidator)
        spn->setValidator(createIntValidator(min, max, validatorIsMandatory));

    return spn;
}

Wt::WDoubleSpinBox *Ms::Widgets::MWidgetFactory::createDoubleSpinBox(double min, double max, double defaultValue, bool createValidator, bool validatorIsMandatory)
{
    Wt::WDoubleSpinBox *spn = new Wt::WDoubleSpinBox();
    spn->setMinimumSize(20, 30);
    spn->setMinimum(min);
    spn->setMaximum(max);
    spn->setValue(defaultValue);
    if(createValidator)
        spn->setValidator(createDoubleValidator(min, max, validatorIsMandatory));

    return spn;
}

Wt::WDateEdit *Ms::Widgets::MWidgetFactory::createDateEdit(const std::string &format, const Wt::WDate &bottom, const Wt::WDate &top, const Wt::WDate &defaultDate,  bool createValidator, bool validatorIsMandatory)
{
    Wt::WDateEdit *dat = new Wt::WDateEdit();
    dat->setMinimumSize(20, 30);
    dat->setBottom(bottom);
    dat->setTop(top);
    dat->setDate(defaultDate);
    if(createValidator)
        dat->setValidator(createDateValidator(format, bottom, top, validatorIsMandatory));

    return dat;
}

Wt::WLineEdit *Ms::Widgets::MWidgetFactory::createLineEdit(const std::string &emptyText, bool createValidator, const std::string &validatorExp, bool validatorIsMandatory)
{
    Wt::WLineEdit *edt = new Wt::WLineEdit();
    edt->setMinimumSize(20, 30);
    edt->setEmptyText(emptyText);
    if(createValidator)
        edt->setValidator(createRegExpValidator(validatorExp, validatorIsMandatory));

    return edt;
}

Wt::WTextArea *Ms::Widgets::MWidgetFactory::createTextArea(const std::string &emptyText, bool createValidator, const std::string &validatorExp, bool validatorIsMandatory)
{
    Wt::WTextArea *txt = new Wt::WTextArea();
    txt->setMinimumSize(50, 150);
    txt->setColumns(50);
    txt->setRows(5);
    txt->setEmptyText(emptyText);
    if(createValidator)
        txt->setValidator(createRegExpValidator(validatorExp, validatorIsMandatory));

    return txt;
}

Wt::WContainerWidget *Ms::Widgets::MWidgetFactory::createField(const std::string &label, Wt::WWidget *widget, bool applyValidationStyle)
{
    Wt::WHBoxLayout *layCntField = new Wt::WHBoxLayout();
    layCntField->setContentsMargins(0,0,0,0);
    layCntField->setSpacing(4);
    Wt::WContainerWidget *cntField = new Wt::WContainerWidget();
    cntField->setLayout(layCntField);

    Wt::WLabel *lblField = new Wt::WLabel(label);
    lblField->setLineHeight(37);

    Wt::WContainerWidget *cntLblField = new Wt::WContainerWidget();
    cntLblField->addWidget(lblField);
    cntLblField->setContentAlignment(Wt::AlignRight);

    layCntField->addWidget(cntLblField);

    Wt::WContainerWidget *cntWidget = new Wt::WContainerWidget();
    cntWidget->addWidget(widget);
    cntWidget->setContentAlignment(Wt::AlignRight);
    if(applyValidationStyle)
        cntWidget->setStyleClass("control-group");

    layCntField->addWidget(cntWidget, 1);

    return cntField;
}

Wt::WRegExpValidator *Ms::Widgets::MWidgetFactory::createRegExpValidator(const std::string &exp, bool setMandatory)
{
    Wt::WRegExpValidator *validator = new Wt::WRegExpValidator(exp);
    validator->setMandatory(setMandatory);

    return validator;
}

Wt::WIntValidator *Ms::Widgets::MWidgetFactory::createIntValidator(int min, int max, bool setMandatory)
{
    Wt::WIntValidator *validator = new Wt::WIntValidator(min, max);
    validator->setMandatory(setMandatory);
    return validator;
}

Wt::WDoubleValidator *Ms::Widgets::MWidgetFactory::createDoubleValidator(double min, double max, bool setMandatory)
{
    Wt::WDoubleValidator *validator = new Wt::WDoubleValidator(min, max);
    validator->setMandatory(setMandatory);
    return validator;
}

Wt::WDateValidator *Ms::Widgets::MWidgetFactory::createDateValidator(const Wt::WString &format, const Wt::WDate &bottom, const Wt::WDate &top, bool setMandatory)
{
    Wt::WDateValidator *validator = new Wt::WDateValidator(format, bottom, top);
    validator->setMandatory(setMandatory);
    return validator;
}
