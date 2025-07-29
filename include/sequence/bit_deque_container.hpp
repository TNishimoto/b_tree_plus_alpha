#pragma once
#include "../bp_tree/bp_tree.hpp"

namespace stool
{
    namespace bptree
    {
        ////////////////////////////////////////////////////////////////////////////////
        /// @brief  A container that stores a short sequence of bits.
        ////////////////////////////////////////////////////////////////////////////////
        class BitDequeContainer
        {
            BitDeque bits;

        public:
            using BitDequeContainerIterator = BitDeque::BitDequeIterator;
            BitDequeContainer()
            {
            }
            BitDequeContainer(std::vector<uint64_t> &_items)
            {
                for (uint64_t x : _items)
                {
                    this->push_back(x);
                }
            }
            BitDequeContainer(std::vector<bool> &_items)
            {
                for (uint64_t i = 0; i < _items.size(); i++)
                {
                    this->push_back(_items[i]);
                }
            }

            void swap(BitDequeContainer &item)
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
                throw std::runtime_error("Error: BitDequeContainer");
            }

            uint64_t at(uint64_t pos) const
            {
                return this->bits.at(pos);
            }
            void print() const
            {
                throw std::runtime_error("Error: BitDequeContainer");
            }

            void clear()
            {
                this->bits.clear();
            }
            static std::string name()
            {
                return "BitDequeContainer";
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
            void push_front(std::vector<uint64_t> &new_items)
            {
                for (int64_t i = new_items.size() - 1; i >= 0; i--)
                {
                    this->bits.push_front(new_items[i] >= 1);
                }

            }
            void push_front(uint64_t new_item)
            {
                this->bits.push_back64(new_item >= 1);
            }

            void push_back(std::vector<uint64_t> &new_items)
            {
                for (uint64_t v : new_items)
                {
                    this->push_back(v >= 1);
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
                std::vector<uint64_t> r;
                for (uint64_t i = 0; i < len; i++)
                {
                    bool b = this->at(0);
                    r.push_back(b ? 1 : 0);
                    this->pop_front();
                }
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
                uint64_t byte_size = BitDeque::get_byte_size(this->bits);
                uint64_t size = this->size();
                for (uint64_t i = 0; i < byte_size; i++)
                {
                    output.push_back(0);
                }
                BitDeque::save(this->bits, output, size);
            }
            /*
            void load_from_data(std::vector<uint64_t> &output){

            }
            */

            static uint64_t get_byte_size(const std::vector<BitDequeContainer> &items)
            {
                uint64_t size = sizeof(uint64_t);
                for (const auto &item : items)
                {
                    size += BitDeque::get_byte_size(item.bits);
                }
                return size;
            }
            static void save(const std::vector<BitDequeContainer> &items, std::vector<uint8_t> &output, uint64_t &pos)
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
                    BitDeque::save(item.bits, output, pos);
                }
            }
            static void save(const std::vector<BitDequeContainer> &items, std::ofstream &os)
            {
                uint64_t items_size = items.size();
                os.write(reinterpret_cast<const char *>(&items_size), sizeof(uint64_t));
    
                for (const auto &item : items)
                {
                    BitDeque::save(item.bits, os);
                }
            }
            static BitDequeContainer load(const std::vector<uint8_t> &data, uint64_t &pos){
                BitDequeContainer r;
                r.bits = BitDeque::load(data, pos);
                return r;
            }
            static BitDequeContainer load(std::ifstream &ifs)
            {
                BitDequeContainer r;
                r.bits = BitDeque::load(ifs);
                return r;
            }
            static std::vector<BitDequeContainer> load_vector(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                uint64_t size = 0;
                std::memcpy(&size, data.data() + pos, sizeof(uint64_t));
                pos += sizeof(uint64_t);
    
                std::vector<BitDequeContainer> output;
                output.resize(size);
                for (uint64_t i = 0; i < size; i++)
                {
                    output[i] = BitDequeContainer::load(data, pos);
                }
                return output;
            }
            static std::vector<BitDequeContainer> load_vector(std::ifstream &ifs)
            {
                uint64_t size = 0;
                ifs.read(reinterpret_cast<char *>(&size), sizeof(uint64_t));
    
                std::vector<BitDequeContainer> output;
                output.resize(size);
                for (uint64_t i = 0; i < size; i++)
                {
                    output[i] = BitDequeContainer::load(ifs);
                }
    
                return output;
            }

            BitDequeContainerIterator begin() const
            {
                return this->bits.begin();
            }
            BitDequeContainerIterator end() const
            {
                return this->bits.end();
            }
        };
    }

}