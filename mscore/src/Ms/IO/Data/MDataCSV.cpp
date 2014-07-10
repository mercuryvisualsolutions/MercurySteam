#include "MDataCSV.h"

Ms::IO::Data::MDataCSV::MDataCSV()
{
}

std::size_t Ms::IO::Data::MDataCSV::numRows()
{
    return _rows.size();
}

void Ms::IO::Data::MDataCSV::addRow(const Ms::IO::Data::MDataCSVRow &row)
{
    _rows.push_back(row);
}

void Ms::IO::Data::MDataCSV::removeRow(size_t index)
{
    _rows.erase(_rows.begin() + index);
}

void Ms::IO::Data::MDataCSV::clear()
{
    for(auto row : _rows)
        row.clear();

    _rows.clear();
}

std::string Ms::IO::Data::MDataCSV::cell(std::size_t column, std::size_t row)
{
    return _rows[row][column];
}

void Ms::IO::Data::MDataCSV::setCell(std::size_t column, std::size_t row, const std::string &value)
{
    _rows[row][column] = value;
}

std::vector<Ms::IO::Data::MDataCSVRow>::iterator Ms::IO::Data::MDataCSV::begin()
{
    return _rows.begin();
}

std::vector<Ms::IO::Data::MDataCSVRow>::const_iterator Ms::IO::Data::MDataCSV::cbegin()
{
    return _rows.cbegin();
}

std::vector<Ms::IO::Data::MDataCSVRow>::iterator Ms::IO::Data::MDataCSV::end()
{
    return _rows.end();
}

std::vector<Ms::IO::Data::MDataCSVRow>::const_iterator Ms::IO::Data::MDataCSV::cend()
{
    return _rows.cend();
}

Ms::IO::Data::MDataCSVRow &Ms::IO::Data::MDataCSV::operator [](std::size_t rowIndex)
{
    return _rows[rowIndex];
}
