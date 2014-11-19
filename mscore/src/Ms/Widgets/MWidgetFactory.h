#ifndef MWIDGETFACTORY_H
#define MWIDGETFACTORY_H

#include <iostream>

#include <Wt/WMessageBox>
#include <Wt/WString>
#include <Wt/WPushButton>
#include <Wt/WLink>
#include <Wt/WObject>
#include <Wt/WLabel>
#include <Wt/WHBoxLayout>
#include <Wt/WContainerWidget>
#include <Wt/WRegExpValidator>
#include <Wt/WIntValidator>
#include <Wt/WDoubleValidator>
#include <Wt/WSpinBox>
#include <Wt/WDoubleSpinBox>
#include <Wt/WDateEdit>
#include <Wt/WDateValidator>
#include <Wt/WLineEdit>
#include <Wt/WTextArea>

#include "MQueryTableViewWidget.h"

namespace Ms
{
    namespace Widgets
    {
        class MWidgetFactory
        {
        public:
            MWidgetFactory();

            //messages
            static void shotInfoMessage(const Wt::WString &title, const Wt::WString &message);
            static void shotWarningMessage(const Wt::WString &title, const Wt::WString &message);

            //widgets
            static Wt::WPushButton *createButton(const std::string &text, const std::string &iconPath = "", const std::string &toolTip = "");
            static Wt::WSpinBox *createSpinBox(int min = INT_MIN, int max = INT_MAX, int defaultValue = 0, bool createValidator = true, bool validatorIsMandatory = true);
            static Wt::WDoubleSpinBox *createDoubleSpinBox(double min = DBL_MIN, double max = DBL_MAX, double defaultValue = 0, bool createValidator = true, bool validatorIsMandatory = true);
            static Wt::WDateEdit *createDateEdit(const std::string &format = "dd/MM/yyyy", const Wt::WDate &bottom = Wt::WDate(1900,1,1), const Wt::WDate &top = Wt::WDate(2099, 1, 1), const Wt::WDate &defaultDate = Wt::WDate::currentDate(), bool createValidator = true, bool validatorIsMandatory = true);
            static Wt::WLineEdit *createLineEdit(const std::string &emptyText = "", bool createValidator = true, const std::string &validatorExp = "[A-Za-z0-9 _-]{4,255}", bool validatorIsMandatory = true);
            static Wt::WTextArea *createTextArea(const std::string &emptyText = "", bool createValidator = true, const std::string &validatorExp = "[A-Za-z0-9 _-]{4,255}", bool validatorIsMandatory = true);
            static Wt::WContainerWidget *createField(const std::string &label, Wt::WWidget *widget, bool applyValidationStyle = true);//wraps the given widget in a layout with a label on it's left
            static Wt::WContainerWidget *createEditField(const std::string &label, Wt::WWidget *widget, bool applyValidationStyle = true);//wraps the given widget in a layout with a label on it's left and a checkbox connected to the widget's enable() and disable() methods
            static Wt::WRegExpValidator *createRegExpValidator(const std::string &exp, bool setMandatory = true);//creates a Wt::WRegExpValidator based on the given expression
            static Wt::WIntValidator *createIntValidator(int min, int max, bool setMandatory = true);
            static Wt::WDoubleValidator *createDoubleValidator(double min, double max, bool setMandatory = true);
            static Wt::WDateValidator *createDateValidator(const Wt::WString &format = "dd/MM/yyyy", const Wt::WDate &bottom = Wt::WDate(1900, 1, 1), const Wt::WDate &top = Wt::WDate(2099, 1, 1), bool setMandatory = true);

            template<typename T>
            static Ms::Widgets::MQueryTableViewWidget<T> *createQueryTableViewWidget(Core::Dbo::MDboSession &dboSession, Wt::WContainerWidget *parent = 0);
        };
    }
}

#include "MWidgetFactory_impl.h"

#endif // MWIDGETFACTORY_H
