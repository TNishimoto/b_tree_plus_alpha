#pragma once
#include "../bp_tree/bp_tree.hpp"

namespace stool
{
    namespace bptree
    {
        ////////////////////////////////////////////////////////////////////////////////
        /// @brief  A container that stores a short sequence of bits. [Unchecked AI's Comment]
        ////////////////////////////////////////////////////////////////////////////////
        template <uint64_t MAX_BIT_SIZE = 8192ULL>
        class BitVectorContainer
        {
            //using BitArrayDeque = typename stool::BitArrayDeque<MAX_BIT_SIZE>;
            using BitArrayDeque = typename stool::NaiveBitVector<MAX_BIT_SIZE>;

            BitArrayDeque bits;

        public:
            //using BitVectorContainerIterator = typename BitArrayDeque::BitArrayDequeIterator;

            using BitVectorContainerIterator = typename BitArrayDeque::NaiveBitVectorIterator;
            BitVectorContainer()
            {
            }
            BitVectorContainer(std::vector<uint64_t> &_items)
            {
                for (uint64_t x : _items)
                {
                    this->push_back(x);
                }
            }
            BitVectorContainer(std::vector<bool> &_items)
            {
                for (uint64_t i = 0; i < _items.size(); i++)
                {
                    this->push_back(_items[i]);
                }
            }

            void swap(BitVectorContainer &item)
            {
                std::swap(this->bits, item.bits);
            }
            uint64_t size() const
            {
                return this->bits.size();
            }
            uint64_t size_in_bytes(bool only_extra_bytes) const
            {
                return this->bits.size_in_bytes(only_extra_bytes);
            }
            uint64_t unused_size_in_bytes() const
            {
                return this->bits.unused_size_in_bytes();
            }

            uint64_t at(uint64_t pos) const
            {
                return this->bits.at(pos);
            }
            void print() const
            {
                throw std::runtime_error("Error: BitVectorContainer");
            }

            void clear()
            {
                this->bits.clear();
            }
            static std::string name()
            {
                return "BitVectorContainer";
            }
            uint64_t psum(uint64_t i) const noexcept
            {
                return this->bits.psum(i);
            }
            uint64_t psum() const noexcept
            {
                return this->bits.psum();
            }
            int64_t search(uint64_t x) const noexcept
            {
                uint64_t v = this->bits.search(x);
                return v;
            }

            std::string to_string() const
            {
                return this->bits.to_string();
            }
            std::vector<uint64_t> to_value_vector() const
            {
                std::vector<uint64_t> r;
                for (uint64_t i = 0; i < this->size(); i++)
                {
                    r.push_back(this->at(i));
                }
                return r;
            }
            uint64_t to_uint64() const
            {
                uint64_t size = this->size();
                if (size > 0)
                {
                    uint64_t value = this->bits.read_64_bit_string();
                    if (size >= 64)
                    {
                        return value;
                    }
                    else
                    {
                        return value >> (64 - size);
                    }
                }
                else
                {
                    return 0;
                }
            }

            template <typename VEC>
            void to_values(VEC &output_vec) const
            {
                output_vec.clear();
                output_vec.resize(this->size());
                for (uint64_t i = 0; i < this->size(); i++)
                {
                    output_vec[i] = this->at(i);
                }

                

            }

            void insert(uint64_t pos, uint64_t value)
            {
                this->bits.insert(pos, value >= 1);
            }
            void remove(uint64_t pos)
            {
                assert(this->size() > 0);
                this->bits.erase(pos);
            }
            uint64_t to_bit_array(const std::vector<uint64_t> &new_items, std::array<uint64_t, MAX_BIT_SIZE / 64> &output){
                uint64_t added_block_size = ((new_items.size()-1) / 64) + 1;
                for(uint64_t i = 0; i < added_block_size; i++){
                    output[i] = 0;
                }
                for(uint64_t i = 0; i < new_items.size(); i++){
                    uint64_t v = i / 64;
                    uint64_t w = i % 64;
                    output[v] |= ((new_items[i] >= 1) ? 1ULL : 0ULL) << (63 - w);
                }
                return added_block_size;
            }
            void push_front(const std::vector<uint64_t> &new_items)
            {
                if(new_items.size() > 0){
                    std::array<uint64_t, MAX_BIT_SIZE / 64> tmp_buffer;
                    uint64_t added_block_size = this->to_bit_array(new_items, tmp_buffer);

                    #ifdef DEBUG
                    uint64_t sum1 = this->bits.psum();
                    uint64_t sum2 = 0;
                    for(uint64_t i = 0; i < new_items.size(); i++){
                        sum2 += new_items[i] >= 1 ? 1 : 0;
                    }
                    uint64_t x_size = this->size();
                    #endif

                    this->bits.push_front64(tmp_buffer, new_items.size(), added_block_size);

                    #ifdef DEBUG
                    if(sum1 + sum2 != this->bits.psum()){
                        std::cout << new_items.size() << "/" << x_size << std::endl;
                        std::cout << sum1 << "/" << sum2 << std::endl;
                        std::cout << this->bits.psum() << std::endl;
                    }
                    assert(sum1 + sum2 == this->bits.psum());
                    #endif
                }

            }
            void push_front(uint64_t new_item)
            {

                this->bits.push_back64(new_item >= 1);
            }

            void push_back(const std::vector<uint64_t> &new_items)
            {

                if(new_items.size() > 0){
                    std::array<uint64_t, MAX_BIT_SIZE / 64> tmp_buffer;
                    uint64_t added_block_size = this->to_bit_array(new_items, tmp_buffer);
                    this->bits.push_back64(tmp_buffer, new_items.size(), added_block_size);
                }
            }
            void push_back(uint64_t value)
            {

                this->bits.push_back(value >= 1);
            }
            void pop_front()
            {

                assert(this->size() > 0);
                this->bits.pop_front();
            }
            std::vector<uint64_t> pop_front(uint64_t len)
            {

                assert(len <= this->size());
                std::array<uint64_t, MAX_BIT_SIZE / 64> tmp_buffer;
                this->bits.pop_front(len, tmp_buffer, MAX_BIT_SIZE / 64);
                std::vector<uint64_t> r;

                for(uint64_t i = 0; i < len; i++){
                    uint64_t v = i / 64;
                    uint64_t w = i % 64;
                    r.push_back((tmp_buffer[v] >> (63 - w)) & 1);
                }

                /*
                for (uint64_t i = 0; i < len; i++)
                {
                    bool b = this->at(0);
                    r.push_back(b ? 1 : 0);
                    this->pop_front();
                }
                */
                return r;
            }
            void pop_back()
            {

                assert(this->size() > 0);
                this->bits.pop_back();
            }

            std::vector<uint64_t> pop_back(uint64_t len)
            {

                assert(len <= this->size());
                std::vector<uint64_t> r;
                r.resize(len);

                for (uint64_t i = 0; i < len; i++)
                {
                    bool b = this->at(this->size() - 1);
                    r[len - i - 1] = b ? 1 : 0;
                    this->pop_back();
                }
                return r;
            }

            uint64_t reverse_psum(uint64_t i) const
            {
                return this->bits.reverse_psum(i);
            }

            uint64_t psum(uint64_t i, uint64_t j) const
            {
                return this->bits.psum(i, j);
            }

            void increment(uint64_t i, int64_t delta)
            {
                this->bits.increment(i, delta);
            }

            int64_t rank1(uint64_t i) const
            {
                return this->bits.rank1(i);
            }

            int64_t rank0(uint64_t i) const
            {
                return i - this->rank1(i);
            }
            int64_t rank(uint64_t i, bool b) const
            {
                return b ? this->rank1(i) : this->rank0(i);
            }

            int64_t select(uint64_t i, bool b) const
            {
                return b ? this->select1(i) : this->select0(i);
            }

            int64_t select1(uint64_t i) const
            {
                return this->bits.select1(i);
            }
            int64_t select0(uint64_t i) const
            {
                return this->bits.select0(i);
            }

            void to_data(std::vector<uint8_t> &output) const
            {
                uint64_t byte_size = BitArrayDeque::get_byte_size(this->bits);
                uint64_t size = this->size();
                for (uint64_t i = 0; i < byte_size; i++)
                {
                    output.push_back(0);
                }
                BitArrayDeque::store_to_bytes(this->bits, output, size);
            }
            /*
            void load_from_data(std::vector<uint64_t> &output){

            }
            */

            static uint64_t get_byte_size(const std::vector<BitVectorContainer> &items)
            {
                uint64_t size = sizeof(uint64_t);
                for (const auto &item : items)
                {
                    size += BitArrayDeque::get_byte_size(item.bits);
                }
                return size;
            }
            static void store_to_bytes(const std::vector<BitVectorContainer> &items, std::vector<uint8_t> &output, uint64_t &pos)
            {
                uint64_t size = get_byte_size(items);
                if (pos + size > output.size())
                {
                    output.resize(pos + size);
                }

                uint64_t items_size = items.size();
                std::memcpy(output.data() + pos, &items_size, sizeof(uint64_t));
                pos += sizeof(uint64_t);

                for (const auto &item : items)
                {
                    BitArrayDeque::store_to_bytes(item.bits, output, pos);
                }
            }
            static void store_to_file(const std::vector<BitVectorContainer> &items, std::ofstream &os)
            {
                uint64_t items_size = items.size();
                os.write(reinterpret_cast<const char *>(&items_size), sizeof(uint64_t));
    
                for (const auto &item : items)
                {
                    BitArrayDeque::store_to_file(item.bits, os);
                }
            }
            static BitVectorContainer load_from_bytes(const std::vector<uint8_t> &data, uint64_t &pos){
                BitVectorContainer r;
                r.bits = BitArrayDeque::load_from_bytes(data, pos);
                return r;
            }
            static BitVectorContainer load_from_file(std::ifstream &ifs)
            {
                BitVectorContainer r;
                r.bits = BitArrayDeque::load_from_file(ifs);
                return r;
            }
            static std::vector<BitVectorContainer> load_vector_from_bytes(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                uint64_t size = 0;
                std::memcpy(&size, data.data() + pos, sizeof(uint64_t));
                pos += sizeof(uint64_t);
    
                std::vector<BitVectorContainer> output;
                output.resize(size);
                for (uint64_t i = 0; i < size; i++)
                {
                    output[i] = BitVectorContainer::load_from_bytes(data, pos);
                }
                return output;
            }
            static std::vector<BitVectorContainer> load_vector_from_file(std::ifstream &ifs)
            {
                uint64_t size = 0;
                ifs.read(reinterpret_cast<char *>(&size), sizeof(uint64_t));
    
                std::vector<BitVectorContainer> output;
                output.resize(size);
                for (uint64_t i = 0; i < size; i++)
                {
                    output[i] = BitVectorContainer::load_from_file(ifs);
                }
    
                return output;
            }

            BitVectorContainerIterator begin() const
            {
                return this->bits.begin();
            }
            BitVectorContainerIterator end() const
            {
                return this->bits.end();
            }
        };
    }

}