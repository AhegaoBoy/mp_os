/*
#include <gtest/gtest.h>
#include "../include/client_logger.h"
TEST(client_logger, test1)
{
    client_logger_builder* builder;
    builder = builder->add_console_stream(logger::severity::trace)->add_console_stream(logger::severity::critical)->add_console_stream(logger::severity::information);
    logger* new_logger = builder->build();


}
int main(
    int argc,
    char *argv[])
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
*/
#include "../include/client_logger.h"
#include<fstream>

int main()
{
    client_logger_builder* builder = new client_logger_builder;
    builder->add_console_stream(logger::severity::debug)->add_console_stream(logger::severity::trace)->add_console_stream(logger::severity::information)->add_file_stream("A:\\mai_stuff\\EarBitSky\\log_files\\log_file1.txt", logger::severity::debug);
    logger* logger = builder->build();
    logger->information("information")->debug("debug")->trace("trace");

    delete builder;
    delete logger;
    return 0;
}
