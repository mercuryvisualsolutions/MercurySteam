#ifndef MQUERYCOMBOBOXDELEGATE_H
#define MQUERYCOMBOBOXDELEGATE_H

#include <Wt/Dbo/ptr>
#include <Wt/Dbo/Query>
#include <Wt/WContainerWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WComboBox>
#include <Wt/Dbo/Session>

namespace Ms
{
    namespace Widgets
    {
        namespace Delegates
        {
            //only supports DBO Fileds with type std::string, for now
            //TODO:: support more dbo field types
            template<typename T>
            class MQueryComboBoxDelegate : public MItemDelegate
            {
            public:
                MQueryComboBoxDelegate();
                MQueryComboBoxDelegate(Wt::Dbo::Session *session, Wt::Dbo::Query<Wt::Dbo::ptr<T>> comboBoxQuery, const std::string &displayColumn, int editRank = 0);

                //functions
                Wt::Dbo::Query<Wt::Dbo::ptr<T>> query();
                void setQuery(Wt::Dbo::Query<Wt::Dbo::ptr<T>> query);
                Wt::Dbo::Session *session();
                void setSession(Wt::Dbo::Session *session);
                Wt::WWidget *createEditor(const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) const;
                void doCloseEditor(Wt::WWidget *editor, bool save) const;
                void setModelData(const boost::any &editState, Wt::WAbstractItemModel *model, const Wt::WModelIndex &index) const;
                boost::any editState(Wt::WWidget *editor) const;
                void setEditState(Wt::WWidget *editor, const boost::any &value) const;
                void setDisplayColumn(const std::string &columnName);
                std::string displayColumn();

            private:
                Wt::WComboBox *_createCmbQuery() const;

                Wt::Dbo::Session *_session;
                std::string _displayColumn;
                Wt::Dbo::Query<Wt::Dbo::ptr<T>> _query;
            };
        }
    }
}

#include "MQueryComboBoxDelegate_impl.h"

#endif // MQUERYCOMBOBOXDELEGATE_H
