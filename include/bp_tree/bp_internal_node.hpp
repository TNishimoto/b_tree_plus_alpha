#pragma once
#include "stool/include/light_stool.hpp"

namespace stool
{
    namespace bptree
    {

        ////////////////////////////////////////////////////////////////////////////////
        /// @class      BPInternalNode
        /// @brief      The internal node of BPTree
        ///
        ////////////////////////////////////////////////////////////////////////////////
        template <typename LEAF_CONTAINER, typename VALUE, uint64_t MAX_DEGREE>
        class BPInternalNode
        {

            #if DEBUG
            public:
            static inline int ID_COUNTER = 0;
            uint64_t id;
            #endif

            using DEQUE_TYPE = stool::NaiveArray<MAX_DEGREE+2>;
            //using DEQUE_TYPE = stool::StaticArrayDeque<(MAX_DEGREE+2)*2, false>;
            //using DEQUE_TYPE = stool::StaticArrayDeque<MAX_DEGREE+2, true>;
            //using DEQUE_TYPE = stool::FasterStaticArrayDeque<MAX_DEGREE+2>;

            private:
            using InternalNode = BPInternalNode<LEAF_CONTAINER, VALUE, MAX_DEGREE>;
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
            void initialize(const std::vector<InternalNode *> _children, bool _is_parent_of_leaves, const std::vector<LEAF_CONTAINER> &_leaf_container_vec)
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
                        this->children_value_count_deque_.push_back(_leaf_container_vec[(uint64_t)child].size());

                        uint64_t psum = _leaf_container_vec[(uint64_t)child].psum();
                        this->children_value_sum_deque_.push_back(psum);
                    }
                }
                else
                {
                    for (InternalNode *child : this->children_)
                    {
                        this->children_value_count_deque_.push_back(child->get_value_count());

                        this->children_value_sum_deque_.push_back(child->get_value_sum());
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
            const DEQUE_TYPE &get_value_count_deque() const
            {
                return this->children_value_count_deque_;
            }
            DEQUE_TYPE &get_value_count_deque()
            {
                return this->children_value_count_deque_;
            }
            const DEQUE_TYPE &get_value_sum_deque() const
            {
                return this->children_value_sum_deque_;
            }
            /*
            stool::SimpleDeque16<uint64_t> &get_value_sum_deque()
            {
                return this->children_value_sum_deque_;
            }
            */
            bool use_psum() const
            {
                return true;
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
            uint64_t get_value_count() const
            {
                return this->children_value_count_deque_.psum();
                /*
                uint64_t sum = 0;
                for (uint64_t p : this->children_value_count_deque_)
                {
                    sum += p;
                }
                return sum;
                */
            }
            uint64_t get_value_sum() const
            {
                return this->children_value_sum_deque_.psum();

                /*
                uint64_t sum = 0;
                for (uint64_t p : this->children_value_sum_deque_)
                {
                    sum += p;
                }
                return sum;
                */
            }
            void __increment_a_value_of_sum_deque(uint64_t pos, int64_t value){                
                #if DEBUG
                if(value < 0){
                    assert((int64_t)this->children_value_sum_deque_.at(pos) >=  -value);
                }
                #endif

                this->children_value_sum_deque_.increment(pos, value);
            }
            void __push_back_on_sum_deque(uint64_t value){
                this->children_value_sum_deque_.push_back(value);
            }
            void __push_front_on_sum_deque(uint64_t value){
                this->children_value_sum_deque_.push_front(value);
            }
            void __pop_front_on_sum_deque(){
                this->children_value_sum_deque_.pop_front();
            }
            void __pop_back_on_sum_deque(){
                this->children_value_sum_deque_.pop_back();
            }
            uint64_t __last_item_on_sum_deque() const {
                return this->children_value_sum_deque_[this->children_value_sum_deque_.size()-1];
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
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "InternalNode ID: " << this->id ;
                #else 
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "InternalNode ID: " << (uint64_t)this;
                #endif
                std::cout << ", is_parent_of_leaves: " << this->is_parent_of_leaves();
                std::cout << ", count: " << this->get_value_count();
                std::cout << ", sum: " << this->get_value_sum() << std::endl;


                auto count_deq_str = this->children_value_count_deque_.to_string();
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "count_deque: " << count_deq_str << std::endl;
                auto sum_deq_str = this->children_value_sum_deque_.to_string();
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "sum_deque: " << sum_deq_str << std::endl;

                // std::cout << "Parent: " << (uint64_t)this->parent << std::endl;

                std::vector<uint64_t> vec;
                for (uint64_t i = 0; i < this->children_.size(); i++)
                {
                    vec.push_back((uint64_t)this->children_[i]);
                }
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "children: " << stool::DebugPrinter::to_integer_string(vec) << std::endl;

                std::cout << stool::Message::get_paragraph_string(message_paragraph)<< "==================================" << std::endl;
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
                    //this->children_value_count_deque_[child_index] += count_delta;
                }

                if (sum_delta != 0)
                {
                    assert(child_index < this->children_value_sum_deque_.size());
                    this->children_value_sum_deque_.increment(child_index, sum_delta);


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
                this->children_value_sum_deque_.insert(pos, child_sum);
            }
            void append_child(InternalNode *child, uint64_t child_count, uint64_t child_sum)
            {
                this->children_.push_back(child);
                this->children_value_count_deque_.push_back(child_count);
                this->children_value_sum_deque_.push_back(child_sum);
            }

            void remove_child(uint64_t pos)
            {
                this->children_.erase(this->children_.begin() + pos);
                this->children_value_count_deque_.erase(pos);
                this->children_value_sum_deque_.erase(pos);
            }

            //@}
        };
    }
}
