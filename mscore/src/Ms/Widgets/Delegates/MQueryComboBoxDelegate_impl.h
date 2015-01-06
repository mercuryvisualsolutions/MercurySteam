#ifndef MQUERYCOMBOBOXDELEGATE_IMPL_H
#define MQUERYCOMBOBOXDELEGATE_IMPL_H

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
            template<typename T>
            Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>::MQueryComboBoxDelegate()
            {
            }

            template<typename T>
            Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>::MQueryComboBoxDelegate(Wt::Dbo::Session *session, Wt::Dbo::Query<Wt::Dbo::ptr<T>> comboBoxQuery,
                                                                                   const std::string &displayColumn, int editRank) :
                m_session(session), m_displayColumn(displayColumn), m_query(comboBoxQuery)
            {
                setEditRank(editRank);
            }

            template<typename T>
            Wt::Dbo::Query<Wt::Dbo::ptr<T>> Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>::query()
            {
                return m_query;
            }

            template<typename T>
            void Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>::setQuery(Wt::Dbo::Query<Wt::Dbo::ptr<T>> query)
            {
                m_query = query;
            }

            template<typename T>
            Wt::Dbo::Session* Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>::session()
            {
                return m_session;
            }

            template<typename T>
            void Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>::setSession(Wt::Dbo::Session *session)
            {
                m_session = session;
            }

            template<typename T>
            Wt::WWidget* Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>::createEditor(const Wt::WModelIndex &index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) const
            {
                Wt::WContainerWidget *result = new Wt::WContainerWidget();

                result->setSelectable(true);
                result->setLayout(new Wt::WVBoxLayout());
                result->layout()->setContentsMargins(0,0,0,0);

                Wt::WComboBox *cmbQuery;

                cmbQuery = createCmbQuery();

                boost::any data = index.data(Wt::EditRole);

                cmbQuery->escapePressed().connect(boost::bind(&Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>::doCloseEditor, this, result, false));
                cmbQuery->clicked().connect(boost::bind(&Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>::doCloseEditor, this, result, true));
                cmbQuery->enterPressed().connect(boost::bind(&Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>::doCloseEditor, this, result, true));

                if(flags & Wt::RenderFocused)
                    cmbQuery->setFocus();

                if(!data.empty())
                    cmbQuery->setValueText(boost::any_cast<std::string>(data));

                result->layout()->addWidget(cmbQuery);

                return result;
            }

            template<typename T>
            void Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>::doCloseEditor(Wt::WWidget *editor, bool save) const
            {
                closeEditor()(editor, save);
            }

            template<typename T>
            void Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>::setModelData(const boost::any &editState, Wt::WAbstractItemModel *model, const Wt::WModelIndex &index) const
            {
                model->setData(index, editState, Wt::EditRole);
            }

            template<typename T>
            boost::any Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>::editState(Wt::WWidget *editor) const
            {
                Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
                Wt::WComboBox *cmb = dynamic_cast<Wt::WComboBox*>(container->widget(0));
                return boost::any(cmb->currentText().toUTF8());
            }

            template<typename T>
            void Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>::setEditState(Wt::WWidget *editor, const boost::any &value) const
            {
                Wt::WContainerWidget *container = dynamic_cast<Wt::WContainerWidget*>(editor);
                Wt::WComboBox *cmb = dynamic_cast<Wt::WComboBox*>(container->widget(0));
                cmb->setValueText(boost::any_cast<std::string>(value));
            }

            template<typename T>
            void Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>::setDisplayColumn(const std::string &columnName)
            {
                m_displayColumn = columnName;
            }

            template<typename T>
            std::string Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>::displayColumn()
            {
                return m_displayColumn;
            }

            template<typename T>
            Wt::WComboBox *Ms::Widgets::Delegates::MQueryComboBoxDelegate<T>::createCmbQuery() const
            {
                Wt::WComboBox *cmbQuery = new Wt::WComboBox();
                try
                {
                    Wt::Dbo::Transaction transaction(*m_session);

                    Wt::Dbo::QueryModel<Wt::Dbo::ptr<T>> *mdlCmbQuery = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<T>>();

                    mdlCmbQuery->setQuery(m_query);

                    transaction.commit();

                    mdlCmbQuery->reload();

                    mdlCmbQuery->addColumn(m_displayColumn, Wt::ItemIsSelectable);

                    cmbQuery->setModel(mdlCmbQuery);
                }
                catch(...)
                {
                    std::cerr << "Error occured while trying to create a QueryComboBoxDelegate model" << std::endl;
                }

                return cmbQuery;
            }
        }
    }
}

#endif // MQUERYCOMBOBOXDELEGATE_IMPL_H
