#ifndef PROGRAMMING_LANGUAGES_AND_METHODS_BIG_INTEGER_H
#define PROGRAMMING_LANGUAGES_AND_METHODS_BIG_INTEGER_H


#include <corecrt.h>
#include <string>
#include <vector>
#include <iostream>

class big_integer final
{

public:
    class division
    {
    public:
        enum division_rule
        {
            trivial
        };

        static void trivial_division(big_integer& dividing_integer, big_integer const& other)
        {
            dividing_integer /= other;
        }
    };

    class multiplication
    {
    public:
        enum multiplication_rule
        {
            trivial
        };
    };




private:

    void clear();

    void copy_from(
            big_integer const &other);

    void initialize_from(
            int const *digits,
            size_t digits_count);

    void initialize_from(
            std::vector<int> const &digits,
            size_t digits_count);

    void initialize_from(
            std::vector<unsigned int> const &digits,
            size_t digits_count);

    void initialize_from(
            std::string const &value,
            size_t base);

private:

    static void print_byte(
            std::ostream &stream,
            unsigned char byte_value);

    static void dump_int_value(
            std::ostream &stream,
            int value);

public:

    void dump_value(
            std::ostream &stream) const
    {
        for (auto i = 0; i < get_digits_count(); ++i)
        {
            auto digit = get_digit(i);
            dump_int_value(stream, *reinterpret_cast<int *>(&digit));
            stream << ' ';
        }
    }

private:

    int _oldest_digit;
    unsigned int *_other_digits;

private:


private:

    big_integer &change_sign();
    size_t binary_power(size_t number, unsigned int power);

    std::vector<unsigned int> convert_to_base(std::string const & biiiiiiiiiiig_number, size_t base);

public:

    inline int get_digits_count() const noexcept;

    inline int sign() const noexcept;

    inline bool is_equal_to_zero() const noexcept;

    inline unsigned int get_digit(
            int position) const noexcept;

public:

    big_integer(
            std::string const &digits);

    big_integer(
            int const *digits,
            size_t digits_count);

    big_integer(
            std::vector<int> const &digits);

    big_integer(
            std::string const &value,
            size_t base);

public:

    big_integer(
            big_integer const &other);

    big_integer &operator=(
            big_integer const &other);

    ~big_integer();

public:
    //static void divide(big_integer& bigInteger1, big_integer bigInteger2, big_integer::division::division_rule divisionRule);

    static  big_integer divide(big_integer& bigInteger1, big_integer bigInteger2, big_integer::division::division_rule divisionRule);

public:

    big_integer &operator+=(
            big_integer const &other);

    big_integer operator+(
            big_integer const &other) const;

    big_integer &operator-=(
            big_integer const &other);

    big_integer operator-(
            big_integer const &other) const;

    big_integer operator-() const;

    big_integer &operator*=(
            big_integer const &other);

    big_integer operator*(
            big_integer const &other) const;

    big_integer &operator/=(
            big_integer const &other);

    big_integer operator/(
            big_integer const &other) const;

    big_integer &operator%=(
            big_integer const &other);

    big_integer operator%(
            big_integer const &other) const;

    bool operator==(
            big_integer const &other) const;

    bool operator!=(
            big_integer const &other) const;

    bool operator<(
            big_integer const &other) const;

    bool operator<=(
            big_integer const &other) const;

    bool operator>(
            big_integer const &other) const;

    bool operator>=(
            big_integer const &other) const;

    big_integer operator~() const;

public:

    big_integer &operator&=(
            big_integer const &other);

    big_integer operator&(
            big_integer const &other) const;

    big_integer &operator|=(
            big_integer const &other);

    big_integer operator|(
            big_integer const &other) const;

    big_integer &operator^=(
            big_integer const &other);

    big_integer operator^(
            big_integer const &other) const;

    big_integer &operator<<=(
            size_t shift_value);

    big_integer operator<<(
            size_t shift_value) const;

    big_integer &operator>>=(
            size_t shift_value);

    big_integer operator>>(
            size_t shift_value) const;

    friend std::ostream &operator<<(
            std::ostream &stream,
            big_integer const &value);

    friend std::istream &operator>>(
            std::istream &stream,
            big_integer &value);

};

#endif //PROGRAMMING_LANGUAGES_AND_METHODS_BIG_INTEGER_H