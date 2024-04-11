#include <not_implemented.h>

#include "../include/allocator_sorted_list.h"

allocator_sorted_list::~allocator_sorted_list()
{

}

allocator_sorted_list::allocator_sorted_list(
    allocator_sorted_list const &other) : _trusted_memory(other._trusted_memory),
    size_of_memory(other.size_of_memory), parent_allocator(other.parent_allocator),
    _fit_mode(other._fit_mode)
{

}

allocator_sorted_list &allocator_sorted_list::operator=(
    allocator_sorted_list const &other)
{
    if(this != &other) this->_trusted_memory = other._trusted_memory;
    return *this;
}

allocator_sorted_list::allocator_sorted_list(
    allocator_sorted_list &&other) noexcept
{
    this->_trusted_memory = other._trusted_memory;
    this->size_of_memory  = other.size_of_memory;
    this->parent_allocator = other.parent_allocator;
    this->_logger = other._logger;
    this->_fit_mode = other._fit_mode;

    other._trusted_memory = nullptr;
    other.parent_allocator = nullptr;
    other.size_of_memory = nullptr;
    other._logger = nullptr;
    other._fit_mode = nullptr;
}

allocator_sorted_list &allocator_sorted_list::operator=(
    allocator_sorted_list &&other) noexcept
{
    if(this != &other)
    {
        this->_trusted_memory = other._trusted_memory;
        this->size_of_memory  = other.size_of_memory;
        this->parent_allocator = other.parent_allocator;
        this->_logger = other._logger;
        this->_fit_mode = other._fit_mode;

        other._trusted_memory = nullptr;
        other.parent_allocator = nullptr;
        other.size_of_memory = nullptr;
        other._logger = nullptr;
        other._fit_mode = nullptr;
    }
    return *this;
}

allocator_sorted_list::allocator_sorted_list(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    this->size_of_memory = &space_size;
    this->parent_allocator = parent_allocator;
    this->_logger = logger;
    this->_fit_mode = &allocate_fit_mode;
}

[[nodiscard]] void *allocator_sorted_list::allocate(
    size_t value_size,
    size_t values_count)
{
    throw not_implemented("[[nodiscard]] void *allocator_sorted_list::allocate(size_t, size_t)", "your code should be here...");
}

void allocator_sorted_list::deallocate(
    void *at)
{
    throw not_implemented("void allocator_sorted_list::deallocate(void *)", "your code should be here...");
}

inline void allocator_sorted_list::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    this->_fit_mode = &mode;
}

inline allocator *allocator_sorted_list::get_allocator() const
{
    return this->parent_allocator;
}

std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept
{
    throw not_implemented("std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept", "your code should be here...");
}

inline logger *allocator_sorted_list::get_logger() const
{
    return this->_logger;
}

inline std::string allocator_sorted_list::get_typename() const noexcept
{
    return typeid(this).name();
}