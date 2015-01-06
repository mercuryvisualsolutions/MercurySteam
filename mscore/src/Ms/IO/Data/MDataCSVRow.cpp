#include "MDataCSVRow.h"

#include <sstream>

Ms::IO::Data::MDataCSVRow::MDataCSVRow(const std::string &str, char separator)
{
    setDataString(str, separator);
}

std::size_t Ms::IO::Data::MDataCSVRow::size() const
{
    return m_data.size();
}

std::string Ms::IO::Data::MDataCSVRow::at(std::size_t index)
{
    return m_data.at(index);
}

void Ms::IO::Data::MDataCSVRow::addCell(const std::string &cellData)
{
    m_data.push_back(cellData);
}

void Ms::IO::Data::MDataCSVRow::removeCell(std::size_t index)
{
    m_data.erase(m_data.begin() + index);
}

void Ms::IO::Data::MDataCSVRow::clear()
{
    m_data.clear();
}

std::string Ms::IO::Data::MDataCSVRow::toString()
{
    std::string result = "";

    for(std::size_t i = 0; i < m_data.size(); ++i)
    {
        result += m_data.at(i);

        if(i < m_data.size() - 1)
            result += ",";
    }

    return result;
}

void Ms::IO::Data::MDataCSVRow::setDataString(const std::string &str, char separator)
{
    m_data.clear();

    std::stringstream strStream(str);
    std::string cell;

    while(std::getline(strStream, cell, separator))
    {
        addCell(cell);
    }
}

std::vector<std::string>::iterator Ms::IO::Data::MDataCSVRow::begin()
{
    return m_data.begin();
}

std::vector<std::string>::const_iterator Ms::IO::Data::MDataCSVRow::cbegin()
{
    return m_data.cbegin();
}

std::vector<std::string>::iterator Ms::IO::Data::MDataCSVRow::end()
{
    return m_data.end();
}

std::vector<std::string>::const_iterator Ms::IO::Data::MDataCSVRow::cend()
{
    return m_data.cend();
}

std::string &Ms::IO::Data::MDataCSVRow::operator[](std::size_t index)
{
    return m_data[index];
}
