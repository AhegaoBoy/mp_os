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
        int height;

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

friend typename binary_search_tree<tkey, tvalue>::insertion_template_method;


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



    class insertion_template_method final:
public binary_search_tree<tkey, tvalue>::insertion_template_method
    {
    private:
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy _insertion_strategy;
        allocator* allocator;
        node** _root;
    public:
        
        explicit insertion_template_method(
            AVL_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);


    private:
        void count_node_height(node*& current)
        {
            if(!current->right_subtree && !current->left_subtree)
            {
                current->height = 1;
                return ;
            }

            if(current->right_subtree && !current->left_subtree)
            {
                current->height = reinterpret_cast<AVL_tree::node*>(current->right_subtree)->height + 1;
                return;
            }

            if(current->left_subtree && !current->right_subtree)
            {
                current->height = reinterpret_cast<AVL_tree::node*>(current->left_subtree)->height + 1;
                return ;
            }

            current->height = std::max(reinterpret_cast<AVL_tree::node*>(current->left_subtree)->height, reinterpret_cast<AVL_tree::node*>(current->right_subtree)->height) + 1;
        }

        int balance_factor(node* current)
        {
            if(!current || !current->right_subtree && !current->left_subtree) return 0;

            if(current->right_subtree && !current->left_subtree) return -reinterpret_cast<AVL_tree::node*>(current->right_subtree)->height;

            if(current->left_subtree && !current->right_subtree) return reinterpret_cast<AVL_tree::node*>(current->left_subtree)->height;

            return reinterpret_cast<AVL_tree::node*>(current->left_subtree)->height - reinterpret_cast<AVL_tree::node*>(current->right_subtree)->height;
        }

        void balance(std::stack<typename binary_search_tree<tkey, tvalue>::node**> & path) override
        {
            while(!path.empty())
            {
                node* current_node = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(*(path.top()));
                typename binary_search_tree<tkey, tvalue>::node* tmp = current_node;
                count_node_height(current_node);

                path.pop();

                if(std::abs(balance_factor(current_node)) <= 1) continue;

                else
                {
                    if(balance_factor(current_node) == 2)
                    {
                        if(balance_factor(reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(current_node->left_subtree)) == 1)
                        {
                            node* new_subtree_root = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(current_node->left_subtree);
                            this->_tree->small_right_rotation(tmp);

                            node* left_subtree_new_node = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root->left_subtree);
                            node* right_subtree_new_node = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root->right_subtree);

                            count_node_height(right_subtree_new_node);
                            count_node_height(left_subtree_new_node);
                            count_node_height(new_subtree_root);

                            if(!path.empty())
                                (*path.top())->left_subtree == current_node ? (*path.top())->left_subtree = new_subtree_root : (*path.top())->right_subtree = new_subtree_root;
                            else
                                *_root = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root);
                        }
                        else
                        {
                            node* new_subtree_root = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(current_node->left_subtree->right_subtree);
                            this->_tree->big_right_rotation(tmp);

                            node* left_subtree_new_node = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root->left_subtree);
                            node* right_subtree_new_node = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root->right_subtree);

                            count_node_height(right_subtree_new_node);
                            count_node_height(left_subtree_new_node);
                            count_node_height(new_subtree_root);
                            if(!path.empty())
                                (*path.top())->left_subtree == current_node ? (*path.top())->left_subtree = new_subtree_root : (*path.top())->right_subtree = new_subtree_root;
                            else
                                *_root = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root);
                        }
                    }

                    else
                    {
                        if(balance_factor(reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(current_node->right_subtree)) == 1)
                        {
                            node* new_subtree_root = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(current_node->right_subtree->left_subtree);
                            this->_tree->big_left_rotation(tmp);

                            node* left_subtree_new_node = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root->left_subtree);
                            node* right_subtree_new_node = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root->right_subtree);

                            count_node_height(right_subtree_new_node);
                            count_node_height(left_subtree_new_node);
                            count_node_height(new_subtree_root);

                            if(!path.empty())
                                (*path.top())->left_subtree == current_node ? (*path.top())->left_subtree = new_subtree_root : (*path.top())->right_subtree = new_subtree_root;
                            else
                                *_root = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root);                        }
                        else
                        {
                            node* new_subtree_root = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(current_node->right_subtree);
                            this->_tree->small_left_rotation(tmp);

                            node* left_subtree_new_node = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root->left_subtree);
                            node* right_subtree_new_node = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root->right_subtree);

                            count_node_height(right_subtree_new_node);
                            count_node_height(left_subtree_new_node);
                            count_node_height(new_subtree_root);

                            if(!path.empty())
                                (*path.top())->left_subtree == current_node ? (*path.top())->left_subtree = new_subtree_root : (*path.top())->right_subtree = new_subtree_root;
                            else
                                *_root = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root);
                        }
                    }
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

    private:



    };
    
    class disposal_template_method final:
        public binary_search_tree<tkey, tvalue>::disposal_template_method
    {
    private:
        node** _root;
    public:
        
        explicit disposal_template_method(
            AVL_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);

        void count_node_height(node*& current)
        {
            if(!current) return;

            if(!current->right_subtree && !current->left_subtree)
            {
                current->height = 1;
                return ;
            }

            if(current->right_subtree && !current->left_subtree)
            {
                current->height = reinterpret_cast<AVL_tree::node*>(current->right_subtree)->height + 1;
                return;
            }

            if(current->left_subtree && !current->right_subtree)
            {
                current->height = reinterpret_cast<AVL_tree::node*>(current->left_subtree)->height + 1;
                return ;
            }

            current->height = std::max(reinterpret_cast<AVL_tree::node*>(current->left_subtree)->height, reinterpret_cast<AVL_tree::node*>(current->right_subtree)->height) + 1;
        }

        int balance_factor(node* current)
        {
            if(!current || !current->right_subtree && !current->left_subtree) return 0;

            if(current->right_subtree && !current->left_subtree) return -reinterpret_cast<AVL_tree::node*>(current->right_subtree)->height;

            if(current->left_subtree && !current->right_subtree) return reinterpret_cast<AVL_tree::node*>(current->left_subtree)->height;

            return reinterpret_cast<AVL_tree::node*>(current->left_subtree)->height - reinterpret_cast<AVL_tree::node*>(current->right_subtree)->height;
        }

        void balance(std::stack<typename binary_search_tree<tkey, tvalue>::node**> & path) override
        {
            while(!path.empty())
            {
                node* current_node = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(*(path.top()));
                typename binary_search_tree<tkey, tvalue>::node* tmp = current_node;
                count_node_height(current_node);

                path.pop();

                if(std::abs(balance_factor(current_node)) <= 1) continue;

                else
                {
                    if(balance_factor(current_node) == 2)
                    {
                        if(balance_factor(reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(current_node->left_subtree)) == 1)
                        {
                            node* new_subtree_root = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(current_node->left_subtree);
                            this->_tree->small_right_rotation(tmp);

                            node* left_subtree_new_node = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root->left_subtree);
                            node* right_subtree_new_node = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root->right_subtree);

                            count_node_height(right_subtree_new_node);
                            count_node_height(left_subtree_new_node);
                            count_node_height(new_subtree_root);

                            if(!path.empty())
                                (*path.top())->left_subtree == current_node ? (*path.top())->left_subtree = new_subtree_root : (*path.top())->right_subtree = new_subtree_root;
                            else
                                *_root = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root);
                        }
                        else
                        {
                            node* new_subtree_root = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(current_node->left_subtree->right_subtree);
                            this->_tree->big_right_rotation(tmp);

                            node* left_subtree_new_node = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root->left_subtree);
                            node* right_subtree_new_node = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root->right_subtree);

                            count_node_height(right_subtree_new_node);
                            count_node_height(left_subtree_new_node);
                            count_node_height(new_subtree_root);
                            if(!path.empty())
                                (*path.top())->left_subtree == current_node ? (*path.top())->left_subtree = new_subtree_root : (*path.top())->right_subtree = new_subtree_root;
                            else
                                *_root = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root);
                        }
                    }

                    else
                    {
                        if(balance_factor(reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(current_node->right_subtree)) == 1)
                        {
                            node* new_subtree_root = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(current_node->right_subtree->left_subtree);
                            this->_tree->big_left_rotation(tmp);

                            node* left_subtree_new_node = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root->left_subtree);
                            node* right_subtree_new_node = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root->right_subtree);

                            count_node_height(right_subtree_new_node);
                            count_node_height(left_subtree_new_node);
                            count_node_height(new_subtree_root);

                            if(!path.empty())
                                (*path.top())->left_subtree == current_node ? (*path.top())->left_subtree = new_subtree_root : (*path.top())->right_subtree = new_subtree_root;
                            else
                                *_root = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root);                        }
                        else
                        {
                            node* new_subtree_root = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(current_node->right_subtree);
                            this->_tree->small_left_rotation(tmp);

                            node* left_subtree_new_node = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root->left_subtree);
                            node* right_subtree_new_node = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root->right_subtree);

                            count_node_height(right_subtree_new_node);
                            count_node_height(left_subtree_new_node);
                            count_node_height(new_subtree_root);

                            if(!path.empty())
                                (*path.top())->left_subtree == current_node ? (*path.top())->left_subtree = new_subtree_root : (*path.top())->right_subtree = new_subtree_root;
                            else
                                *_root = reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(new_subtree_root);
                        }
                    }
                }
            }
        }
        
    };

public:
    
    explicit AVL_tree(
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        std::function<int(tkey const &, tkey const &)> comparer =  std::less<tkey>(),
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);


private:

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

    typename binary_search_tree<tkey, tvalue>::iterator_data* create_iterator_data() const
    {
        return new iterator_data;
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
    binary_search_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(tree, insertion_strategy),
    _root(reinterpret_cast<AVL_tree<tkey, tvalue>::node**>(&tree->_root))
{

}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    AVL_tree<tkey, tvalue> *tree):
    binary_search_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(tree)
{

}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    AVL_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) :
    binary_search_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(tree, disposal_strategy),
    _root(reinterpret_cast<AVL_tree<tkey, tvalue>::node**>(&tree->_root))
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
                new typename AVL_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy),
                new typename AVL_tree<tkey, tvalue>::obtaining_template_method(this),
                new typename AVL_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy),
                comparer,
                allocator,
                logger)

{

}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H