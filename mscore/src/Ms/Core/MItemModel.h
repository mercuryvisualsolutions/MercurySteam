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

            // WAbstractItemModel interface
        public:
            virtual int columnCount(const Wt::WModelIndex &parent) const;
            virtual int rowCount(const Wt::WModelIndex &parent) const;
            virtual Wt::WModelIndex parent(const Wt::WModelIndex &index) const;
            virtual Wt::WModelIndex index(int row, int column, const Wt::WModelIndex &parent) const;
        };
    }
}

#endif // MITEMMODEL_H
