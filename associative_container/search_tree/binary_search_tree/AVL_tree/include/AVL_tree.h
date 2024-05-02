#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H

#include <binary_search_tree.h>

template<
    typename tkey,
    typename tvalue>
class AVL_tree final:
    public binary_search_tree<tkey, tvalue>
{

public:
    explicit AVL_tree<tkey, tvalue>(std::function<int(tkey const &, tkey const &)> keys_comparer = std::less<tkey>(),
            logger* logger = nullptr,
            allocator* allocator = nullptr) : binary_search_tree<tkey, tvalue>(keys_comparer, logger, allocator)
    {

    }
private:
    
    struct node final:
        binary_search_tree<tkey, tvalue>::node
    {
        size_t height;

    public:
        node(
                tkey const &key,
                tvalue const &value
                ) : binary_search_tree<tkey, tvalue>::node(key, value), height(1){}
        node(
                tkey const &key,
                tvalue &&value
                ) : binary_search_tree<tkey, tvalue>::node(key, value), height(1){}
                ~node() noexcept override = default;
    };

class height_and_balance : public binary_search_tree<tkey, tvalue>::template_method_basics
{
protected:
    void count_node_height(node*& current)
    {
        if(!current->right_subtree && !current->left_subtree) current->height = 1;

        if(current->right_subtree && !current->left_subtree) current->height = current->right_subtree->heght + 1;

        if(current->left_subtree && !current->right_subtree) current->height = current->left_subtree->height + 1;

        max(current->left_subtree->height + current->right_subtree->height) + 1;
    }

    size_t balance_factor(node* current)
    {
        if(!current || !current->right_subtree && !current->left_subtree) return 0;

        if(current->right_subtree && !current->left_subtree) return -current->right_subtree->height;

        if(current->left_subtree && !current->right_subtree) return current->left_subtree->height;

        return current->left_subtree->height - current->right_subtree->height;
    }

    void balance(std::stack<typename binary_search_tree<tkey, tvalue>::node**> & path) override
    {
        while(!path.empty())
        {
            node* current_node = path.top();
            count_node_height(current_node);

            path.pop();

            if(std::abs(balance_factor(current_node)) <= 1) continue;

            else
            {
                if(balance_factor(current_node) == 2)
                {
                    if(balance_factor(current_node->left_subtree) == 1)
                    {
                        AVL_tree<tkey, tvalue>::small_right_rotation(current_node);
                    }
                    else
                    {
                        AVL_tree<tkey, tvalue>::big_right_rotation(current_node);
                    }
                }

                else
                {
                    if(balance_factor(current_node->right_subtree) == 1) AVL_tree<tkey, tvalue>::small_left_rotation(current_node);
                    else AVL_tree<tkey, tvalue>::big_left_rotation(current_node);
                }
                count_node_height(current_node->left_subtree);
                count_node_height(current_node->right_subtree);
                count_node_height(current_node);
            }
        }
    }
};


public:
    
    struct iterator_data final:
        public binary_search_tree<tkey, tvalue>::iterator_data
    {

    public:
        unsigned int subtree_height;

    public:
        size_t get_subtree_height() const
        {
            if(!binary_search_tree<tkey, tvalue>::iterator_data::_is_state_initialized) throw std::logic_error("state is unitialized");
            return subtree_height;
        }
    
    public:
        
        explicit iterator_data(
            unsigned int depth,
            tkey const &key,
            tvalue const &value,
            size_t subtree_height);

        iterator_data(): binary_search_tree<tkey, tvalue>::iterator_data()
        {

        }
        
    };

private:
    
    class insertion_template_method final:
        public binary_search_tree<tkey, tvalue>::insertion_template_method, public height_and_balance
    {
    private:
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy _insertion_strtegy;
        allocator* allocator;
    public:
        
        explicit insertion_template_method(
            AVL_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);

    public:
        void insert(tkey const &key, tvalue const &value)
        {
            binary_search_tree<tkey, tvalue>::insertion_template_method::insert(key, value);
        }

        void insert(tkey const &key, tvalue &&value)
        {
            binary_search_tree<tkey, tvalue>::insertion_template_method::insert(key, value);
        }

        void set_insertion_strategy(
                typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
        {
            binary_search_tree<tkey, tvalue>::set_insertion_strategy(insertion_strategy);
        }

        void count_node_height(node*& current)
        {
            if(!current->right_subtree && !current->left_subtree) current->height = 1;

            if(current->right_subtree && !current->left_subtree) current->height = current->right_subtree->heght + 1;

            if(current->left_subtree && !current->right_subtree) current->height = current->left_subtree->height + 1;

            max(current->left_subtree->height + current->right_subtree->height) + 1;
        }

        size_t balance_factor(node* current)
        {
            if(!current || !current->right_subtree && !current->left_subtree) return 0;

            if(current->right_subtree && !current->left_subtree) return -current->right_subtree->height;

            if(current->left_subtree && !current->right_subtree) return current->left_subtree->height;

            return current->left_subtree->height - current->right_subtree->height;
        }

        void balance(std::stack<typename binary_search_tree<tkey, tvalue>::node**> & path) override
        {
            while(!path.empty())
            {
                node* current_node = path.top();
                count_node_height(current_node);

                path.pop();

                if(std::abs(balance_factor(current_node)) <= 1) continue;

                else
                {
                    if(balance_factor(current_node) == 2)
                    {
                        if(balance_factor(current_node->left_subtree) == 1)
                        {
                            AVL_tree<tkey, tvalue>::small_right_rotation(current_node);
                        }
                        else
                        {
                            AVL_tree<tkey, tvalue>::big_right_rotation(current_node);
                        }
                    }

                    else
                    {
                        if(balance_factor(current_node->right_subtree) == 1) AVL_tree<tkey, tvalue>::small_left_rotation(current_node);
                        else AVL_tree<tkey, tvalue>::big_left_rotation(current_node);
                    }
                    count_node_height(current_node->left_subtree);
                    count_node_height(current_node->right_subtree);
                    count_node_height(current_node);
                }
            }
        }

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
        public binary_search_tree<tkey, tvalue>::disposal_template_method, public height_and_balance
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
        std::function<int(tkey const &, tkey const &)> comparer =  std::less<tkey>(),
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:
    
/*    ~AVL_tree() noexcept final;
    
    AVL_tree(
        AVL_tree<tkey, tvalue> const &other);
    
    AVL_tree<tkey, tvalue> &operator=(
        AVL_tree<tkey, tvalue> const &other);
    
    AVL_tree(
        AVL_tree<tkey, tvalue> &&other) noexcept;
    
    AVL_tree<tkey, tvalue> &operator=(
        AVL_tree<tkey, tvalue> &&other) noexcept;*/

private:

    node *_root;

    insertion_template_method *_insertion_template;

    obtaining_template_method *_obtaining_template;

    disposal_template_method *_disposal_template;



    size_t get_node_size() const noexcept override final
    {
        return sizeof(typename AVL_tree<tkey, tvalue>::node);
    }

    void call_node_constructor(
            typename binary_search_tree<tkey, tvalue>::node* raw_space,
            const tkey& key,
            const tvalue& value) override {
        allocator::construct(reinterpret_cast<typename AVL_tree::node*>(raw_space), key, value);
    }

    void call_node_constructor(
            typename binary_search_tree<tkey, tvalue>::node* raw_space,
            tkey const & key,
            tvalue && value) override{
        allocator::construct(static_cast<typename AVL_tree::node*>(raw_space), key, std::move(value));
    }



    void inject_additional_data(
            typename binary_search_tree<tkey, tvalue>::iterator_data *destination,
            typename binary_search_tree<tkey, tvalue>::node *source) override
    {
        static_cast<AVL_tree<tkey, tvalue>::iterator_data*>(destination)->subtree_height = static_cast<AVL_tree<tkey, tvalue>::node*>(source)->height;
    }
};

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value,
    size_t subtree_height):
    binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value), subtree_height(subtree_height)
{

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

template<typename tkey, typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
        allocator *allocator,
        logger *logger,
        std::function<int(tkey const &, tkey const &)> comparer,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) :
        binary_search_tree<tkey, tvalue>::binary_search_tree(
                reinterpret_cast<AVL_tree<tkey, tvalue>::insertion_template_method*>(new typename binary_search_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy)),
                reinterpret_cast<AVL_tree<tkey, tvalue>::obtaining_template_method*>(new typename binary_search_tree<tkey, tvalue>::obtaining_template_method(this)),
                reinterpret_cast<AVL_tree<tkey, tvalue>::disposal_template_method*>(new typename binary_search_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy)),
                comparer,
                allocator,
                logger)

{
}

/*
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
}*/

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H