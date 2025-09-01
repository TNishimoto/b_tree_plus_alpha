/**
 * @file dynamic_sequence64.hpp
 * @brief A dynamic data structure that maintains a sequence of 64-bit non-negative integers.
 */

#pragma once
#include "../bp_tree/bp_tree.hpp"

namespace stool
{
    namespace bptree
    {
        /**
         * @class DynamicSequence64
         * @brief A dynamic data structure that maintains a sequence of 64-bit non-negative integers.
         *
         * This class provides a dynamic sequence of 64-bit non-negative integers with operations for insertion, deletion, and modification.
         */
        template <typename LEAF_CONTAINER = stool::NaiveFLCVector<>, uint64_t TREE_DEGREE = 62, uint64_t LEAF_CONTAINER_MAX_SIZE = 256>
        class DynamicSequence64
        {
        public:
            using NodePointer = bptree::BPNodePointer<LEAF_CONTAINER, uint64_t, TREE_DEGREE>;
            using Tree = bptree::BPTree<LEAF_CONTAINER, uint64_t, false, false, TREE_DEGREE, LEAF_CONTAINER_MAX_SIZE>;

        private:
            Tree tree;

        public:
            /**
             * @brief Returns an iterator to the beginning of the sequence.
             * @return An iterator to the first element of the sequence.
             */
            typename Tree::ValueForwardIterator begin() const
            {
                return this->tree.get_value_forward_iterator_begin();
            }
            /**
             * @brief Returns an iterator to the end of the sequence.
             * @return An iterator to the element following the last element of the sequence.
             */
            typename Tree::ValueForwardIterator end() const
            {
                return this->tree.get_value_forward_iterator_end();
            }

            /**
             * @brief Default constructor initializes the tree with a default degree.
             */
            DynamicSequence64()
            {
                this->tree.initialize();
            }
            /**
             * @brief Swaps the contents of this sequence with another.
             * @param item The sequence to swap with.
             */
            void swap(DynamicSequence64 &item)
            {
                this->tree.swap(item.tree);
            }

            DynamicSequence64 &operator=(const DynamicSequence64 &) = delete;
            DynamicSequence64(DynamicSequence64 &&) noexcept = default;
            DynamicSequence64 &operator=(DynamicSequence64 &&) noexcept = default;

        public:
            /**
             * @brief Clears all elements from the sequence.
             */
            void clear()
            {
                this->tree.clear();
            }
            /**
             * @brief Verifies the integrity of the sequence.
             */
            void verify() const
            {
                this->tree.verify();
            }
            /**
             * @brief Returns a reference to the underlying tree.
             * @return A reference to the tree.
             */
            Tree &get_tree()
            {
                return this->tree;
            }
            /**
             * @brief Returns the degree of the tree.
             * @return The degree of the tree.
             */
            uint64_t get_degree() const
            {
                return this->tree.get_max_degree_of_internal_node();
            }

            /**
             * @brief Builds a new sequence from a vector of items.
             * @param items The vector of items to build the sequence from.
             * @param tree_degree The degree of the tree.
             * @return A new DynamicSequence64 instance.
             */
            static DynamicSequence64 build(const std::vector<uint64_t> &items)
            {
                DynamicSequence64 r;
                r.tree.initialize();
                r.tree.build(items);
                return r;
            }

            /**
             * @brief Returns the number of elements in the sequence.
             * @return The size of the sequence.
             */
            uint64_t size() const
            {
                return this->tree.size();
            }
            /**
             * @brief Adds an element to the end of the sequence.
             * @param value The value to add.
             */
            void push_back(int64_t value)
            {
                this->tree.push_back(value);
            }
            /**
             * @brief Adds an element to the beginning of the sequence.
             * @param value The value to add.
             */
            void push_front(int64_t value)
            {
                this->tree.push_front(value);
            }

            /**
             * @brief Inserts a value at a specified position in the sequence.
             * @param pos The position to insert the value at.
             * @param value The value to insert.
             */
            void insert(uint64_t pos, uint64_t value)
            {
                this->tree.insert(pos, value, value);
            }
            /**
             * @brief Removes the element at a specified position from the sequence.
             * @param pos The position of the element to remove.
             */
            void remove(uint64_t pos)
            {
                this->tree.remove(pos);
            }
            /**
             * @brief Converts the sequence to a vector.
             * @return A vector containing the elements of the sequence.
             */
            std::vector<uint64_t> to_vector() const
            {
                return this->tree.to_value_vector();
            }

            /**
             * @brief Returns the value at a specified position in the sequence.
             * @param pos The position to get the value from.
             * @return The value at the specified position.
             */
            uint64_t at(uint64_t pos) const
            {
                return this->tree.at(pos);
            }
            /**
             * @brief Sets the value at a specified position in the sequence.
             * @param pos The position to set the value at.
             * @param value The new value to set.
             */
            void set_value(uint64_t pos, uint64_t value)
            {
                uint64_t v = this->at(pos);
                if (v < value)
                {
                    this->increment(pos, value - v);
                }
                else if (v > value)
                {
                    this->decrement(pos, v - value);
                }
            }
            /**
             * @brief Increments the value at a specified position by a given delta.
             * @param i The position to increment.
             * @param delta The amount to increment by.
             */
            void increment(uint64_t i, int64_t delta)
            {
                this->tree.increment(i, delta);
            }
            /**
             * @brief Decrements the value at a specified position by a given delta.
             * @param i The position to decrement.
             * @param delta The amount to decrement by.
             */
            void decrement(uint64_t i, int64_t delta)
            {
                this->tree.increment(i, -delta);
            }

            /**
             * @brief Returns the name of the sequence.
             * @return A string representing the name of the sequence.
             */
            static std::string name()
            {
                std::string s;
                s += "SPSI(";
                s += VLCDeque::name();
                s += ")";
                return s;
            }
            void push_front(uint64_t value)
            {
                this->tree.push_front(value);
            }
            void pop_back()
            {
                this->tree.remove(this->size() - 1);
            }
            void pop_front()
            {
                this->tree.remove(0);
            }


            /**
             * @brief Adds multiple elements to the end of the sequence.
             * @param items The vector of items to add.
             */
            void push_many(const std::vector<uint64_t> &items)
            {
                this->tree.push_many(items);
            }
            /**
             * @brief Returns the size of the sequence in bytes.
             * @return The size of the sequence in bytes.
             */
            uint64_t size_in_bytes() const
            {
                return this->tree.size_in_bytes();
            }
            /**
             * @brief Returns information about the memory usage of the sequence.
             * @param message_paragraph The paragraph level for the message.
             * @return A vector of strings containing memory usage information.
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
            void print_info() const {
                this->print_statistics();
            }
            /**
             * @brief Prints the memory usage of the sequence.
             * @param message_paragraph The paragraph level for the message.
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
             * @brief Prints statistics about the sequence.
             * @param message_paragraph The paragraph level for the message.
             */
            void print_statistics(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Statistics(DynamicSequence64):" << std::endl;
                this->tree.print_statistics(message_paragraph + 1);
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[END]" << std::endl;
            }

            /**
             * @brief Saves the sequence to a vector of bytes.
             * @param item The sequence to save.
             * @param output The vector to save the sequence to.
             * @param pos The position in the vector to start saving at.
             */
            static void save(DynamicSequence64 &item, std::vector<uint8_t> &output, uint64_t &pos)
            {
                item.tree.save(output, pos);
            }
            /**
             * @brief Saves the sequence to a file.
             * @param item The sequence to save.
             * @param os The output stream to save the sequence to.
             */
            static void save(DynamicSequence64 &item, std::ofstream &os)
            {
                item.tree.save(os);
            }

            /**
             * @brief Builds a sequence from a vector of bytes.
             * @param data The vector of bytes to build the sequence from.
             * @param pos The position in the vector to start building from.
             * @return A new DynamicSequence64 instance.
             */
            static DynamicSequence64 build_from_data(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                DynamicSequence64 r;
                r.tree.build_from_data(data, pos);
                return r;
            }

            static DynamicSequence64 load(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                return DynamicSequence64::build_from_data(data, pos);
            }
            /**
             * @brief Builds a sequence from a file.
             * @param ifs The input stream to build the sequence from.
             * @return A new DynamicSequence64 instance.
             */
            static DynamicSequence64 build_from_data(std::ifstream &ifs)
            {
                DynamicSequence64 r;
                r.tree.build_from_data(ifs);
                return r;
            }
            static DynamicSequence64 load(std::ifstream &ifs)
            {
                return DynamicSequence64::build_from_data(ifs);
            }
            /**
             * @brief Returns the value at a specified position in the sequence.
             * @param n The position to get the value from.
             * @return The value at the specified position.
             */
            uint64_t operator[](uint64_t n) const
            {
                return this->tree.at(n);
            }
            /**
             * @brief Converts the sequence to a string.
             * @return A string representation of the sequence.
             */
            std::string to_string() const
            {
                std::stringstream ss;
                auto vec = this->to_vector();
                ss << stool::DebugPrinter::to_integer_string(vec);
                return ss.str();
            }

            /**
             * @brief Prints the sequence to the console.
             */
            void print() const
            {
                auto vec = this->to_vector();
                std::cout << "================== SPSI ==================" << std::endl;
                stool::DebugPrinter::print_integers(vec, "values");

                std::cout << "================== SPSI[END] =============" << std::endl;
            }
        };


        using SimpleDynamicSequence64 = DynamicSequence64<stool::NaiveFLCVector<>, 62, 256>;
    }
}
