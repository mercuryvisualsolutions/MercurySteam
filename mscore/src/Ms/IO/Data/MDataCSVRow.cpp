#include "MDataCSVRow.h"

#include <sstream>

Ms::IO::Data::MDataCSVRow::MDataCSVRow(const std::string &str, char separator)
{
    setDataString(str, separator);
}

std::size_t Ms::IO::Data::MDataCSVRow::size() const
{
    return _data.size();
}

std::string Ms::IO::Data::MDataCSVRow::at(std::size_t index)
{
    return _data.at(index);
}

void Ms::IO::Data::MDataCSVRow::addCell(const std::string &cellData)
{
    _data.push_back(cellData);
}

void Ms::IO::Data::MDataCSVRow::removeCell(std::size_t index)
{
    _data.erase(_data.begin() + index);
}

void Ms::IO::Data::MDataCSVRow::clear()
{
    _data.clear();
}

std::string Ms::IO::Data::MDataCSVRow::toString()
{
    std::string result = "";

    for(std::size_t i = 0; i < _data.size(); ++i)
    {
        result += _data.at(i);

        if(i < _data.size() - 1)
            result += ",";
    }

    return result;
}

void Ms::IO::Data::MDataCSVRow::setDataString(const std::string &str, char separator)
{
    _data.clear();

    std::stringstream strStream(str);
    std::string cell;

    while(std::getline(strStream, cell, separator))
    {
        addCell(cell);
    }
}

std::vector<std::string>::iterator Ms::IO::Data::MDataCSVRow::begin()
{
    return _data.begin();
}

std::vector<std::string>::const_iterator Ms::IO::Data::MDataCSVRow::cbegin()
{
    return _data.cbegin();
}

std::vector<std::string>::iterator Ms::IO::Data::MDataCSVRow::end()
{
    return _data.end();
}

std::vector<std::string>::const_iterator Ms::IO::Data::MDataCSVRow::cend()
{
    return _data.cend();
}

std::string &Ms::IO::Data::MDataCSVRow::operator[](std::size_t index)
{
    return _data[index];
}
