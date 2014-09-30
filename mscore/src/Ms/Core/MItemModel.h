#ifndef MITEMMODEL_H
#define MITEMMODEL_H

#include "MTableViewColumn.h"

#include <Wt/WAbstractItemModel>

namespace Ms
{
    namespace Core
    {
        class MItemModel : protected Wt::WAbstractItemModel
        {
        public:
            MItemModel();

            void addColumn(const MTableViewColumn &column) const;
            void removeColumn(const MTableViewColumn &column) const;
            void clearColumns() const;
        };
    }
}

#endif // MITEMMODEL_H
