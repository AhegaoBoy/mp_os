#include <gtest/gtest.h>

#include <big_integer.h>
#include <client_logger.h>
#include <operation_not_supported.h>

logger *create_logger(
    std::vector<std::pair<std::string, logger::severity>> const &output_file_streams_setup,
    bool use_console_stream = true,
    logger::severity console_stream_severity = logger::severity::debug)
{
    logger_builder *builder = new client_logger_builder();
    
    if (use_console_stream)
    {
        builder->add_console_stream(console_stream_severity);
    }
    
    for (auto &output_file_stream_setup: output_file_streams_setup)
    {
        builder->add_file_stream(output_file_stream_setup.first, output_file_stream_setup.second);
    }
    
    logger *built_logger = builder->build();
    
    delete builder;
    
    return built_logger;
}

TEST(positive_tests, test1)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });


    
    big_integer bigint_1("32850346459076457453464575686784654");
    big_integer bigint_2("423534596495087569087908753095322");
    big_integer result_of_sum = bigint_1 + bigint_2;

    big_integer bg1("-242");

    big_integer bg2("121");

    big_integer result = big_integer::euclidean_algorithm(bg1, bg2);

    std::cout<<result<<std::endl;




    std::stringstream ss;
    ss << result_of_sum;
    std::string result_string = ss.str();
    
    EXPECT_TRUE(result_string == "33273881055571545022552484439879976");
    
    delete logger;
}

TEST(positive_tests, test2)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_integer bigint_1("32850346459076457453464575686784654");
    big_integer bigint_2("0000042353459649508756908790875309532245366457546765745645647567575");
    
    big_integer result_of_sub = bigint_1 - bigint_2;
    
    std::stringstream ss;
    ss << result_of_sub;
    std::string result_string = ss.str();
    
    EXPECT_TRUE(result_string == "-42353459649508756908790875276681898907381089312281069960782921");
    
    delete logger;
}

TEST(positive_tests, test3)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });

    big_integer bk("1");
    bk <<= 160;
    big_integer remainder("0");
    remainder = bk % big_integer("-1");

    std::cout<<remainder<<std::endl;

    big_integer bigint_1("-32850346459076457453464575686784654");



    big_integer bigint_2("12342357553253");


    big_integer result_of_remainder = bigint_1 % bigint_2;

    std::cout<<result_of_remainder<<std::endl;

    std::stringstream ss;
    ss << result_of_remainder;
    std::string result_string = ss.str();

    EXPECT_TRUE(result_string == "3232571319826");

    delete logger;
}

TEST(positive_tests, test4)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_integer bigint_1("-54357893745346457544353");
    big_integer bigint_2("-54357893745346457544354");
    
    EXPECT_TRUE(bigint_1 > bigint_2);
    
    delete logger;
}

TEST(positive_tests, test5)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_integer bigint_1("423895435432");
    big_integer bigint_2("423895435432");
    
    EXPECT_TRUE(bigint_1 >= bigint_2);
    
    delete logger;
}

TEST(positive_tests, test6)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_integer bigint_1("-423895435432312432534645756753");
    big_integer bigint_2("0");
    
    EXPECT_TRUE(bigint_1 < bigint_2);
    
    delete logger;
}

TEST(positive_tests, test7)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_integer bigint_1("4238954354324222200000000");
    big_integer bigint_2("4238954354324222222222234");
    
    EXPECT_TRUE(bigint_1 <= bigint_2);
    
    delete logger;
}

TEST(positive_tests, test8)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
//    std::cout<<big_integer("3940467042472152277375391128553094203799416680559842321024") / big_integer("2482841961665992469840006678237417515525324775583493805108")<<std::endl;
    std::cout<<big_integer("703687441776640000000000000000") % big_integer("703687441776640000000000000000")<<std::endl;
    std::cout<<big_integer("3940467042472152277375391128553094203799416680559842321024") % big_integer("2482841961665992469840006678237417515525324775583493805108")<<std::endl;
    std::cout<<big_integer::euclidean_algorithm(big_integer("703687441776640000000000000000"), big_integer("c"))<<std::endl;
    
    big_integer bigint_1("4238954354321");
    big_integer bigint_2("423895435432");
    
    EXPECT_TRUE(bigint_1 != bigint_2);
    
    delete logger;
}

TEST(positive_tests, test9)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_integer bigint_1("-00000044234235347865897389456748953795739648996453238954354321");
    big_integer bigint_2("-00000044234235347865897389456748953795739648996453238954354321");
    
    EXPECT_TRUE(bigint_1 == bigint_2);
    
    delete logger;
}

int main(
    int argc,
    char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}