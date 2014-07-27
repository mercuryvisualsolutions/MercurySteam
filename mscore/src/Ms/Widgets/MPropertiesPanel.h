#ifndef MPROPERTIESPANEL_H
#define MPROPERTIESPANEL_H

#include <Wt/WVBoxLayout>
#include <Wt/WContainerWidget>
#include <Wt/WStackedWidget>
#include <Wt/WText>

namespace Ms
{
    namespace Widgets
    {
        class MPropertiesPanel : public Wt::WContainerWidget
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
            Wt::WVBoxLayout *_layMain;
            Wt::WText *_txtPropertiesTitle;
            Wt::WContainerWidget *_cntTxtPropertiesTitle;

            //
            std::map<std::string,Wt::WWidget*> _views;

            //functions
            void _prepareView();
        };
    }
}

#endif // MPROPERTIESPANEL_H
