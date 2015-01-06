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
            std::string m_name;
            std::string m_fullName;
            std::string m_extension;
            u_int64_t m_size;
            std::string m_lastModifiedDate;

            //functions
            void extractName();
            void extractExtension();
        };
    }
}

#endif // MFILEINFO_H
