#ifndef MDBO_IMPL_H
#define MDBO_IMPL_H

#include <Wt/Dbo/Dbo>

namespace Ms
{
    namespace Dbo
    {
        template<class Action>
        void MDbo::persistExtras_(Action &a)
        {
            Wt::Dbo::field(a, thumbnail_, "Thumbnail");
            Wt::Dbo::field(a, dateCreated_, "Date_Created");
            Wt::Dbo::field(a, createdBy_, "Created_By");
            Wt::Dbo::field(a, lastModifiedDate_, "Last_Modified_Date");
            Wt::Dbo::field(a, lastModifiedBy_, "Last_Modified_By");
            Wt::Dbo::field(a, viewRank_, "View_Rank");
            Wt::Dbo::field(a, editRank_, "Edit_Rank");
            Wt::Dbo::field(a, removeRank_, "Remove_Rank");
            Wt::Dbo::field(a, active_, "Active");
        }
    }
}

#endif // MDBO_IMPL_H
