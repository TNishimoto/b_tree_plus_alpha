#pragma once
#include "stool/include/light_stool.hpp"

namespace stool
{
    namespace bptree
    {

        ////////////////////////////////////////////////////////////////////////////////
        /// @class      BPInternalNode
        /// @brief      The internal node of BPTree [Unchecked AI's Comment]
        ///
        ////////////////////////////////////////////////////////////////////////////////
        template <typename LEAF_CONTAINER, typename VALUE, uint64_t MAX_DEGREE, bool USE_PSUM>
        class BPInternalNode
        {

#if DEBUG
        public:
            static inline int ID_COUNTER = 0;
            uint64_t id;
#endif

            using DEQUE_TYPE = stool::NaiveIntegerArray<MAX_DEGREE + 2>;
            //using DEQUE_TYPE = stool::NaiveIntegerArrayForFasterPsum<(MAX_DEGREE + 2)>;
            //using DEQUE_TYPE = stool::EytzingerLayoutForPsum<MAX_DEGREE + 2>;
            

        private:
            using InternalNode = BPInternalNode<LEAF_CONTAINER, VALUE, MAX_DEGREE, USE_PSUM>;
            stool::SimpleDeque16<InternalNode *> children_;
            DEQUE_TYPE children_value_count_deque_;
            DEQUE_TYPE children_value_sum_deque_;
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
            void initialize(const std::vector<InternalNode *> &_children, bool _is_parent_of_leaves, const std::vector<LEAF_CONTAINER> &_leaf_container_vec)
            {
                this->is_parent_of_leaves_ = _is_parent_of_leaves;
                this->children_.clear();
                for (InternalNode *node : _children)
                {
                    this->children_.push_back(node);
                }

                this->children_value_count_deque_.clear();
                this->children_value_sum_deque_.clear();
                if (this->is_parent_of_leaves_)
                {
                    for (InternalNode *child : this->children_)
                    {
                        uint64_t child_count = _leaf_container_vec[(uint64_t)child].size();
                        this->children_value_count_deque_.push_back(child_count);
                        if constexpr (USE_PSUM)
                        {
                            uint64_t psum = _leaf_container_vec[(uint64_t)child].psum();
                            this->children_value_sum_deque_.push_back(psum);
                        }
                    }
                }
                else
                {
                    for (InternalNode *child : this->children_)
                    {
                        uint64_t child_count_psum = child->psum_on_count_deque();
                        this->children_value_count_deque_.push_back(child_count_psum);
                        if constexpr (USE_PSUM)
                        {
                            this->children_value_sum_deque_.push_back(child->psum_on_sum_deque());
                        }
                    }
                }


            }
            void initialize(BPInternalNode *_left_node, BPInternalNode *_right_node, const std::vector<LEAF_CONTAINER> &_leaf_container_vec)
            {
                std::vector<InternalNode *> _children;
                _children.push_back(_left_node);
                _children.push_back(_right_node);
                this->initialize(_children, false, _leaf_container_vec);
            }
            void initialize(uint64_t _left_node, uint64_t _right_node, const std::vector<LEAF_CONTAINER> &leaf_container_vec)
            {
                std::vector<InternalNode *> _children;
                _children.push_back((InternalNode *)_left_node);
                _children.push_back((InternalNode *)_right_node);
                this->initialize(_children, true, leaf_container_vec);
            }
            void initialize(bool _is_parent_of_leaves, const std::vector<LEAF_CONTAINER> &_leaf_container_vec)
            {
                std::vector<InternalNode *> _children;
                this->initialize(_children, _is_parent_of_leaves, _leaf_container_vec);
            }

            void set_parent([[maybe_unused]] InternalNode *_parent)
            {
                throw std::runtime_error("This function is not suppported");
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Operations on the count deque
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            int64_t search_query_on_count_deque(uint64_t value, uint64_t &sum) const
            {

                return this->children_value_count_deque_.search(value, sum);
            }
            uint64_t psum_on_count_deque(uint64_t pos) const
            {
                return this->children_value_count_deque_.psum(pos);
            }
            uint64_t access_count_deque(uint64_t pos) const
            {
                return this->children_value_count_deque_[pos];
            }
            uint64_t psum_on_count_deque() const
            {
                return this->children_value_count_deque_.psum();
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

            int64_t search_query_on_sum_deque(uint64_t value, uint64_t &sum) const
            {
                if constexpr (USE_PSUM)
                {
                    return this->children_value_sum_deque_.search(value, sum);
                }
                else
                {
                    throw std::runtime_error("search_query_on_sum_deque() is not supported");
                }
            }
            void pop_back_on_sum_deque()
            {
                this->children_value_sum_deque_.pop_back();
            }

            void pop_front_on_sum_deque()
            {
                this->children_value_sum_deque_.pop_front();
            }

            void push_front_on_sum_deque(uint64_t value)
            {
                this->children_value_sum_deque_.push_front(value);
            }

            void push_back_on_sum_deque(uint64_t value)
            {
                this->children_value_sum_deque_.push_back(value);
            }

            void increment_on_sum_deque(uint64_t pos, int64_t value)
            {
                this->children_value_sum_deque_.increment(pos, value);
            }
            void decrement_on_sum_deque(uint64_t pos, int64_t value)
            {
                this->children_value_sum_deque_.decrement(pos, value);
            }

            uint64_t access_last_item_on_sum_deque() const
            {
                if constexpr (USE_PSUM)
                {
                    return this->children_value_sum_deque_[this->children_value_sum_deque_.size() - 1];
                }
                else
                {
                    throw std::runtime_error("access_last_item_on_sum_deque() is not supported");
                }
            }

            uint64_t psum_on_sum_deque() const
            {
                return this->children_value_sum_deque_.psum();
            }

            uint64_t access_sum_deque(uint64_t pos) const
            {
                if constexpr (USE_PSUM)
                {
                    return this->children_value_sum_deque_[pos];
                }
                else
                {
                    throw std::runtime_error("access_sum_deque() is not supported");
                }
            }

            uint64_t psum_on_sum_deque(uint64_t pos) const
            {
                if constexpr (USE_PSUM)
                {
                    return this->children_value_sum_deque_.psum(pos);
                }
                else
                {
                    throw std::runtime_error("psum_on_sum_deque() is not supported");
                }
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

            bool has_parent_pointer_field() const
            {
                return false;
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
                return sizeof(BPInternalNode) + (this->children_.size_in_bytes(true) + this->children_value_count_deque_.size_in_bytes(true) + this->children_value_sum_deque_.size_in_bytes(true));
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
                throw std::runtime_error("BPInternalNode::get_parent(): this function is not supported");
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Print and verification functions
            ///   Print and verification functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            void print_info(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "================= NODE =================" << std::endl;
#if DEBUG
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "InternalNode ID: " << this->id;
#else
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "InternalNode ID: " << (uint64_t)this;
#endif
                std::cout << ", is_parent_of_leaves: " << this->is_parent_of_leaves();
                std::cout << ", count: " << this->psum_on_count_deque();
                std::cout << ", sum: " << this->psum_on_sum_deque() << std::endl;

                auto count_deq_str = this->children_value_count_deque_.to_string();
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "count_deque: " << count_deq_str << std::endl;

                if constexpr (USE_PSUM)
                {
                    auto sum_deq_str = this->children_value_sum_deque_.to_string();
                    std::cout << stool::Message::get_paragraph_string(message_paragraph) << "sum_deque: " << sum_deq_str << std::endl;
                }

                // std::cout << "Parent: " << (uint64_t)this->parent << std::endl;

                std::vector<uint64_t> vec;
                for (uint64_t i = 0; i < this->children_.size(); i++)
                {
                    vec.push_back((uint64_t)this->children_[i]);
                }
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "children: " << stool::DebugPrinter::to_integer_string(vec) << std::endl;

                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "==================================" << std::endl;
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
                this->children_value_sum_deque_.clear();

            }

            void increment(uint64_t child_index, int64_t count_delta, int64_t sum_delta)
            {
                assert(child_index < this->children_count());
                if (count_delta != 0)
                {
                    assert(child_index < this->children_value_count_deque_.size());
                    this->children_value_count_deque_.increment(child_index, count_delta);
                }

                if constexpr (USE_PSUM)
                {
                    if (sum_delta != 0)
                    {
                        assert(child_index < this->children_value_sum_deque_.size());
                        this->children_value_sum_deque_.increment(child_index, sum_delta);
                    }
                }
            }

            void move_container_index(uint64_t child_index, uint64_t new_leaf_index, std::vector<LEAF_CONTAINER> &leaf_container_vec)
            {
                assert(this->is_parent_of_leaves_);
                uint64_t old_leaf = (uint64_t)this->children_[child_index];
                leaf_container_vec[new_leaf_index].swap(leaf_container_vec[old_leaf]);
                this->children_[child_index] = (InternalNode *)new_leaf_index;
            }
            void insert_child(uint64_t pos, InternalNode *child, uint64_t child_count, uint64_t child_sum)
            {
                this->children_.insert(this->children_.begin() + pos, child);
                this->children_value_count_deque_.insert(pos, child_count);
                if constexpr (USE_PSUM)
                {
                    this->children_value_sum_deque_.insert(pos, child_sum);
                }

            }
            void append_child(InternalNode *child, uint64_t child_count, uint64_t child_sum)
            {
                this->children_.push_back(child);
                this->children_value_count_deque_.push_back(child_count);
                if constexpr (USE_PSUM)
                {
                    this->children_value_sum_deque_.push_back(child_sum);
                }

            }

            void remove_child(uint64_t pos)
            {
                this->children_.erase(this->children_.begin() + pos);
                this->children_value_count_deque_.erase(pos);
                if constexpr (USE_PSUM)
                {
                    this->children_value_sum_deque_.erase(pos);
                }

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
                s += ", sum: " + std::to_string(this->psum_on_sum_deque());
                return s;
            }

            //@}
        };
    }
}
