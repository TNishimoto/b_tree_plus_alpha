#pragma once
#include "../bp_tree/bp_tree.hpp"
#include "./bit_container.hpp"

namespace stool
{
    namespace bptree
    {

        /**
         * @brief A forward iterator for traversing the bits stored in a BP-tree. [Unchecked AI's Comment]
         * \ingroup BitClasses
         */
        template <typename CONTAINER, typename CONTAINER_ITERATOR, uint64_t MAX_TREE_DEGREE, uint64_t MAX_BIT_CONTAINER_SIZE>
        class BitForwardIterator
        {
            using NodePointer = bptree::BPNodePointer<CONTAINER, bool, MAX_TREE_DEGREE, true>;
            using T = uint64_t;
            using Tree = bptree::BPTree<CONTAINER, bool, MAX_TREE_DEGREE, MAX_BIT_CONTAINER_SIZE, false, true>;
            using LeafForwardIterator = typename Tree::LeafForwardIterator;

        public:
            LeafForwardIterator _lf_iterator;
            Tree *tree = nullptr;
            uint64_t bits;
            CONTAINER_ITERATOR container_iterator;
            bool container_iterator_end_flag = true;
            // uint64_t bits;
            // int16_t index_in_container = 0;

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

                    if (this->_lf_iterator.is_end())
                    {
                        this->container_iterator_end_flag = true;
                    }
                    else
                    {
                        uint64_t leaf_index = *this->_lf_iterator;
                        CONTAINER &container = this->tree->get_leaf_container(leaf_index);
                        this->container_iterator_end_flag = false;
                        this->container_iterator = container.begin();
                        ++(*this);
                    }
                }
                else
                {
                    this->_lf_iterator.idx = UINT64_MAX;
                }
            }

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Public const Methods
            ///   The public const methods of this class.
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            bool is_end() const
            {
                return this->_lf_iterator.is_end() && this->container_iterator_end_flag;
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
                    if (this->container_iterator != other.container_iterator)
                    {
                        if (this->container_iterator < other.container_iterator)
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
                        bool b1 = this->container_iterator_end_flag;
                        bool b2 = other.container_iterator_end_flag;
                        if(b1 != b2){
                            if(b1){
                                return -1;
                            }else{
                                return 1;
                            }
                        }else{
                            return 0;
                        }
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
                int64_t current_bits = 0;
                int64_t current_bit_size = 0;


                if (this->is_end())
                {
                    throw std::invalid_argument("Error: BitForwardIterator::operator++()");
                }


                if (this->_lf_iterator.is_end() && !this->container_iterator_end_flag)
                {
                    this->container_iterator_end_flag = true;
                    this->bits = 0;
                    return *this;
                }
                else
                {
                    while (current_bit_size < 64 && !this->is_end())
                    {
                        if (!this->container_iterator_end_flag)
                        {

                            uint64_t xbits_size = std::min((uint64_t)this->container_iterator.get_size() - this->container_iterator.index, 64ULL);

                            uint64_t xbits = (this->container_iterator.read_64bits_string() >> (64 - xbits_size)) << (64 - xbits_size);
                            assert(xbits_size >=1);

                            assert(xbits_size <= 64);
                            current_bits = current_bits | (xbits >> current_bit_size);

                            if (current_bit_size + xbits_size >= 64)
                            {
                                uint64_t read_bit_size = 64 - current_bit_size;
                                this->container_iterator += read_bit_size;
                                current_bit_size += read_bit_size;
                            }
                            else
                            {
                                this->container_iterator += xbits_size;
                                current_bit_size += xbits_size;
                            }

                            if (this->container_iterator.is_end())
                            {
                                this->container_iterator_end_flag = true;
                            }
                        }
                        else if (!this->_lf_iterator.is_end())
                        {

                            ++this->_lf_iterator;

                            this->container_iterator_end_flag = false;
                            if (!this->_lf_iterator.is_end())
                            {
                                uint64_t leaf_index = *this->_lf_iterator;
                                CONTAINER &container = this->tree->get_leaf_container(leaf_index);
                                this->container_iterator = container.begin();

                            }
                            else
                            {

                                break;
                            }
                        }
                        else
                        {
                            throw std::invalid_argument("Error: BitForwardIterator::operator++()");
                        }
                    }
                    this->bits = current_bits;
                    return *this;
                }
            }
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