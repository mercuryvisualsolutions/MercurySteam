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
            const std::string name() const;
            const std::string fullName() const;
            const std::string extension() const;
            u_int64_t size() const;
            const std::string lastModifiedDate() const;

        private:
            //variables
            std::string _name;
            std::string _fullName;
            std::string _extension;
            u_int64_t _size;
            std::string _lastModifiedDate;

            //functions
            void _extractName();
            void _extractExtension();
        };
    }
}

#endif // MFILEINFO_H
