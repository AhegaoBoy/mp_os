#include <not_implemented.h>
#include "../include/client_logger.h"
#include <windows.h>
#include "nlohmann/json.hpp"
#include <fstream>
//#include "../include/client_logger_builder.h"

client_logger_builder::client_logger_builder()
{

}

client_logger_builder::client_logger_builder(
        client_logger_builder const &other): _streams(other._streams)
{

}

client_logger_builder &client_logger_builder::operator=(
        client_logger_builder const &other)
{
    if(this != &other) this->_streams = other._streams;
    return *this;
}

client_logger_builder::client_logger_builder(
        client_logger_builder &&other) noexcept: _streams(std::move(other._streams))
{

}

client_logger_builder &client_logger_builder::operator=(
        client_logger_builder &&other) noexcept
{
    if(this != &other) this->_streams = std::move(other._streams);
    return *this;
}


client_logger_builder::~client_logger_builder() noexcept
{

}

client_logger_builder *client_logger_builder::add_file_stream(
        std::string const &stream_file_path,
        logger::severity severity)
{
    std::string full_path = "";

    if(!stream_file_path.empty())
    {
        char buffer[1024];
        char path_in_char[stream_file_path.size()];

        for(size_t i = 0; i < stream_file_path.size(); ++i)
            path_in_char[i] = stream_file_path[i];

        DWORD len = GetFullPathNameA(path_in_char, 1024, buffer, nullptr);

        if(len) full_path = buffer;
    }

    this->_streams[full_path].insert(severity);
    return this;

}

client_logger_builder *client_logger_builder::add_console_stream(
        logger::severity severity)
{
    return this->add_file_stream("", severity);
}

client_logger_builder* client_logger_builder::transform_with_configuration(
        std::string const &configuration_file_path,
        std::string const &configuration_path)
{
    std::ifstream jsonFileStream(configuration_file_path);
    nlohmann::json data = nlohmann::json::parse(jsonFileStream);

    nlohmann::json element;
    try
    {
        element = data.at(configuration_path);
    }
    catch (nlohmann::json::out_of_range& ex)
    {
        throw(std::string("Configuration path" + configuration_path +  "hasn't been found"));
    }
    std::string file = element["file"].get<std::string>();
    std::vector<logger::severity> severities = element["severity"];
    for(size_t i = 0; i < severities.size(); ++i)
    {
        if(file.empty()) add_console_stream(severities[i]);
        else add_file_stream(file, severities[i]);
    }
    return this;
}

client_logger_builder *client_logger_builder::clear()
{
    this->_streams.clear();
    return this;
}

logger *client_logger_builder::build() const
{
    auto* logger = new client_logger;

    for(auto iter = this->_streams.begin(); iter !=this->_streams.end(); ++iter)
        logger->insert_in_stream(iter->first, iter->second);
    return logger;
}