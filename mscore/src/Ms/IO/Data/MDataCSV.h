#ifndef MDATACSV_H
#define MDATACSV_H

#include <iostream>
#include <map>
#include <vector>
#include <fstream>

#include "MDataCSVRow.h"

namespace Ms
{
    namespace IO
    {
        namespace Data
        {
            class MDataCSV
            {
            public:
                MDataCSV();

                //functions
                std::size_t numRows();
                void addRow(const Ms::IO::Data::MDataCSVRow &row);
                void removeRow(size_t index);
                void clear();
                std::string cell(std::size_t column, std::size_t row);
                void setCell(std::size_t column, std::size_t row, const std::string &value);

                std::vector<Ms::IO::Data::MDataCSVRow>::iterator begin();
                std::vector<Ms::IO::Data::MDataCSVRow>::const_iterator cbegin();
                std::vector<Ms::IO::Data::MDataCSVRow>::iterator end();
                std::vector<Ms::IO::Data::MDataCSVRow>::const_iterator cend();

                MDataCSVRow &operator [](std::size_t rowIndex);


            private:
                //variables
                std::vector<Ms::IO::Data::MDataCSVRow> m_rows;
            };
        }
    }
}

#endif // MDATACSV_H
