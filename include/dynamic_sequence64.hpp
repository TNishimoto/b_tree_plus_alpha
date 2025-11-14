/**
 * @file dynamic_sequence64.hpp
 * @brief A dynamic data structure that maintains a sequence of 64-bit non-negative integers.
 */

#pragma once
#include "./bp_tree.hpp"

namespace stool
{
    namespace bptree
    {

        /**
         * @brief A dynamic data structure that maintains a sequence of 64-bit non-negative integers S[0..n-1].
         * \ingroup SequenceClasses
         * \ingroup MainClasses
         */
        template <typename LEAF_CONTAINER = stool::NaiveFLCVector<>, uint64_t TREE_DEGREE = 62, uint64_t LEAF_CONTAINER_MAX_SIZE = 256>
        class DynamicSequence64
        {
        public:
            using NodePointer = bptree::BPNodePointer<LEAF_CONTAINER, uint64_t, TREE_DEGREE, false>;
            using Tree = bptree::BPTree<LEAF_CONTAINER, uint64_t, TREE_DEGREE, LEAF_CONTAINER_MAX_SIZE, false, false>;

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
            DynamicSequence64()
            {
                this->tree.initialize();
            }

            /**
             * @brief Default move constructor.
             */
            DynamicSequence64(DynamicSequence64 &&) noexcept = default;

            //@}

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
            DynamicSequence64 &operator=(const DynamicSequence64 &) = delete;
            /**
             * @brief Default move assignment operator.
             */
            DynamicSequence64 &operator=(DynamicSequence64 &&) noexcept = default;

            /**
             * @brief The alias for at query
             */
            uint64_t operator[](uint64_t n) const
            {
                return this->tree.at(n);
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
            ///   @name Main queries
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            /**
             * @brief Return the (i+1)-th element of \p S[i]
             * @note O(log n) time
             */
            uint64_t at(uint64_t pos) const
            {
                return this->tree.at(pos);
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
            void set_value(uint64_t i, uint64_t v)
            {
                uint64_t old_v = this->at(i);
                if (old_v < v)
                {
                    this->increment(i, v - old_v);
                }
                else if (old_v > v)
                {
                    this->decrement(i, old_v - v);
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
            void swap(DynamicSequence64 &item)
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
                assert(pos <= this->size());
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
                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "=DynamicSequence64: " + std::to_string(this->size_in_bytes()) + " bytes =");
                for (std::string &s : log1)
                {
                    r.push_back(s);
                }
                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "==");
                return r;
            }
            /**
             * @brief Print the statistics of this data structure
             */
            void print_info() const
            {
                auto vec = this->to_vector();
                std::cout << "================== SPSI ==================" << std::endl;
                stool::DebugPrinter::print_integers(vec, "values");

                std::cout << "================== SPSI[END] =============" << std::endl;

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
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Statistics(DynamicSequence64):" << std::endl;
                this->tree.print_statistics(message_paragraph + 1);
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[END]" << std::endl;
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
             * @brief Build a new DynamicSequence64 from a given sequence \p items
             */
            static DynamicSequence64 build(const std::vector<uint64_t> &items)
            {
                DynamicSequence64 r;
                r.tree.initialize();
                r.tree.build(items);
                return r;
            }
            /**
             * @brief Returns the DynamicSequence64 instance loaded from a byte vector \p data at the position \p pos
             */
            static DynamicSequence64 load_from_bytes(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                DynamicSequence64 r;
                Tree tree = Tree::load_from_bytes(data, pos);
                r.tree.swap(tree, false);
                return r;
            }

            /**
             * @brief Returns the DynamicPrefixSum instance loaded from a file stream \p ifs
             */
            static DynamicSequence64 load_from_file(std::ifstream &ifs)
            {
                DynamicSequence64 r;
                Tree tree = Tree::load_from_file(ifs);
                r.tree.swap(tree, false);
                return r;
            }

            /**
             * @brief Save the given instance \p item to a byte vector \p output at the position \p pos
             */
            static void store_to_bytes(DynamicSequence64 &item, std::vector<uint8_t> &output, uint64_t &pos)
            {
                Tree::store_to_bytes(item.tree, output, pos);
            }

            /**
             * @brief Save the given instance \p item to a file stream \p os
             */
            static void store_to_file(DynamicSequence64 &item, std::ofstream &os)
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
                s += "SPSI(";
                s += VLCDeque::name();
                s += ")";
                return s;
            }
            //@}
        };

        using SimpleDynamicSequence64 = DynamicSequence64<stool::NaiveFLCVector<>, 62, 256>;
    }
}
