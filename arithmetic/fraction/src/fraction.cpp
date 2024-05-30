#include "../include/fraction.h"

fraction::fraction(
    big_integer &&numerator,
    big_integer &&denominator):
        _numerator(std::forward<big_integer>(numerator)),
        _denominator(std::forward<big_integer>(denominator))
{

}

fraction::~fraction()
{

}

fraction::fraction(
    fraction const &other):
        _numerator(other._numerator),
        _denominator(other._denominator)
{

}

fraction &fraction::operator=(
    fraction const &other)
{
    if(*this != other)
    {
        this->_numerator = other._numerator;
        this->_denominator = other._denominator;
    }
    return *this;
}

fraction::fraction(
    fraction &&other) noexcept:
        _numerator(std::move(other._numerator)),
        _denominator(std::move(other._denominator))
{

}

fraction &fraction::operator=(
    fraction &&other) noexcept
{
    if(*this != other)
    {
        this->_numerator = std::move(other._numerator);
        this->_denominator = std::move(other._denominator);
    }
    return *this;
}

fraction &fraction::operator+=(
    fraction const &other)
{
    this->_denominator *= other._denominator;
    this->_numerator = this->_numerator * other._denominator + other._numerator * this->_denominator;

    return *this;
}

fraction fraction::operator+(
    fraction const &other) const
{
    return fraction(*this) += other;
}

fraction &fraction::operator-=(
    fraction const &other)
{
    this->_numerator = this->_numerator * other._denominator - other._numerator * this->_denominator;
    this->_denominator *= other._denominator;
    return *this;
}

fraction fraction::operator-(
    fraction const &other) const
{
    return fraction(*this) -= other;
}

fraction &fraction::operator*=(
    fraction const &other)
{
    this->_numerator *= other._numerator;
    this->_denominator *= other._denominator;

    return *this;
}

fraction fraction::operator*(
    fraction const &other) const
{
    return fraction(*this) *= other;
}

fraction &fraction::operator/=(
    fraction const &other)
{
    this->_numerator *= other._denominator;
    this->_denominator *= other._numerator;

    return *this;
}

fraction fraction::operator/(
    fraction const &other) const
{
    return fraction(*this) /= other;
}

bool fraction::operator==(
    fraction const &other) const
{
    if(this->_numerator == other._numerator && this->_denominator == other._denominator)
        return true;

    return false;
}

bool fraction::operator!=(
    fraction const &other) const
{
    return !(*this == other);
}

bool fraction::operator>=(
    fraction const &other) const
{
    fraction copied_this(*this);
    fraction copied_other(other);

    copied_this._denominator *= other._denominator;
    copied_this._numerator *= other._denominator;

    copied_other._denominator *= this->_denominator;
    copied_other._numerator *= this->_denominator;

    return copied_this._numerator >= copied_other._numerator;
}

bool fraction::operator>(
    fraction const &other) const
{
    return fraction(*this) >= other;
}

bool fraction::operator<=(
    fraction const &other) const
{
    fraction copied_this(*this);
    fraction copied_other(other);

    copied_this._denominator *= other._denominator;
    copied_this._numerator *= other._denominator;

    copied_other._denominator *= this->_denominator;
    copied_other._numerator *= this->_denominator;

    return copied_this._numerator <= copied_other._numerator;
}

bool fraction::operator<(
    fraction const &other) const
{
    return fraction(*this) <= other;
}

std::ostream &operator<<(
    std::ostream &stream,
    fraction const &obj)
{

    return stream;
}

std::istream &operator>>(
    std::istream &stream,
    fraction &obj)
{
    std::string numerator;
    std::string denomerator;

    stream>>numerator>>denomerator;



    return stream;
}

fraction fraction::sin(
    fraction const &epsilon) const
{
}

fraction fraction::cos(
    fraction const &epsilon) const
{
}

fraction fraction::tg(
    fraction const &epsilon) const
{
}

fraction fraction::ctg(
    fraction const &epsilon) const
{
}

fraction fraction::sec(
    fraction const &epsilon) const
{
}

fraction fraction::cosec(
    fraction const &epsilon) const
{
}

fraction fraction::arcsin(
    fraction const &epsilon) const
{
}

fraction fraction::arccos(
    fraction const &epsilon) const
{
}

fraction fraction::arctg(
    fraction const &epsilon) const
{
}

fraction fraction::arcctg(
    fraction const &epsilon) const
{
}

fraction fraction::arcsec(
    fraction const &epsilon) const
{
}

fraction fraction::arccosec(
    fraction const &epsilon) const
{
}

fraction fraction::pow(
    size_t degree) const
{
}

fraction fraction::root(
    size_t degree,
    fraction const &epsilon) const
{
}

fraction fraction::log2(
    fraction const &epsilon) const
{
}

fraction fraction::ln(
    fraction const &epsilon) const
{
}

fraction fraction::lg(
    fraction const &epsilon) const
{
}