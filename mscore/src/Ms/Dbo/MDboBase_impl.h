#ifndef MDBO_IMPL_H
#define MDBO_IMPL_H

#include <Wt/Dbo/Dbo>

namespace Ms
{
    namespace Dbo
    {
        template<class Action>
        void Ms::Dbo::MDboBase::persist(Action &a)
        {
            Wt::Dbo::field(a, m_thumbnail, "Thumbnail");
            Wt::Dbo::field(a, m_dateCreated, "Date_Created");
            Wt::Dbo::field(a, m_createdBy, "Created_By");
            Wt::Dbo::field(a, m_lastModifiedDate, "Last_Modified_Date");
            Wt::Dbo::field(a, m_lastModifiedBy, "Last_Modified_By");
            Wt::Dbo::field(a, m_viewRank, "View_Rank");
            Wt::Dbo::field(a, m_editRank, "Edit_Rank");
            Wt::Dbo::field(a, m_removeRank, "Remove_Rank");
            Wt::Dbo::field(a, m_active, "Active");
        }
    }
}

#endif // MDBO_IMPL_H
