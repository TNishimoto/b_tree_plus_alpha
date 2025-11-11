#pragma once
#include "./plain_spsi_container.hpp"
#include "stool/include/light_stool.hpp"

namespace stool
{
    namespace bptree
    {
        /// \defgroup PrefixSumClasses Classes for dynamic prefix sums
        /// @{
        /// @}

        /// \defgroup MainClasses Main dynamic data structures
        /// @{
        /// @}


        /**
         * @brief A dynamic data structure supporting prefix-sum query on a unsigned 64-bit integer sequence S[0..n-1]
         * \ingroup PrefixSumClasses
         * \ingroup MainClasses
         */
        template <typename LEAF_CONTAINER = VLCDeque, uint64_t TREE_DEGREE = bptree::DEFAULT_MAX_DEGREE_OF_INTERNAL_NODE, uint64_t LEAF_CONTAINER_MAX_SIZE = bptree::DEFAULT_MAX_COUNT_OF_VALUES_IN_LEAF>
        class DynamicPrefixSum
        {
        public:
            using NodePointer = bptree::BPNodePointer<LEAF_CONTAINER, uint64_t, TREE_DEGREE, true>;
            using Tree = bptree::BPTree<LEAF_CONTAINER, uint64_t, TREE_DEGREE, LEAF_CONTAINER_MAX_SIZE, false, true>;
            // static inline constexpr int DEFAULT_MAX_COUNT_OF_VALUES_IN_LEAF = 126;

            // static inline constexpr int DEFAULT_MAX_COUNT_OF_VALUES_IN_LEAF = 126;
            // static inline constexpr int DEFAULT_MAX_COUNT_OF_VALUES_IN_LEAF = 1024;

        private:
            Tree tree;

        public:
            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Constructors and Destructor
            ////////////////////////////////////////////////////////////////////////////////
            //@{


            /**
             * @brief Default constructor with |S| = 0
             */
            DynamicPrefixSum()
            {
                this->tree.initialize();
            }

            /**
             * @brief Default constructor with S = S_
             */
            DynamicPrefixSum(const std::vector<uint64_t> &S_)
            {
                this->tree.initialize();
                this->tree.build(S_);
                assert(this->size() == S_.size());
            }
            /**
             * @brief Default move constructor.
             */
            DynamicPrefixSum(DynamicPrefixSum &&) noexcept = default;

            //}@

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Iterators
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            /**
             * @brief Returns an iterator to the beginning of the sequence \p S.
             */
            typename Tree::ValueForwardIterator begin() const
            {
                return this->tree.get_value_forward_iterator_begin();
            }

            /**
             * @brief Returns an iterator to the end of the sequence \p S.
             */
            typename Tree::ValueForwardIterator end() const
            {
                return this->tree.get_value_forward_iterator_end();
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Operators
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Deleted copy assignment operator.
             */
            DynamicPrefixSum &operator=(const DynamicPrefixSum &) = delete;
            /**
             * @brief Default move assignment operator.
             */
            DynamicPrefixSum &operator=(DynamicPrefixSum &&) noexcept = default;
            /**
             * @brief The alias for at query
             */
            uint64_t operator[](uint64_t i) const
            {
                return this->tree.at(i);
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Lightweight functions for accessing to properties of this class
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            /**
             * @brief Get the internal tree storing the sequence \p S.
             */
            Tree &__get_tree()
            {
                return this->tree;
            }

            /**
             * @brief Return the maximum degree of internal nodes of the internal tree storing the sequence \p S.
             */
            uint64_t get_degree() const
            {
                return this->tree.get_max_degree_of_internal_node();
            }

            /**
             * @brief Return |S|
             */
            uint64_t size() const
            {
                return this->tree.size();
            }

            /**
             * @brief Returns the total memory usage in bytes
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
             * @brief Return \p S as a vector.
             */
            std::vector<uint64_t> to_vector() const
            {
                return this->tree.to_value_vector();
            }

            /**
             * @brief Return \p S as a vector of uint8_t.
             */
            std::vector<uint8_t> to_u8_vector() const
            {
                std::vector<uint8_t> r;
                r.resize(this->size(), 0);
                uint64_t x = 0;
                for (uint8_t c : *this)
                {
                    r[x++] = c;
                }
                return r;
            }

            /**
             * @brief Return \p S as a string.
             */
            std::string to_string() const
            {
                std::stringstream ss;
                auto vec = this->to_vector();
                ss << stool::ConverterToString::to_integer_string(vec);
                return ss.str();
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Main queries (Access, search, and psum operations)
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            /**
             * @brief Return the (i+1)-th element of \p S[i]
             * @note O(log n) time
             */
            uint64_t at(uint64_t i) const
            {
                return this->tree.at(i);
            }

            /**
             * @brief Return the sum of \p S[0..n-1].
             * @note O(1) time
             */
            uint64_t psum() const
            {
                return this->tree.psum();
            }

            /**
             * @brief Return the sum of the first (i+1) values of \p S[0..n-1].
             * @note O(log n) time
             */
            uint64_t psum(uint64_t i) const
            {
                return this->tree.psum(i);
            }

            /**
             * @brief Return the sum of \p S[i..j].
             * @note O(log n) time
             */
            uint64_t psum(uint64_t i, uint64_t j) const
            {
                if(i > 0){
                    return this->tree.psum(j) - this->tree.psum(i-1);
                }else{
                    return this->tree.psum(j);
                }
            }

            /**
             * @brief Returns the sum of integers in \p S[(n-1)-i..n-1]
             * @note \p O(\log n) time
             */
            uint64_t reverse_psum([[maybe_unused]] uint64_t i) const
            {
                uint64_t size = this->size();
                if (size == 0)
                {
                    return 0;
                }
                else if (i + 1 == size)
                {
                    return this->psum(size - i - 1, size - 1);
                }
                else
                {
                    return this->psum(size - i - 1, size - 1);
                }
            }

            /**
             * @brief Return the smallest i such that psum(i) >= x if such a position exists, otherwise returns -1
             */
            int64_t search(uint64_t x) const
            {
                return this->tree.search(x);
            }

            /**
             * @brief Return the largest i such that psum(i) <= x if such a position exists, otherwise returns -1
             * @note O(log n) time
             */
            int64_t predecessor_index(uint64_t x) const
            {
                int64_t size = this->size();

                if (size > 0)
                {
                    uint64_t fst_value = this->at(0);
                    if (x >= fst_value)
                    {
                        if (x > this->psum())
                        {
                            return size - 1;
                        }
                        else
                        {
                            int64_t idx = this->search(x);
                            if (idx >= size)
                            {
                                return size - 1;
                            }
                            else
                            {
                                uint64_t v = this->psum(idx);

                                assert(v >= x);
                                if (v > x)
                                {
                                    assert(idx - 1 < size);
                                    return idx - 1;
                                }
                                else
                                {
                                    assert(idx < size);
                                    return idx;
                                }
                            }
                        }
                    }
                    else
                    {
                        return -1;
                    }
                }
                else
                {
                    return -1;
                }
            }

            /**
             * @brief Return the smallest i such that psum(i) >= x if such a position exists, otherwise returns -1
             * @note O(log n) time
             */
            int64_t successor_index(uint64_t x) const
            {
                int64_t size = this->size();
                if (size > 0)
                {
                    uint64_t lst_value = this->psum();
                    if (x <= lst_value)
                    {
                        int64_t idx = this->search(x);
                        if (idx >= size)
                        {
                            return -1;
                        }
                        else
                        {
                            assert(this->psum(idx) >= x);
                            return idx;
                        }
                    }
                    else
                    {
                        return -1;
                    }
                }
                else
                {
                    return -1;
                }
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Update operations
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Set a given value \p v at a given position \p i in \p S
             * @note \p O(log n) time
             */
            void set_value(uint64_t i, uint64_t value)
            {
                uint64_t old_value = this->at(i);
                if (old_value > value)
                {
                    this->decrement(i, old_value - value);
                }
                else if (old_value < value)
                {
                    this->increment(i, value - old_value);
                }
            }

            /**
             * @brief Replaces the |Q| values \p S[i..i+|Q|-1] with the given values Q[0..|Q|-1]
             * @note O(|Q| log n) time
             */
            void set_values(uint64_t i, const std::vector<uint64_t> &values_Q)
            {
                assert(i < this->size());
                assert(i + values_Q.size() <= this->size());

                for (uint64_t j = 0; j < values_Q.size(); j++)
                {
                    this->set_value(i + j, values_Q[j]);
                }
            }

            /**
             * @brief Set the value \p S[i+delta] at a given position \p i in \p S
             * @note \p O(log n) time
             */
            void increment(uint64_t i, int64_t delta)
            {
                this->tree.increment(i, delta);
            }

            /**
             * @brief Set the value \p S[i-delta] at a given position \p i in \p S
             * @note \p O(log n) time
             */
            void decrement(uint64_t i, int64_t delta)
            {
                this->tree.increment(i, -delta);
            }

            /**
             * @brief Swap operation
             */
            void swap(DynamicPrefixSum &item)
            {
                this->tree.swap(item.tree);
            }

            /**
             * @brief Clear the elements in \p S.
             */
            void clear()
            {
                this->tree.clear();
            }

            /**
             * @brief Add a given integer to the end of \p S
             * @note O(log n) time
             */
            void push_back(uint64_t value)
            {
                this->tree.push_back(value);
            }

            /**
             * @brief Add a given sequence \p Q[0..k-1] to the end of \p S[0..n-1] (i.e., \p S = S[0..n-1]Q[0..k-1])
             * @note O(|Q| log n) time
             */
            void push_back_many(const std::vector<uint64_t> &items_Q)
            {
                this->tree.push_many(items_Q);
            }

            /**
             * @brief Alias for push_back_many
             */
            void push_many(const std::vector<uint64_t> &items)
            {
                this->tree.push_many(items);
            }

            /**
             * @brief Add a given value to the beginning of \p S
             * @note \p O(log n) time
             */
            void push_front(uint64_t value)
            {
                this->tree.push_front(value);
            }

            /**
             * @brief Remove the last element from \p S
             * @note \p O(log n) time
             */
            void pop_back()
            {
                this->tree.remove(this->size() - 1);
            }

            /**
             * @brief Remove the first element from \p S
             * @note \p O(log n) time
             */
            void pop_front()
            {
                this->tree.remove(0);
            }

            /**
             * @brief Insert a given integer \p value into \p S as the \p (pos+1)-th element
             * @note \p O(log n) time
             */
            void insert(uint64_t pos, uint64_t value)
            {
                this->tree.insert(pos, value, value);
            }

            /**
             * @brief Remove the element at the position \p pos from \p S and return it
             * @note \p O(log n) time
             */
            void remove(uint64_t pos)
            {
                this->tree.remove(pos);
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Print and verification functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            /**
             * @brief Return the memory usage information of this data structure as a vector of strings
             * @param message_paragraph The paragraph depth of message logs
             */
            std::vector<std::string> get_memory_usage_info(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::vector<std::string> log1 = this->tree.get_memory_usage_info(message_paragraph + 1);

                std::vector<std::string> r;
                uint64_t size_in_bytes = this->size_in_bytes();
                uint64_t size = this->size();
                double bits_per_element = size > 0 ? ((double)size_in_bytes / (double)size) : 0;

                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "=DynamicPrefixSum: " + std::to_string(this->size_in_bytes()) + " bytes, " + std::to_string(size) + " elements, " + std::to_string(bits_per_element) + " bytes per element =");

                for (std::string &s : log1)
                {
                    r.push_back(s);
                }

                uint64_t total_bits_if_sequence_is_plain = 0;
                for (uint64_t i = 0; i < size; ++i)
                {
                    total_bits_if_sequence_is_plain += stool::LSBByte::get_code_length(this->at(i));
                }

                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "The number of bits in input sequence: " + std::to_string(total_bits_if_sequence_is_plain));
                if (total_bits_if_sequence_is_plain > 0)
                {
                    r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "Memory per input bit: " + std::to_string((double)size_in_bytes / (double)total_bits_if_sequence_is_plain) + " bytes");
                }

                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "==");

                return r;
            }

            /**
             * @brief Print the memory usage information of this data structure
             * @param message_paragraph The paragraph depth of message logs (-1 for no output)
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
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Statistics(DynamicPrefixSum):" << std::endl;
                this->tree.print_statistics(message_paragraph + 1);
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[END]" << std::endl;
            }

            /**
             * @brief Print the performance information of this data structure
             * @param message_paragraph The paragraph depth of message logs (-1 for no output)
             */
            void print_information_about_performance(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Performance Information (DynamicPrefixSum)[" << std::endl;
                this->tree.print_information_about_performance(message_paragraph + 1);
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "]" << std::endl;
            }

            /**
             * @brief Print the form of the internal tree of this data structure
             * @param message_paragraph The paragraph depth of message logs (-1 for no output)
             */
            void print_tree(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {

                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Tree(DynamicPrefixSum)[" << std::endl;
                this->tree.print_tree();
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "]" << std::endl;
            }

            /**
             * @brief Print the statistics of this data structure
             */
            void print_info() const
            {
                this->print_statistics();
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
             * @brief Build a new DynamicPrefixSum from a given sequence \p items
             */
            static DynamicPrefixSum build(const std::vector<uint64_t> &items)
            {
                DynamicPrefixSum r;
                r.tree.initialize();
                r.tree.build(items);
                assert(r.size() == items.size());
                return r;
            }

            /**
             * @brief Returns the DynamicPrefixSum instance loaded from a byte vector \p data at the position \p pos
             */
            static DynamicPrefixSum load_from_bytes(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                DynamicPrefixSum r;
                Tree tree = Tree::load_from_bytes(data, pos);
                r.tree.swap(tree, false);
                return r;
            }

            /**
             * @brief Returns the DynamicPrefixSum instance loaded from a file stream \p ifs
             */
            static DynamicPrefixSum load_from_file(std::ifstream &ifs)
            {
                DynamicPrefixSum r;
                Tree tree = Tree::load_from_file(ifs);
                r.tree.swap(tree, false);
                return r;
            }

            /**
             * @brief Save the given instance \p item to a byte vector \p output at the position \p pos
             */
            static void store_to_bytes(DynamicPrefixSum &item, std::vector<uint8_t> &output, uint64_t &pos)
            {
                Tree::store_to_bytes(item.tree, output, pos);
            }

            /**
             * @brief Save the given instance \p item to a file stream \p os
             */
            static void store_to_file(DynamicPrefixSum &item, std::ofstream &os)
            {
                Tree::store_to_file(item.tree, os);
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Other static functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Return the name of the DynamicPrefixSum for debugging
             */
            static std::string name()
            {
                std::string s;
                s += "DynamicPrefixSum(";
                s += LEAF_CONTAINER::name();
                s += ")";
                return s;
            }
            //@}
        };

        using SimpleDynamicPrefixSum = DynamicPrefixSum<stool::NaiveFLCVector<>, 62, 256>;
        using PlainDynamicPrefixSum = DynamicPrefixSum<PlainSPSIContainer>;
        using VLCDequeDynamicPrefixSum = DynamicPrefixSum<VLCDeque>;
        // using DynamicSuccinctPrefixSum = DynamicPrefixSum<stool::NaiveVLCArray<4096>, 62, 128>;
        // using EFDynamicPrefixSum = DynamicPrefixSum<stool::NaiveFLCVector<>, 62, 256>;

        // using GapEncodedSPSI = SPSI<GapEncodedContainer>;

    }

}