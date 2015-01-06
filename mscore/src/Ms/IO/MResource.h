#ifndef MRESOURCE_H
#define MRESOURCE_H

#include <Wt/WResource>

namespace Ms
{
    namespace IO
    {
        class MResource
        {
        public:
            MResource();

            //functions
            bool isInUse();

        protected:
            bool m_isInUse;
        };
    }
}

#endif // MRESOURCE_H
