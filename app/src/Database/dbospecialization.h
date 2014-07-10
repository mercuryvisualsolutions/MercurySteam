//file contains all my dbo_traits modifications (surrogate id field, version...)

#ifndef DBOSPECIALIZATION_H
#define DBOSPECIALIZATION_H

#include <iostream>
#include <string>

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>

namespace Wt
{
    namespace Dbo
    {
        //specify how std::string type are presisted
        template <class Action>
        void field(Action& action, const std::string &filedName, const std::string &columnName, int size = -1)
        {
            field(action, filedName, columnName, size);
        }
    }
}

#endif // DBOSPECIALIZATION_H
