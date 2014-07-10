#ifndef MFILEINFO_H
#define MFILEINFO_H

#include <iostream>
#include <sys/types.h>
#include <time.h>

namespace Ms
{
    namespace IO
    {
        class MFileInfo
        {
        public:
            MFileInfo();
            MFileInfo(const std::string &fileName);

            //functions
            std::string name();
            std::string fullName();
            u_int64_t size();
            std::string lastModifiedDate();

        private:
            //variables
            std::string _name;
            std::string _fullName;
            u_int64_t _size;
            std::string _lastModifiedDate;

            //functions
            void _extractName();
        };
    }
}

#endif // MFILEINFO_H
