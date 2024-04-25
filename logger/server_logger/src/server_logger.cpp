#include <not_implemented.h>

#include "../include/server_logger.h"

server_logger::server_logger(
    server_logger const &other): _streams(other._streams), _format(other._format)
{

}

server_logger &server_logger::operator=(
    server_logger const &other)
{
    if(this != &other)
    {
        this->_streams = other._streams;
        this->_format = other._format;
    }
    return *this;
}

server_logger::server_logger(
    server_logger &&other) noexcept: _streams(std::move(other._streams)), _format(std::move(other._format))
{

}

server_logger &server_logger::operator=(
    server_logger &&other) noexcept
{
    if(this != &other)
    {
        this->_streams = std::move(other._streams);
        this->_format = std::move(other._format);
    }
    return *this;
}

server_logger::~server_logger() noexcept
{
    throw not_implemented("server_logger::~server_logger() noexcept", "your code should be here...");
}

void server_logger::add_file_stream(const std::string &file_path_name,
                                                   logger::severity const &severity)
{
    _streams[file_path_name].emplace(severity);
}
void server_logger::add_console_severity(logger::severity const &severity)
{
    _streams[""].emplace(severity);
}
logger const *server_logger::log(
    const std::string &text,
    logger::severity severity) const noexcept
{
    throw not_implemented("logger const *server_logger::log(const std::string &text, logger::severity severity) const noexcept", "your code should be here...");
}