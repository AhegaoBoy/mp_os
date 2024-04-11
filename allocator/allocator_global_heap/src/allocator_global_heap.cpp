#include <not_implemented.h>
#include <sstream>
#include "../include/allocator_global_heap.h"

allocator_global_heap::allocator_global_heap(
    logger *logger): _logger(logger)
{
//    debug_with_guard("CONSTRUCTOR OF ALLOCATOR CALLED");
}

allocator_global_heap::~allocator_global_heap()
{
    debug_with_guard("DESTRUCTOR OF ALLOCATOR CALLED");
}

allocator_global_heap::allocator_global_heap(
    allocator_global_heap &&other) noexcept : _logger(std::move(other._logger))
{
    debug_with_guard("MOVE OPERATOR CALLED");
}

allocator_global_heap &allocator_global_heap::operator=(
    allocator_global_heap &&other) noexcept
{
    if(this != &other) this->_logger = std::move(other._logger);
    return *this;
}

std::string allocator_global_heap::get_bytes(void* pointer, size_t end) const noexcept
{
    std::string result = "";
    unsigned char* start = static_cast<unsigned char*>(pointer);
    for(size_t i = 0; i < end; ++i)
    {
        result += std::to_string(static_cast<int>(start[i])) + " ";
    }
    return result;
}
std::string allocator_global_heap::convert_address_to_string(const void *address)
{
    std::ostringstream get_the_address;
    get_the_address << address;
    std::string address_in_str = get_the_address.str();
    return address_in_str;
}

[[nodiscard]] void *allocator_global_heap::allocate(
    size_t value_size,
    size_t values_count)
{
    debug_with_guard("METHOD 'allocate' IS CALLED!");
    auto size_to_allocate = value_size * values_count;
    void* pointer_to_allocated_memory = nullptr;

    try {
        pointer_to_allocated_memory = ::operator new(size_to_allocate + sizeof(allocator*) + sizeof(size_t*));
    }
    catch(std::exception const & ex)
    {
        error_with_guard("MEMORY ALLOCATION FAILED!!!");
        debug_with_guard("METHOD ALLOCATION IS FINISHED WITH EXCEPTION");
        throw std::bad_alloc{};
    }

    size_t* allocated_block_size = reinterpret_cast<size_t*>(pointer_to_allocated_memory);
    *allocated_block_size = size_to_allocate + sizeof(allocator**) + sizeof(size_t*);

    allocator** this_allocator = reinterpret_cast<allocator**>(allocated_block_size + 1);
    *this_allocator = this;

    debug_with_guard("MEMORY ALLOCATED SUCCESSFULLY! META DATA SIZE: "
                     + std::to_string(*allocated_block_size) + " REQUIRED MEMORY " + std::to_string(size_to_allocate));
    debug_with_guard("METHOD 'allocate' FINISHED!!! RETURNING POINTER: " + convert_address_to_string(reinterpret_cast<void*>(this_allocator + 1)));

    return reinterpret_cast<void*>(this_allocator + 1);

}

void allocator_global_heap::deallocate(
    void *at)
{
    allocator_global_heap* object = *reinterpret_cast<allocator_global_heap**>(reinterpret_cast<allocator**>(at) - 1);
    if(object != this)
    {
        error_with_guard("MEMORY WAS ALLOCATED IN ANOTHER OBJECT!!!!");
        return;
    }

    size_t size_of_client_memory = *reinterpret_cast<size_t*>(reinterpret_cast<allocator_global_heap**>(reinterpret_cast<allocator**>(at) - 1) - 1)
            - sizeof(allocator**) - sizeof(size_t*);
    std::string address = convert_address_to_string(at);

    debug_with_guard("MEMORY SIZE OF " + std::to_string(size_of_client_memory) + " AT ADDRESS: " + convert_address_to_string(at) + " WITH DATA " +
                             get_bytes(
                                     at, size_of_client_memory) + " WAS SUCCESSFULLY DEALLOCATED");
    operator delete(reinterpret_cast<size_t*>(reinterpret_cast<allocator_global_heap**>(reinterpret_cast<allocator**>(at) - 1) - 1), *reinterpret_cast<size_t*>(reinterpret_cast<allocator_global_heap**>(reinterpret_cast<allocator**>(at) - 1) - 1));
}

inline logger *allocator_global_heap::get_logger() const
{
    return this->_logger;
}

inline std::string allocator_global_heap::get_typename() const noexcept
{
    trace_with_guard("GET TYPENAME CALLED");
    return typeid(this).name();
}