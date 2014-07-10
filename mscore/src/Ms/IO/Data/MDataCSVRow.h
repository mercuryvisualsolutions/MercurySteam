#ifndef MDATACSVROW_H
#define MDATACSVROW_H

#include <iostream>
#include <vector>

namespace Ms
{
    namespace IO
    {
        namespace Data
        {
            class MDataCSVRow
            {
            public:
                MDataCSVRow(const std::string &str, char separator = ',');
                std::size_t size() const;
                std::string at(std::size_t index);
                void addCell(const std::string &cellData);
                void removeCell(std::size_t index);
                void clear();
                std::string toString();
                void setDataString(const std::string &str, char separator = ',');

                std::vector<std::string>::iterator begin();
                std::vector<std::string>::const_iterator cbegin();
                std::vector<std::string>::iterator end();
                std::vector<std::string>::const_iterator cend();

                std::string &operator[](std::size_t index);

            private:
                std::vector<std::string> _data;
            };
        }
    }
}

#endif // MDATACSVROW_H
