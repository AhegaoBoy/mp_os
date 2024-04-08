#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H

#include <logger.h>
#include "client_logger_builder.h"
#include <map>
#include <set>
#include <unordered_map>

class client_logger final:
    public logger
{
private:
    std::map<std::string, std::pair<std::ofstream*, std::set<logger::severity>>> _streams;

    static std::unordered_map<std::string, std::pair<std::ofstream*, size_t>> _all_streams;

public:

    client_logger() = default;

    client_logger(
        client_logger const &other);

    client_logger &operator=(
        client_logger const &other);

    client_logger(
        client_logger &&other) noexcept;

    client_logger &operator=(
        client_logger &&other) noexcept;

    ~client_logger() noexcept final;

public:

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;

public:

    void insert_in_stream(std::string const& path,
                          std::set<logger::severity>);
private:

    void add_to_all_streams();

    void clear_object();

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H