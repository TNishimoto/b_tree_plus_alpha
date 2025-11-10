#pragma once
#include "../bp_tree/bp_tree.hpp"

namespace stool
{
    namespace bptree
    {
        /**
         * @brief A container stored in the BPTree of SPSI. The values of this container are stored in a vector. [Unchecked AI's Comment]
         * \ingroup PrefixSumClasses
         */
        class PlainSPSIContainer
        {
            std::vector<uint64_t> items;

        public:
            PlainSPSIContainer()
            {
            }
            PlainSPSIContainer(std::vector<uint64_t> &_items)
            {
                for(uint64_t x : _items){
                    this->items.push_back(x);
                }
            }


            uint64_t size() const
            {
                return this->items.size();
            }
            uint64_t size_in_bytes() const
            {
                return sizeof(std::vector<uint64_t>) + (this->items.capacity() * sizeof(uint64_t));
            }

            uint64_t at(uint64_t pos) const
            {
                return this->items[pos];
            }
            void print() const
            {
            }

            void clear()
            {
                this->items.clear();
                this->items.shrink_to_fit();
            }
            void swap(PlainSPSIContainer &item)
            {
                this->items.swap(item.items);
            }
            static std::string name()
            {
                return "plain integers";
            }
            uint64_t psum(uint64_t i) const noexcept
            {
                uint64_t _sum = 0;
                assert(i < this->items.size());

                for (uint64_t j = 0; j < this->items.size(); j++)
                {
                    _sum += this->items[j];

                    if (j == i)
                    {
                        // stool::psum_count += i;
                        return _sum;
                    }
                }
                return _sum;
            }
            uint64_t psum() const noexcept
            {
                if (this->items.size() == 0)
                {
                    return 0;
                }
                else
                {
                    return this->psum(this->items.size() - 1);
                }
            }
            int64_t search(uint64_t x) const noexcept
            {
                uint64_t _sum = 0;
                for (uint64_t i = 0; i < this->items.size(); i++)
                {
                    _sum += this->items[i];

                    if (_sum >= x)
                    {
                        // stool::search_count += i;

                        return i;
                    }
                }
                return -1;
            }

            std::string to_string() const
            {
                std::string s;
                s.push_back('[');
                for (uint64_t i = 0; i < this->items.size(); i++)
                {
                    s += std::to_string(this->items[i]);
                    if (i + 1 < this->items.size())
                    {
                        s += ", ";
                    }
                }
                s.push_back(']');
                return s;
            }
            std::vector<uint64_t> to_value_vector() const
            {
                std::vector<uint64_t> r;
                for (uint64_t i : this->items)
                {
                    r.push_back(i);
                }
                return r;
            }
            std::vector<uint64_t> to_packed_vector() const
            {
                return this->to_value_vector();
            }

            template<typename VEC>
            void to_values(VEC &output_vec) const{
                output_vec.clear();
                output_vec.resize(this->size());
                for (uint64_t i = 0; i < this->size(); i++)
                {
                    output_vec[i] = this->items[i];
                }
            }


            void insert(uint64_t pos, uint64_t value)
            {
                assert(pos <= this->size());
                this->items.insert(this->items.begin() + pos, value);
            }
            void remove(uint64_t pos)
            {
                assert(pos < this->size());

                this->items.erase(this->items.begin() + pos);
            }
            void push_front(std::vector<uint64_t> &new_items)
            {
                std::vector<uint64_t> tmp;
                for (uint64_t v : new_items)
                {
                    tmp.push_back(v);
                }
                for (uint64_t v : this->items)
                {
                    tmp.push_back(v);
                }
                this->items.swap(tmp);
            }
            void push_front(uint64_t new_item)
            {

                std::vector<uint64_t> tmp;
                tmp.push_back(new_item);
                for (uint64_t v : this->items)
                {
                    tmp.push_back(v);
                }
                this->items.swap(tmp);
            }

            void push_back(std::vector<uint64_t> &new_items)
            {
                for (uint64_t v : new_items)
                {
                    this->items.push_back(v);
                }
            }
            void push_back(uint64_t value)
            {
                this->items.push_back(value);
            }
            std::vector<uint64_t> pop_front(uint64_t len)
            {

                std::vector<uint64_t> tmp1, tmp2;
                for (uint64_t i = 0; i < len; i++)
                {
                    tmp1.push_back(this->items[i]);
                }
                for (uint64_t i = len; i < this->items.size(); i++)
                {
                    tmp2.push_back(this->items[i]);
                }
                this->items.swap(tmp2);
                return tmp1;
            }
            std::vector<uint64_t> pop_back(uint64_t len)
            {

                std::vector<uint64_t> tmp1;
                tmp1.resize(len);
                int64_t k = len - 1;
                while (k >= 0)
                {
                    assert(this->items.size() > 0);
                    tmp1[k--] = this->items[this->items.size() - 1];
                    this->items.pop_back();
                }
                return tmp1;
            }

            uint64_t reverse_psum(uint64_t i) const
            {
                size_t len = i + 1;
                uint64_t sum = 0;
                size_t size = this->items.size();
                for (size_t x = 0; x < len; x++)
                {
                    sum += this->items[size - 1 - x];
                }
                return sum;
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
                this->items[i] += delta;
            }

            std::vector<uint64_t>::const_iterator begin() const {
                return this->items.begin();
            }

            std::vector<uint64_t>::const_iterator end() const {
                return this->items.end();
            }

            int64_t one_based_rank(uint64_t i, uint64_t c) const {
                return stool::StringFunctions::rank_query(this->items, i, c);
            }
            int64_t one_based_rank0(uint64_t i) const {
                return this->one_based_rank(0, i);
            }
            int64_t one_based_rank1(uint64_t i) const {
                return this->one_based_rank(1, i);
            }

            int64_t select(uint64_t i, uint64_t c) const {
                return stool::StringFunctions::select_query(this->items, i, c);
            }
            int64_t select0(uint64_t i) const {
                return this->select(0, i);
            }
            int64_t select1(uint64_t i) const {
                return this->select(1, i);
            }
            void sort_leaf_containers()
            {
            }
            void verify() const{
                
            }



        };
    }

}