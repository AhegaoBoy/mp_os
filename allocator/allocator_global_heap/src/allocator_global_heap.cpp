#include <not_implemented.h>

#include "../include/allocator_global_heap.h"

allocator_global_heap::allocator_global_heap(
    logger *logger): _logger(logger)
{
}

allocator_global_heap::~allocator_global_heap()
{

}

allocator_global_heap::allocator_global_heap(
    allocator_global_heap &&other) noexcept : _logger(std::move(other._logger))
{
}

allocator_global_heap &allocator_global_heap::operator=(
    allocator_global_heap &&other) noexcept
{
    if(this != &other) this->_logger = std::move(other._logger);
    return *this;
}

[[nodiscard]] void *allocator_global_heap::allocate(
    size_t value_size,
    size_t values_count)
{
    auto size_to_allocate = value_size * values_count;
    void* pointer_to_allocated_memory = nullptr;

    try {
        pointer_to_allocated_memory = ::operator new(size_to_allocate + sizeof(allocator*));
    }
    catch(std::exception& ex)
    {
        _logger->debug("couldn't allocate memory");
        return pointer_to_allocated_memory;
    }

    _logger->debug("allocated memory sizeof" + std::to_string(size_to_allocate));
    *reinterpret_cast<allocator**>(pointer_to_allocated_memory) = this;
    return pointer_to_allocated_memory;

}

void allocator_global_heap::deallocate(
    void *at)
{
    if(*reinterpret_cast<allocator**>(at) != this)
    {
        _logger->debug("memory was allocated by another allocator!");
    }
    ::operator delete(at);
    _logger->debug("memory was deallocated");
}

inline logger *allocator_global_heap::get_logger() const
{
    return this->_logger;
}

inline std::string allocator_global_heap::get_typename() const noexcept
{
    return typeid(this).name();
}