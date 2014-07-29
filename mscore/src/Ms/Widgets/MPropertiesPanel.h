#ifndef MPROPERTIESPANEL_H
#define MPROPERTIESPANEL_H

#include <Wt/WVBoxLayout>
#include <Wt/WContainerWidget>
#include <Wt/WStackedWidget>
#include <Wt/WText>

#include "MContainerWidget.h"

namespace Ms
{
    namespace Widgets
    {
        class MPropertiesPanel : public Ms::Widgets::MContainerWidget
        {
        public:
            MPropertiesPanel();

            //functions
            void showView(const std::string &name);
            void addPropertiesView(const std::string &name, Wt::WWidget *widget);
            void removePropertiesView(const std::string &name);
            bool propertiesViewExists(const std::string &name) const;
            Wt::WWidget *currentShownWidget() const;
            std::string currentShownView() const;

        private:
            //variables
            //ui
            Wt::WStackedWidget *_stkMain;

            //different views
            std::map<std::string,Wt::WWidget*> _views;

            //functions
            void _prepareView();
        };
    }
}

#endif // MPROPERTIESPANEL_H
