#include <not_implemented.h>

#include <fstream>
#include "../include/client_logger.h"

std::unordered_map<std::string, std::pair<std::ofstream*, size_t>> client_logger::_all_streams;
client_logger::client_logger(
        client_logger const &other)
{
    this->_streams = other._streams;
    this->add_to_all_streams();
}

client_logger &client_logger::operator=(
        client_logger const &other)
{
    if(this != &other)
    {
        this->_streams = other._streams;
        this->add_to_all_streams();
    }
    return *this;
}

client_logger::client_logger(
        client_logger &&other) noexcept
{
    this->clear_object();
    this->_streams = std::move(other._streams);
}

client_logger &client_logger::operator=(
        client_logger &&other) noexcept
{
    if(this != &other)
    {
        // destructor step
        this->clear_object();
        this->_streams = std::move(other._streams);
    }
    return *this;
}

client_logger::~client_logger() noexcept
{
    for(auto iter = _streams.begin(); iter != _streams.end(); ++iter)
    {
        auto tmp = _all_streams[iter->first];
        if(tmp.second == 1)
        {
            tmp.first->close();
            //delete tmp.first;
            --tmp.second;
        }
    }

}

void client_logger::add_to_all_streams()
{
    for(auto iter = this->_streams.cbegin(); iter!=this->_streams.cend(); ++iter)
        ++_all_streams.find(iter->first)->second.second;

}

void::client_logger::clear_object()
{
    for(auto iter = this->_streams.cbegin(); iter != this->_streams.cend(); ++iter)
    {
        size_t to_check = --_all_streams.find(iter->first)->second.second;
        if(to_check == 0) _all_streams.find(iter->first)->second.first->close();
    }
}

void client_logger::insert_in_stream(const std::string &path, std::set<logger::severity> severity_set)
{
    auto* stream = new(std::ofstream);
    if(path.empty())
    {
        stream = reinterpret_cast<std::ofstream*>(&std::cout);
        this->_streams.emplace(std::make_pair(path, std::make_pair(stream, std::move(severity_set))));
    }

    else
    {
        stream->open(path);
        this->_streams.emplace(std::make_pair(path, std::make_pair(stream, std::move(severity_set))));
    }

    if(_all_streams.find(path) != _all_streams.end()) ++_all_streams.find(path)->second.second;
    else
    {
        //stream->open(path);
        _all_streams.emplace(std::make_pair(path, std::make_pair(stream, 1)));
    }

}
logger const *client_logger::log(
        const std::string &text,
        logger::severity severity) const noexcept
{
    std::string path_of_log_file = "";
    auto iter = _streams.begin();
    for(; iter != _streams.end(); ++iter)//ищу файл с подходящей жесткостью
    {
        if(iter->second.second.find(severity) != iter->second.second.end())
        {
            path_of_log_file = iter->first;

            std::ofstream* file = path_of_log_file.empty() ? reinterpret_cast<std::ofstream*>(&std::cout) : _all_streams.find(path_of_log_file)->second.first;

            *file<<severity_to_string(severity)<<" "<<text<<" "<<current_datetime_to_string()<<std::endl << std::endl;
        }
    }
    return this;
}