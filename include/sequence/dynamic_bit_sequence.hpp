/**
 * @file dynamic_bit_sequence.hpp
 * @brief A dynamic data structure supporting rank and select queries on a bit sequence.
 */

#pragma once
#include "../bp_tree/bp_tree.hpp"
#include "./bit_container.hpp"

#include "./bit_forward_iterator.hpp"
#include "./bit_deque_container.hpp"
#include "stool/include/light_stool.hpp"
namespace stool
{
    namespace bptree
    {

        /**
         * @brief A dynamic data structure supporting rank and select queries on a bit sequence B[0..n-1]
         * \ingroup BitClasses
         * \ingroup MainClasses
         */
        template <typename CONTAINER, typename CONTAINER_ITERATOR, uint64_t MAX_TREE_DEGREE, uint64_t MAX_BIT_CONTAINER_SIZE>
        class DynamicBitSequence
        {

            using NodePointer = bptree::BPNodePointer<CONTAINER, bool, MAX_TREE_DEGREE, true>;
            using T = uint64_t;
            using Tree = bptree::BPTree<CONTAINER, bool, MAX_TREE_DEGREE, MAX_BIT_CONTAINER_SIZE, false, true>;

            static inline constexpr int DEFAULT_CONTAINER_DEGREE = 62;
            // static inline constexpr int DEFAULT_CONTAINER_DEGREE = 124;
            Tree tree;

        public:
            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Constructors and Destructor
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            /**
             * @brief Default constructor with |B| = 0
             */
            DynamicBitSequence()
            {
            }

            /**
             * @brief Deleted copy constructor.
             */
            DynamicBitSequence(const DynamicBitSequence &) = delete;
            /**
             * @brief Default move constructor.
             */
            DynamicBitSequence(DynamicBitSequence &&) noexcept = default;

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Iterators
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Returns an iterator to the beginning of the bit sequence \p B.
             */
            BitForwardIterator<CONTAINER, CONTAINER_ITERATOR, MAX_TREE_DEGREE, MAX_BIT_CONTAINER_SIZE> get_bit_forward_iterator_begin() const
            {
                auto leaf_it = this->tree.get_leaf_forward_iterator_begin();
                return BitForwardIterator<CONTAINER, CONTAINER_ITERATOR, MAX_TREE_DEGREE, MAX_BIT_CONTAINER_SIZE>(&leaf_it, &this->tree);
            }

            /**
             * @brief Returns an iterator to the end of the bit sequence \p B.
             */
            BitForwardIterator<CONTAINER, CONTAINER_ITERATOR, MAX_TREE_DEGREE, MAX_BIT_CONTAINER_SIZE> get_leaf_forward_iterator_end() const
            {
                return BitForwardIterator<CONTAINER, CONTAINER_ITERATOR, MAX_TREE_DEGREE, MAX_BIT_CONTAINER_SIZE>(nullptr, &this->tree);
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Operators
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Deleted copy assignment operator.
             */
            DynamicBitSequence &operator=(const DynamicBitSequence &) = delete;

            /**
             * @brief Default move assignment operator.
             */
            DynamicBitSequence &operator=(DynamicBitSequence &&) noexcept = default;

            /**
             * @brief The alias for at query
             */
            bool operator[](uint64_t i) const
            {
                return this->at(i);
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Lightweight functions for accessing to properties of this class
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Checks if \p B is empty.
             * @return bool True if the sequence is empty, false otherwise.
             */
            bool empty() const
            {
                return this->tree.empty();
            }

            /**
             * @brief Return \p |B|
             */
            uint64_t size() const
            {
                return this->tree.size();
            }

            /**
             * @brief Return the height of the internal tree storing the bit sequence \p B.
             */
            uint64_t height() const
            {
                return this->tree.height();
            }
            /**
             * @brief Return the total memory usage in bytes
             * @param only_dynamic_memory If true, only the size of the dynamic memory is returned
             */
            uint64_t size_in_bytes(bool only_dynamic_memory = false) const
            {
                return this->tree.size_in_bytes(only_dynamic_memory);
            }
            /**
             * @brief Return the density of the internal tree
             */
            double density() const
            {
                return this->tree.get_value_density();
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Conversion functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Return \p B as a packed vector of uint64_t (i.e., the length of the vector is |B| / 64).
             */
            std::vector<uint64_t> to_packed_vector() const
            {
                auto vec = this->tree.to_value_vector();
                std::vector<uint64_t> r;
                r.resize(vec.size());
                for (uint64_t i = 0; i < vec.size(); i++)
                {
                    r[i] = vec[i];
                }
                return r;
            }

            /**
             * @brief Return \p B as a binary string
             */
            std::string to_string() const
            {
                std::string s;
                std::vector<uint64_t> bits = this->to_packed_vector();
                s.push_back('[');
                for (uint64_t i = 0; i < bits.size(); i++)
                {
                    s.push_back(bits[i] >= 1 ? '1' : '0');
                    // assert(bits[i] == this->at(i));
                }
                s.push_back(']');
                return s;
            }

            /**
             * @brief Return \p B as a vector of bool.
             */
            std::vector<bool> to_vector() const
            {
                uint64_t _size = this->size();
                std::vector<bool> r;
                r.resize(_size, false);
                uint64_t counter = 0;

                auto _end = this->get_leaf_forward_iterator_end();

                BitForwardIterator<CONTAINER, CONTAINER_ITERATOR, MAX_TREE_DEGREE, MAX_BIT_CONTAINER_SIZE> it = this->get_bit_forward_iterator_begin();
                while (!it.is_end())
                {
                    uint64_t bits = *it;
                    uint64_t i = 0;

                    while (i < 64 && counter < _size)
                    {
                        bool b = stool::MSBByte::get_bit(bits, i);
                        r[counter] = b;

                        counter++;
                        i++;
                    }
                    ++it;
                }

                assert(counter == _size);
                return r;
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Main queries
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            /**
             * @brief Return \p B[i]
             * @note O(log n) time
             */
            bool at(uint64_t i) const
            {
                return this->tree.at(i) & 1;
            }

            /**
             * @brief Returns the number of 1 in \p B[0..i-1].
             * @note O(log n) time
             */
            int64_t one_based_rank1(uint64_t i) const
            {
                if (i == 0)
                {
                    return 0;
                }
                else if (i <= this->size())
                {
                    uint64_t p = this->tree.psum(i - 1);

                    return p;
                }
                else
                {
                    assert(false);
                    throw std::range_error("Error: DynamicBitSequence::rank1()");
                }
            }

            /**
             * @brief Returns the number of 0 in \p B[0..i-1].
             * @note O(log n) time
             */
            int64_t one_based_rank0(uint64_t i) const
            {
                return i - this->one_based_rank1(i);
            }

            /**
             * @brief Returns the number of \p c in \p B[0..i-1].
             * @note O(log n) time
             */
            int64_t one_based_rank(uint64_t i, bool c) const
            {
                return c ? this->one_based_rank1(i) : this->one_based_rank0(i);
            }

            /**
             * @brief Returns the position \p p of the (i+1)-th 1 in \p B if such a position exists, otherwise returns -1
             * @note O(log n) time
             */
            int64_t select1(uint64_t i) const
            {
                int64_t p = this->tree.search(i + 1);
                if (p == -1)
                {
                    return p;
                }
                else
                {
                    if (p >= (int64_t)this->size())
                    {
                        return -1;
                    }
                    else
                    {
                        return p;
                    }
                }
            }

            /**
             * @brief Returns the position \p p of the (i+1)-th 0 in \p B if such a position exists, otherwise returns -1
             * @note O(log n) time
             */
            int64_t select0(uint64_t i) const
            {
                int64_t p = this->tree.select0(i);
                if (p == -1)
                {
                    return p;
                }
                else
                {
                    if (p >= (int64_t)this->size())
                    {
                        return -1;
                    }
                    else
                    {
                        return p;
                    }
                }
            }

            /**
             * @brief Returns the position \p p of the (i+1)-th \p c in \p B if such a position exists, otherwise returns -1
             * @note O(log n) time
             */
            int64_t select(uint64_t i, bool c) const
            {
                return c ? this->select1(i) : this->select0(i);
            }

            /**
             * @brief Return the number of 1 in \p B[0..n-1]
             * @note O(1) time
             */
            int64_t count1() const
            {
                uint64_t _size = this->size();
                if (_size > 0)
                {
                    return this->one_based_rank1(_size);
                }
                else
                {
                    return 0;
                }
            }

            /**
             * @brief Return the number of 0 in \p B[0..n-1]
             * @note O(1) time
             */
            int64_t count0() const
            {
                uint64_t _size = this->size();
                if (_size > 0)
                {
                    return this->one_based_rank0(_size);
                }
                else
                {
                    return 0;
                }
            }

            /**
             * @brief Return the number of \p c in \p B[0..n-1]
             * @note O(1) time
             */
            int64_t count_c(bool c) const
            {
                uint64_t _size = this->size();
                if (_size > 0)
                {
                    return this->one_based_rank(_size, c);
                }
                else
                {
                    return 0;
                }
            }

            /**
             * @brief Returns the number of 1s in the bit sequence \p B[0..i]
             * @note O(log n) time
             */
            uint64_t psum(uint64_t i) const
            {
                return this->tree.psum(i);
            }

            /**
             * @brief Returns the number of 1s in the bit sequence \p B[0..n-1]
             * @note \p O(1) time
             */
            uint64_t psum() const
            {
                return this->tree.psum();
            }

            /**
             * @brief Returns the first position \p p such that psum(p) >= x if such a position exists, otherwise returns -1
             * @note O(log n) time
             */
            int64_t search(uint64_t x) const
            {
                return this->tree.search(x);
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Update operations
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            /**
             * @brief Swap operation
             */
            void swap(DynamicBitSequence &item)
            {
                this->tree.swap(item.tree);
            }

            /**
             * @brief Clear the elements in \p B.
             */
            void clear()
            {
                this->tree.clear();
            }

            /**
             * @brief Adds a bit to the end of the sequence \p B.
             * @note O(log n) time
             */
            void push_back(bool value)
            {
#if DEBUG
                uint64_t p = this->size();
#endif
                this->tree.push_back(value);
#if DEBUG
                assert(p + 1 == this->size());
#endif
            }

            /**
             * @brief Add a given sequence \p Q[0..k-1] to the end of \p S[0..n-1] (i.e., \p S = S[0..n-1]Q[0..k-1])
             * @note O(|Q| log n) time
             */
            void push_many(const std::vector<bool> &items_Q)
            {
                this->tree.push_many(items_Q);
            }

            /**
             * @brief Adds a bit to the beginning of the sequence \p B.
             * @note O(log n) time
             */
            void push_front(bool value)
            {
                this->tree.push_front(value);
            }

            /**
             * @brief Insert a bit \p v at the position \p p in the bits \p B
             * @note O(log n) time
             */
            void insert(uint64_t p, bool v)
            {
                this->tree.insert(p, v, v);
            }

            /**
             * @brief Removes the bit at the position \p p in the bits \p B.
             * @note O(log n) time
             */
            void remove(uint64_t p)
            {
                if (p < this->size())
                {
                    this->tree.remove(p);
                }
                else
                {
                    throw std::range_error("Error: DynamicBitSequence::remove(p)");
                }
            }

            /**
             * @brief Replace the bit \p B[i] with the bit \p b
             * @note O(log n) time
             */
            void set_bit(uint64_t i, bool b)
            {
                bool b1 = this->at(i);
                if (b != b1)
                {
                    if (b)
                    {
                        this->tree.increment(i, 1);
                    }
                    else
                    {
                        this->tree.increment(i, -1);
                    }
                }
            }

            /**
             * @brief Replace the bits \p B[i..i+m-1] with the bits \p bits[0..m-1]
             * @note O(m log n) time
             */
            void set_bits(uint64_t i, std::vector<bool> &bits)
            {
                for (uint64_t j = 0; j < bits.size(); j++)
                {
                    this->set_bit(i + j, bits[j]);
                }
            }

            /**
             * @brief Sorts the leaf containers of the internal tree
             * @note O(n) time
             */
            void sort_leaf_containers()
            {
                bool b = this->tree.check_if_leaf_container_vec_is_sorted();
                if (!b)
                {
                    std::cout << "unsorted" << std::endl;
                    this->tree.sort_leaf_containers();
                }
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Print and verification functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            /**
             * @brief Print debug information about this instance
             */
            void print_debug_info() const
            {
                std::cout << "DynamicBitSequence::print_debug_info()" << std::endl;
                this->tree.print_debug_info();
            }

            /**
             * @brief Return the memory usage information of this data structure as a vector of strings
             * @param message_paragraph The paragraph depth of message logs (-1 for no output)
             */
            std::vector<std::string> get_memory_usage_info(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::vector<std::string> log1 = this->tree.get_memory_usage_info(message_paragraph + 1);

                std::vector<std::string> r;
                uint64_t size_in_bytes = this->size_in_bytes();
                uint64_t size = this->size();
                double bits_per_element = size > 0 ? ((double)size_in_bytes / (double)size) : 0;

                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "=DynamicBitSequence: " + std::to_string(this->size_in_bytes()) + " bytes, " + std::to_string(size) + " elements, " + std::to_string(bits_per_element) + " bytes per element =");
                for (std::string &s : log1)
                {
                    r.push_back(s);
                }
                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "==");
                return r;
            }

            /**
             * @brief Print the memory usage information of this data structure
             * @param message_paragraph The paragraph depth of message logs
             */
            void print_memory_usage(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::vector<std::string> log = this->get_memory_usage_info(message_paragraph);
                for (std::string &s : log)
                {
                    std::cout << s << std::endl;
                }
            }

            /**
             * @brief Print the statistics of this data structure
             * @param message_paragraph The paragraph depth of message logs
             */
            void print_statistics(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Statistics(DynamicBitSequence):" << std::endl;
                this->tree.print_statistics(message_paragraph + 1);
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[END]" << std::endl;
            }

            /**
             * @brief Print the performance information of this data structure
             * @param message_paragraph The paragraph depth of message logs
             */
            void print_information_about_performance(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Performance Information (DynamicSequence)[" << std::endl;
                this->tree.print_information_about_performance(message_paragraph + 1);
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "]" << std::endl;
            }

            /**
             * @brief Print the bit sequence \p B.
             * @param name The name to print.
             * @param message_paragraph The paragraph depth of message logs
             */
            void print_content(std::string name = "DynamicBitSequence", int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << name << ": " << this->to_string() << std::endl;
            }

            /**
             * @brief Verify the internal consistency of this data structure.
             */
            void verify() const
            {
                this->tree.verify();
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Load, save, and builder functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{


            /**
             * @brief Build a new DynamicBitSequence from a given sequence \p items
             */
            static DynamicBitSequence build(const std::vector<bool> &items)
            {
                DynamicBitSequence r;
                r.tree.initialize();
                r.tree.build(items);
                return r;
            }

            /**
             * @brief Returns the DynamicBitSequence instance loaded from a byte vector \p data at the position \p pos
             */
            static DynamicBitSequence load_from_bytes(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                DynamicBitSequence r;
                Tree tree = Tree::load_from_bytes(data, pos);
                r.tree.swap(tree, false);
                return r;
            }

            /**
             * @brief Returns the DynamicBitSequence instance loaded from a file stream \p ifs
             */
            static DynamicBitSequence load_from_file(std::ifstream &ifs)
            {
                DynamicBitSequence r;
                Tree tree = Tree::load_from_file(ifs);
                r.tree.swap(tree, false);
                return r;
            }


            /**
             * @brief Save the given instance \p item to a byte vector \p output at the position \p pos
             */
            static void store_to_bytes(DynamicBitSequence &item, std::vector<uint8_t> &output, uint64_t &pos)
            {
                Tree::store_to_bytes(item.tree, output, pos);
            }

            /**
             * @brief Save the given instance \p item to a file stream \p os
             */
            static void store_to_file(DynamicBitSequence &item, std::ofstream &os)
            {
                Tree::store_to_file(item.tree, os);
            }


            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Other static functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Return the name of the DynamicBitSequence for debugging
             */
            static std::string name()
            {
                std::string s;
                s += "DynamicSequence(";
                s += CONTAINER::name();
                s += ")";
                return s;
            }
            //@}
        };

        using BDC = typename stool::bptree::BitVectorContainer<10000ULL>;

        using DynamicBitDequeSequence = DynamicBitSequence<BDC, BDC::BitVectorContainerIterator, bptree::DEFAULT_MAX_DEGREE_OF_INTERNAL_NODE, 1024>;
        // using SimpleDynamicBitSequence = DynamicBitSequence<BDC, BDC::BitVectorContainerIterator, 62, 8192>;
        using SimpleDynamicBitSequence = DynamicBitSequence<BDC, BDC::BitVectorContainerIterator, 62, 8192>;

        /*
        using DynamicBitDequeSequenceA = DynamicBitSequence<BDC, BDC::BitVectorContainerIterator, 62, 512>;
        using DynamicBitDequeSequenceB = DynamicBitSequence<BDC, BDC::BitVectorContainerIterator, 62, 1024>;
        using DynamicBitDequeSequenceC = DynamicBitSequence<BDC, BDC::BitVectorContainerIterator, 62, 2048>;
        using DynamicBitDequeSequenceD = DynamicBitSequence<BDC, BDC::BitVectorContainerIterator, 62, 4096>;

        using DynamicBitDequeSequence1 = DynamicBitSequence<BDC, BDC::BitVectorContainerIterator, 14, 8192>;
        using DynamicBitDequeSequence2 = DynamicBitSequence<BDC, BDC::BitVectorContainerIterator, 30, 8192>;
        using DynamicBitDequeSequence3 = DynamicBitSequence<BDC, BDC::BitVectorContainerIterator, 62, 8192>;
        using DynamicBitDequeSequence4 = DynamicBitSequence<BDC, BDC::BitVectorContainerIterator, 126, 8192>;
        */

        // using DynamicBitDequeSequenceE = DynamicBitSequence<stool::bptree::BitVectorContainer, stool::bptree::BitVectorContainer::BitVectorContainerIterator, 8190, 2048>;

        // template <typename T>
        // using VLCDequeSeq = DynamicSequence<VLCDeque, T>;
    }

}
