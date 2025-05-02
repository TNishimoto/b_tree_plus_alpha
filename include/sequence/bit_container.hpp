#pragma once
#include "../bp_tree/bp_tree.hpp"

namespace stool
{
    namespace bptree
    {
        ////////////////////////////////////////////////////////////////////////////////
        /// @class      BitContainer
        /// @brief
        ///
        ////////////////////////////////////////////////////////////////////////////////
        class BitContainer
        {
            uint64_t bits = 1;

        public:
            BitContainer()
            {
            }
            BitContainer(std::vector<uint64_t> &_items)
            {
                for (uint64_t x : _items)
                {
                    this->push_back(x);
                }
            }
            BitContainer(std::vector<bool> &_items)
            {
                for(uint64_t i = 0; i < _items.size();i++){
                    this->push_back(_items[i]);
                }
            }

            void swap(BitContainer &item){
                std::swap(this->bits, item.bits);
            }
            uint64_t size() const
            {
                return stool::Byte::get_code_length(this->bits) - 1;
            }
            uint64_t size_in_bytes() const
            {
                return sizeof(uint64_t);
            }

            uint64_t at(uint64_t pos) const
            {
                return stool::Byte::get_bit(this->bits, pos);
            }
            void print() const
            {
                throw std::runtime_error("Error: BitContainer");
            }

            void clear()
            {
                this->bits = 1;
            }
            static std::string name()
            {
                return "Bits";
            }
            uint64_t psum(uint64_t i) const noexcept
            {
                uint64_t x = (this->bits << (63 - i));
                return __builtin_popcountll(x);
            }
            uint64_t psum() const noexcept
            {
                return __builtin_popcountll(this->bits) - 1;
            }
            int64_t search(uint64_t x) const noexcept
            {
                if (x == 0)
                {
                    return 0;
                }
                else
                {
                    uint64_t count = this->psum();
                    if (x <= count)
                    {
                        return stool::Byte::select1(this->bits, x-1);
                    }
                    else
                    {
                        return -1;
                    }
                }
            }

            std::string to_string() const
            {
                std::string s;
                for (uint64_t i = 0; i < this->size(); i++)
                {
                    if (this->at(i) >= 1)
                    {
                        s.push_back('1');
                    }
                    else
                    {
                        s.push_back('0');
                    }
                }

                return s;
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
            uint64_t to_uint64() const {
                return stool::Byte::write_bit(this->bits, this->size(), false);
            }

            template<typename VEC>
            void to_values(VEC &output_vec) const{
                output_vec.clear();
                output_vec.resize(this->size());
                for (uint64_t i = 0; i < this->size(); i++)
                {
                    output_vec[i] = this->at(i);
                }
            }


            void insert(uint64_t pos, uint64_t value)
            {
                assert(this->size() + 1 < 64);
                this->bits = stool::Byte::insert_bit(this->bits, pos, value >= 1);
            }
            void remove(uint64_t pos)
            {
                assert(this->size() > 0);
                this->bits = stool::Byte::remove_bit(this->bits, pos);
            }
            void push_front(std::vector<uint64_t> &new_items)
            {
                assert(this->size() + new_items.size() < 64);

                for (int64_t i = new_items.size() - 1; i >= 0; i--)
                {
                    this->push_front(new_items[i]);
                }
            }
            void push_front(uint64_t new_item)
            {
                assert(this->size() + 1 < 64);
                this->bits = (this->bits << 1) | (new_item >= 1 ? 1 : 0);
            }

            void push_back(std::vector<uint64_t> &new_items)
            {
                assert(this->size() + 1 < 64);
                assert(this->size() + new_items.size() < 64);

                for (uint64_t v : new_items)
                {
                    this->push_back(v);
                }
            }
            void push_back(uint64_t value)
            {
                assert(this->size() + 1 < 64);
                this->bits = stool::Byte::insert_bit(this->bits, this->size(), value >= 1);
            }
            void pop_front()
            {
                assert(this->size() > 0);
                this->bits = this->bits >> 1;
            }
            std::vector<uint64_t> pop_front(uint64_t len)
            {
                assert(len <= this->size());
                std::vector<uint64_t> r;
                for (uint64_t i = 0; i < len; i++)
                {
                    r.push_back(this->at(0));
                    this->pop_front();
                }
                return r;
            }
            void pop_back()
            {
                assert(this->size() > 0);
                this->remove(this->size() - 1);
            }

            std::vector<uint64_t> pop_back(uint64_t len)
            {
                assert(len <= this->size());
                std::vector<uint64_t> r;
                r.resize(len);

                for (uint64_t i = 0; i < len; i++)
                {
                    r[len - i - 1] = this->at(this->size() - 1);
                    this->pop_back();
                }
                return r;
            }

            uint64_t reverse_psum(uint64_t i) const
            {
                if (i == this->size() - 1)
                {
                    return this->psum();
                }
                else
                {
                    uint64_t len1 = i + 1;
                    uint64_t idx = this->size() - len1;
                    return this->psum() - this->psum(idx - 1);
                }
            }

            uint64_t psum(uint64_t i, uint64_t j) const
            {
                if (i == j)
                {
                    return this->at(i);
                }
                else
                {
                    throw std::runtime_error("No implementation");
                }
            }

            void increment(uint64_t i, int64_t delta)
            {
                if (delta >= 1)
                {
                    this->bits = stool::Byte::write_bit(this->bits, i, true);
                }
                else if (delta <= -1)
                {
                    this->bits = stool::Byte::write_bit(this->bits, i, false);
                }
            }

            std::vector<uint64_t>::const_iterator begin() const
            {
                throw std::runtime_error("Error: BitContainer");
            }

            std::vector<uint64_t>::const_iterator end() const
            {
                throw std::runtime_error("Error: BitContainer");
            }

            int64_t rank1(uint64_t i) const{
                if(i == 0){
                    return 0;
                }else{
                    return this->psum(i-1);
                }
            }

            int64_t rank0(uint64_t i) const{
                return i - this->rank1(i);
            }
            int64_t rank(uint64_t i, bool b) const {
                return b ? this->rank1(i) : this->rank0(i);
            }


            int64_t select(uint64_t i, bool b) const
            {
                return b ? this->select1(i) : this->select0(i);
            }


            int64_t select1(uint64_t i) const
            {
                    return this->search(i+1);
            }
            int64_t select0(uint64_t i) const
            {
                        return stool::Byte::select0(this->bits, i);
            }
            
            void to_data(std::vector<uint8_t> &output) const{
                output.push_back(this->bits);
            }
            /*
            void load_from_data(std::vector<uint64_t> &output){

            }
            */

            static uint64_t get_byte_size(const std::vector<BitContainer> &items){
                return sizeof(uint64_t) + (items.size() * sizeof(BitContainer));
            }
            static void save(const std::vector<BitContainer> &items, std::vector<uint8_t> &output, uint64_t &pos){
                uint64_t size = get_byte_size(items);
                if(pos + size > output.size()){
                    output.resize(pos + size);
                }

                uint64_t items_size = items.size();
                std::memcpy(output.data() + pos, &items_size, sizeof(uint64_t));
                pos += sizeof(uint64_t);

                std::memcpy(output.data() + pos, items.data(), items_size * sizeof(BitContainer));
                pos += size * sizeof(BitContainer);
            }
            static void save(const std::vector<BitContainer> &items, std::ofstream &os){
                uint64_t items_size = items.size();
	            os.write(reinterpret_cast<const char*>(&items_size), sizeof(uint64_t));
	            os.write(reinterpret_cast<const char*>(items.data()), items.size() * sizeof(BitContainer));
            }


            static std::vector<BitContainer> load_vector(const std::vector<uint8_t> &data, uint64_t &pos){
                uint64_t size = 0;
                std::memcpy(&size, data.data() + pos, sizeof(uint64_t));
                pos += sizeof(uint64_t);

                std::vector<BitContainer> output;
                output.resize(size);
                std::memcpy(output.data(), data.data() + pos, size * sizeof(BitContainer));
                pos += size * sizeof(BitContainer);
                return output;

            }
            static std::vector<BitContainer> load_vector(std::ifstream &ifs){
                uint64_t size = 0;
            	ifs.read(reinterpret_cast<char*>(&size), sizeof(uint64_t));

                std::vector<BitContainer> output;
                output.resize(size);
                
            	ifs.read(reinterpret_cast<char*>(output.data()), size * sizeof(BitContainer));
                

                return output;

            }
            
        };
    }

}