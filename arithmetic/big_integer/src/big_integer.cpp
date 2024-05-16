#include <cstring>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <sstream>

#include "big_integer.h"

void big_integer::clear()
{
    _oldest_digit = 0;
    delete[] _other_digits;
    _other_digits = nullptr;
}

void big_integer::copy_from(
        big_integer const &other)
{
    _oldest_digit = other._oldest_digit;
    _other_digits = nullptr;
    if (other._other_digits == nullptr)
    {
        return;
    }

    _other_digits = new unsigned int[*other._other_digits];
    std::memcpy(_other_digits, other._other_digits, sizeof(unsigned int) * (*other._other_digits));
}

void big_integer::initialize_from(
        int const *digits,
        size_t digits_count)
{
    if (digits == nullptr)
    {
        throw std::logic_error("pointer to digits array must not be nullptr");
    }

    if (digits_count == 0)
    {
        throw std::logic_error("digits array length must  be GT 0");
    }

    _oldest_digit = digits[digits_count - 1];
    _other_digits = (digits_count == 1
                     ? nullptr
                     : new unsigned int[digits_count]);

    if (_other_digits == nullptr)
    {
        return;
    }

    *_other_digits = (unsigned int)digits_count;

    std::memcpy(_other_digits + 1, digits, sizeof(unsigned int) * (digits_count - 1));
}

void big_integer::initialize_from(
        std::vector<int> const &digits,
        size_t digits_count)
{
    _other_digits = nullptr;

    if (digits.empty() || digits_count == 0)
    {
        throw std::logic_error("std::vector<int> of digits should not be empty");
    }

    _oldest_digit = digits[digits_count - 1];

    if (digits_count == 1)
    {
        return;
    }

    _other_digits = new unsigned int[digits_count];
    *_other_digits = digits_count;

    for (auto i = 0; i < digits_count - 1; ++i)
    {
        _other_digits[1 + i] = *reinterpret_cast<unsigned int const *>(&digits[i]);
    }
}

void big_integer::initialize_from(
        std::string const &value,
        size_t base)
{
    std::vector<int> converted = convert_to_base(value);
    for(int i = 0; i < converted.size(); ++i) std::cout<<converted[i]<<" ";
    std::cout<<std::endl;
    initialize_from(converted, converted.size());
}

void big_integer::print_byte(
        std::ostream &stream,
        unsigned char byte_value)
{
    for (int i = 0; i < 8; i++)
    {
        stream << ((byte_value >> (7 - i)) & 1);
    }
}

void big_integer::dump_int_value(
        std::ostream &stream,
        int value)
{
    auto *p = (unsigned char *)&value;
    for (int i = 0; i < sizeof(int); i++)
    {
        print_byte(stream, *p++);
        stream << ' ';
    }
}

big_integer &big_integer::change_sign()
{
    _oldest_digit ^= (1 << ((sizeof(int) << 3) - 1));

    return *this;
}

size_t big_integer::binary_power(size_t number, unsigned int power)
{
    size_t result = 1;

    while (power)
    {
        if(power & 1) result *= number;
        number*=number;
        power>>=1;
    }
    return result;
}
std::vector<int> big_integer::convert_to_base(std::string const &biiiiiiiiiiig_number)
{

    unsigned int base = 1<<(8 * sizeof(int) - 2);
    bool is_negative = false;

    int pos = 0;

    if(biiiiiiiiiiig_number[0] == '-')
    {
        is_negative = true;
        pos = 1;
    }
//TODO: remove leading zeros!!!!
    auto divide_str_on_int = [](std::string str, size_t number, int position = 0) -> std::vector<int>
    {
        std::vector<int> result;
        size_t converted = 0;

        while(position != str.length())
        {
            std::string next_number_to_divide("");

            while(converted < number)
            {
                if(position == str.length()) break;

                converted = converted * 10 + (str[position] - '0');
                position ++;
            }
            if(position == str.length())
            {
                if(converted >= number)
                {
                    result.push_back(converted % number);
                    converted /= number;
                }
                result.push_back(converted);
                return  result;
            }

            while(position != str.length())
            {

                if(converted >= number)
                {
                    next_number_to_divide.push_back(converted/number + '0');
                    converted %= number;
                }
                else next_number_to_divide.push_back('0');

                if(position != str.length()) converted = converted * 10 + (str[position] - '0');

                position++;
            }
            if(converted >= number)
            {
                next_number_to_divide.push_back(converted/number + '0');
                converted %= number;
            }
            result.push_back(converted);
            str = std::move(next_number_to_divide);
            converted = 0;
            position = 0;
        }

        return result;
    };

    std::vector<int> result = divide_str_on_int(biiiiiiiiiiig_number, base, pos ? pos : 0);

    if(is_negative)
    {
        result[result.size() - 1] |= 1<<(sizeof(int) * 8 - 1);
    }
    return result;

}

inline int big_integer::get_digits_count() const noexcept
{
    return static_cast<int>(_other_digits == nullptr
                            ? 1
                            : *_other_digits);
}

inline int big_integer::sign() const noexcept
{
    if (is_equal_to_zero())
    {
        return 0;
    }

    return 1 - (static_cast<int>((*reinterpret_cast<unsigned int const *>(&_oldest_digit) >> ((sizeof(int) << 3) - 1))) << 1);
}

inline bool big_integer::is_equal_to_zero() const noexcept
{
    return _oldest_digit == 0 && _other_digits == nullptr;
}

inline unsigned int big_integer::get_digit(
        int position) const noexcept
{
    if (_other_digits == nullptr)
    {
        return position == 0
               ? _oldest_digit
               : 0;
    }

    int const digits_count = get_digits_count();

    if (position < digits_count - 1)
    {
        return _other_digits[position + 1];
    }

    if (position == digits_count - 1)
    {
        return _oldest_digit;
    }

    return 0;
}

big_integer::big_integer(std::string const& digits)
{
    initialize_from(digits, digits.size());
}

big_integer::big_integer(
        int const *digits,
        size_t digits_count)
{
    initialize_from(digits, digits_count);
}

big_integer::big_integer(
        std::vector<int> const &digits)
{
    // TODO: remove additional zeros
    initialize_from(digits, digits.size());
}

big_integer::big_integer(
        std::string const &value,
        size_t base)
{
    initialize_from(value, base);
}

big_integer::big_integer(
        big_integer const &other)
{
    copy_from(other);
}

big_integer &big_integer::operator=(
        big_integer const &other)
{
    if (this != &other)
    {
        clear();
        copy_from(other);
    }

    return *this;
}

big_integer::~big_integer()
{
    clear();
}

big_integer big_integer::divide(big_integer &bigInteger1, big_integer bigInteger2,
                         big_integer::division::division_rule divisionRule)
{
    switch (divisionRule)
    {
        case division::trivial:
            big_integer::division::trivial_division(bigInteger1, bigInteger2);
    }

    return bigInteger1;
}

big_integer &big_integer::operator+=(
        big_integer const &other)
{
    if (other.is_equal_to_zero())
    {
        return *this;
    }

    if (is_equal_to_zero())
    {
        return *this = other;
    }

    if (sign() == -1)
    {
        change_sign();
        *this += -other;
        return change_sign();
    }

    if (other.sign() == -1)
    {
        return *this -= -other;
    }

    auto const first_value_digits_count = get_digits_count();
    auto const second_value_digits_count = other.get_digits_count();
    auto const digits_count = std::max(first_value_digits_count, second_value_digits_count);

    unsigned int operation_result = 0;

    constexpr int shift = sizeof(unsigned int) << 2;
    constexpr int mask = (1 << shift) - 1;

    std::vector<int> result_digits(digits_count + 1);

    for (int i = 0; i < digits_count; ++i)
    {
        unsigned int first_value_digit = get_digit(i);
        unsigned int second_value_digit = other.get_digit(i);
        result_digits[i] = 0;

        for (int j = 0; j < 2; ++j)
        {
            operation_result += (first_value_digit & mask) + (second_value_digit & mask);
            first_value_digit >>= shift;
            second_value_digit >>= shift;
            *reinterpret_cast<unsigned int *>(&result_digits[i]) |= ((operation_result & mask) << shift * j);
            operation_result >>= shift;
        }
    }

    result_digits.back() += *reinterpret_cast<int *>(&operation_result);

    auto *maybe_overflowed_digit_ptr = reinterpret_cast<unsigned int *>(&*(result_digits.end() - 2));
    if (*maybe_overflowed_digit_ptr >> ((sizeof(unsigned int) << 3) - 1))
    {
        *maybe_overflowed_digit_ptr ^= (1 << ((sizeof(unsigned int) << 3) - 1));
        ++result_digits.back();
    }

    auto result_digits_count = result_digits.size();
    if (result_digits.back() == 0)
    {
        --result_digits_count;
    }

    clear();
    initialize_from(result_digits, result_digits_count);

    return *this;
}

big_integer big_integer::operator+(
        big_integer const &other) const
{
    return big_integer(*this) += other;
}

big_integer &big_integer::operator-=(
        big_integer const &other)
{

}

big_integer big_integer::operator-(
        big_integer const &other) const
{
    return big_integer(*this) -= other;
}

big_integer big_integer::operator-() const
{
    return big_integer(*this).change_sign();
}

big_integer &big_integer::operator*=(
        big_integer const &other)
{
    big_integer result("0");
    big_integer copied_other(other);

    bool _is_result_negative;

    if(this->sign() == -1 && copied_other.sign() == 1 || this->sign() == 1 && copied_other.sign() == -1)
    {
        _is_result_negative = true;
        this->sign() == -1 ? this->change_sign() : copied_other.change_sign();
    }
    else if(this->sign() == -1 && copied_other.sign() == -1)
    {
        _is_result_negative = false;
        this->change_sign();
        copied_other.change_sign();
    }
    else _is_result_negative = false;



    int const size_of_this = this->get_digits_count();
    int const size_of_other = copied_other.get_digits_count();
    unsigned int const base = 1<<(sizeof(int) * 8 - 2);

    constexpr int shift = sizeof(unsigned int) << 2;
    constexpr unsigned int mask = (1U << shift) - 1;

    for(int i = 0; i < size_of_this; ++i)
    {

        int remainder = 0;
        std::vector<int> digits_array;
        int real_size = 0;
        

        result <<= i;
        for(int j = 0; j < size_of_other; ++j)
        {

            auto first_number = this->get_digit(i);
            auto second_number = copied_other.get_digit(j);

            unsigned int A1 = (first_number >> shift) & mask;
            unsigned int A0 = first_number & mask;
            unsigned int B1 = (second_number >> shift) & mask;
            unsigned int B0 = second_number & mask;

            unsigned long long A1B1 = static_cast<unsigned long long>(A1) * B1;
            unsigned long long A1B0 = static_cast<unsigned long long>(A1) * B0;
            unsigned long long A0B1 = static_cast<unsigned long long>(A0) * B1;
            unsigned long long A0B0 = static_cast<unsigned long long>(A0) * B0;


            unsigned long long multiply_result = (A1B1 << (2 * shift)) + ((A1B0 + A0B1) << shift) + A0B0;
            big_integer number(std::to_string(multiply_result));
            result<<i;
            result += number;


            /*if(multiply_result + remainder >= base)
            {
                digits_array.push_back((multiply_result + remainder) % base);
                remainder = (multiply_result + remainder) / base;
            }
            else
            {
                digits_array.push_back(multiply_result + remainder);
                remainder = 0;
            }*/
        }

       /* if(remainder)
        {
            digits_array.push_back(remainder);
        }
        big_integer tmp(digits_array);
        std::stringstream check_tmp;
        check_tmp<<tmp;
        std::cout<<"tmp № "<<" "<<i<<" "<<check_tmp.str()<<std::endl;
        result += tmp;*/

    }

    *this = std::move(result);

    if(_is_result_negative) this->change_sign();
    return *this;
}

big_integer big_integer::operator*(
        big_integer const &other) const
{
    return big_integer(*this) *= other;
}

big_integer &big_integer::operator/=(
        big_integer const &other)
{

}

big_integer big_integer::operator/(
        big_integer const &other) const
{
    return big_integer(*this) /= other;
}

big_integer &big_integer::operator%=(
        big_integer const &other)
{
    return *this = *this - (*this/other) * other;
}

big_integer big_integer::operator%(
        big_integer const &other) const
{
    return big_integer(*this) %= other;
}

bool big_integer::operator==(
        big_integer const &other) const
{
    if(this->_oldest_digit == other._oldest_digit)
    {
        if(this->_other_digits == other._other_digits)
        {
            int quantity = *this->_other_digits;
            for(int i = 1; i <= quantity; ++i) if(this->_other_digits[i] != other._other_digits[i]) return false;
        }
        else return false;
    }

    return false;
}

bool big_integer::operator!=(
        big_integer const &other) const
{
    return !(*this == other);
}

bool big_integer::operator<(
        big_integer const &other) const
{
    if(this->sign() != other.sign()) return this->sign() == 1 ? false : true;

    else
    {
        if(*this->_other_digits > *other._other_digits) return this->sign() == 1 ? false : true;
        else if(*this->_other_digits < *other._other_digits) return this->sign() == 1 ? true : false;

        else
        {
            int digits_count = *this->_other_digits;

            for(int i = 1; i <= digits_count; ++i)
            {
                if(this->_other_digits[i] == other._other_digits[i]) continue;

                if(this->_other_digits[i] > other._other_digits[i]) return this->sign()==1 ? false : true;

                else if(this->_other_digits[i] < other._other_digits[i]) return this->sign() == 1 ? true : false;


            }
            return false;
        }

    }
}

bool big_integer::operator<=(
        big_integer const &other) const
{
    if(this->sign() != other.sign()) return this->sign() == 1 ? false : true;

    else
    {
        if(*this->_other_digits > *other._other_digits) return this->sign() == 1 ? false : true;
        else if(*this->_other_digits < *other._other_digits) return this->sign() == 1 ? true : false;

        else
        {
            int digits_count = *this->_other_digits;

            for(int i = 1; i <= digits_count; ++i)
            {
                if(this->_other_digits[i] == other._other_digits[i]) continue;

                if(this->_other_digits[i] > other._other_digits[i]) return this->sign()==1 ? false : true;

                else if(this->_other_digits[i] < other._other_digits[i]) return this->sign() == 1 ? true : false;


            }
            return true;
        }

    }
}

bool big_integer::operator>(
        big_integer const &other) const
{
    if(this->sign() != other.sign()) return this->sign() == 1 ? true : false;

    else
    {
        if(*this->_other_digits > *other._other_digits) return this->sign() == 1 ? true : false;
        else if(*this->_other_digits < *other._other_digits) return this->sign() == 1 ? false : true;

        else
        {
            int digits_count = *this->_other_digits;

            for(int i = 1; i <= digits_count; ++i)
            {
                if(this->_other_digits[i] == other._other_digits[i]) continue;

                if(this->_other_digits[i] > other._other_digits[i]) return this->sign()==1 ? true : false;

                else if(this->_other_digits[i] < other._other_digits[i]) return this->sign() == 1 ? false : true;


            }
            return false;
        }

    }
}

bool big_integer::operator>=(
        big_integer const &other) const
{
    if(this->sign() != other.sign()) return this->sign() == 1 ? true : false;

    else
    {
        if(*this->_other_digits > *other._other_digits) return this->sign() == 1 ? true : false;
        else if(*this->_other_digits < *other._other_digits) return this->sign() == 1 ? false : true;

        else
        {
            int digits_count = *this->_other_digits;

            for(int i = 1; i <= digits_count; ++i)
            {
                if(this->_other_digits[i] == other._other_digits[i]) continue;

                if(this->_other_digits[i] > other._other_digits[i]) return this->sign()==1 ? true : false;

                else if(this->_other_digits[i] < other._other_digits[i]) return this->sign() == 1 ? false : true;


            }
            return true;
        }

    }
}

big_integer big_integer::operator~() const
{
    auto digits = get_digits_count();

    int* new_digits = new int[digits + 1];
    new_digits[0] = digits;

    for(int i = 1; i < digits; ++i)
    {
        auto current = get_digit(i);
        current = ~_other_digits[i];
        new_digits[i] = current;
    }

    big_integer result(new_digits, *new_digits);
    return result;

}

big_integer &big_integer::operator&=(
        big_integer const &other)
{
    int const size_1 = this->get_digits_count();
    int const size_2 = other.get_digits_count();

    int const new_size = std::min(size_1, size_2);

    int* new_digits = new int[new_size + 1];
    *new_digits = new_size;

    for(int i = 1; i <= new_size; ++i)
    {
        new_digits[i] = this->get_digit(i) & other.get_digit(i);
    }

    this->clear();
    this->initialize_from(new_digits, new_size);

    return *this;
}

big_integer big_integer::operator&(
        big_integer const &other) const
{
    return big_integer(*this) &= other;
}

big_integer &big_integer::operator|=(
        big_integer const &other)
{
    int const size_1 = this->get_digits_count();
    int const size_2 = other.get_digits_count();

    int const new_size =  std::max(size_1, size_2);
    int* new_digits = new int[new_size + 1];

    *new_digits = new_size;

    for(int i = 0; i < new_size; ++i)
    {
        if(i != size_1 && i != size_2) new_digits[i] = this->get_digit(i) | other.get_digit(i);
        else
        {
             new_digits[i] = i == size_1 ? other.get_digit(i) : this->get_digit(i);
        }
    }
    this->clear();
    this->initialize_from(new_digits, new_size);

    return *this;
}

big_integer big_integer::operator|(
        big_integer const &other) const
{
    return big_integer(*this) |= other;
}

big_integer &big_integer::operator^=(
        big_integer const &other)
{
    int const size_1 = this->get_digits_count();
    int const size_2 = other.get_digits_count();

    int const new_size =  std::max(size_1, size_2);
    int* new_digits = new int[new_size + 1];

    *new_digits = new_size;

    for(int i = 0; i < new_size; ++i)
    {
        if(i != size_1 && i != size_2) new_digits[i] = this->get_digit(i) | other.get_digit(i);
        else
        {
            new_digits[i] = i == size_1 ? other.get_digit(i) ^ 0 : this->get_digit(i) ^ 0;
        }
    }
    this->clear();
    this->initialize_from(new_digits, new_size);

    return *this;
}

big_integer big_integer::operator^(
        big_integer const &other) const
{
    return big_integer(*this) ^= other;
}

big_integer &big_integer::operator<<=(
        size_t shift_value)
{
    if (is_equal_to_zero() || shift_value == 0)
    {
        return *this;
    }

    auto value_sign = sign();
    if (value_sign == -1)
    {
        change_sign();
    }

    auto const added_by_shift_at_other_digits_digits_count = shift_value / (sizeof(unsigned int) << 3);
    shift_value %= (sizeof(unsigned int) << 3);

    auto added_by_shift_at_oldest_digit_digits_count = 0;
    if (_oldest_digit != 0)
    {
        unsigned int oldest_digit = *reinterpret_cast<unsigned int *>(&_oldest_digit);
        int oldest_value_bit_index = 0;
        while (oldest_digit != 1)
        {
            oldest_digit >>= 1;
            ++oldest_value_bit_index;
        }

        if (oldest_value_bit_index + shift_value > (sizeof(int) << 3) - 2)
        {
            ++added_by_shift_at_oldest_digit_digits_count;
        }
    }

    if (added_by_shift_at_oldest_digit_digits_count != 0 || added_by_shift_at_other_digits_digits_count != 0)
    {
        auto const added_digits_count = added_by_shift_at_oldest_digit_digits_count + added_by_shift_at_other_digits_digits_count;

        if (_other_digits == nullptr)
        {
            _other_digits = new unsigned int[added_digits_count + 1];
            *_other_digits = added_digits_count + 1;
            std::memset(_other_digits + 1, 0, sizeof(unsigned int) * (added_digits_count - 1));
            if (added_by_shift_at_oldest_digit_digits_count != 0)
            {
                _other_digits[*_other_digits - 1] = _oldest_digit;
                _oldest_digit = 0;
            }
            else
            {
                _other_digits[*_other_digits - 1] = 0;
            }
        }
        else
        {
            auto *new_digits = new unsigned int[added_digits_count + *_other_digits];
            std::memset(new_digits + 1, 0, sizeof(unsigned int) * added_digits_count);
            if (added_by_shift_at_oldest_digit_digits_count != 0)
            {
                new_digits[added_digits_count + *_other_digits - 1] = _oldest_digit;
                _oldest_digit = 0;
            }
            std::memcpy(new_digits + 1 + added_by_shift_at_other_digits_digits_count, _other_digits + 1, sizeof(unsigned int) * (*_other_digits - 1));
            *new_digits = *_other_digits + added_digits_count;

            clear();
            _other_digits = new_digits;
        }
    }

    if (shift_value != 0)
    {
        auto const digits_count = get_digits_count();
        unsigned int part_to_move_to_next_digit = 0;
        for (auto i = 0; i < digits_count; ++i)
        {
            auto digit_value = get_digit(i);
            auto *digit_address = i == digits_count - 1
                                  ? reinterpret_cast<unsigned int *>(&_oldest_digit)
                                  : _other_digits + 1 + i;
            *digit_address <<= shift_value;
            *digit_address |= part_to_move_to_next_digit;
            part_to_move_to_next_digit = digit_value >> ((sizeof(unsigned int) << 3) - shift_value);
        }
    }

    if (value_sign == -1)
    {
        change_sign();
    }

    return *this;
}

big_integer big_integer::operator<<(
        size_t shift_value) const
{
    return big_integer(*this) <<= shift_value;
}

big_integer &big_integer::operator>>=(
        size_t shift_value)
{

}

big_integer big_integer::operator>>(
        size_t shift_value) const
{
    return big_integer(*this) >>= shift_value;
}

std::ostream &operator<<(
        std::ostream &stream,
        big_integer const &value)
{

    int const base = 1<<(sizeof(int) * 8 - 2);
    std::string base_in_str = std::to_string(base);

    big_integer integer(value);

    if(value.sign() == -1)
        integer.change_sign();



    auto multiply_strings = [](const std::string &num1, const std::string &num2) {
        int n1 = num1.size();
        int n2 = num2.size();
        std::vector<int> result(n1 + n2, 0);


        std::string num1Rev(num1.rbegin(), num1.rend());
        std::string num2Rev(num2.rbegin(), num2.rend());


        for (int i = 0; i < n1; ++i) {
            for (int j = 0; j < n2; ++j) {
                result[i + j] += (num1Rev[i] - '0') * (num2Rev[j] - '0');
                result[i + j + 1] += result[i + j] / 10;
                result[i + j] %= 10;
            }
        }


        int i = result.size() - 1;
        while (i > 0 && result[i] == 0)
            --i;


        std::string res;
        while (i >= 0)
            res.push_back(result[i--] + '0');

        return res;
    };

    auto sum_two_numbers_in_string = [](std::string& result, std::string& str_to_add)->std::string
    {
        std::reverse(result.begin(), result.end());
        std::reverse(str_to_add.begin(), str_to_add.end());

        std::string tmp("");

        const int size = std::max(result.length(), str_to_add.length());

        const int size_diff = result.length() - str_to_add.length();

        if(size_diff != 0)
            size_diff < 0 ? result.resize(result.length()-size_diff, '0') : str_to_add.resize(size_diff, '0');

        int next_degree = 0;

        for(int i = 0; i < size; ++i)
        {
            int first_digit = str_to_add[i] - '0';
            int second_digit = result[i] - '0';

            int sum_result = first_digit + second_digit + next_degree;

            tmp.push_back((sum_result % 10) + '0');
            next_degree = sum_result / 10;
        }
        if(next_degree) tmp.push_back('1');

        std::reverse(tmp.begin(), tmp.end());
        return tmp;
    };

    std::string converted("");

    int const digits_count = integer.get_digits_count();

    for(int i = 0; i < digits_count - 1; ++i)
    {
        auto number =  integer.get_digit(i);

        std::string current_digit_in_str("1");

        for(int j = 0; j < i; ++j)
            current_digit_in_str = multiply_strings(current_digit_in_str, base_in_str);


        current_digit_in_str = multiply_strings(current_digit_in_str, std::to_string(number));

        std::string tmp = sum_two_numbers_in_string(converted, current_digit_in_str);
        converted = std::move(tmp);
    }

    {
        auto last_digit = integer._oldest_digit;

        std::string current_digit_in_str("1");

        for(int i = 0; i < digits_count - 1; ++i)
            current_digit_in_str = multiply_strings(current_digit_in_str, base_in_str);

        current_digit_in_str = multiply_strings(current_digit_in_str, std::to_string(last_digit));

        std::string tmp = sum_two_numbers_in_string(converted, current_digit_in_str);
        converted = std::move(tmp);
    }


    if(value.sign() == -1) converted = "-" + converted;
    stream<<(converted);
    return stream;
}

std::istream &operator>>(
        std::istream &stream,
        big_integer &value)
{
    std::string source;
    stream >> source;
    big_integer tmp(source);

    value = std::move(tmp);

}