#include "MDataCSV.h"

Ms::IO::Data::MDataCSV::MDataCSV()
{
}

std::size_t Ms::IO::Data::MDataCSV::numRows()
{
    return m_rows.size();
}

void Ms::IO::Data::MDataCSV::addRow(const Ms::IO::Data::MDataCSVRow &row)
{
    m_rows.push_back(row);
}

void Ms::IO::Data::MDataCSV::removeRow(size_t index)
{
    m_rows.erase(m_rows.begin() + index);
}

void Ms::IO::Data::MDataCSV::clear()
{
    for(auto row : m_rows)
        row.clear();

    m_rows.clear();
}

std::string Ms::IO::Data::MDataCSV::cell(std::size_t column, std::size_t row)
{
    return m_rows[row][column];
}

void Ms::IO::Data::MDataCSV::setCell(std::size_t column, std::size_t row, const std::string &value)
{
    m_rows[row][column] = value;
}

std::vector<Ms::IO::Data::MDataCSVRow>::iterator Ms::IO::Data::MDataCSV::begin()
{
    return m_rows.begin();
}

std::vector<Ms::IO::Data::MDataCSVRow>::const_iterator Ms::IO::Data::MDataCSV::cbegin()
{
    return m_rows.cbegin();
}

std::vector<Ms::IO::Data::MDataCSVRow>::iterator Ms::IO::Data::MDataCSV::end()
{
    return m_rows.end();
}

std::vector<Ms::IO::Data::MDataCSVRow>::const_iterator Ms::IO::Data::MDataCSV::cend()
{
    return m_rows.cend();
}

Ms::IO::Data::MDataCSVRow &Ms::IO::Data::MDataCSV::operator [](std::size_t rowIndex)
{
    return m_rows[rowIndex];
}
