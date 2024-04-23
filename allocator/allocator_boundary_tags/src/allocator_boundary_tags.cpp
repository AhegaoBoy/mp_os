#include <not_implemented.h>
#include <mutex>
#include <sstream>
#include "../include/allocator_boundary_tags.h"
#include<stdexcept>

allocator_boundary_tags::~allocator_boundary_tags()
{

}

allocator_boundary_tags::allocator_boundary_tags(
    allocator_boundary_tags const &other) : _trusted_memory(other._trusted_memory)
{

}

allocator_boundary_tags &allocator_boundary_tags::operator=(
    allocator_boundary_tags const &other)
{
    if(this != &other) this->_trusted_memory = other._trusted_memory;
    return *this;
}

allocator_boundary_tags::allocator_boundary_tags(
    allocator_boundary_tags &&other) noexcept : _trusted_memory(other._trusted_memory)
{
    other._trusted_memory = nullptr;
}

allocator_boundary_tags &allocator_boundary_tags::operator=(
    allocator_boundary_tags &&other) noexcept
{
    if(this != &other)
    {
        this->_trusted_memory = other._trusted_memory;
        other._trusted_memory = nullptr;
    }
    return *this;
}

allocator_boundary_tags::allocator_boundary_tags(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger_,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if(parent_allocator == nullptr)
    {
        try{
            this->_trusted_memory = ::operator new(space_size + meta_size());
        }
        catch(std::bad_alloc)
        {
            throw;
        }
    }
    else
    {
        try{
            this->_trusted_memory = parent_allocator->allocate(sizeof(size_t), space_size);
        }
        catch(std::bad_alloc)
        {
            throw;
        }
    }

    size_t* allocator_size = reinterpret_cast<size_t*>(_trusted_memory);
    *allocator_size = space_size;

    allocator** _allocator = reinterpret_cast<allocator**>(allocator_size + 1);
    *_allocator = parent_allocator;

    logger** _logger = reinterpret_cast<logger**>(_allocator + 1);
    *_logger = logger_;

    allocator_with_fit_mode::fit_mode* fitMode = reinterpret_cast<allocator_with_fit_mode::fit_mode*>(_logger + 1);
    *fitMode = allocate_fit_mode;

    std::mutex* mutex_object = reinterpret_cast<std::mutex*>(fitMode + 1);
    //mutex_object->unlock();

    void** first_occupied_block = reinterpret_cast<void**>(mutex_object  + 1);
    *first_occupied_block = nullptr;


    debug_with_guard("start address: " + convert_address_to_string(_trusted_memory));
    debug_with_guard("         end " + convert_address_to_string(reinterpret_cast<unsigned char*>(_trusted_memory) + meta_size()));
    debug_with_guard("META SIZE: " + std::to_string(meta_size()) + " CLIENT SIZE: " + std::to_string(*allocator_size));
}

[[nodiscard]] void *allocator_boundary_tags::allocate(
    size_t value_size,
    size_t values_count) {
    debug_with_guard("ALLOCATION METHOD STARTED");
    //std::mutex* mutex = get_mutex();
    //mutex->lock();
    size_t client_size = values_count * value_size;
    void *pointer_to_allocated_memory = nullptr;
    size_t *block = nullptr;

    {
        allocator_with_fit_mode::fit_mode fitMode = get_fit_mode();
        size_t size_between;
        void* previous_occupied_block = nullptr;
        void* current_occupied_block = get_first_occupied();
        void* fit_free_block = nullptr;

        void** next_occupied = nullptr;
        size_t* size_of_block;

        void* saved_prev = nullptr;
        void* saved_next = nullptr;

        size_t size_of_fit_block = allocator_with_fit_mode::fit_mode::the_best_fit == get_fit_mode() ? SIZE_MAX : 0;


        if(!current_occupied_block)
        {
            if (client_size + meta_size_of_block() > get_allocator_size())
                throw std::bad_alloc();


            void** previous = reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(_trusted_memory) + meta_size());
            *previous = nullptr;

            //debug_with_guard("first block address " + convert_address_to_string(previous));

            void** next = reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(previous) + sizeof(void**));
            *next = nullptr;

            allocator** current_allocator = reinterpret_cast<allocator**>(reinterpret_cast<unsigned char*>(next) + sizeof(void**));
            *current_allocator = this;

            size_of_block = reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(current_allocator) + sizeof(allocator**));
            *size_of_block = client_size;

            set_first_occupied(reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(_trusted_memory) + meta_size()));


            size_of_block = reinterpret_cast<size_t*>(current_allocator + 1);
            *size_of_block = client_size;

            fit_free_block = reinterpret_cast<void *>(size_of_block + 1);
            debug_with_guard("ALLOCATED BLOCK:  " + convert_address_to_string(fit_free_block) + " SIZE OF " + std::to_string(meta_size_of_block() + client_size) + "ADDRESS OF NEXT POINT: " +
                                     convert_address_to_string(reinterpret_cast<unsigned char*>(fit_free_block) + 10*sizeof(int) + 2*sizeof(void**) + sizeof(allocator**) + sizeof(size_t*)));
            return fit_free_block;
        }

        previous_occupied_block = current_occupied_block;
        current_occupied_block = get_next_occupied(current_occupied_block);

        size_between = reinterpret_cast<unsigned char*>(previous_occupied_block) - reinterpret_cast<unsigned char*>(_trusted_memory) - meta_size();

        if(size_between && (fitMode == allocator_with_fit_mode::fit_mode::first_fit
           && (size_between >= client_size + meta_size_of_block())
           ||(fitMode == allocator_with_fit_mode::fit_mode::the_best_fit &&
              (size_between <= size_of_fit_block && (size_between >= client_size + meta_size_of_block())))
           ||(fitMode == allocator_with_fit_mode::fit_mode::the_worst_fit &&
              (size_between >= size_of_fit_block && (size_between >= client_size + meta_size_of_block())))))
        {
            fit_free_block = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(_trusted_memory) + meta_size());
            saved_prev = nullptr;
            saved_next = previous_occupied_block;
        }

        while(current_occupied_block)
        {
            size_between = get_size_between_occupied(previous_occupied_block, current_occupied_block);
            if(size_between != 0)
            {


                if(fitMode == allocator_with_fit_mode::fit_mode::first_fit
                    && (size_between >= client_size + meta_size_of_block())
                    ||(fitMode == allocator_with_fit_mode::fit_mode::the_best_fit &&
                        (size_between <= size_of_fit_block && (size_between >= client_size + meta_size_of_block())))
                    ||(fitMode == allocator_with_fit_mode::fit_mode::the_worst_fit &&
                        (size_between >= size_of_fit_block && (size_between >= client_size + meta_size_of_block()))))
                {
                    fit_free_block = block_after_occupied(previous_occupied_block);
                    saved_prev = nullptr;
                    saved_next = previous_occupied_block;
                }
            }

            previous_occupied_block = current_occupied_block;
            current_occupied_block = get_next_occupied(current_occupied_block);


        }

        if(!fit_free_block && !current_occupied_block)
        {
            debug_with_guard("WHERE???");
            if(reinterpret_cast<unsigned char*>(_trusted_memory) + meta_size() + get_allocator_size() - reinterpret_cast<unsigned char*>(previous_occupied_block) - meta_size_of_block() - get_size_of_block(previous_occupied_block)  < client_size + meta_size_of_block())
                    throw std::bad_alloc();


            void** previous = reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(previous_occupied_block) + meta_size_of_block() +
                    get_size_of_block(previous_occupied_block));

            *previous = previous_occupied_block;


            void** next_for_current = reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(previous_occupied_block) + sizeof(void**));
            *next_for_current = reinterpret_cast<void*>(previous);

            void** next = reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(previous) + sizeof(void**));
                *next = nullptr;

            allocator** current_allocator = reinterpret_cast<allocator**>(reinterpret_cast<unsigned char*>(next) + sizeof(void**));
            *current_allocator = this;

            size_of_block = reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(current_allocator) + sizeof(allocator**));
            *size_of_block = client_size;


            fit_free_block = reinterpret_cast<void *>(size_of_block + 1);
            debug_with_guard("ALLOCATED BLOCK:  " + convert_address_to_string(fit_free_block) + " SIZE OF " + std::to_string(meta_size_of_block() + client_size));

            return fit_free_block;
        }
        else if(fit_free_block)
        {
            void** previous_for_free = reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(fit_free_block));
            *previous_for_free = saved_prev;


            void** next_for_free = reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(fit_free_block) + sizeof(void**));
            *next_for_free = reinterpret_cast<void*>(saved_next);

            if(saved_prev)
            {
                void** next_for_previous = reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(saved_prev) + sizeof(void**));
                *next_for_previous = fit_free_block;
            }

            else set_first_occupied(fit_free_block);

            void** prev_for_next = reinterpret_cast<void**>(saved_next);
            *prev_for_next = fit_free_block;

            allocator** current_allocator = reinterpret_cast<allocator**>(reinterpret_cast<unsigned char*>(next_for_free) + sizeof(void**));
            *current_allocator = this;

            size_of_block = reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(current_allocator) + sizeof(allocator**));
            *size_of_block = client_size;

            pointer_to_allocated_memory = reinterpret_cast<void*>(size_of_block + 1);

            debug_with_guard("ALLOCATED BLOCK:  " + convert_address_to_string(pointer_to_allocated_memory) + " SIZE OF " + std::to_string(meta_size_of_block() + client_size));

            return pointer_to_allocated_memory;
        }
        else throw std::bad_alloc();
    }
}

void allocator_boundary_tags::deallocate(
    void *at)
{
/*    std::mutex* mutex = get_mutex();
    mutex->lock();*/
    debug_with_guard("BLOCK TO DEALLOCATE: " + convert_address_to_string(at));

    if(get_allocator_for_occupied(at) != this)
    {
        debug_with_guard("MEMORY WAS ALLOCATED IN ANOTHER ALLOCATOR OBJECT!!!!!!!!!!!!!!!!!");
        //->unlock();
        throw std::logic_error("allocated in another object");
    }

    void* previous_occupied = nullptr;
    void* at_block = nullptr;
    void* next_occupied = nullptr;

    {
        void* current = get_first_occupied();
        void* previous = nullptr;

        while(current < at && current)
        {
            previous = current;
            current = get_next_occupied(current);
        }
        at_block = previous;
        next_occupied = current;
        previous_occupied = at_block ? get_previous_occupied(at_block) : nullptr;
    }

/*    debug_with_guard("PREVIOUS BLOCK ADDRESS " + convert_address_to_string(previous_occupied));
    debug_with_guard("NEXT BLOCK ADDREESS " + convert_address_to_string(next_occupied));*/

    if(!previous_occupied  && !next_occupied)
    {
        void** pointer_to_first_allocated = reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(_trusted_memory) + meta_size() - sizeof(void**));

        *pointer_to_first_allocated = nullptr;
    }

    else if(!previous_occupied)
    {
        set_first_occupied(next_occupied);
        void** pointer_to_start = reinterpret_cast<void**>(next_occupied);
        *pointer_to_start = nullptr;
    }

    else if(!next_occupied)
    {
        void** pointer_to_next = reinterpret_cast<void**>(previous_occupied);

        *pointer_to_next = nullptr;
    }

    else
    {
        void** prev = reinterpret_cast<void**>(next_occupied);
        *prev = previous_occupied;

        void** next = reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(previous_occupied) + sizeof(void**));
        *next = next_occupied;

    }

    //TODO: allocator state remake!!!

    std::vector<allocator_test_utils::block_info> allocator_state = std::move(get_blocks_info());
    std::string state_in_string("");
    for(size_t i  = 0; i < allocator_state.size(); ++i)
    {
        state_in_string += "Block №" + std::to_string(i + 1) + " is " + (allocator_state[i].is_block_occupied ? "occupied " : "free ") + "size: " + std::to_string(allocator_state[i].block_size) + "\n";
    }
    debug_with_guard("BLOCK WAS SUCCESSFULLY DEALLOCATED, CURRENT ALLOCATOR STATE\n" + state_in_string + "\n");
    //mutex->unlock();

}

inline void allocator_boundary_tags::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(reinterpret_cast<logger**>(reinterpret_cast<allocator**>(reinterpret_cast<size_t*>(_trusted_memory) + 1) + 1) + 1) = mode;
}

inline allocator *allocator_boundary_tags::get_allocator() const
{
    return *reinterpret_cast<allocator**>(reinterpret_cast<size_t*>(_trusted_memory) + 1);
}

std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info() const noexcept
{
    void* previous = nullptr;
    void* current = get_first_occupied();
    std::vector<allocator_test_utils::block_info> vc;

    allocator_test_utils::block_info block_sate;

    if(reinterpret_cast<unsigned char*>(current) - reinterpret_cast<unsigned char*>(_trusted_memory) - meta_size() != 0)
    {
        block_sate.block_size = reinterpret_cast<unsigned char*>(current) - reinterpret_cast<unsigned char*>(_trusted_memory) - meta_size();
        block_sate.is_block_occupied = false;
        vc.push_back(block_sate);



        block_sate.is_block_occupied = true;
        block_sate.block_size = get_size_of_block(current) + meta_size_of_block();

        vc.push_back(block_sate);
    }
    else
    {
        block_sate.block_size = get_size_of_block(current) + meta_size_of_block();
        block_sate.is_block_occupied = true;

        debug_with_guard("BLOCK 1: " + convert_address_to_string(current) + " IS OCCUPIED");
        vc.push_back(block_sate);
    }

    previous = current;
    current = get_next_occupied(current);

    while(current)
    {
        size_t size_between = get_size_between_occupied(previous, current);
        if(size_between != 0)
        {
            block_sate.block_size = size_between;
            block_sate.is_block_occupied = false;

            vc.push_back(block_sate);
            debug_with_guard("BLOCK :" + convert_address_to_string(reinterpret_cast<unsigned char*>(previous) + meta_size_of_block() +                                                                        get_size_of_block(current)) + " IS FREE ");
        }
        block_sate.is_block_occupied = true;
        block_sate.block_size = get_size_of_block(current) + meta_size_of_block();

        debug_with_guard("BLOCK: " + convert_address_to_string(current) + " IS OCCUPIED");
        vc.push_back(block_sate);

        previous = current;
        current = get_next_occupied(current);

    }

    if(reinterpret_cast<unsigned char*>(_trusted_memory) + meta_size() + get_allocator_size() - reinterpret_cast<unsigned char*>(previous) - meta_size_of_block() - get_size_of_block(previous) != 0)
    {
        block_sate.is_block_occupied = false;
        block_sate.block_size = reinterpret_cast<unsigned char*>(_trusted_memory) + meta_size() + get_allocator_size() - reinterpret_cast<unsigned char*>(previous) - get_size_of_block(previous) - meta_size_of_block();

        debug_with_guard("BLOCK: " + convert_address_to_string(reinterpret_cast<unsigned char*>(previous) + meta_size_of_block() + get_size_of_block(previous)) + " IS FREE");
        vc.push_back(block_sate);
    }
    return vc;
}

inline logger *allocator_boundary_tags::get_logger() const
{
    return *reinterpret_cast<logger**>(reinterpret_cast<allocator**>(reinterpret_cast<size_t*>(_trusted_memory) + 1) + 1);
}

inline std::string allocator_boundary_tags::get_typename() const noexcept
{
    return typeid(this).name();
}

inline std::mutex* allocator_boundary_tags::get_mutex() const
{
    return reinterpret_cast<std::mutex*>(reinterpret_cast<allocator_with_fit_mode::fit_mode*>(reinterpret_cast<logger**>(reinterpret_cast<allocator**>(reinterpret_cast<size_t*>(_trusted_memory) + 1) + 1) + 1) + 1);
}

inline allocator_with_fit_mode::fit_mode allocator_boundary_tags::get_fit_mode() const
{
    return *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(reinterpret_cast<logger**>(reinterpret_cast<allocator**>(reinterpret_cast<size_t*>(_trusted_memory) + 1) + 1) + 1);
}

size_t allocator_boundary_tags::get_allocator_size() const noexcept
{
    return *reinterpret_cast<size_t*>(_trusted_memory);
}
size_t allocator_boundary_tags::meta_size_of_block() const noexcept
{
    return sizeof(size_t*) + 2*sizeof(void**) + sizeof(allocator**);
}

size_t allocator_boundary_tags::meta_size() const noexcept
{
    return sizeof(size_t*) + sizeof(allocator**) + sizeof(logger**) + sizeof(allocator_with_fit_mode::fit_mode*) + sizeof(std::mutex*) + sizeof(void**);
}

size_t allocator_boundary_tags::get_size_of_block(void * pointer) const noexcept
{
    return *(reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(pointer) + meta_size_of_block() - sizeof(size_t*)));
}

void* allocator_boundary_tags::get_first_occupied() const noexcept
{
    return *(reinterpret_cast<void**>(reinterpret_cast<std::mutex*>(reinterpret_cast<allocator_with_fit_mode::fit_mode*>(reinterpret_cast<logger**>(reinterpret_cast<allocator**>(reinterpret_cast<size_t*>(_trusted_memory) + 1) + 1) + 1) + 1) + 1));
}

void allocator_boundary_tags::set_first_occupied(void* pointer) const noexcept
{
    void** pointer_to_first_occupied = reinterpret_cast<void**>(reinterpret_cast<std::mutex*>(reinterpret_cast<allocator_with_fit_mode::fit_mode*>(reinterpret_cast<allocator**>(reinterpret_cast<logger**> (reinterpret_cast<size_t*>(_trusted_memory) + 1) + 1) + 1) + 1) + 1);
    *pointer_to_first_occupied = pointer;
}
std::string allocator_boundary_tags::convert_address_to_string(const void *address) const {
    std::ostringstream get_the_address;
    get_the_address << address;
    std::string address_in_str = get_the_address.str();
    return address_in_str;
}

void* allocator_boundary_tags::get_next_occupied(void* pointer) const noexcept
{
    return *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(pointer) + sizeof(void**));
}

void* allocator_boundary_tags::get_previous_occupied(void* pointer) const noexcept
{
    return *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(pointer));
}

size_t allocator_boundary_tags::get_size_between_occupied(void * previous, void * current) const noexcept
{
    unsigned char* end_of_previous_block = reinterpret_cast<unsigned char*>(previous) + meta_size_of_block() + get_size_of_block(previous);

    unsigned char* start_of_current = reinterpret_cast<unsigned char*>(current);

    return start_of_current - end_of_previous_block;
}

void* allocator_boundary_tags::block_after_occupied(void* this_occupied) const noexcept
{
    return reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(this_occupied) + meta_size_of_block() + get_size_of_block(this_occupied));
}

allocator* allocator_boundary_tags::get_allocator_for_occupied(void* block) const noexcept
{
    return *reinterpret_cast<allocator**>(reinterpret_cast<unsigned char*>(block) - sizeof(size_t*) - sizeof(allocator**));
}