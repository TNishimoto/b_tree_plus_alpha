#pragma once
#include "./bp_internal_node_functions.hpp"
#include "./bp_postorder_iterator.hpp"
#include "./bp_value_forward_iterator.hpp"
#include "./bp_leaf_forward_iterator.hpp"

namespace stool
{
    namespace bptree
    {
        ////////////////////////////////////////////////////////////////////////////////
        /// @class      BPTree
        /// @brief      An implementation of B+-tree
        ///
        ////////////////////////////////////////////////////////////////////////////////
        template <typename LEAF_CONTAINER, typename VALUE, bool USE_PARENT_FIELD, bool USE_PSUM>
        class BPTree
        {
        public:
            using NodePointer = BPNodePointer<LEAF_CONTAINER, VALUE>;
            using Node = BPInternalNode<LEAF_CONTAINER, VALUE>;
            using PostorderIterator = BPPostorderIterator<LEAF_CONTAINER, VALUE>;
            using ValueForwardIterator = BPValueForwardIterator<LEAF_CONTAINER, VALUE>;
            using LeafForwardIterator = BPLeafForwardIterator<LEAF_CONTAINER, VALUE>;

            using BPFunctions = BPInternalNodeFunctions<LEAF_CONTAINER, VALUE, USE_PARENT_FIELD, USE_PSUM>;

            static inline constexpr int DEFAULT_MAX_DEGREE_OF_INTERNAL_NODE = 126;
            static inline constexpr int DEFAULT_MAX_COUNT_OF_VALUES_IN_LEAF = 126;
            /*
            static uint64_t default_max_degree_of_internal_node()
            {
                return 126;
            }
            static uint64_t default_max_count_of_values_in_leaf()
            {
                return 126;
            }
            */

        private:
            std::vector<LEAF_CONTAINER> leaf_container_vec;
            std::vector<Node *> parent_vec;
            std::stack<uint64_t> unused_leaf_container_indexes;
            std::vector<Node *> unused_node_pointers;
            std::vector<NodePointer> tmp_path;

            uint64_t _max_degree_of_internal_node = 8;
            uint64_t _max_count_of_values_in_leaf = 8;
            Node *root = nullptr;
            BPTree *linked_tree_ = nullptr;
            double density_threshold = 0.5;
            uint64_t density1 = 0;
            uint16_t height_ = 0;
            bool root_is_leaf_ = false;

        public:
            ~BPTree()
            {
                this->clear();
            }
            BPTree()
            {
            }
            BPTree(const BPTree &) = delete;
            BPTree(BPTree &&other) noexcept
            {
                this->leaf_container_vec = std::move(other.leaf_container_vec);
                this->parent_vec = std::move(other.parent_vec);
                this->unused_leaf_container_indexes = std::move(other.unused_leaf_container_indexes);
                this->unused_node_pointers = std::move(other.unused_node_pointers);
                this->tmp_path = std::move(other.tmp_path);

                this->_max_degree_of_internal_node = other._max_degree_of_internal_node;
                this->_max_count_of_values_in_leaf = other._max_count_of_values_in_leaf;
                this->root = other.root;
                this->linked_tree_ = other.linked_tree_;
                this->density_threshold = other.density_threshold;
                this->density1 = other.density1;
                this->height_ = other.height_;
                this->root_is_leaf_ = other.root_is_leaf_;

                other.leaf_container_vec.clear();
                other.parent_vec.clear();
                other.unused_node_pointers.clear();
                other.tmp_path.clear();
                while (other.unused_leaf_container_indexes.size() > 0)
                {
                    other.unused_leaf_container_indexes.pop();
                }

                other.root = nullptr;
                other.linked_tree_ = nullptr;
                other.height_ = 0;
                other.root_is_leaf_ = false;
            }
            BPTree &operator=(BPTree &&other) noexcept
            {
                if (this != &other)
                {
                    this->clear();
                    this->leaf_container_vec = std::move(other.leaf_container_vec);
                    this->parent_vec = std::move(other.parent_vec);
                    this->unused_leaf_container_indexes = std::move(other.unused_leaf_container_indexes);
                    this->unused_node_pointers = std::move(other.unused_node_pointers);
                    this->tmp_path = std::move(other.tmp_path);

                    this->_max_degree_of_internal_node = other._max_degree_of_internal_node;
                    this->_max_count_of_values_in_leaf = other._max_count_of_values_in_leaf;
                    this->root = other.root;
                    this->linked_tree_ = other.linked_tree_;
                    this->density_threshold = other.density_threshold;
                    this->density1 = other.density1;
                    this->height_ = other.height_;
                    this->root_is_leaf_ = other.root_is_leaf_;

                    other.leaf_container_vec.clear();
                    other.parent_vec.clear();
                    other.unused_node_pointers.clear();
                    other.tmp_path.clear();
                    while (other.unused_leaf_container_indexes.size() > 0)
                    {
                        other.unused_leaf_container_indexes.pop();
                    }

                    other.root = nullptr;
                    other.linked_tree_ = nullptr;
                    other.height_ = 0;
                    other.root_is_leaf_ = false;

                    // other->clear();
                }
                return *this;
            }
            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Initializers
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            void initialize(uint64_t __max_degree_of_internal_node)
            {
                this->initialize(__max_degree_of_internal_node, __max_degree_of_internal_node);
            }

            void initialize(uint64_t __max_degree_of_internal_node, uint64_t __max_count_of_values_in_leaf)
            {
                if (__max_degree_of_internal_node < 4)
                {
                    throw std::runtime_error("Error: BPTree::initialize(__max_degree_of_internal_node, __max_count_of_values_in_leaf). The __max_degree_of_internal_node must be larger than 3.");
                }
                if (__max_count_of_values_in_leaf < 4)
                {
                    throw std::runtime_error("Error: BPTree::initialize(__max_degree_of_internal_node, __max_count_of_values_in_leaf). The __max_count_of_values_in_leaf must be larger than 3.");
                }

                this->clear();
                this->_max_degree_of_internal_node = __max_degree_of_internal_node;
                this->_max_count_of_values_in_leaf = __max_count_of_values_in_leaf;
                this->root = nullptr;
                this->root_is_leaf_ = false;
            }

            void swap(BPTree &_tree, bool swap_linked_tree)
            {
                this->leaf_container_vec.swap(_tree.leaf_container_vec);
                this->parent_vec.swap(_tree.parent_vec);
                this->unused_node_pointers.swap(_tree.unused_node_pointers);
                this->unused_leaf_container_indexes.swap(_tree.unused_leaf_container_indexes);
                this->tmp_path.swap(_tree.tmp_path);
                std::swap(this->_max_degree_of_internal_node, _tree._max_degree_of_internal_node);
                std::swap(this->_max_count_of_values_in_leaf, _tree._max_count_of_values_in_leaf);
                std::swap(this->root, _tree.root);
                if (swap_linked_tree)
                {
                    std::swap(this->linked_tree_, _tree.linked_tree_);
                }
                std::swap(this->density_threshold, _tree.density_threshold);
                std::swap(this->density1, _tree.density1);
                std::swap(this->height_, _tree.height_);
                std::swap(this->root_is_leaf_, _tree.root_is_leaf_);
            }

            void swap(BPTree &_tree)
            {
                this->swap(_tree, true);
            }
            void clear()
            {
                std::stack<Node *> nodes;
                if (!this->empty() && !this->root_is_leaf_)
                {
                    nodes.push(this->root);
                }
                while (nodes.size() > 0)
                {
                    Node *top = nodes.top();
                    nodes.pop();
                    if (!top->is_parent_of_leaves())
                    {
                        const stool::SimpleDeque16<Node *> &children = top->get_children();
                        for (Node *child : children)
                        {
                            nodes.push(child);
                        }
                    }
                    delete top;
                }
                this->root = nullptr;
                this->root_is_leaf_ = false;
                this->height_ = 0;

                while (unused_node_pointers.size() > 0)
                {
                    Node *top = unused_node_pointers[unused_node_pointers.size() - 1];
                    delete top;
                    unused_node_pointers.pop_back();
                }

                this->leaf_container_vec.resize(0);
                this->parent_vec.resize(0);

                while (this->unused_leaf_container_indexes.size() > 0)
                {
                    this->unused_leaf_container_indexes.pop();
                }
            }
            //}@

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Properties
            ///   The properties of this class.
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            const std::vector<NodePointer> &get_temporary_path() const
            {
                return this->tmp_path;
            }

            /**
             * @brief Return the max degree of this tree. The number of children of every internal node does not exceed the max degree.
             */
            uint64_t get_max_degree_of_internal_node() const
            {
                return this->_max_degree_of_internal_node;
            }
            uint64_t get_max_count_of_values_in_leaf() const
            {
                return this->_max_count_of_values_in_leaf;
            }

            /**
             * @brief Return the height of this tree
             */
            uint64_t height() const
            {
                return this->height_;
            }

            /**
             * @brief Return the number of values stored in this tree
             */
            uint64_t size() const
            {
                if (this->empty())
                {
                    return 0;
                }
                else
                {
                    if (this->root_is_leaf_)
                    {
                        return this->leaf_container_vec[(uint64_t)this->root].size();
                    }
                    else
                    {
                        return this->root->get_value_count();
                    }
                }
            }
            /**
             * @brief Return the number of leaves in this tree
             */
            uint64_t get_leaf_count() const
            {
                return this->leaf_container_vec.size() - this->unused_leaf_container_indexes.size();
            }

            /**
             * @brief Return the size of the vector that stores leaf containers
             */
            uint64_t get_leaf_container_vector_size() const
            {
                return this->leaf_container_vec.size();
            }
            /*
            uint64_t get_threshold_of_degree() const
            {
                return this->max_degree;
            }
            */
            //}@

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Public standard functions
            ///   Public standard functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            PostorderIterator get_postorder_iterator_begin() const
            {
                if (this->empty())
                {
                    return PostorderIterator(nullptr);
                }
                else
                {
                    if (this->root_is_leaf_)
                    {
                        return PostorderIterator((uint64_t)this->root);
                    }
                    else
                    {
                        return PostorderIterator(this->root);
                    }
                }
            }

            PostorderIterator get_postorder_iterator_end() const
            {
                return PostorderIterator(nullptr);
            }

            ValueForwardIterator get_value_forward_iterator_begin() const
            {
                if (this->empty())
                {
                    return ValueForwardIterator(nullptr, nullptr);
                }
                else
                {
                    PostorderIterator node_it = this->get_postorder_iterator_begin();
                    return ValueForwardIterator(&node_it, &this->leaf_container_vec);
                }
            }
            ValueForwardIterator get_value_forward_iterator_end() const
            {
                return ValueForwardIterator(nullptr, nullptr);
            }

            LeafForwardIterator get_leaf_forward_iterator_begin() const
            {
                if (this->empty())
                {
                    return LeafForwardIterator(nullptr);
                }
                else
                {
                    return LeafForwardIterator(this->root);
                }
            }
            LeafForwardIterator get_leaf_forward_iterator_end() const
            {
                return LeafForwardIterator(nullptr);
            }

            bool empty() const
            {
                return this->height() == 0;
            }
            uint64_t psum() const
            {
                if (!this->empty())
                {
                    if (this->root_is_leaf_)
                    {
                        return this->leaf_container_vec[(uint64_t)this->root].psum();
                    }
                    else
                    {
                        return BPFunctions::psum(*this->root);
                    }
                }
                else
                {
                    return 0;
                }
            }
            uint64_t psum(uint64_t i) const
            {
                if (i >= this->size())
                {
                    throw std::invalid_argument("Error: BPTree::psum(i). The i must be less than the size of the tree.");
                }
                assert(!this->empty());
                if (!this->empty())
                {
                    if (this->root_is_leaf_)
                    {
                        return this->leaf_container_vec[(uint64_t)this->root].psum(i);
                    }
                    else
                    {
                        return BPFunctions::psum(*this->root, i, this->leaf_container_vec);
                    }
                }
                else
                {
                    throw std::runtime_error("Error:psum(i)");
                }
            }
            int64_t select0(uint64_t i) const
            {
                if (!this->empty())
                {
                    if (this->root_is_leaf_)
                    {
                        return this->leaf_container_vec[(uint64_t)this->root].select0(i);
                    }
                    else
                    {
                        return BPFunctions::select0(*this->root, i, this->leaf_container_vec);
                    }
                }
                else
                {
                    return -1;
                }
            }

            int64_t search(uint64_t sum) const
            {
                if (!this->empty())
                {
                    if (this->root_is_leaf_)
                    {
                        return this->leaf_container_vec[(uint64_t)this->root].search(sum);
                    }
                    else
                    {
                        return BPFunctions::search(*this->root, sum, this->leaf_container_vec);
                    }
                }
                else
                {
                    return -1;
                }
            }
            VALUE at(uint64_t pos) const
            {
                if (!this->empty())
                {
                    assert(pos < this->size());
                    std::vector<NodePointer> path;
                    uint64_t idx = this->compute_path_from_root_to_leaf(pos);
                    assert(this->tmp_path.size() > 0);
                    uint64_t leaf = this->tmp_path[this->tmp_path.size() - 1].get_leaf_container_index();
                    return this->leaf_container_vec[leaf].at(idx);
                }
                else
                {
                    throw std::invalid_argument("Error: BPTree::at()");
                }
            }
            uint64_t get_value_index(uint64_t leaf_index, uint64_t position_in_leaf_container) const
            {
                if (USE_PARENT_FIELD)
                {
                    assert(leaf_index < this->parent_vec.size());
                    Node *parent = this->parent_vec[leaf_index];
                    uint64_t dist = position_in_leaf_container;

                    if (parent == nullptr)
                    {
                        return dist;
                    }
                    else
                    {

                        Node *node = (Node *)leaf_index;
                        while (parent != nullptr)
                        {
                            /*
                            this->print_info();
                            this->print_tree();
                            parent->print_info();
                            std::cout << "IDX: " << (uint64_t)node << std::endl;
                            */

                            int64_t idx = parent->get_index(node);
                            assert(idx != -1);

                            const stool::SimpleDeque16<uint64_t> &deq = parent->get_value_count_deque();
                            uint64_t sum = std::reduce(std::begin(deq), std::next(deq.begin(), idx));

                            dist += sum;
                            node = parent;
                            parent = node->get_parent();
                        }
                        return dist;
                    }
                }
                else
                {
                    throw std::runtime_error("BPTree::get_value_index(): this function is not supported.");
                }
            }

            const LEAF_CONTAINER &get_leaf_container(uint64_t leaf_index) const
            {
                return this->leaf_container_vec[leaf_index];
            }
            LEAF_CONTAINER &get_leaf_container(uint64_t leaf_index)
            {
                assert(leaf_index < this->get_leaf_container_vector_size());

                return this->leaf_container_vec[leaf_index];
            }
            /*
            bool check_whether_if_leaf_containers_are_sorted() const
            {
                uint64_t half_degree = this->get_max_count_of_values_in_leaf() / 2;
                uint64_t x = 0;
                uint64_t leaf_count = this->leaf_container_vec.size();
                for (PostorderIterator it = this->get_postorder_iterator_begin(); it != this->get_postorder_iterator_end(); ++it)
                {
                    if ((*it).is_leaf())
                    {
                        uint64_t leaf_index = (*it).get_leaf_container_index();

                        if (x != leaf_index)
                        {
                            return false;
                        }

                        if (x + 1 != leaf_count && half_degree != this->leaf_container_vec[leaf_index].size())
                        {
                            return false;
                        }
                        x++;
                    }
                }
                return true;
            }
            */

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Convertion functions
            ///   Convertion functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /*
            std::string to_string() const
            {
                std::string r;
                std::vector<uint64_t> idx_vector = this->get_leaf_container_index_vector();
                Printer::print(idx_vector);
                for (uint64_t idx : idx_vector)
                {

                    r += this->leaf_container_vec[idx].to_string();
                }
                return r;
            }
            */
            std::vector<VALUE> to_value_vector() const
            {

                if (!this->empty())
                {
                    if (this->root_is_leaf_)
                    {
                        std::vector<VALUE> r;
                        this->leaf_container_vec[(uint64_t)this->root].to_values(r);
                        return r;
                    }
                    else
                    {
                        std::vector<VALUE> r;
                        BPFunctions::to_value_vector(*this->root, r, this->leaf_container_vec);
                        return r;
                    }
                }
                else
                {
                    std::vector<VALUE> r;
                    return r;
                }
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Const print and verification functions
            ///   Const print functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{
        public:
            bool verify() const
            {
                bool b = true;
                uint64_t p = 0;
                for (PostorderIterator it = this->get_postorder_iterator_begin(); it != this->get_postorder_iterator_end(); ++it)
                {
                    NodePointer pt = *it;
                    if (pt.is_leaf())
                    {
                        assert(pt.get_leaf_container_index() < this->leaf_container_vec.size());
                        uint64_t size = this->leaf_container_vec[pt.get_leaf_container_index()].size();
                        if (this->root_is_leaf_ && pt.get_leaf_container_index() == (uint64_t)this->root)
                        {
                            b = b && (size > 0);
                            assert(b);
                        }
                        else
                        {

                            b = b && (size > 0) && ((this->get_max_count_of_values_in_leaf() / 2) <= size);
                            assert(b);
                        }
                        p += size;
                    }
                    else
                    {
                        Node *_node = pt.get_node();
                        b = b && BPFunctions::verify(*_node, this->leaf_container_vec, this->get_max_degree_of_internal_node(), this->root == pt.get_node());
                        assert(b);
                    }
                    // std::cout << it.idx << "//" << it._st.size() << "/" << std::flush;
                }
                if (p != this->size())
                {
                    throw std::logic_error("Error(BPTree::verify)");
                }
                return b;
            }
            void print_tree() const
            {
                std::vector<std::string> tree;
                if (!this->empty())
                {
                    if (this->root_is_leaf_)
                    {
                        tree.push_back(this->leaf_container_vec[(uint64_t)this->root].to_string());
                    }
                    else
                    {
                        BPFunctions::get_tree_string(*this->root, tree, this->leaf_container_vec);
                    }
                }
                else
                {
                    tree.push_back("[]");
                }

                for (int64_t i = tree.size() - 1; i >= 0; i--)
                {
                    std::cout << tree[i] << std::endl;
                }
            }

            void print_info() const
            {
                std::cout << "========= BPTREE =========" << std::endl;
                std::cout << "Height: " << this->height_ << std::endl;

                // uint64_t id = 0;
                for (PostorderIterator it = this->get_postorder_iterator_begin(); it != this->get_postorder_iterator_end(); ++it)
                {
                    NodePointer pt = *it;
                    if (pt.is_leaf())
                    {
                        std::cout << "LIDX: " << pt.get_leaf_container_index() << ": " << this->leaf_container_vec[pt.get_leaf_container_index()].to_string() << std::endl;
                        // std::vector<VALUE> vec = this->leaf_container_vec[pt.get_leaf_container_index()].to_value_vector();
                        // stool::Printer::print(std::to_string(id), vec);
                    }
                    else
                    {
                        pt.get_node()->print_info();
                    }
                    // id++;
                }

                if (USE_PARENT_FIELD)
                {
                    std::vector<uint64_t> r;
                    for (Node *x : this->parent_vec)
                    {
                        r.push_back((uint64_t)x);
                    }
                    stool::Printer::print(r);
                }
                std::cout << "==========================" << std::endl;
            }
            void print_leaves() const
            {
                // uint64_t id = 0;
                for (PostorderIterator it = this->get_postorder_iterator_begin(); it != this->get_postorder_iterator_end(); ++it)
                {
                    NodePointer pt = *it;
                    if (pt.is_leaf())
                    {
                        std::cout << "leaf_index: " << pt.get_leaf_container_index() << ", " << this->leaf_container_vec[pt.get_leaf_container_index()].to_string() << std::endl;
                    }
                    // id++;
                }
            }
            void print_leaf_containers() const
            {
                std::cout << "============ LEAVES ============" << std::endl;
                for (uint64_t i = 0; i < this->leaf_container_vec.size(); i++)
                {
                    std::cout << "leaf_container_vec[" << i << "] = " << this->leaf_container_vec[i].to_string() << std::endl;
                }
                std::cout << "============ LEAVES[END] ============" << std::endl;
            }

            uint64_t size_in_bytes() const
            {
                return this->get_internal_node_memory() + this->get_unused_leaf_container_vector_memory() + this->get_unused_leaf_container_vector_memory() + this->get_leaf_container_memory() + this->get_unused_node_pointers_memory() +
                       this->get_parent_vector_memory();
            }
            uint64_t compute_internal_node_count() const
            {
                uint64_t counter = 0;

                for (BPPostorderIterator it = this->get_postorder_iterator_begin(); it != this->get_postorder_iterator_end(); ++it)
                {
                    NodePointer np = *it;
                    if (!np.is_leaf())
                    {
                        counter++;
                    }
                }
                return counter;
            }
            uint64_t compute_leaf_container_count() const
            {
                uint64_t counter = 0;

                for (BPPostorderIterator it = this->get_postorder_iterator_begin(); it != this->get_postorder_iterator_end(); ++it)
                {
                    NodePointer np = *it;
                    if (np.is_leaf())
                    {
                        counter++;
                    }
                }
                return counter;
            }

            uint64_t get_internal_node_memory() const
            {
                uint64_t sum1 = 0;

                for (BPPostorderIterator it = this->get_postorder_iterator_begin(); it != this->get_postorder_iterator_end(); ++it)
                {
                    NodePointer pt = *it;
                    if (!pt.is_leaf())
                    {
                        sum1 += pt.get_node()->size_in_bytes();
                        sum1 += sizeof(Node *);
                    }
                }
                return sum1;
            }
            uint64_t get_unused_leaf_container_vector_memory() const
            {
                return sizeof(std::stack<uint64_t>) + (this->unused_leaf_container_indexes.size() * sizeof(uint64_t));
            }
            uint64_t get_leaf_container_vector_memory() const
            {
                return sizeof(std::vector<uint64_t>) + (this->leaf_container_vec.capacity() * sizeof(uint64_t));
            }
            uint64_t get_leaf_container_memory() const
            {
                uint64_t sum2 = 0;
                for (const LEAF_CONTAINER &leaf : this->leaf_container_vec)
                {
                    sum2 += leaf.size_in_bytes();
                }
                return sum2;
            }
            uint64_t get_unused_node_pointers_memory() const
            {
                uint64_t sum6 = 0;
                for (Node *node : this->unused_node_pointers)
                {
                    sum6 += node->size_in_bytes();
                }

                sum6 += (sizeof(Node *) * this->unused_node_pointers.capacity()) + sizeof(std::vector<Node *>);
                return sum6;
            }
            uint64_t get_parent_vector_memory() const
            {
                return (sizeof(Node *) * this->parent_vec.capacity()) + sizeof(std::vector<Node *>);
            }

            std::vector<std::string> get_memory_usage_info(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::vector<std::string> log;
                log.push_back(stool::Message::get_paragraph_string(message_paragraph)  + "=BP Tree: " + std::to_string(this->size_in_bytes()) + " bytes =");
                log.push_back(stool::Message::get_paragraph_string(message_paragraph)  + " Max Degree of Internal Node: " + std::to_string(this->get_max_degree_of_internal_node()));
                log.push_back(stool::Message::get_paragraph_string(message_paragraph)  + " Max Count of Values in Leaf: " + std::to_string(this->get_max_count_of_values_in_leaf()));
                log.push_back(stool::Message::get_paragraph_string(message_paragraph)  + " The number of values: " + std::to_string(this->size()));

                log.push_back(stool::Message::get_paragraph_string(message_paragraph)  +" Internal Nodes: " + std::to_string(this->get_internal_node_memory()) + " bytes");
                log.push_back(stool::Message::get_paragraph_string(message_paragraph)  +" Ununsed Leaf Pointers: " + std::to_string(this->get_unused_leaf_container_vector_memory()) + " bytes");
                log.push_back(stool::Message::get_paragraph_string(message_paragraph)  +" Ununsed Node Pointers: " + std::to_string(this->get_unused_node_pointers_memory()) + " bytes");
                log.push_back(stool::Message::get_paragraph_string(message_paragraph)  +" Leaf Vector: " + std::to_string(this->get_leaf_container_vector_memory()) + " bytes (vector capacity: " + std::to_string(this->leaf_container_vec.capacity()) + ")");

                uint64_t value_count = this->size();
                uint64_t leaf_total_memory = this->get_leaf_container_memory();
                uint64_t byte_per_value = value_count > 0 ? leaf_total_memory / value_count : 0;
                log.push_back(stool::Message::get_paragraph_string(message_paragraph)  +" Leaves : " + std::to_string(leaf_total_memory) + " bytes (" + std::to_string(byte_per_value) + " bytes per value, leaf count: " + std::to_string(this->leaf_container_vec.size()) + ")");
                log.push_back(stool::Message::get_paragraph_string(message_paragraph)  +" Parent Vector : " + std::to_string(this->get_parent_vector_memory()) + " bytes");
                log.push_back(stool::Message::get_paragraph_string(message_paragraph)  +"==");
                return log;
            }

            void print_memory_usage(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::vector<std::string> log = this->get_memory_usage_info(message_paragraph);
                for (std::string &s : log)
                {
                    std::cout << s << std::endl;
                }
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Const private functions
            ///   Const private functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{

        public:
            void get_path_from_root_to_first_leaf(std::vector<NodePointer> &output_path) const
            {
                if (!this->empty())
                {
                    Node *current_node = this->root;
                    bool is_leaf = this->root_is_leaf_;

                    if (!is_leaf)
                    {
                        output_path.push_back(NodePointer::build_internal_node_pointer(this->root, -1));
                    }
                    else
                    {
                        output_path.push_back(NodePointer::build_leaf_pointer((uint64_t)this->root, -1));
                    }

                    while (!is_leaf)
                    {
                        is_leaf = current_node->is_parent_of_leaves();
                        current_node = current_node->get_child(0);
                        if (!is_leaf)
                        {
                            output_path.push_back(NodePointer::build_internal_node_pointer(current_node, 0));
                        }
                        else
                        {
                            output_path.push_back(NodePointer::build_leaf_pointer((uint64_t)current_node, 0));
                        }
                    }
                }
            }

            void get_path_from_root_to_last_leaf(std::vector<NodePointer> &output_path) const
            {
                if (!this->empty())
                {
                    Node *current_node = this->root;
                    bool is_leaf = this->root_is_leaf_;

                    if (!is_leaf)
                    {
                        output_path.push_back(NodePointer::build_internal_node_pointer(this->root, -1));
                    }
                    else
                    {
                        output_path.push_back(NodePointer::build_leaf_pointer((uint64_t)this->root, -1));
                    }

                    while (!is_leaf)
                    {
                        is_leaf = current_node->is_parent_of_leaves();
                        uint64_t idx = current_node->get_degree() - 1;
                        current_node = current_node->get_child(idx);
                        if (!is_leaf)
                        {
                            output_path.push_back(NodePointer::build_internal_node_pointer(current_node, idx));
                        }
                        else
                        {
                            output_path.push_back(NodePointer::build_leaf_pointer((uint64_t)current_node, idx));
                        }
                    }
                }
            }

            int64_t compute_path_from_root_to_leaf(uint64_t i) const
            {
                std::vector<NodePointer> &path = const_cast<std::vector<NodePointer> &>(this->tmp_path);
                return this->compute_path_from_root_to_leaf(i, path);
            }

            int64_t compute_path_from_root_to_leaf(uint64_t i, std::vector<NodePointer> &output_path) const
            {
                if (output_path.size() != this->height_)
                {
                    output_path.resize(this->height_);
                }
                uint64_t y = 0;

                if (!this->empty())
                {

                    Node *current_node = this->root;
                    uint64_t current_i = i;
                    bool is_leaf = this->root_is_leaf_;

                    output_path[y++] = is_leaf ? NodePointer::build_leaf_pointer((uint64_t)this->root, -1) : NodePointer::build_internal_node_pointer(this->root, -1);

                    while (!is_leaf)
                    {
                        assert(current_i <= current_node->get_value_count());
                        std::pair<int64_t, uint64_t> result = BPFunctions::access_child_index_by_value_index(*current_node, current_i);
                        if (result.first != -1)
                        {
                            is_leaf = current_node->is_parent_of_leaves();
                            current_node = current_node->get_child(result.first);
                            current_i = result.second;
                            output_path[y++] = is_leaf ? NodePointer::build_leaf_pointer((uint64_t)current_node, result.first) : NodePointer::build_internal_node_pointer(current_node, result.first);
                        }
                        else
                        {
                            throw std::runtime_error("Error: get_path_from_root_to_leaf2(1)");
                        }
                    }
                    assert(output_path.size() == y);
                    return current_i;
                }
                else
                {
                    return -1;
                }
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Non-const private functions
            ///   Non-const private functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{
        private:
            void defragmentation()
            {
                std::vector<uint64_t> tmp;
                while (this->unused_leaf_container_indexes.size() > 0)
                {
                    tmp.push_back(this->unused_leaf_container_indexes.top());
                    this->unused_leaf_container_indexes.pop();
                }

                std::sort(tmp.begin(), tmp.end(), [](const uint64_t &lhs, const uint64_t &rhs)
                          { return lhs > rhs; });

                std::vector<std::pair<Node *, uint64_t>> tmp_nodes;
                {
                    std::unordered_set<uint64_t> checker2;
                    {
                        std::unordered_set<uint64_t> checker1;
                        for (uint64_t p : tmp)
                        {
                            checker1.insert(p);
                        }

                        for (int64_t i = this->leaf_container_vec.size() - 1; i >= 0; --i)
                        {
                            auto xs = checker1.find(i);
                            if (xs == checker1.end())
                            {
                                checker2.insert(i);
                            }
                            if (checker2.size() > tmp.size())
                            {
                                break;
                            }
                        }
                    }

                    for (BPPostorderIterator it = this->get_postorder_iterator_begin(); it != this->get_postorder_iterator_end(); ++it)
                    {
                        NodePointer pt = *it;
                        if (!pt.is_leaf() && pt.get_node()->is_parent_of_leaves())
                        {
                            Node *node = pt.get_node();
                            for (int64_t i = 0; i < node->children_count(); i++)
                            {
                                uint64_t x = (uint64_t)node->get_child(i);
                                auto xs = checker2.find(x);
                                if (xs != checker2.end())
                                {
                                    tmp_nodes.push_back(std::pair<Node *, uint64_t>(node, i));
                                }
                            }
                        }
                    }
                    assert(tmp_nodes.size() == checker2.size());
                }

                std::sort(tmp_nodes.begin(), tmp_nodes.end(), [](const std::pair<Node *, uint64_t> &lhs, const std::pair<Node *, uint64_t> &rhs)
                          { 
                        uint64_t x = (uint64_t)lhs.first->get_child(lhs.second);
                        uint64_t y = (uint64_t)rhs.first->get_child(rhs.second);
                        return x < y; });

                uint64_t k = 0;
                while (tmp.size() > 0 && tmp_nodes.size() > 0)
                {
                    std::pair<Node *, uint64_t> top = tmp_nodes[tmp_nodes.size() - 1];
                    uint64_t idx = (uint64_t)top.first->get_child(top.second);
                    uint64_t x = tmp[tmp.size() - 1];
                    if (x < idx)
                    {
                        top.first->move_container_index(top.second, x, this->leaf_container_vec);
                        tmp_nodes.pop_back();
                        tmp.pop_back();
                        k++;
                    }
                    else
                    {
                        tmp.pop_back();
                        k++;
                    }
                }
                this->leaf_container_vec.resize(this->leaf_container_vec.size() - k);
                /*
                //this->leaf_container_vec.shrink_to_fit();
                uint64_t gap = this->leaf_container_vec.size() * this->density_threshold;
                //this->leaf_container_vec.reserve(this->leaf_container_vec.size() + gap);
                this->density1 = gap;
                */
            }
            void clear_unused_node_pointers()
            {
                for (uint64_t i = 0; i < this->unused_node_pointers.size(); i++)
                {
                    delete this->unused_node_pointers[i];
                }
                this->unused_node_pointers.clear();
                this->unused_node_pointers.shrink_to_fit();
            }
            void create_root_leaf(VALUE value)
            {
                uint64_t p = this->get_new_container_index();
                this->leaf_container_vec[p].push_back(value);
                this->root = (Node *)p;
                this->root_is_leaf_ = true;
                this->height_++;
                if (USE_PARENT_FIELD)
                {
                    this->parent_vec[p] = nullptr;
                }
            }

            void remove_emply_leaf(uint64_t idx, Node *parent, int64_t child_index)
            {
                if (parent != nullptr)
                {
                    parent->remove_child(child_index);
                }
                if (USE_PARENT_FIELD)
                {
                    this->parent_vec[idx] = nullptr;
                }
                // this->height_ = 0;
                this->unused_leaf_container_indexes.push(idx);
                this->leaf_container_vec[idx].clear();
            }
            void remove_emply_node(Node *node, Node *parent, int64_t child_index)
            {
                // Node *parent = node->get_parent();
                if (parent != nullptr)
                {
                    parent->remove_child(child_index);

                    // uint64_t edge = parent->find_child_index_by_child_pointer(node);
                    // parent->remove_child(edge);
                }
                if (this->root == node)
                {
                    this->root = nullptr;
                }

                node->clear();
                if (this->unused_node_pointers.size() <= 4096)
                {
                    this->unused_node_pointers.push_back(node);
                }
                else
                {
                    delete node;
                }
            }

            uint64_t get_new_container_index()
            {
                if (this->unused_leaf_container_indexes.size() == 0)
                {
                    this->leaf_container_vec.push_back(LEAF_CONTAINER());
                    if (USE_PARENT_FIELD)
                    {
                        this->parent_vec.push_back(nullptr);
                    }
                    this->unused_leaf_container_indexes.push(this->leaf_container_vec.size() - 1);
                }
                assert(this->unused_leaf_container_indexes.size() > 0);
                uint64_t idx = this->unused_leaf_container_indexes.top();
                this->unused_leaf_container_indexes.pop();

                return idx;
            }
            Node *get_new_node_pointer()
            {
                Node *new_node = nullptr;
                if (this->unused_node_pointers.size() > 0)
                {
                    new_node = this->unused_node_pointers[this->unused_node_pointers.size() - 1];
                    this->unused_node_pointers.pop_back();
                }
                else
                {
                    new_node = new Node();
                }
                return new_node;
            }

            void split_process(const std::vector<NodePointer> &path, uint64_t t)
            {
                const NodePointer &top = path[t];

                Node *node = top.get_node();
                Node *_new_right_node = nullptr;
                uint64_t parent_edge_index = 0;
                if (!top.is_leaf())
                {
                    _new_right_node = this->get_new_node_pointer();
                    _new_right_node->initialize(top.get_node()->is_parent_of_leaves(), this->leaf_container_vec);
                }
                else
                {
                    _new_right_node = (Node *)this->get_new_container_index();
                }

                Node *_parent = nullptr;
                if (t > 0)
                {
                    _parent = path[t - 1].get_node();
                    _parent->insert_child(top.get_parent_edge_index() + 1, _new_right_node, 0, 0);
                    parent_edge_index = top.get_parent_edge_index();
                }
                else
                {
                    _parent = this->get_new_node_pointer();
                    if (top.is_leaf())
                    {
                        _parent->initialize((uint64_t)node, (uint64_t)_new_right_node, this->leaf_container_vec);
                    }
                    else
                    {
                        _parent->initialize(node, _new_right_node, this->leaf_container_vec);
                    }
                    parent_edge_index = 0;

                    this->root = _parent;
                    this->root_is_leaf_ = false;
                    this->height_++;

                    if (USE_PARENT_FIELD)
                    {
                        if (top.is_leaf())
                        {
                            this->parent_vec[(uint64_t)node] = _parent;
                        }
                        else
                        {
                            node->set_parent(_parent);
                        }
                    }
                }
                if (USE_PARENT_FIELD)
                {
                    if (top.is_leaf())
                    {
                        this->parent_vec[(uint64_t)_new_right_node] = _parent;
                    }
                    else
                    {
                        _new_right_node->set_parent(_parent);
                    }
                }

                this->split_node(node, _new_right_node, top.is_leaf(), _parent, parent_edge_index, this->parent_vec);
            }

            void balance_for_insertion(const std::vector<NodePointer> &path, bool superLeftPushMode = false)
            {
                for (int64_t t = path.size() - 1; t >= 0; --t)
                {
                    const NodePointer &top = path[t];
                    uint64_t degree = top.get_degree(this->leaf_container_vec);
                    uint64_t threshold = top.is_leaf() ? this->get_max_count_of_values_in_leaf() : this->get_max_degree_of_internal_node();

                    if (degree > threshold)
                    {
                        // this->split_process(path, t);

                        if (t > 0)
                        {
                            Node *parent = path[t - 1].get_node();
                            uint64_t parent_edge_index = top.get_parent_edge_index();
                            Node *leftSibling = parent_edge_index > 0 ? parent->get_child(parent_edge_index - 1) : nullptr;
                            Node *rightSibling = parent_edge_index + 1 < parent->children_count() ? parent->get_child(parent_edge_index + 1) : nullptr;
                            uint64_t leftSiblingDegree = UINT64_MAX;
                            if (parent_edge_index > 0)
                            {
                                leftSiblingDegree = top.is_leaf() ? this->leaf_container_vec[(uint64_t)leftSibling].size() : leftSibling->get_degree();
                                assert(leftSiblingDegree <= threshold);
                            }

                            uint64_t rightSiblingDegree = UINT64_MAX;
                            if (parent_edge_index + 1 < parent->children_count())
                            {
                                rightSiblingDegree = top.is_leaf() ? this->leaf_container_vec[(uint64_t)rightSibling].size() : rightSibling->get_degree();
                                assert(rightSiblingDegree <= threshold);
                            }

                            if (leftSiblingDegree < threshold || rightSiblingDegree < threshold)
                            {
                                if (leftSiblingDegree < threshold)
                                {
                                    if (superLeftPushMode)
                                    {
                                        uint64_t diff = threshold - leftSiblingDegree;
                                        this->move_values_left(leftSibling, top.get_node(), diff, top.is_leaf(), parent, parent_edge_index);
                                    }
                                    else
                                    {
                                        this->move_values_left(leftSibling, top.get_node(), 1, top.is_leaf(), parent, parent_edge_index);
                                    }
                                }
                                else
                                {
                                    this->move_values_right(top.get_node(), rightSibling, 1, top.is_leaf(), parent, parent_edge_index);
                                }
                                break;
                            }
                            else
                            {
                                this->split_process(path, t);
                            }
                        }
                        else
                        {
                            this->split_process(path, t);
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }

            void balance_for_removal(const std::vector<NodePointer> &path)
            {
                // Node *current_node = &node;

                for (int64_t t = path.size() - 1; t >= 0; --t)
                {
                    const NodePointer &top = path[t];
                    uint64_t max_size = top.is_leaf() ? this->get_max_count_of_values_in_leaf() : this->get_max_degree_of_internal_node();
                    uint64_t threshold = max_size / 2;

                    uint64_t degree = top.get_degree(this->leaf_container_vec);
                    if (degree >= threshold)
                    {
                        break;
                    }
                    else
                    {
                        if (t > 0)
                        {
                            Node *parent = path[t - 1].get_node();
                            uint64_t parent_edge_index = top.get_parent_edge_index();
                            Node *leftSibling = parent_edge_index > 0 ? parent->get_child(parent_edge_index - 1) : nullptr;
                            Node *rightSibling = parent_edge_index + 1 < parent->children_count() ? parent->get_child(parent_edge_index + 1) : nullptr;
                            uint64_t leftSiblingDegree = 0;
                            if (parent_edge_index > 0)
                            {
                                leftSiblingDegree = top.is_leaf() ? this->leaf_container_vec[(uint64_t)leftSibling].size() : leftSibling->get_degree();
                            }

                            uint64_t rightSiblingDegree = 0;
                            if (parent_edge_index + 1 < parent->children_count())
                            {
                                rightSiblingDegree = top.is_leaf() ? this->leaf_container_vec[(uint64_t)rightSibling].size() : rightSibling->get_degree();
                            }

                            if (leftSiblingDegree != 0 || rightSiblingDegree != 0)
                            {
                                if (leftSiblingDegree > threshold)
                                {
                                    this->move_values_right(leftSibling, top.get_node(), 1, top.is_leaf(), parent, parent_edge_index - 1);
                                    break;
                                }
                                else if (rightSiblingDegree > threshold)
                                {
                                    this->move_values_left(top.get_node(), rightSibling, 1, top.is_leaf(), parent, parent_edge_index + 1);
                                    break;
                                }
                                else
                                {
                                    assert(leftSiblingDegree == threshold || rightSiblingDegree == threshold);
                                    if (leftSiblingDegree == threshold)
                                    {

                                        this->move_values_left(leftSibling, top.get_node(), degree, top.is_leaf(), parent, parent_edge_index);
                                        if (top.is_leaf())
                                        {
                                            this->remove_emply_leaf(top.get_leaf_container_index(), parent, parent_edge_index);
                                        }
                                        else
                                        {
                                            this->remove_emply_node(top.get_node(), parent, parent_edge_index);
                                        }
                                    }
                                    else
                                    {

                                        this->move_values_right(top.get_node(), rightSibling, degree, top.is_leaf(), parent, parent_edge_index);
                                        if (top.is_leaf())
                                        {
                                            this->remove_emply_leaf(top.get_leaf_container_index(), parent, parent_edge_index);
                                        }
                                        else
                                        {
                                            this->remove_emply_node(top.get_node(), parent, parent_edge_index);
                                        }
                                    }
                                }
                            }
                            else
                            {
                                throw std::logic_error("Error(BPTree::balance_for_removal(1))");
                            }
                        }
                        else
                        {
                            assert(t == 0);
                            if (degree == 0)
                            {
                                throw std::logic_error("Error(BPTree::balance_for_removal(2))");
                            }
                            else if (degree == 1)
                            {
                                if (!this->root_is_leaf_)
                                {
                                    bool b = this->root->is_parent_of_leaves();
                                    Node *new_root = this->root->get_child(0);
                                    this->root->remove_child(0);
                                    this->remove_emply_node(this->root, nullptr, -1);
                                    this->root = new_root;
                                    this->root_is_leaf_ = b;
                                    if (USE_PARENT_FIELD)
                                    {
                                        if (b)
                                        {
                                            this->parent_vec[(uint64_t)new_root] = nullptr;
                                        }
                                        else
                                        {
                                            new_root->set_parent(nullptr);
                                        }
                                    }
                                    this->height_--;
                                }
                                break;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Non-const public functions
            ///   Non-const public functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{
        public:
            void push_back(VALUE value)
            {
                std::vector<VALUE> r;
                r.push_back(value);
                this->push_many(r);
            }
            void push_front(VALUE value)
            {
                std::vector<NodePointer> path;
                this->get_path_from_root_to_first_leaf(path);
                if (path.size() > 0)
                {
                    uint64_t leaf = path[path.size() - 1].get_leaf_container_index();
                    this->leaf_container_vec[leaf].push_front(value);

                    uint64_t sum = this->leaf_container_vec[leaf].psum(0);

                    for (int64_t i = path.size() - 2; i >= 0; --i)
                    {
                        Node *parent = path[i].get_node();
                        uint64_t parent_edge_index = path[i + 1].get_parent_edge_index();
                        parent->increment(parent_edge_index, 1, sum);
                    }
                    this->balance_for_insertion(path, true);
                }
                else
                {
                    this->create_root_leaf(value);
                }
            }

        private:
            uint64_t push_many_to_leaf(const std::vector<VALUE> &values, uint64_t value_pos, const std::vector<NodePointer> &path)
            {
                uint64_t x = 0;
                uint64_t leaf = path[path.size() - 1].get_leaf_container_index();
                uint64_t len = 0;
                while ((value_pos + x) < values.size() && leaf_container_vec[leaf].size() <= this->get_max_count_of_values_in_leaf())
                {

                    this->leaf_container_vec[leaf].push_back(values[value_pos + x]);
                    x++;
                    len++;
                }
                uint64_t sum = len > 0 ? this->leaf_container_vec[leaf].reverse_psum(len - 1) : 0;

                for (int64_t j = path.size() - 2; j >= 0; --j)
                {
                    Node *parent = path[j].get_node();
                    uint64_t parent_edge_index = path[j + 1].get_parent_edge_index();
                    assert(parent_edge_index < parent->children_count());

                    parent->increment(parent_edge_index, len, sum);
                }

                this->balance_for_insertion(path, true);

                assert(x > 0);
                return x;
            }
            void push_back_to_internal_node(uint64_t leaf_index, const std::vector<NodePointer> &path)
            {
                Node *node = path[path.size() - 1].get_node();
                uint64_t child_count = this->leaf_container_vec[leaf_index].size();
                uint64_t child_sum = 0;
                if (USE_PSUM)
                {
                    child_sum = this->leaf_container_vec[leaf_index].psum();
                }
                node->append_child(leaf_index, child_count, child_sum);

                for (int64_t j = path.size() - 2; j >= 0; --j)
                {
                    Node *parent = path[j].get_node();
                    uint64_t parent_edge_index = path[j + 1].get_parent_edge_index();
                    assert(parent_edge_index < parent->children_count());
                    parent->increment(parent_edge_index, child_count, child_sum);
                }

                this->balance_for_insertion(path, true);
            }

        public:
            void push_many(std::vector<LEAF_CONTAINER> &containers)
            {
                uint64_t i = 0;
                std::vector<NodePointer> path;
                while (i < containers.size())
                {
                    path.clear();
                    this->get_path_from_root_to_last_leaf(path);
                    if (path.size() > 1)
                    {
                        uint64_t p = this->get_new_container_index();
                        this->leaf_container_vec[p].swap(containers[i]);
                        path.pop_back();
                        this->push_back_to_internal_node(p, path);
                    }
                    else
                    {
                        std::vector<VALUE> tmp_values;
                        containers[i].to_values(tmp_values);
                        this->push_many(tmp_values);
                    }
                    i++;
                }
            }

            void push_many(const std::vector<VALUE> &values)
            {
                uint64_t i = 0;
                std::vector<NodePointer> path;

                while (i < values.size())
                {
                    path.clear();
                    this->get_path_from_root_to_last_leaf(path);
                    if (path.size() > 0)
                    {
                        i += this->push_many_to_leaf(values, i, path);
                    }
                    else
                    {
                        this->create_root_leaf(values[i]);
                        i++;
                    }
                }
            }

            void remove_using_path(const std::vector<NodePointer> &path, uint64_t position_in_leaf_container)
            {
                uint64_t x = path[path.size() - 1].get_leaf_container_index();
                int64_t delta = leaf_container_vec[x].psum(position_in_leaf_container, position_in_leaf_container);
                leaf_container_vec[x].remove(position_in_leaf_container);
                for (int64_t i = path.size() - 2; i >= 0; i--)
                {
                    Node *node = path[i].get_node();
                    uint64_t child_index = path[i + 1].get_parent_edge_index();
                    node->increment(child_index, -1, -delta);
                }

                if (path.size() > 1)
                {
                    this->balance_for_removal(this->tmp_path);
                }
                else
                {
                    assert(path.size() == 1);
                    if (leaf_container_vec[x].size() == 0)
                    {
                        this->remove_emply_leaf((uint64_t)this->root, nullptr, -1);
                        this->root = nullptr;
                        this->root_is_leaf_ = false;
                        this->height_ = 0;
                    }
                }
            }

            void remove(uint64_t pos)
            {
                if (!this->empty())
                {
                    assert(pos < this->size());

                    int64_t position_to_remove = this->compute_path_from_root_to_leaf(pos);
                    this->remove_using_path(this->tmp_path, position_to_remove);
                }
                else
                {
                    throw std::invalid_argument("Remove Error");
                }
                /*
                if (this->need_deflag())
                {
                    this->defragmentation();
                }
                */
            }
            void insert(uint64_t pos, VALUE value, uint64_t sum_delta)
            {
                if (pos < this->size())
                {
                    if (!this->empty())
                    {
                        assert(pos <= this->size());

                        int64_t position_to_insert = this->compute_path_from_root_to_leaf(pos);

                        if (position_to_insert != -1)
                        {
                            uint64_t x = this->tmp_path[this->tmp_path.size() - 1].get_leaf_container_index();
                            this->leaf_container_vec[x].insert(position_to_insert, value);

                            for (int64_t i = this->tmp_path.size() - 2; i >= 0; i--)
                            {
                                Node *node = this->tmp_path[i].get_node();
                                uint64_t child_index = this->tmp_path[i + 1].get_parent_edge_index();
                                node->increment(child_index, 1, sum_delta);
                            }
                            this->balance_for_insertion(this->tmp_path);
                        }
                        else
                        {
                            throw std::runtime_error("Error: insert");
                        }
                    }
                    else
                    {
                        if (pos == 0)
                        {
                            this->create_root_leaf(value);
                        }
                        else
                        {
                            throw std::invalid_argument("Error(BPTree::insert)");
                        }
                    }
                }
                else
                {
                    assert(pos == this->size());
                    this->push_back(value);
                }
            }
            /*
            void insert(uint64_t pos, VALUE value, uint64_t sum_delta)
            {
                std::vector<NodePointer> path;
                this->insert(pos, value, sum_delta, path);

            }
            */
            void resize(uint64_t _size, VALUE default_value)
            {
                if (this->size() < _size)
                {
                    uint64_t i = 0;
                    std::vector<NodePointer> path;
                    uint64_t diff = _size - this->size();

                    while (i < diff)
                    {
                        path.clear();
                        this->get_path_from_root_to_last_leaf(path);
                        if (path.size() > 0)
                        {
                            uint64_t leaf = path[path.size() - 1].get_leaf_container_index();
                            uint64_t len = 0;
                            while (i < diff && leaf_container_vec[leaf].size() <= this->get_max_count_of_values_in_leaf())
                            {
                                this->leaf_container_vec[leaf].push_back(default_value);
                                i++;
                                len++;
                            }
                            uint64_t sum = len > 0 ? this->leaf_container_vec[leaf].reverse_psum(len - 1) : 0;

                            for (int64_t j = path.size() - 2; j >= 0; --j)
                            {
                                Node *parent = path[j].get_node();
                                uint64_t parent_edge_index = path[j + 1].get_parent_edge_index();
                                parent->increment(parent_edge_index, len, sum);
                            }
                            this->balance_for_insertion(path, true);
                        }
                        else
                        {
                            this->create_root_leaf(default_value);
                            i++;
                        }
                    }
                }
                else
                {
                    while (this->size() > _size)
                    {
                        this->remove(this->size() - 1);
                    }
                }
            }

            void set_linked_tree(BPTree *_tree)
            {
                this->linked_tree_ = _tree;
            }
            BPTree *get_linked_tree() const
            {
                return this->linked_tree_;
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Non-const private functions for updating nodes
            ///   Non-const private functions for updating nodes
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            void move_values_right(Node *left_node, Node *right_node, uint64_t len, bool is_leaf, Node *parent, int64_t parent_edge_index_of_left_node)
            {
                if (is_leaf)
                {
                    uint64_t left_leaf = (uint64_t)left_node;
                    uint64_t right_leaf = (uint64_t)right_node;
                    assert(left_leaf < this->leaf_container_vec.size());
                    assert(right_leaf < this->leaf_container_vec.size());

                    if (USE_PSUM && parent != nullptr)
                    {
                        int64_t sum = len != 0 ? this->leaf_container_vec[left_leaf].reverse_psum(len - 1) : 0;
                        parent->__increment_a_value_of_sum_deque(parent_edge_index_of_left_node, -sum);
                        parent->__increment_a_value_of_sum_deque(parent_edge_index_of_left_node + 1, sum);

                        // stool::SimpleDeque16<uint64_t> &parent_sum_deq = parent->get_value_sum_deque();
                        // parent_sum_deq[parent_edge_index_of_left_node] -= sum;
                        // parent_sum_deq[parent_edge_index_of_left_node + 1] += sum;
                    }

                    if (parent != nullptr)
                    {

                        stool::SimpleDeque16<uint64_t> &parent_count_deq = parent->get_value_count_deque();

                        parent_count_deq[parent_edge_index_of_left_node] -= len;
                        parent_count_deq[parent_edge_index_of_left_node + 1] += len;
                    }
                    auto items = this->leaf_container_vec[left_leaf].pop_back(len);
                    this->leaf_container_vec[right_leaf].push_front(items);
                }
                else
                {
                    BPFunctions::move_right(*left_node, *right_node, len, parent, parent_edge_index_of_left_node, this->parent_vec);
                }
            }
            void move_values_left(Node *left_node, Node *right_node, uint64_t len, bool is_leaf, Node *parent, int64_t parent_edge_index_of_right_node)
            {
                if (is_leaf)
                {
                    uint64_t left_leaf = (uint64_t)left_node;
                    uint64_t right_leaf = (uint64_t)right_node;
                    assert(left_leaf < this->leaf_container_vec.size());

                    if (USE_PSUM && parent != nullptr)
                    {
                        uint64_t sum = len != 0 ? this->leaf_container_vec[right_leaf].psum(len - 1) : 0;
                        parent->__increment_a_value_of_sum_deque(parent_edge_index_of_right_node, -sum);
                        parent->__increment_a_value_of_sum_deque(parent_edge_index_of_right_node - 1, sum);

                        // stool::SimpleDeque16<uint64_t> &parent_sum_deq = parent->get_value_sum_deque();
                        // parent_sum_deq[parent_edge_index_of_right_node] -= sum;
                        // parent_sum_deq[parent_edge_index_of_right_node - 1] += sum;
                    }

                    if (parent != nullptr)
                    {
                        stool::SimpleDeque16<uint64_t> &parent_count_deq = parent->get_value_count_deque();
                        parent_count_deq[parent_edge_index_of_right_node] -= len;
                        parent_count_deq[parent_edge_index_of_right_node - 1] += len;
                    }
                    auto items = this->leaf_container_vec[right_leaf].pop_front(len);
                    this->leaf_container_vec[left_leaf].push_back(items);
                }
                else
                {
                    BPFunctions::move_left(*left_node, *right_node, len, parent, parent_edge_index_of_right_node, this->parent_vec);
                }
            }

            void split_node(Node *left_node, Node *right_node, bool is_leaf, Node *parent, int64_t parent_edge_index_of_left_node, [[maybe_unused]] std::vector<Node *> &parent_vec)
            {
                uint64_t degree = 0;
                if (is_leaf)
                {
                    // std::cout << "SPLIT: " << (uint64_t)left_node << "/" << (uint64_t)right_node << "/" << (uint64_t)this << std::endl;
                    //  this->print_leaves();
                    //  this->leaf_container_vec[(uint64_t)left_node].print();
                    degree = leaf_container_vec[(uint64_t)left_node].size();
                    assert((uint64_t)left_node < this->leaf_container_vec.size());
                    assert((uint64_t)right_node < this->leaf_container_vec.size());
                    assert(this->leaf_container_vec[(uint64_t)left_node].size() == this->get_max_count_of_values_in_leaf() + 1);
                }
                else
                {
                    degree = left_node->get_degree();
                }
                uint64_t left_len = degree / 2;
                uint64_t right_len = degree - left_len;
                this->move_values_right(left_node, right_node, right_len, is_leaf, parent, parent_edge_index_of_left_node);
            }

            //@}

            void increment(uint64_t i, int64_t delta)
            {
                uint64_t pos = this->compute_path_from_root_to_leaf(i);
                NodePointer &leaf_pointer = this->tmp_path[this->tmp_path.size() - 1];
                LEAF_CONTAINER &leaf = this->leaf_container_vec[leaf_pointer.get_leaf_container_index()];
                leaf.increment(pos, delta);
                for (int64_t i = this->tmp_path.size() - 2; i >= 0; --i)
                {
                    uint64_t idx = this->tmp_path[i + 1].get_parent_edge_index();
                    Node *parent = this->tmp_path[i].get_node();
                    parent->increment(idx, 0, delta);
                }
            }

            bool check_if_leaf_container_vec_is_sorted() const
            {
                auto _end = this->get_leaf_forward_iterator_end();
                int64_t prev = -1;
                bool b = true;
                for (auto it = this->get_leaf_forward_iterator_begin(); it != _end; ++it)
                {
                    if (prev + 1 != (int64_t)(*it))
                    {
                        b = false;
                        break;
                    }
                    else
                    {
                        prev = *it;
                    }
                }
                return b;
            }

        private:
            void preprocess_for_the_exchange_of_two_leaves([[maybe_unused]] uint64_t leaf_index1, [[maybe_unused]] uint64_t leaf_index2)
            {
            }
            void exchange(stool::SimpleDeque16<Node *> &children1, uint64_t j, uint64_t leaf_index2)
            {

                uint64_t leaf_index1 = (uint64_t)children1[j];
                assert(this->parent_vec[leaf_index1] != nullptr);

                if (leaf_index1 != leaf_index2)
                {
                    this->preprocess_for_the_exchange_of_two_leaves(leaf_index1, leaf_index2);

                    if (this->parent_vec[leaf_index2] != nullptr)
                    {
                        auto &children2 = this->parent_vec[leaf_index2]->get_children();
                        uint64_t idx = this->parent_vec[leaf_index2]->get_index((Node *)leaf_index2);
                        this->leaf_container_vec[leaf_index2].swap(this->leaf_container_vec[leaf_index1]);
                        children1[j] = (Node *)leaf_index2;
                        children2[idx] = (Node *)leaf_index1;
                    }
                    else
                    {
                        this->leaf_container_vec[leaf_index2].swap(this->leaf_container_vec[leaf_index1]);
                        children1[j] = (Node *)leaf_index2;
                    }

                    Node *tmp_n = this->parent_vec[leaf_index1];
                    this->parent_vec[leaf_index1] = this->parent_vec[leaf_index2];
                    this->parent_vec[leaf_index2] = tmp_n;
                }
            }

        public:
            void sort_leaf_containers()
            {
                if (this->size() == 0 || this->root_is_leaf_)
                {
                    return;
                }
                /*
                if (this->linked_tree_ != nullptr)
                {
                    std::cout << "INFO" << std::endl;
                    //this->print_tree();
                    this->print_leaves();
                    //this->linked_tree_->print_tree();
                    this->linked_tree_->print_leaves();
                    std::cout << "INFO[END]" << std::endl;
                }
                */

                auto _end = this->get_postorder_iterator_end();

                if (!USE_PARENT_FIELD)
                {
                    this->parent_vec.resize(this->leaf_container_vec.size(), nullptr);

                    for (PostorderIterator it = this->get_postorder_iterator_begin(); it != _end; ++it)
                    {
                        NodePointer np = *it;
                        if (!np.is_leaf())
                        {
                            Node *node = np.get_node();
                            if (node->is_parent_of_leaves())
                            {
                                auto &children = node->get_children();
                                for (Node *v : children)
                                {
                                    uint64_t leaf = (uint64_t)v;
                                    this->parent_vec[leaf] = node;
                                }
                            }
                        }
                    }
                }
                uint64_t counter = 0;

                for (PostorderIterator it = this->get_postorder_iterator_begin(); it != _end; ++it)
                {
                    NodePointer np = *it;
                    if (!np.is_leaf())
                    {
                        Node *node = np.get_node();
                        if (node->is_parent_of_leaves())
                        {
                            uint64_t _size = node->children_count();
                            auto &children1 = node->get_children();

                            for (uint64_t j = 0; j < _size; j++)
                            {
                                this->exchange(children1, j, counter);
                                /*
                                uint64_t leaf = (uint64_t)children1[j];

                                if (counter != leaf)
                                {
                                    if (this->parent_vec[counter] != nullptr)
                                    {
                                        auto &children2 = this->parent_vec[counter]->get_children();
                                        uint64_t idx = this->parent_vec[counter]->get_index((Node *)counter);
                                        this->leaf_container_vec[counter].swap(this->leaf_container_vec[leaf]);
                                        children[j] = (Node *)counter;
                                        children2[idx] = (Node *)leaf;
                                    }
                                    else
                                    {
                                        this->leaf_container_vec[counter].swap(this->leaf_container_vec[leaf]);
                                        children[j] = (Node *)counter;
                                    }

                                    Node *tmp_n = this->parent_vec[leaf];
                                    this->parent_vec[leaf] = this->parent_vec[counter];
                                    this->parent_vec[counter] = tmp_n;
                                }
                                */
                                counter++;
                            }
                        }
                    }
                }
                while (this->leaf_container_vec.size() > counter)
                {
                    this->leaf_container_vec.pop_back();
                    this->parent_vec.pop_back();
                }
                while (this->unused_leaf_container_indexes.size() > 0)
                {
                    this->unused_leaf_container_indexes.pop();
                }

                if (!USE_PARENT_FIELD)
                {
                    std::vector<Node *> tmp;
                    this->parent_vec.swap(tmp);
                }

                if (this->root_is_leaf_)
                {
                    this->root = (Node *)0;
                }

                assert(this->check_if_leaf_container_vec_is_sorted());
            }

            std::vector<Node *> build_from_leaf_containers_sub2(std::vector<Node *> &nodes, uint64_t current_height)
            {
                assert(nodes.size() > 0);
                std::vector<Node *> r;
                if (nodes.size() == 1)
                {
                    this->root = nodes[0];
                    this->root_is_leaf_ = false;
                    this->height_ = current_height;
                }
                else if (nodes.size() <= this->_max_degree_of_internal_node)
                {

                    Node *root = this->get_new_node_pointer();
                    root->initialize(false, this->leaf_container_vec);
                    for (uint64_t i = 0; i < nodes.size(); i++)
                    {
                        if (USE_PARENT_FIELD)
                        {
                            nodes[i]->set_parent(root);
                        }
                        uint64_t count = nodes[i]->get_value_count();
                        uint64_t sum = 0;
                        if (USE_PSUM)
                        {
                            sum = nodes[i]->get_value_sum();
                        }
                        root->append_child(nodes[i], count, sum);
                    }
                    r.push_back(root);
                }
                else
                {
                    uint64_t i = 0;
                    uint64_t counter = nodes.size();
                    uint64_t threshold = this->_max_degree_of_internal_node * 2;
                    while (counter > 0)
                    {
                        uint64_t d = 0;
                        if (counter >= threshold)
                        {
                            d = this->_max_degree_of_internal_node;
                        }
                        else if (counter > this->_max_degree_of_internal_node)
                        {
                            d = this->_max_degree_of_internal_node / 2;
                        }
                        else
                        {
                            d = counter;
                        }
                        Node *node = this->get_new_node_pointer();
                        node->initialize(false, this->leaf_container_vec);

                        for (uint64_t j = 0; j < d; j++)
                        {
                            if (USE_PARENT_FIELD)
                            {
                                nodes[i]->set_parent(node);
                            }
                            uint64_t count = nodes[i]->get_value_count();
                            uint64_t sum = 0;
                            if (USE_PSUM)
                            {
                                sum = nodes[i]->get_value_sum();
                            }
                            node->append_child(nodes[i], count, sum);
                            i++;
                            counter--;
                        }
                        r.push_back(node);
                    }
                }
                return r;
            }

            std::vector<Node *> build_from_leaf_containers_sub1()
            {
                std::vector<Node *> r;

                if (this->leaf_container_vec.size() == 0)
                {
                    this->root = nullptr;
                    this->root_is_leaf_ = false;
                    this->height_ = 0;
                }
                else if (this->leaf_container_vec.size() == 1)
                {
                    this->root = (Node *)0;
                    this->root_is_leaf_ = true;
                    this->height_ = 1;
                }
                else if (this->leaf_container_vec.size() <= this->_max_degree_of_internal_node)
                {
                    Node *root = this->get_new_node_pointer();
                    root->initialize(true, this->leaf_container_vec);
                    for (uint64_t i = 0; i < this->leaf_container_vec.size(); i++)
                    {
                        uint64_t psum = 0;
                        if (USE_PSUM)
                        {
                            psum += this->leaf_container_vec[i].psum();
                        }
                        if (USE_PARENT_FIELD)
                        {
                            this->parent_vec[i] = root;
                        }

                        root->append_child((Node *)i, this->leaf_container_vec[i].size(), psum);
                    }
                    r.push_back(root);
                }
                else
                {

                    uint64_t i = 0;
                    uint64_t counter = this->leaf_container_vec.size();
                    uint64_t threshold = this->_max_degree_of_internal_node * 2;
                    while (counter > 0)
                    {
                        uint64_t d = 0;
                        if (counter >= threshold)
                        {
                            d = this->_max_degree_of_internal_node;
                        }
                        else if (counter > this->_max_degree_of_internal_node)
                        {
                            d = this->_max_degree_of_internal_node / 2;
                        }
                        else
                        {
                            d = counter;
                        }
                        Node *node = this->get_new_node_pointer();
                        node->initialize(true, this->leaf_container_vec);

                        for (uint64_t j = 0; j < d; j++)
                        {
                            uint64_t psum = 0;
                            if (USE_PSUM)
                            {
                                psum += this->leaf_container_vec[i].psum();
                            }
                            if (USE_PARENT_FIELD)
                            {
                                this->parent_vec[i] = node;
                            }
                            node->append_child((Node *)i, this->leaf_container_vec[i].size(), psum);
                            i++;
                            counter--;
                        }
                        r.push_back(node);
                    }
                }
                return r;
            }

            void build_sub(const std::vector<VALUE> &_values)
            {
                uint64_t i = 0;
                if (_values.size() == 0)
                {
                }
                else if (_values.size() <= this->_max_count_of_values_in_leaf)
                {
                    uint64_t leaf_index = this->get_new_container_index();

                    while (i < _values.size())
                    {
                        this->leaf_container_vec[leaf_index].push_back(_values[i]);
                        i++;
                    }
                }
                else
                {
                    uint64_t threshold = this->_max_count_of_values_in_leaf * 2;
                    uint64_t counter = _values.size();

                    while (i < _values.size())
                    {
                        uint64_t d = 0;
                        if (counter >= threshold)
                        {
                            d = this->_max_count_of_values_in_leaf;
                        }
                        else if (counter > this->_max_count_of_values_in_leaf)
                        {
                            d = this->_max_count_of_values_in_leaf / 2;
                        }
                        else
                        {
                            d = counter;
                        }
                        assert(counter >= (this->_max_count_of_values_in_leaf / 2));

                        uint64_t leaf_index = this->get_new_container_index();
                        for (uint64_t j = 0; j < d; j++)
                        {
                            this->leaf_container_vec[leaf_index].push_back(_values[i]);
                            i++;
                            counter--;
                        }
                    }
                }
            }

            void build(const std::vector<VALUE> &_values)
            {
                this->clear();
                this->build_sub(_values);

                std::vector<Node *> layer = this->build_from_leaf_containers_sub1();
                uint64_t current_height = 2;
                while (layer.size() > 0)
                {

                    std::vector<Node *> next_layer = this->build_from_leaf_containers_sub2(layer, current_height);
                    layer.swap(next_layer);
                    current_height++;
                }
            }

            void build_from_leaf_containers(std::vector<LEAF_CONTAINER> &_leaf_containers)
            {
                this->clear();

                this->leaf_container_vec.swap(_leaf_containers);
                if (USE_PARENT_FIELD)
                {
                    this->parent_vec.resize(this->leaf_container_vec.size(), nullptr);
                }

                std::vector<Node *> layer = this->build_from_leaf_containers_sub1();
                uint64_t current_height = 2;
                while (layer.size() > 0)
                {
                    std::vector<Node *> next_layer = this->build_from_leaf_containers_sub2(layer, current_height);
                    layer.swap(next_layer);
                    current_height++;
                }
            }

            void save(std::vector<uint8_t> &output, uint64_t &pos)
            {
                if (!this->check_if_leaf_container_vec_is_sorted())
                {
                    this->sort_leaf_containers();
                }

                uint64_t _size = sizeof(this->_max_degree_of_internal_node) + sizeof(this->_max_count_of_values_in_leaf) + LEAF_CONTAINER::get_byte_size(this->leaf_container_vec);
                if (pos + _size > output.size())
                {
                    output.resize(pos + _size);
                }

                std::memcpy(output.data() + pos, &this->_max_degree_of_internal_node, sizeof(uint64_t));
                pos += sizeof(uint64_t);
                std::memcpy(output.data() + pos, &this->_max_count_of_values_in_leaf, sizeof(uint64_t));
                pos += sizeof(uint64_t);

                LEAF_CONTAINER::save(this->leaf_container_vec, output, pos);
            }

            void save(std::ofstream &os)
            {
                if (!this->check_if_leaf_container_vec_is_sorted())
                {
                    this->sort_leaf_containers();

                }

                os.write((const char *)(&this->_max_degree_of_internal_node), sizeof(uint64_t));
                os.write((const char *)(&this->_max_count_of_values_in_leaf), sizeof(uint64_t));
                LEAF_CONTAINER::save(this->leaf_container_vec, os);
            }

            void build_from_data(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                this->clear();
                std::memcpy(&this->_max_degree_of_internal_node, data.data() + pos, sizeof(uint64_t));
                pos += sizeof(uint64_t);
                std::memcpy(&this->_max_count_of_values_in_leaf, data.data() + pos, sizeof(uint64_t));
                pos += sizeof(uint64_t);

                auto tmp = LEAF_CONTAINER::load_vector(data, pos);

                this->build_from_leaf_containers(tmp);
            }

            void build_from_data(std::ifstream &ifs)
            {
                this->clear();
                ifs.read((char *)(&this->_max_degree_of_internal_node), sizeof(uint64_t));
                ifs.read((char *)(&this->_max_count_of_values_in_leaf), sizeof(uint64_t));

                auto tmp = LEAF_CONTAINER::load_vector(ifs);

                this->build_from_leaf_containers(tmp);
            }

            void print_statistics(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                uint64_t internal_node_count = 0;
                uint64_t leaf_count = 0;
                uint64_t total_degree_of_internal_node = 0;

                for (BPPostorderIterator it = this->get_postorder_iterator_begin(); it != this->get_postorder_iterator_end(); ++it)
                {
                    NodePointer np = *it;
                    if (!np.is_leaf())
                    {
                        internal_node_count++;
                        total_degree_of_internal_node += np.get_node()->children_count();
                    }
                    else
                    {
                        leaf_count++;
                    }
                }

                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Statistics(BPTree):" << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "The number of internal nodes: " << internal_node_count << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "The number of used leaf containers: " << leaf_count << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "The height of this tree: " << this->height_ << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "The number of stored values: " << this->size() << std::endl;

                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "The max degree of internal node (parameter): " << this->_max_degree_of_internal_node << std::endl;
                if (internal_node_count > 0)
                {
                    std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "The average degree of internal node: " << (total_degree_of_internal_node / internal_node_count) << std::endl;
                }
                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "The max count of values in leaf (parameter): " << this->_max_count_of_values_in_leaf << std::endl;
                if (this->size() > 0)
                {
                    std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "The average count of values in leaf (parameter): " << (this->size() / leaf_count) << std::endl;
                }

                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "The number of unused nodes: " << this->unused_node_pointers.size() << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "The number of unused leaf containers: " << this->unused_leaf_container_indexes.size() << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "The number of stored parent pointers: " << this->parent_vec.size() << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "Estimated memory usage: " << this->size_in_bytes() << " bytes" << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[END]" << std::endl;


            }
        };
    }
}
