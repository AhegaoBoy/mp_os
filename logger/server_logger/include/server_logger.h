#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_H

#include <logger.h>
#include "server_logger_builder.h"
#include <mutex>
#include <sys/msg.h>
#include <map>
#include <set>

class server_logger final:
    public logger
{
private:

mutable std::mutex _mutex;
std::string _format;
std::map<std::string, std::set<logger::severity>> _streams;

public:
    server_logger() = default;

    server_logger(std::string format, std::string port);

    server_logger(
        server_logger const &other);

    server_logger &operator=(
        server_logger const &other);

    server_logger(
        server_logger &&other) noexcept;

    server_logger &operator=(
        server_logger &&other) noexcept;

    ~server_logger() noexcept final;

public:

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;

private:

    void add_file_stream(
            std::string const &file_path_name,
            logger::severity const &severity);

    void add_console_severity(
            logger::severity const &severity);

    void zmq_send_message(
            const std::string &text,
            logger::severity severity) const;

    void set_port(std::string const& port_to_set);

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_H