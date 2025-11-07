#pragma once
#include "../bp_tree/bp_internal_node.hpp"
#include "./permutation_container.hpp"

namespace stool
{
    /// \defgroup PermutationClasses Classes for dynamic permutations
    /// @{
    /// @}

    namespace bptree
    {

        /**
         * @brief BPInternalNode for dynamic permutations [Unchecked AI's Comment]
         * \ingroup PermutationClasses
         */
        template <uint64_t MAX_DEGREE>
        class BPInternalNode<stool::bptree::PermutationContainer, stool::bptree::PermutationItem, MAX_DEGREE, false>
        {
#if DEBUG
        public:
            static inline int ID_COUNTER = 0;
            uint64_t id;
#endif

        private:
            using InternalNode = BPInternalNode<stool::bptree::PermutationContainer, stool::bptree::PermutationItem, MAX_DEGREE, false>;
            stool::SimpleDeque16<InternalNode *> children_;
            stool::NaiveIntegerArray<MAX_DEGREE + 2> children_value_count_deque_;
            // stool::NaiveIntegerArrayForFasterPsum<MAX_DEGREE+2> children_value_count_deque_;

            InternalNode *parent_ = nullptr;
            bool is_parent_of_leaves_ = false;

        public:
            BPInternalNode()
            {
#if DEBUG
                this->id = ID_COUNTER++;
#endif
            }

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Initializers
            ///   Initializers
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            void initialize(const std::vector<InternalNode *> _children, bool _is_parent_of_leaves, const std::vector<stool::bptree::PermutationContainer> &_leaf_container_vec)
            {
                this->is_parent_of_leaves_ = _is_parent_of_leaves;
                this->children_.clear();
                for (InternalNode *node : _children)
                {
                    this->children_.push_back(node);
                }

                this->children_value_count_deque_.clear();
                if (this->is_parent_of_leaves_)
                {
                    for (InternalNode *child : this->children_)
                    {
                        this->children_value_count_deque_.push_back(_leaf_container_vec[(uint64_t)child].size());
                    }
                }
                else
                {
                    for (InternalNode *child : this->children_)
                    {
                        this->children_value_count_deque_.push_back(child->psum_on_count_deque());
                    }
                }
            }
            void set_parent(InternalNode *_parent)
            {
                this->parent_ = _parent;
            }
            void initialize(BPInternalNode *_left_node, BPInternalNode *_right_node, const std::vector<stool::bptree::PermutationContainer> &_leaf_container_vec)
            {
                std::vector<InternalNode *> _children;
                _children.push_back(_left_node);
                _children.push_back(_right_node);
                this->initialize(_children, false, _leaf_container_vec);
            }
            void initialize(uint64_t _left_node, uint64_t _right_node, const std::vector<stool::bptree::PermutationContainer> &leaf_container_vec)
            {
                std::vector<InternalNode *> _children;
                _children.push_back((InternalNode *)_left_node);
                _children.push_back((InternalNode *)_right_node);
                this->initialize(_children, true, leaf_container_vec);
            }
            void initialize(bool _is_parent_of_leaves, const std::vector<stool::bptree::PermutationContainer> &_leaf_container_vec)
            {
                std::vector<InternalNode *> _children;
                this->initialize(_children, _is_parent_of_leaves, _leaf_container_vec);
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Operations on the count deque
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            uint64_t psum_on_count_deque(uint64_t pos) const
            {
                return this->children_value_count_deque_.psum(pos);
            }

            uint64_t psum_on_count_deque() const
            {
                return this->children_value_count_deque_.psum();
            }
            int64_t search_query_on_count_deque(uint64_t value, uint64_t &sum) const
            {
                return this->children_value_count_deque_.search(value, sum);
            }
            uint64_t access_count_deque(uint64_t pos) const
            {
                return this->children_value_count_deque_[pos];
            }
            void pop_back_many_on_count_deque(uint64_t len)
            {
                this->children_value_count_deque_.pop_back_many(len);
            }
            void pop_front_many_on_count_deque(uint64_t len)
            {
                this->children_value_count_deque_.pop_front_many(len);
            }
            void push_front_many_on_count_deque(std::vector<uint64_t> values)
            {
                this->children_value_count_deque_.push_front_many(values);
            }
            void push_back_many_on_count_deque(std::vector<uint64_t> values)
            {
                this->children_value_count_deque_.push_back_many(values);
            }
            void increment_on_count_deque(uint64_t pos, int64_t value)
            {
                this->children_value_count_deque_.increment(pos, value);
            }
            void decrement_on_count_deque(uint64_t pos, int64_t value)
            {
                this->children_value_count_deque_.decrement(pos, value);
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Operations on the sum deque
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            uint64_t psum_on_sum_deque() const
            {
                throw std::runtime_error("BPInternalNode<PermutationContainer, PermutationItem>::psum_on_sum_deque(): No Implementation");
            }

            uint64_t access_last_item_on_sum_deque() const
            {
                throw std::runtime_error("BPInternalNode<PermutationContainer, PermutationItem>::access_last_item_on_sum_deque(): No Implementation");
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Properties
            ///   Properties
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            const stool::SimpleDeque16<InternalNode *> &get_children() const
            {
                return this->children_;
            }
            stool::SimpleDeque16<InternalNode *> &get_children()
            {
                return this->children_;
            }

            bool use_psum() const
            {
                return false;
            }
            bool has_parent_pointer_field() const
            {
                return true;
            }

            bool is_parent_of_leaves() const
            {
                return this->is_parent_of_leaves_;
            }
            uint64_t children_count() const
            {
                return this->children_.size();
            }
            InternalNode *get_child(uint64_t ith) const
            {
                return this->children_[ith];
            }

            uint64_t get_degree() const
            {
                return this->children_.size();
            }
            uint64_t get_height() const
            {
                if (this->is_parent_of_leaves())
                {
                    return 1;
                }
                else
                {
                    return this->children_[0]->get_height() + 1;
                }
            }
            uint64_t size_in_bytes() const
            {
                return sizeof(BPInternalNode<stool::bptree::PermutationContainer, stool::bptree::PermutationItem, MAX_DEGREE, false>) + this->children_.size_in_bytes(true) + this->children_value_count_deque_.size_in_bytes(true);
            }
            int64_t get_index(InternalNode *node) const
            {
                uint64_t i = 0;
                for (InternalNode *child : this->children_)
                {
                    if (child == node)
                    {
                        return i;
                    }
                    else
                    {
                        i++;
                    }
                }
                return -1;
            }
            InternalNode *get_parent() const
            {
                return this->parent_;
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name SPSI functions
            ///   SPSI functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Print and verification functions
            ///   Print and verification functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            void print_info([[maybe_unused]] int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::cout << "=================" << std::endl;
                std::cout << "This: " << (uint64_t)this;
                std::cout << ", parent: " << (uint64_t)this->parent_;
                std::cout << ", is_parent_of_leaves: " << this->is_parent_of_leaves();
                std::cout << ", count: " << this->psum_on_count_deque();
                std::cout << ", sum: " << this->psum_on_sum_deque() << std::endl;

                // std::cout << "Parent: " << (uint64_t)this->parent << std::endl;

                std::vector<uint64_t> vec;
                for (uint64_t i = 0; i < this->children_.size(); i++)
                {
                    vec.push_back((uint64_t)this->children_[i]);
                }
                stool::DebugPrinter::print_integers(vec, "children");
                std::cout << "=================" << std::endl;
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Non-const functions
            ///   Non-const functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            void clear()
            {
                this->children_.clear();
                this->children_value_count_deque_.clear();
                this->parent_ = nullptr;
            }

            void increment(uint64_t child_index, int64_t count_delta, [[maybe_unused]] int64_t sum_delta)
            {
                this->children_value_count_deque_.increment(child_index, count_delta);
            }

            void move_container_index(uint64_t child_index, uint64_t new_leaf_index, std::vector<stool::bptree::PermutationContainer> &leaf_container_vec)
            {
                assert(this->is_parent_of_leaves_);
                uint64_t old_leaf = (uint64_t)this->children_[child_index];
                leaf_container_vec[new_leaf_index].swap(leaf_container_vec[old_leaf]);
                this->children_[child_index] = (InternalNode *)new_leaf_index;
            }
            void insert_child(uint64_t pos, InternalNode *child, uint64_t child_count, [[maybe_unused]] uint64_t child_sum)
            {
                this->children_.insert(this->children_.begin() + pos, child);
                this->children_value_count_deque_.insert(pos, child_count);
            }
            void remove_child(uint64_t pos)
            {
                this->children_.erase(this->children_.begin() + pos);
                this->children_value_count_deque_.erase(pos);
            }
            void append_child(InternalNode *child, uint64_t child_count, [[maybe_unused]] uint64_t child_sum)
            {
                this->children_.push_back(child);
                this->children_value_count_deque_.push_back(child_count);
            }
            std::string to_string() const
            {
                std::string s;
#if DEBUG
                s += "InternalNode ID: " + std::to_string(this->id);
#else
                s += "InternalNode ID: " + std::to_string((uint64_t)this);
#endif
                s += ", is_parent_of_leaves: " + std::to_string(this->is_parent_of_leaves());
                s += ", count: " + std::to_string(this->psum_on_count_deque());

                s += ", Count Array: ";
                s += this->children_value_count_deque_.to_string();

                return s;
            }

            //@}
        };
    }
}
