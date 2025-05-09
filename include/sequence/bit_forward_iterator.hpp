#pragma once
#include "../bp_tree/bp_tree.hpp"
#include "./bit_container.hpp"

namespace stool
{
    namespace bptree
    {

        ////////////////////////////////////////////////////////////////////////////////
        /// @brief A forward iterator for traversing the bits stored in a BP-tree.
        ///
        ////////////////////////////////////////////////////////////////////////////////
        class BitForwardIterator
        {
            using NodePointer = bptree::BPNodePointer<BitContainer, bool>;
            using T = uint64_t;
            using Tree = bptree::BPTree<BitContainer, bool, false, true>;
            using LeafForwardIterator = Tree::LeafForwardIterator;

        public:
            LeafForwardIterator _lf_iterator;
            Tree *tree = nullptr;
            uint64_t bits;
            int16_t index_in_container = 0;

            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;

            BitForwardIterator()
            {
            }

            BitForwardIterator(LeafForwardIterator *_root, const Tree *_tree)
            {
                this->tree = const_cast<Tree *>(_tree);
                if (_root != nullptr)
                {
                    _root->copy_to(this->_lf_iterator);
                    ++(*this);
                }
                else
                {
                    this->_lf_iterator.idx = UINT64_MAX;
                    this->index_in_container = 0;
                }
            }

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Public const Methods
            ///   The public const methods of this class.
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            bool is_end() const
            {
                return this->_lf_iterator.is_end() && this->index_in_container == 0;
            }
            int compare(const BitForwardIterator &other) const
            {
                if (this->_lf_iterator.idx != other._lf_iterator.idx)
                {
                    if (this->_lf_iterator.idx < other._lf_iterator.idx)
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
                    if (this->index_in_container != other.index_in_container)
                    {
                        if (this->index_in_container < other.index_in_container)
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

            //@}
            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Operators
            ///   The operators of this class
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            uint64_t operator*() const
            {
                return this->bits;
            }

            BitForwardIterator &operator++()
            {
                uint64_t current_bits = 0;
                uint64_t current_bit_size = 0;
                while (current_bit_size < 64 && !this->_lf_iterator.is_end())
                {
                    uint64_t leaf_index = *this->_lf_iterator;
                    BitContainer &container = this->tree->get_leaf_container(leaf_index);
                    uint64_t container_size = container.size();


                    if ((int64_t)this->index_in_container < (int64_t)container_size)
                    {
                        uint64_t _bits = (container.to_uint64() >> this->index_in_container);
                        uint64_t _bits_size = container.size() - this->index_in_container;
                        uint64_t _bits_size2 = current_bit_size + _bits_size <= 64 ? _bits_size : (64 - current_bit_size);

                        current_bits = current_bits | (_bits << current_bit_size);
                        current_bit_size += _bits_size2;

                        this->index_in_container += _bits_size2;
                    }
                    else
                    {
                        this->index_in_container = 0;
                        ++this->_lf_iterator;
                        /*
                        if(!this->_lf_iterator.is_end()){
                            uint64_t _leaf_index = *this->_lf_iterator;
                            BitContainer &_container = this->tree->get_leaf_container(_leaf_index);                            
                        }
                        */
                    }
                }
                if (current_bit_size == 0)
                {
                    this->bits = 0;
                        this->index_in_container = 0;

                }
                else
                {
                    this->bits = current_bits;
                    if(this->_lf_iterator.is_end()){
                        this->index_in_container = -1;
                    }
                }
                                return *this;
            }
            /*
            bool operator==(const BitForwardIterator &other) const { return this->idx == other.idx; }
            bool operator!=(const BitForwardIterator &other) const { return this->idx != other.idx; }
            bool operator<(const BitForwardIterator &other) const { return this->idx < other.idx; }
            bool operator>(const BitForwardIterator &other) const { return this->idx > other.idx; }
            bool operator<=(const BitForwardIterator &other) const { return this->idx <= other.idx; }
            bool operator>=(const BitForwardIterator &other) const { return this->idx >= other.idx; }
            */

            bool operator==(const BitForwardIterator &other) const { return this->compare(other) == 0; }
            bool operator!=(const BitForwardIterator &other) const { return this->compare(other) != 0; }
            bool operator<(const BitForwardIterator &other) const { return this->compare(other) < 0; }
            bool operator>(const BitForwardIterator &other) const { return this->compare(other) > 0; }
            bool operator<=(const BitForwardIterator &other) const { return this->compare(other) <= 0; }
            bool operator>=(const BitForwardIterator &other) const { return this->compare(other) >= 0; }

            //@}
        };
    }
}