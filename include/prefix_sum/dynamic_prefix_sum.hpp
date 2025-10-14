#pragma once
#include "./plain_spsi_container.hpp"
#include "stool/include/light_stool.hpp"
#include "stool/include/develop/short_integer_vector.hpp"

namespace stool
{
    namespace bptree
    {

        ////////////////////////////////////////////////////////////////////////////////
        /// @brief      A dynamic data structure supporting prefix-sum query [Unchecked AI's Comment]
        ///
        ////////////////////////////////////////////////////////////////////////////////
        template <typename LEAF_CONTAINER = VLCDeque, uint64_t TREE_DEGREE = bptree::DEFAULT_MAX_DEGREE_OF_INTERNAL_NODE, uint64_t LEAF_CONTAINER_MAX_SIZE = bptree::DEFAULT_MAX_COUNT_OF_VALUES_IN_LEAF>
        class DynamicPrefixSum
        {
        public:
            using NodePointer = bptree::BPNodePointer<LEAF_CONTAINER, uint64_t, TREE_DEGREE, true>;
            using Tree = bptree::BPTree<LEAF_CONTAINER, uint64_t, TREE_DEGREE, LEAF_CONTAINER_MAX_SIZE, false, true>;
            //static inline constexpr int DEFAULT_MAX_COUNT_OF_VALUES_IN_LEAF = 126;

            //static inline constexpr int DEFAULT_MAX_COUNT_OF_VALUES_IN_LEAF = 126;
            //static inline constexpr int DEFAULT_MAX_COUNT_OF_VALUES_IN_LEAF = 1024;

        private:
            Tree tree;

        public:
            DynamicPrefixSum()
            {
                this->tree.initialize();
            }
            DynamicPrefixSum(const std::vector<uint64_t> &items){
                this->tree.initialize();
                this->tree.build(items);
                assert(this->size() == items.size());
            }
            DynamicPrefixSum &operator=(const DynamicPrefixSum &) = delete;
            DynamicPrefixSum(DynamicPrefixSum &&) noexcept = default;
            DynamicPrefixSum &operator=(DynamicPrefixSum &&) noexcept = default;

        public:
            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Iterators
            ///   The iterators supported this data structure.
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            typename Tree::ValueForwardIterator begin() const
            {
                return this->tree.get_value_forward_iterator_begin();
            }
            typename Tree::ValueForwardIterator end() const
            {
                return this->tree.get_value_forward_iterator_end();
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Properties
            ///   The properties of this class.
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            /**
             * @brief Get the internal tree of this data structure.
             * @return The internal tree of this data structure.
             */
            Tree &__get_tree()
            {
                return this->tree;
            }

            /**
             * @brief Get the maximum degree of internal nodes of the internal tree of this data structure.
             * @return The maximum degree of internal nodes of the internal tree of this data structure.
             */
            uint64_t get_degree() const
            {
                return this->tree.get_max_degree_of_internal_node();
            }

            /**
             * @brief Return the number of elements stored in this data structure.
             * @return The number of elements stored in this data structure.
             */
            uint64_t size() const
            {
                return this->tree.size();
            }

            /**
             * @brief Return the element at the given position.
             * @param pos The position of the element to return.
             * @return The element at the given position.
             */
            uint64_t at(uint64_t pos) const
            {
                return this->tree.at(pos);
            }

            /**
             * @brief Return the size of this data structure in bytes.
             * @return The size of this data structure in bytes.
             */
            uint64_t size_in_bytes(bool only_extra_bytes = false) const
            {
                return this->tree.size_in_bytes(only_extra_bytes);
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Conversion functions
            ///   The conversion functions supported this data structure.
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Return the elements stored in this data structure as a vector.
             * @return The elements stored in this data structure as a vector.
             */
            std::vector<uint64_t> to_vector() const
            {
                return this->tree.to_value_vector();
            }

            /**
             * @brief Return the elements stored in this data structure as a vector of uint8_t.
             * @return The elements stored in this data structure as a vector of uint8_t.
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

            std::string to_string() const
            {
                std::stringstream ss;
                auto vec = this->to_vector();
                ss << stool::DebugPrinter::to_integer_string(vec);
                return ss.str();
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Queries
            ///   The queries supported this data structure.
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            uint64_t reverse_psum([[maybe_unused]] uint64_t i) const
            {
                throw std::runtime_error("No implementation");
            }

            /**
             * @brief Return the sum of the first (i+1) values stored in this data structure.
             */
            uint64_t psum(uint64_t i) const
            {
                return this->tree.psum(i);
            }

            /**
             * @brief Return the sum of the stored values.
             */
            uint64_t psum() const
            {
                return this->tree.psum();
            }

            /**
             * @brief Return the smallest i such that psum(i) >= x
             */
            int64_t search(uint64_t x) const
            {
                return this->tree.search(x);
            }

            int64_t predecessor_index(uint64_t value) const
            {
                int64_t size = this->size();

                if (size > 0)
                {
                    uint64_t fst_value = this->at(0);
                    if (value >= fst_value)
                    {
                        if (value > this->psum())
                        {
                            return size - 1;
                        }
                        else
                        {
                            int64_t idx = this->search(value);
                            if (idx >= size)
                            {
                                return size - 1;
                            }
                            else
                            {
                                uint64_t v = this->psum(idx);

                                assert(v >= value);
                                if (v > value)
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
            int64_t successor_index(uint64_t value) const
            {
                int64_t size = this->size();
                if (size > 0)
                {
                    uint64_t lst_value = this->psum();
                    if (value <= lst_value)
                    {
                        int64_t idx = this->search(value);
                        if (idx >= size)
                        {
                            return -1;
                        }
                        else
                        {
                            [[maybe_unused]] uint64_t v = this->psum(idx);
                            assert(v >= value);
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
            uint64_t operator[](uint64_t n) const
            {
                return this->tree.at(n);
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Update operations
            ///   The update operations supported this data structure.
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            void swap(DynamicPrefixSum &item)
            {
                this->tree.swap(item.tree);
            }

            /**
             * @brief Clear the elements stored in this data structure.
             */
            void clear()
            {
                this->tree.clear();
            }

            /**
             * @brief Verify the internal consistency of this data structure.
             */
            void verify() const
            {
                this->tree.verify();
            }
            void push_back(uint64_t value)
            {
                this->tree.push_back(value);
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

            void insert(uint64_t pos, uint64_t value)
            {
                this->tree.insert(pos, value, value);
            }
            void remove(uint64_t pos)
            {
                this->tree.remove(pos);
            }

            void increment(uint64_t i, int64_t delta)
            {
                this->tree.increment(i, delta);
            }
            void decrement(uint64_t i, int64_t delta)
            {
                this->tree.increment(i, -delta);
            }
            void push_many(const std::vector<uint64_t> &items)
            {
                this->tree.push_many(items);
            }
            void set_value(uint64_t i, uint64_t value){
                uint64_t old_value = this->at(i);
                if(old_value > value){
                    this->decrement(i, old_value - value);
                }
                else if(old_value < value){
                    this->increment(i, value - old_value);
                }
            }
            void set_values(uint64_t i, const std::vector<uint64_t> &values){
                assert(i < this->size());
                assert(i + values.size() <= this->size());

                for(uint64_t j = 0; j < values.size(); j++){
                    this->set_value(i + j, values[j]);
                }
            }

            /*
            void set_degree(uint64_t degree)
            {
                this->tree.initialize(degree);
            }
            */
            double density() const{
                return this->tree.get_value_density();
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Print functions
            ///   The functions for printing messages.
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            std::vector<std::string> get_memory_usage_info(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::vector<std::string> log1 = this->tree.get_memory_usage_info(message_paragraph + 1);

                std::vector<std::string> r;
                uint64_t size_in_bytes = this->size_in_bytes();
                uint64_t size = this->size();
                double bits_per_element = size > 0 ? ((double)size_in_bytes / (double)size) : 0;

                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "=DynamicPrefixSum: " + std::to_string(this->size_in_bytes()) 
                + " bytes, " + std::to_string(size) + " elements, " + std::to_string(bits_per_element)  + " bytes per element =");

                for (std::string &s : log1)
                {
                    r.push_back(s);
                }

                uint64_t total_bits_if_sequence_is_plain = 0;
                for(uint64_t i = 0; i < size; ++i){
                    total_bits_if_sequence_is_plain += stool::LSBByte::get_code_length(this->at(i));
                }


                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "The number of bits in input sequence: " + std::to_string(total_bits_if_sequence_is_plain));
                if(total_bits_if_sequence_is_plain > 0){
                    r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "Memory per input bit: " + std::to_string((double)size_in_bytes / (double)total_bits_if_sequence_is_plain) + " bytes");
                }


                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "==");

            
                return r;
            }
            void print_memory_usage(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::vector<std::string> log = this->get_memory_usage_info(message_paragraph);
                for (std::string &s : log)
                {
                    std::cout << s << std::endl;
                }
            }
            void print_statistics(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Statistics(DynamicPrefixSum):" << std::endl;
                this->tree.print_statistics(message_paragraph + 1);
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[END]" << std::endl;
            }
            void print_information_about_performance(int message_paragraph = stool::Message::SHOW_MESSAGE) const{
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Performance Information (DynamicPrefixSum)[" << std::endl;                
                this->tree.print_information_about_performance(message_paragraph + 1);
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "]" << std::endl;
            }

            void print_tree(int message_paragraph = stool::Message::SHOW_MESSAGE) const{
                
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Tree(DynamicPrefixSum)[" << std::endl;
                this->tree.print_tree();
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "]" << std::endl;
                
            }
            void print_info() const {
                this->print_statistics();
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Builder and Writer functions
            ///   The functions for building and writing this data structure.
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            static DynamicPrefixSum build(const std::vector<uint64_t> &items)
            {
                DynamicPrefixSum r;
                r.tree.initialize();
                r.tree.build(items);
                assert(r.size() == items.size());
                return r;
            }
            /*
            static DynamicPrefixSum load_from_bytes(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                DynamicPrefixSum r;
                r.tree.load_from_bytes(data, pos);
                return r;
            }
            */
            static DynamicPrefixSum load_from_bytes(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                DynamicPrefixSum r;
                r.tree.load_from_bytes(data, pos);
                return r;
            }

            static DynamicPrefixSum load_from_file(std::ifstream &ifs)
            {
                DynamicPrefixSum r;
                r.tree.load_from_file(ifs);
                return r;
            }
            static DynamicPrefixSum load(std::ifstream &ifs)
            {
                return DynamicPrefixSum::load_from_file(ifs);
            }

            static void store_to_bytes(DynamicPrefixSum &item, std::vector<uint8_t> &output, uint64_t &pos)
            {
                Tree::store_to_bytes(item.tree, output, pos);
            }
            static void store_to_file(DynamicPrefixSum &item, std::ofstream &os)
            {
                Tree::store_to_file(item.tree, os);
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Other static functions
            ///   The other static functions supported this data structure.
            ////////////////////////////////////////////////////////////////////////////////
            //@{
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
        //using DynamicSuccinctPrefixSum = DynamicPrefixSum<stool::NaiveVLCArray<4096>, 62, 128>;
        //using EFDynamicPrefixSum = DynamicPrefixSum<stool::NaiveFLCVector<>, 62, 256>;

        // using GapEncodedSPSI = SPSI<GapEncodedContainer>;

    }

}