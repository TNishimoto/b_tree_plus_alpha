#pragma once
#include "./bp_postorder_iterator.hpp"



namespace stool
{
    namespace bptree
    {
        ////////////////////////////////////////////////////////////////////////////////
        /// @class      BPValueForwardIterator
        /// @brief      The forward iterator of the values stored in BPTree
        ///
        ////////////////////////////////////////////////////////////////////////////////
        template <typename LEAF_CONTAINER, typename VALUE, uint64_t MAX_DEGREE>
        class BPValueForwardIterator
        {
        private:
            void sub_proceed()
            {
                while (!this->node_it.is_end() && !this->node_it.get_current_node().is_leaf())
                {
                    ++this->node_it;

                }
                
                if (this->node_it.is_end())
                {
                    this->tmp_values.clear();
                    this->node_it.idx = UINT64_MAX;
                    this->tmp_idx = UINT32_MAX;
                }
                else
                {
                
                    this->tmp_values.clear();
                    LEAF_CONTAINER &cont = (*this->ref)[this->node_it.get_current_node().get_leaf_container_index()];

                    for (const VALUE it : cont)
                    {
                        this->tmp_values.push_back(it);
                    }
                    this->tmp_idx = 0;
                }

            }

        public:
            using SNode = StackNode<LEAF_CONTAINER, VALUE, MAX_DEGREE>;
            using NodePointer = BPNodePointer<LEAF_CONTAINER, VALUE, MAX_DEGREE>;
            using Node = BPInternalNode<LEAF_CONTAINER, VALUE, MAX_DEGREE>;
            using NodeIterator = BPPostorderIterator<LEAF_CONTAINER, VALUE, MAX_DEGREE>;

            NodeIterator node_it;
            std::vector<uint64_t> tmp_values;
            std::vector<LEAF_CONTAINER> *ref = nullptr;
            uint32_t tmp_idx;

            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;

            BPValueForwardIterator(NodeIterator *_root, const std::vector<LEAF_CONTAINER> *_ref)
            {
                this->tmp_values.clear();
                this->tmp_idx = 0;
                this->ref = const_cast<std::vector<LEAF_CONTAINER> *>(_ref);
                if (_root != nullptr)
                {
                    _root->copy_to(this->node_it);
                    if (this->node_it.is_end())
                    {
                        this->node_it.idx = UINT64_MAX;
                        this->tmp_idx = UINT32_MAX;
                    }
                    else
                    {
                        this->sub_proceed();
                    }
                }
                else
                {
                    this->node_it.idx = UINT64_MAX;
                    this->tmp_idx = UINT32_MAX;
                }
            }
            int compare(const BPValueForwardIterator &other) const
            {
                if (this->node_it.idx != other.node_it.idx)
                {
                    if (this->node_it.idx < other.node_it.idx)
                    {
                        return -1;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    if (this->tmp_idx != other.tmp_idx)
                    {
                        if (this->tmp_idx < other.tmp_idx)
                        {
                            return -1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
            }

            VALUE operator*() const
            {
                if (this->node_it.is_end())
                {
                    throw std::runtime_error("Error:BPValueForwardIterator");
                }
                else
                {
                    return this->tmp_values[this->tmp_idx];
                }
            }
            bool is_end() const
            {
                return this->tmp_idx == UINT32_MAX;
            }

            BPValueForwardIterator &operator++()
            {
                if (this->tmp_idx + 1 < this->tmp_values.size())
                {
                    this->tmp_idx++;
                }
                else if (this->tmp_idx + 1 == this->tmp_values.size())
                {
                    ++this->node_it;
                    this->sub_proceed();
                }
                else
                {
                    throw std::runtime_error("Error:BPValueForwardIterator");
                }
                return *this;
            }

            bool operator==(const BPValueForwardIterator &other) const { return this->compare(other) == 0; }
            bool operator!=(const BPValueForwardIterator &other) const { return this->compare(other) != 0; }
            bool operator<(const BPValueForwardIterator &other) const { return this->compare(other) < 0; }
            bool operator>(const BPValueForwardIterator &other) const { return this->compare(other) > 0; }
            bool operator<=(const BPValueForwardIterator &other) const { return this->compare(other) <= 0; }
            bool operator>=(const BPValueForwardIterator &other) const { return this->compare(other) >= 0; }
        };
    }
}
