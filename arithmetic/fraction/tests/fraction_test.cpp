
#include <fraction.h>
#include <sstream>

int main(
        int argc,
        char** argv)
{
    fraction fraction_1(big_integer("2313123"), big_integer("33215"));
    fraction fraction_2(big_integer("104348"), big_integer("33215"));

    fraction result_of_summ = fraction_1 - fraction_2;
    std::cout<<result_of_summ<<std::endl;

//    std::stringstream ss;
//    ss<<result_of_summ;
//    std::string result = ss.str();
//std::cout<<fraction_1/fraction_2<<std::endl;

//std::cout<<fraction_1 + fraction_2<<std::endl;

//    fraction multiplication_result = fraction_1 - fraction_2;
    std::cout<<fraction_1.cos(fraction(big_integer("1"), big_integer("1000")));
//    std::cout<<fraction_1.root(2, fraction(big_integer("1"), big_integer("10000")))<<std::endl;

//    std::cout<<fraction_1.sin(fraction(big_integer("1"), big_integer("10000")))<<std::endl;

    return 0;
}