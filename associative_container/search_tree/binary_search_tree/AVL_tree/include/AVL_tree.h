#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H

#include <binary_search_tree.h>

template<
    typename tkey,
    typename tvalue>
class AVL_tree final:
    public binary_search_tree<tkey, tvalue>
{

private:
    
    struct node final:
        binary_search_tree<tkey, tvalue>::node
    {
        tkey key;
        tvalue value;
        node* left_subtree;
        node* right_subtree;
        size_t height;
    };

public:
    
    struct iterator_data final:
        public binary_search_tree<tkey, tvalue>::iterator_data
    {

    public:
        unsigned int depth;
    private:
        tkey* _key;
        tvalue* _value;
        size_t* subtree_height;
        bool _is_state_initialized;

    public:
        size_t get_subtree_height() const
        {
            if(!_is_state_initialized) throw std::logic_error("state is unitialized");
            return *subtree_height;
        }
    
    public:
        
        explicit iterator_data(
            unsigned int depth,
            tkey const &key,
            tvalue const &value,
            size_t subtree_height);

        iterator_data():
                _key(reinterpret_cast<tkey*>(::operator new(sizeof (tkey)))),
                _value(reinterpret_cast<tvalue*>(::operator new(sizeof(tvalue)))),
                subtree_height(reinterpret_cast<size_t*>(::operator new(sizeof (size_t))))
        {

        }

    public:
        iterator_data(iterator_data const& other)  noexcept :
                _key(reinterpret_cast<tkey*>(::operator new(sizeof(tkey)))),
                _value(reinterpret_cast<tvalue*>(operator new(sizeof(tvalue)))),
                subtree_height(reinterpret_cast<size_t*>(::operator new(sizeof(size_t)))),
                _is_state_initialized(other._is_state_initialized),
                depth(other.depth)
        {
            allocator::construct(this->_key, *(other._key));
            allocator::construct(this->_value, *(other._value));
            allocator::construct(this->subtree_height, *(other.subtree_height));
        }

        iterator_data& operator=(iterator_data const& other) noexcept
        {
            if(this != &other)
            {
                this->_key = other._key;
                this->_value = other._value;
                this->_is_state_initialized = other._is_state_initialized;
                this->subtree_height = other.subtree_height;
                this->depth = other.depth;
            }
            return *this;
        }

        iterator_data (iterator_data&& other) noexcept : _key(other._key), _value(other._value), _is_state_initialized(other._is_state_initialized), depth(other.depth)
        {
            other._value = nullptr;
            other._key = nullptr;
            other.subtree_height = nullptr;
            other.depth = 0;
            other._is_state_initialized = false;
        }

        iterator_data& operator=(iterator_data&& other) noexcept
        {
            if(this != &other)
            {
                this->_key = other._key;
                this->_value = other._value;
                this->_is_state_initialized = other._is_state_initialized;
                this->subtree_height = other.subtree_height;
                this->depth = other.depth;

                other._value = nullptr;
                other._key = nullptr;
                other.subtree_height = nullptr;
                other.depth = 0;
                other._is_state_initialized = false;
            }
            return *this;
        }


        virtual ~iterator_data() noexcept
        {
            if (_is_state_initialized)
            {
                allocator::destruct(_key);
                allocator::destruct(_value);
                allocator::destruct((subtree_height));

            }

            ::operator delete(_key);
            _key = nullptr;
            ::operator delete(_value);
            _value = nullptr;
            ::operator delete (subtree_height);
            subtree_height = nullptr;

            _is_state_initialized = false;
        };
        
    };

private:
    
    class insertion_template_method final:
        public binary_search_tree<tkey, tvalue>::insertion_template_method
    {
    private:
        binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy _insertion_strtegy;
        allocator* allocator;
    public:
        
        explicit insertion_template_method(
            AVL_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);
    
    private:
        
        // TODO: think about it!
        
    };
    
    class obtaining_template_method final:
        public binary_search_tree<tkey, tvalue>::obtaining_template_method
    {
    
    public:
        
        explicit obtaining_template_method(
            AVL_tree<tkey, tvalue> *tree);
        
        // TODO: think about it!
        
    };
    
    class disposal_template_method final:
        public binary_search_tree<tkey, tvalue>::disposal_template_method
    {
    
    public:
        
        explicit disposal_template_method(
            AVL_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);
        
        // TODO: think about it!
        
    };

public:
    
    explicit AVL_tree(
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:
    
    ~AVL_tree() noexcept final;
    
    AVL_tree(
        AVL_tree<tkey, tvalue> const &other);
    
    AVL_tree<tkey, tvalue> &operator=(
        AVL_tree<tkey, tvalue> const &other);
    
    AVL_tree(
        AVL_tree<tkey, tvalue> &&other) noexcept;
    
    AVL_tree<tkey, tvalue> &operator=(
        AVL_tree<tkey, tvalue> &&other) noexcept;


private:

    node *_root;

    insertion_template_method *_insertion_template;

    obtaining_template_method *_obtaining_template;

    disposal_template_method *_disposal_template;

private:
    void clear(node* &subtree_root) final;

    node* copy(node const * subtree_root) override final
    {
        if (subtree_root == nullptr)
        {
            return nullptr;
        }

        node *subtree_root_copied = reinterpret_cast<node *>(this->allocate_with_guard(get_node_size(), 1));
        call_node_constructor(subtree_root_copied, subtree_root->key, subtree_root->value);
        subtree_root_copied->left_subtree = copy(subtree_root->left_subtree);
        subtree_root_copied->right_subtree = copy(subtree_root->right_subtree);

        return subtree_root_copied;
    }


    size_t get_node_size() const override final
    {
        return sizeof(AVL_tree<tkey, tvalue>::node);
    }

    void call_node_constructor(
            node* raw_space,
            tkey const &key,
            tvalue && value) override final
    {
        allocator::construct(raw_space, key, std::move(value));
    }

    iterator_data* create_iterator_data() const;

    void inject_additional_data(iterator_data* destination,
                                node* source) override
    {

    }

public:
    void insert(tkey const &key, tvalue const &value) override final;

    void insert(tkey const &key, tvalue &&value) override final;

    tvalue const &obtain(
            tkey const &key) final;

    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
            tkey const &lower_bound,
            tkey const &upped_bound,
            bool lower_bond_inclusive,
            bool upped_bound_inclusive) override final;

    tvalue dispose(tkey const &key) final;

public:
    void set_insertion_strategy(
            typename AVL_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy
            ) noexcept;

    void set_removal_strategy(typename AVL_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;


};

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value,
    size_t subtree_height):
    binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value)
{
    this->_key = reinterpret_cast<tkey*>(::operator new(sizeof(tkey)));
    this->_value = reinterpret_cast<tvalue*>(::operator new(sizeof(tvalue)));
    this->subtree_height = reinterpret_cast<size_t>(::operator new(sizeof(size_t)));

    allocator::construct(this->_key, key);
    allocator::construct(this->_value, value);
    allocator::construct(this->subtree_height,subtree_height);
    _is_state_initialized = true;
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    AVL_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
    binary_search_tree<tkey, tvalue>::insertion_template_method(tree, insertion_strategy)
{

}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    AVL_tree<tkey, tvalue> *tree):
        binary_search_tree<tkey, tvalue>::obtaining_template_method(tree)
{

}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    AVL_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) : binary_search_tree<tkey, tvalue>::disposal_template_method(tree)
{

}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
    allocator *allocator,
    logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy)

{
    throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue>::AVL_tree(allocator *, logger *, typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy, typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::~AVL_tree() noexcept
{

}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
    AVL_tree<tkey, tvalue> const &other)
{
    throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue>::AVL_tree(AVL_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(
    AVL_tree<tkey, tvalue> const &other)
{
    throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(AVL_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
    AVL_tree<tkey, tvalue> &&other) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue>::AVL_tree(AVL_tree<tkey, tvalue> &&) noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(
    AVL_tree<tkey, tvalue> &&other) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(AVL_tree<tkey, tvalue> &&) noexcept", "your code should be here...");
}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H