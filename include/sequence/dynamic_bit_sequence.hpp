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

        ////////////////////////////////////////////////////////////////////////////////
        /// @brief      A dynamic data structure supporting rank and select queries on a bit sequence
        ///
        ////////////////////////////////////////////////////////////////////////////////
        template <typename CONTAINER, typename CONTAINER_ITERATOR, uint64_t MAX_TREE_DEGREE, uint64_t MAX_BIT_CONTAINER_SIZE>
        class DynamicBitSequence
        {
            
            using NodePointer = bptree::BPNodePointer<CONTAINER, bool, MAX_TREE_DEGREE, true>;
            using T = uint64_t;
            using Tree = bptree::BPTree<CONTAINER, bool, MAX_TREE_DEGREE, MAX_BIT_CONTAINER_SIZE,false, true>;
            
            static inline constexpr int DEFAULT_CONTAINER_DEGREE = 62;
            //static inline constexpr int DEFAULT_CONTAINER_DEGREE = 124;

        public:
            Tree tree;

            /**
             * @brief Deleted copy constructor.
             */
            DynamicBitSequence(const DynamicBitSequence &) = delete;

            /**
             * @brief Default constructor initializes the tree with default degree.
             */
            DynamicBitSequence()
            {
            }

            /**
             * @brief Deleted copy assignment operator.
             */
            DynamicBitSequence &operator=(const DynamicBitSequence &) = delete;

            /**
             * @brief Default move constructor.
             */
            DynamicBitSequence(DynamicBitSequence &&) noexcept = default;

            /**
             * @brief Default move assignment operator.
             */
            DynamicBitSequence &operator=(DynamicBitSequence &&) noexcept = default;

        public:
            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Properties
            ///   The properties of this class.
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Returns the size of the bit sequence.
             * @return uint64_t The number of bits in the sequence.
             */
            uint64_t size() const
            {
                return this->tree.size();
            }

            /**
             * @brief Returns the size of the bit sequence in bytes.
             * @return uint64_t The number of bytes used by the sequence.
             */
            uint64_t size_in_bytes() const
            {
                return this->tree.size_in_bytes();
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Queries
            ///   The queries supported this data structure.
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Checks if the bit sequence is empty.
             * @return bool True if the sequence is empty, false otherwise.
             */
            bool empty() const
            {
                return this->tree.empty();
            }

            /**
             * @brief Returns the bit at the specified position.
             * @param pos The position to query.
             * @return bool The bit value at the specified position.
             */
            bool at(uint64_t pos) const
            {
                return this->tree.at(pos) & 1;
            }

            /**
             * @brief Returns the number of 1s up to position i.
             * @param i The position to query.
             * @return int64_t The number of 1s.
             */
            int64_t rank1(uint64_t i) const
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
             * @brief Returns the number of 0s up to position i.
             * @param i The position to query.
             * @return int64_t The number of 0s.
             */
            int64_t rank0(uint64_t i) const
            {
                return i - this->rank1(i);
            }

            /**
             * @brief Returns the number of bits with value c up to position i.
             * @param i The position to query.
             * @param c The bit value to count.
             * @return int64_t The number of bits with value c.
             */
            int64_t rank(uint64_t i, bool c) const
            {
                return c ? this->rank1(i) : this->rank0(i);
            }

            /**
             * @brief Returns the position of the i-th 1.
             * @param i The index of the 1 to find.
             * @return int64_t The position of the i-th 1, or -1 if not found.
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
             * @brief Returns the position of the i-th 0.
             * @param i The index of the 0 to find.
             * @return int64_t The position of the i-th 0, or -1 if not found.
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
             * @brief Returns the position of the i-th bit with value c.
             * @param i The index of the bit to find.
             * @param c The bit value to find.
             * @return int64_t The position of the i-th bit with value c, or -1 if not found.
             */
            int64_t select(uint64_t i, bool c) const
            {
                return c ? this->select1(i) : this->select0(i);
            }

            /**
             * @brief Returns the number of bits with value c in the sequence.
             * @param c The bit value to count.
             * @return int64_t The number of bits with value c.
             */
            int64_t count_c(bool c) const
            {
                uint64_t _size = this->size();
                if (_size > 0)
                {
                    return this->rank(_size, c);
                }
                else
                {
                    return 0;
                }
            }

            int64_t count0() const {
                uint64_t _size = this->size();
                if(_size > 0){
                    return this->rank0(_size);
                }else{
                    return 0;
                }
            }
            int64_t count1() const{
                uint64_t _size = this->size();
                if(_size > 0){
                    return this->rank1(_size);
                }else{
                    return 0;
                }
            }

            /**
             * @brief Returns the bit at the specified position.
             * @param n The position to query.
             * @return bool The bit value at the specified position.
             */
            bool operator[](uint64_t n) const
            {
                return this->at(n);
            }

            /**
             * @brief Returns the prefix sum up to position x.
             * @param x The position to query.
             * @return uint64_t The prefix sum.
             */
            uint64_t psum(uint64_t x) const
            {
                return this->tree.psum(x);
            }

            /**
             * @brief Returns the total prefix sum.
             * @return uint64_t The total prefix sum.
             */
            uint64_t psum() const
            {
                return this->tree.psum();
            }

            /**
             * @brief Searches for the position where the prefix sum equals sum.
             * @param sum The sum to search for.
             * @return int64_t The position where the prefix sum equals sum, or -1 if not found.
             */
            int64_t search(uint64_t sum) const
            {
                return this->tree.search(sum);
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Update operations
            ///   The update operations supported this data structure.
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Sets the degree of the tree.
             * @param degree The degree to set.
             */
            void set_degree(uint64_t degree)
            {
                this->tree.initialize(degree, DynamicBitSequence::DEFAULT_CONTAINER_DEGREE);
            }

            /**
             * @brief Swaps the contents of this sequence with another.
             * @param item The sequence to swap with.
             */
            void swap(DynamicBitSequence &item)
            {
                this->tree.swap(item.tree);
            }

            /**
             * @brief Clears the bit sequence.
             */
            void clear()
            {
                this->tree.clear();
            }

            /**
             * @brief Adds a bit to the end of the sequence.
             * @param value The bit value to add.
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
             * @brief Adds a bit to the beginning of the sequence.
             * @param value The bit value to add.
             */
            void push_front(bool value)
            {
                this->tree.push_front(value);
            }

            /**
             * @brief Inserts a bit at the specified position.
             * @param pos The position to insert at.
             * @param value The bit value to insert.
             */
            void insert(uint64_t pos, bool value)
            {
                this->tree.insert(pos, value, value);
            }

            /**
             * @brief Removes the bit at the specified position.
             * @param pos The position to remove from.
             */
            void remove(uint64_t pos)
            {
                assert(pos < this->size());
                this->tree.remove(pos);
            }

            /**
             * @brief Sets the bit at the specified position.
             * @param i The position to set.
             * @param b The bit value to set.
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
             * @brief Sorts the leaf containers.
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

            /**
             * @brief Adds multiple bits to the end of the sequence.
             * @param items The bits to add.
             */
            void push_many(const std::vector<bool> &items)
            {
                this->tree.push_many(items);
            }

            void print_debug_info() const{
                std::cout << "DynamicBitSequence::print_debug_info()" << std::endl;
                this->tree.print_debug_info();
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Print functions
            ///   The functions for printing messages.
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Returns memory usage information.
             * @param message_paragraph The paragraph level for messages.
             * @return std::vector<std::string> The memory usage information.
             */
            std::vector<std::string> get_memory_usage_info(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::vector<std::string> log1 = this->tree.get_memory_usage_info(message_paragraph + 1);

                std::vector<std::string> r;
                uint64_t size_in_bytes = this->size_in_bytes();
                uint64_t size = this->size();
                double bits_per_element = size > 0 ? ((double)size_in_bytes / (double)size) : 0;

                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "=DynamicBitSequence: " + std::to_string(this->size_in_bytes()) 
                + " bytes, " + std::to_string(size) + " elements, " + std::to_string(bits_per_element)  + " bytes per element =");
                for (std::string &s : log1)
                {
                    r.push_back(s);
                }
                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "==");
                return r;
            }

            /**
             * @brief Prints memory usage information.
             * @param message_paragraph The paragraph level for messages.
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
             * @brief Prints statistics about the bit sequence.
             * @param message_paragraph The paragraph level for messages.
             */
            void print_statistics(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Statistics(DynamicBitSequence):" << std::endl;
                this->tree.print_statistics(message_paragraph + 1);
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[END]" << std::endl;
            }

            /**
             * @brief Prints the content of the bit sequence.
             * @param message_paragraph The paragraph level for messages.
             */
            void print_content(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Content(DynamicBitSequence):" << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "Bits: " << this->to_string() << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[END]" << std::endl;
            }

            /**
             * @brief Prints the bit sequence.
             * @param name The name to print.
             * @param message_paragraph The paragraph level for messages.
             */
            void print(std::string name = "DynamicBitSequence", int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << name << ": " << this->to_string() << std::endl;
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Conversion functions
            ///   The conversion functions supported this data structure.
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Converts the bit sequence to a vector of uint64_t.
             * @return std::vector<uint64_t> The vector representation.
             */
            std::vector<uint64_t> to_value_vector() const
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
             * @brief Converts the bit sequence to a string.
             * @return std::string The string representation.
             */
            std::string to_string() const
            {
                std::string s;
                std::vector<uint64_t> bits = this->to_value_vector();
                s.push_back('[');
                for (uint64_t i = 0; i < bits.size(); i++)
                {
                    s.push_back(bits[i] >= 1 ? '1' : '0');
                    //assert(bits[i] == this->at(i));
                }
                s.push_back(']');
                return s;
            }

            /**
             * @brief Converts the bit sequence to a vector of bool.
             * @return std::vector<bool> The vector representation.
             */
            std::vector<bool> to_vector() const
            {
                uint64_t _size = this->size();
                std::vector<bool> r;
                r.resize(_size, false);
                uint64_t counter = 0;




                auto _end = this->get_leaf_forward_iterator_end();

                BitForwardIterator<CONTAINER, CONTAINER_ITERATOR, MAX_TREE_DEGREE, MAX_BIT_CONTAINER_SIZE> it = this->get_bit_forward_iterator_begin();
                while(!it.is_end()){
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

                /*
                for (BitForwardIterator<CONTAINER, CONTAINER_ITERATOR, MAX_TREE_DEGREE, MAX_BIT_CONTAINER_SIZE> it = this->get_bit_forward_iterator_begin(); it != _end; ++it)
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
                }
                */
                assert(counter == _size);
                return r;
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Builder and Writer functions
            ///   The functions for building and writing this data structure.
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Saves the bit sequence to a vector.
             * @param item The bit sequence to save.
             * @param output The output vector.
             * @param pos The position in the output vector.
             */
            static void save(DynamicBitSequence &item, std::vector<uint8_t> &output, uint64_t &pos)
            {
                item.tree.save(output, pos);
            }

            /**
             * @brief Saves the bit sequence to a file.
             * @param item The bit sequence to save.
             * @param os The output file stream.
             */
            static void save(DynamicBitSequence &item, std::ofstream &os)
            {
                item.tree.save(os);
            }

            /**
             * @brief Builds a bit sequence from a vector of bool.
             * @param items The vector of bool to build from.
             * @param tree_degree The degree of the tree.
             * @return DynamicBitSequence The built bit sequence.
             */
            static DynamicBitSequence build(const std::vector<bool> &items)
            {
                DynamicBitSequence r;
                r.tree.initialize();
                r.tree.build(items);
                return r;
            }


            /**
             * @brief Builds a bit sequence from a vector of uint8_t.
             * @param data The vector of uint8_t to build from.
             * @param pos The position in the input vector.
             * @return DynamicBitSequence The built bit sequence.
             */
            static DynamicBitSequence build_from_data(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                DynamicBitSequence r;
                r.tree.build_from_data(data, pos);
                return r;
            }

            /**
             * @brief Builds a bit sequence from a file.
             * @param ifs The input file stream.
             * @return DynamicBitSequence The built bit sequence.
             */
            static DynamicBitSequence build_from_data(std::ifstream &ifs)
            {
                DynamicBitSequence r;
                r.tree.build_from_data(ifs);
                return r;
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Iterators
            ///   The iterators supported this data structure.
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Returns an iterator to the beginning of the bit sequence.
             * @return BitForwardIterator The iterator.
             */
            BitForwardIterator<CONTAINER, CONTAINER_ITERATOR, MAX_TREE_DEGREE, MAX_BIT_CONTAINER_SIZE> get_bit_forward_iterator_begin() const
            {
                auto leaf_it = this->tree.get_leaf_forward_iterator_begin();
                return BitForwardIterator<CONTAINER, CONTAINER_ITERATOR, MAX_TREE_DEGREE, MAX_BIT_CONTAINER_SIZE>(&leaf_it, &this->tree);
            }

            /**
             * @brief Returns an iterator to the end of the bit sequence.
             * @return BitForwardIterator The iterator.
             */
            BitForwardIterator<CONTAINER, CONTAINER_ITERATOR, MAX_TREE_DEGREE, MAX_BIT_CONTAINER_SIZE> get_leaf_forward_iterator_end() const
            {
                return BitForwardIterator<CONTAINER, CONTAINER_ITERATOR, MAX_TREE_DEGREE, MAX_BIT_CONTAINER_SIZE>(nullptr, &this->tree);
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Other static functions
            ///   The other static functions supported this data structure.
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Returns the name of the bit sequence.
             * @return std::string The name.
             */
            static std::string name()
            {
                std::string s;
                s += "DynamicSequence(";
                s += CONTAINER::name();
                s += ")";
                return s;
            }

            void print_information_about_performance(int message_paragraph = stool::Message::SHOW_MESSAGE) const{
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Performance Information (DynamicSequence)[" << std::endl;                
                this->tree.print_information_about_performance(message_paragraph + 1);
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "]" << std::endl;
            }

            double density() const{
                return this->tree.get_value_density();
            }

            void verify() const{
                this->tree.verify();
            }


            //@}



            
        };

        using BDC = typename stool::bptree::BitVectorContainer<10000ULL>;

        using DynamicBitDequeSequence = DynamicBitSequence<BDC, BDC::BitVectorContainerIterator, bptree::DEFAULT_MAX_DEGREE_OF_INTERNAL_NODE, 1024>;
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

        //using DynamicBitDequeSequenceE = DynamicBitSequence<stool::bptree::BitVectorContainer, stool::bptree::BitVectorContainer::BitVectorContainerIterator, 8190, 2048>;

        // template <typename T>
        // using VLCDequeSeq = DynamicSequence<VLCDeque, T>;
    }

}
