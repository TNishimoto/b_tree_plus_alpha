#pragma once
#include "./permutation_item.hpp"

namespace stool
{
    namespace bptree
    {
        ////////////////////////////////////////////////////////////////////////////////
        /// @class      PermutationContainer
        /// @brief      The container stored in the BPTree of DynamicPermutation
        ///
        ////////////////////////////////////////////////////////////////////////////////
        class PermutationContainer
        {
            //using Tree = bptree::BPTree<PermutationContainer, PermutationItem, true, false, bptree::DEFAULT_MAX_DEGREE_OF_INTERNAL_NODE, bptree::DEFAULT_MAX_COUNT_OF_VALUES_IN_LEAF>;

            stool::NaiveFLCVector<false> keys;
            stool::NaiveFLCVector<false> pointers;

        public:
            static constexpr uint64_t ___PermutationLeafSize = 252;
            using Tree = bptree::BPTree<PermutationContainer, PermutationItem, true, false, bptree::DEFAULT_MAX_DEGREE_OF_INTERNAL_NODE, ___PermutationLeafSize>;

        ////////////////////////////////////////////////////////////////////////////////
            /// @class      PermutationIterator
            /// @brief      The forward iterator of values stored in PermutationContainer
            ///
            ////////////////////////////////////////////////////////////////////////////////
            class PermutationIterator
            {

            public:
                PermutationContainer *_m_deq;
                uint16_t _m_idx;

                using iterator_category = std::bidirectional_iterator_tag;
                using difference_type = std::ptrdiff_t;
                PermutationIterator(PermutationContainer *_deque, uint16_t _idx) : _m_deq(_deque), _m_idx(_idx) {}

                PermutationItem operator*() const
                {
                    return _m_deq->at(this->_m_idx);
                }

                PermutationIterator &operator++()
                {
                    assert(this->_m_idx < this->_m_deq->size());

                    ++this->_m_idx;
                    return *this;
                }

                PermutationIterator operator++(int)
                {
                    PermutationIterator temp = *this;

                    ++(*this);
                    return temp;
                }

                PermutationIterator &operator--()
                {
                    --this->_m_idx;
                    return *this;
                }

                PermutationIterator operator--(int)
                {
                    PermutationIterator temp = *this;
                    --(*this);
                    return temp;
                }
                // ランダムアクセス演算子（i + n）
                PermutationIterator operator+(difference_type n) const
                {
                    int16_t sum = (int16_t)this->_m_idx + (int16_t)n;
                    assert((int64_t)sum < (int64_t)this->_m_deq->size());

                    return PermutationIterator(this->_m_deq, sum);
                }

                // ランダムアクセス演算子（i - n）
                PermutationIterator operator-(difference_type n) const
                {
                    int16_t sum = (int16_t)this->_m_idx - (int16_t)n;
                    return PermutationIterator(this->_m_deq, sum);
                }

                // イテレータ同士の差（i - j）
                difference_type operator-(const PermutationIterator &other) const
                {
                    return (int16_t)this->_m_idx - (int16_t)other._m_idx;
                }
                /*
                void change_element_without_changing_psum(PermutationItem item)
                {
                    this->_m_deq->keys[this->_m_idx] = item.key;
                    this->_m_deq->pointers.set_value(this->_m_idx, item.pointer);
                    //this->_m_deq->pointers[this->_m_idx] = item.pointer;
                }
                */

                bool operator==(const PermutationIterator &other) const { return _m_idx == other._m_idx; }
                bool operator!=(const PermutationIterator &other) const { return _m_idx != other._m_idx; }
                bool operator<(const PermutationIterator &other) const { return this->_m_idx < other._m_idx; }
                bool operator>(const PermutationIterator &other) const { return this->_m_idx > other._m_idx; }
                bool operator<=(const PermutationIterator &other) const { return this->_m_idx <= other._m_idx; }
                bool operator>=(const PermutationIterator &other) const { return this->_m_idx >= other._m_idx; }
            };

            PermutationContainer()
            {
            }

            PermutationIterator begin() const
            {
                return PermutationIterator(const_cast<PermutationContainer *>(this), 0);
            }
            PermutationIterator end() const
            {
                return PermutationIterator(const_cast<PermutationContainer *>(this), this->size());
            }

            uint64_t size() const
            {
                return this->pointers.size();
            }
            uint64_t size_in_bytes(bool only_extra_bytes) const
            {
                if(only_extra_bytes){
                    return this->keys.size_in_bytes(true) + this->pointers.size_in_bytes(true);
                }else{
                    return sizeof(PermutationContainer) + this->keys.size_in_bytes(true) + this->pointers.size_in_bytes(true);
                }
            }
            uint64_t unused_size_in_bytes() const
            {
                return this->keys.unused_size_in_bytes() + this->pointers.unused_size_in_bytes();
            }


            PermutationItem at(uint64_t pos) const
            {
                assert(pos < this->keys.size());
                assert(pos < this->pointers.size());

                return PermutationItem(this->pointers.at(pos), this->keys[pos]);
            }
            void print() const
            {
                std::cout << this->to_string() << std::endl;
            }

            void clear()
            {
                this->keys.clear();
                this->pointers.clear();

                assert(this->keys.size() == this->pointers.size());
            }
            void swap(PermutationContainer &item)
            {
                assert(item.keys.size() == item.pointers.size());

                this->keys.swap(item.keys);
                this->pointers.swap(item.pointers);

                assert(this->keys.size() == this->pointers.size());
            }
            static std::string name()
            {
                return "X";
            }
            uint64_t psum([[maybe_unused]] uint64_t i) const noexcept
            {
                return 0;
            }
            uint64_t psum() const noexcept
            {
                return 0;
            }
            int64_t search([[maybe_unused]] uint64_t x) const noexcept
            {
                return -1;
            }
            int64_t get_index(const PermutationItem &&item) const
            {
                int64_t size = this->pointers.size();
                for(int64_t i = 0; i < size; i++){
                    if(this->pointers[i] == item.pointer && this->keys[i] == item.key){
                        return i;
                    }
                }
                return -1;

                /*
                for (uint64_t pointer : this->pointers)
                {
                    if (pointer == item.pointer && this->keys[i] == item.key)
                    {
                        return i;
                    }
                    i++;
                }
                */
            }

            std::string to_string() const
            {
                assert(this->keys.size() == this->pointers.size());

                std::string s;
                s.push_back('[');

                int64_t size = this->pointers.size();
                for(int64_t i = 0; i < size; i++){
                    s += "(" + std::to_string(this->pointers[i]) + ", " + std::to_string(this->keys[i]) + ")";
                    if (i + 1 < (int64_t)this->size())
                    {
                        s += ", ";
                    }
                }

                s.push_back(']');
                return s;
            }

            std::vector<PermutationItem> to_value_vector() const
            {
                throw std::runtime_error("PermutationContainer::NO IMP1");
            }

            void insert(uint64_t pos, PermutationItem value)
            {
                this->keys.insert(pos, value.key);
                this->pointers.insert(pos, value.pointer);

                assert(this->keys.size() == this->pointers.size());
            }
            void remove(uint64_t pos)
            {
                this->keys.remove(pos);
                this->pointers.remove(pos);

                assert(this->keys.size() == this->pointers.size());
            }
            void push_front(const std::vector<PermutationItem> &new_items)
            {
                std::vector<uint64_t> tmp_keys;
                std::vector<uint64_t> tmp_pointers;
                tmp_keys.resize(new_items.size());
                tmp_pointers.resize(new_items.size());

                for (int64_t i = 0; i < (int64_t)new_items.size(); i++)
                {
                    tmp_keys[i] = new_items[i].key;
                    tmp_pointers[i] = new_items[i].pointer;
                }
                this->keys.push_front(tmp_keys);
                this->pointers.push_front(tmp_pointers);

                /*
                for (int64_t i = new_items.size() - 1; i >= 0; i--)
                {
                    this->keys.push_front(new_items[i].key);
                    this->pointers.push_front(new_items[i].pointer);
                }
                */

                assert(this->keys.size() == this->pointers.size());
            }
            void push_front(const PermutationItem &new_item)
            {
                this->keys.push_front(new_item.key);
                this->pointers.push_front(new_item.pointer);

                assert(this->keys.size() == this->pointers.size());
            }

            void push_back(const std::vector<PermutationItem> &new_items)
            {
                for (const PermutationItem &item : new_items)
                {
                    this->keys.push_back(item.key);
                    this->pointers.push_back(item.pointer);
                }

                assert(this->keys.size() == this->pointers.size());
            }
            void push_back(PermutationItem value)
            {
                this->keys.push_back(value.key);
                this->pointers.push_back(value.pointer);

                assert(this->keys.size() == this->pointers.size());
            }
            PermutationItem pop_front()
            {
                uint64_t key = this->keys[0];
                uint64_t pointer = this->pointers.head();
                this->keys.pop_front();
                this->pointers.pop_front();

                assert(this->keys.size() == this->pointers.size());
                return PermutationItem(pointer, key);
            }

            std::vector<PermutationItem> pop_front(uint64_t len)
            {
                std::vector<PermutationItem> r;
                r.resize(len);

                for (uint64_t i = 0; i < len; i++)
                {
                    uint64_t key = this->keys[i];
                    uint64_t pointer = this->pointers[i];
                    r[i] = PermutationItem(pointer, key);
                }
                this->keys.pop_front(len);
                this->pointers.pop_front(len);

                assert(this->keys.size() == this->pointers.size());
                return r;

            }
            PermutationItem pop_back()
            {
                assert(this->keys.size() > 0 && this->pointers.size() > 0);
                uint64_t key = this->keys[this->keys.size() - 1];
                uint64_t pointer = this->pointers.tail();
                this->keys.pop_back();
                this->pointers.pop_back();

                assert(this->keys.size() == this->pointers.size());
                return PermutationItem(pointer, key);
            }

            std::vector<PermutationItem> pop_back(uint64_t len)
            {
                std::vector<PermutationItem> tmp1;
                tmp1.resize(len);
                int64_t k = len - 1;
                while (k >= 0)
                {
                    tmp1[k--] = this->pop_back();
                }

                assert(this->keys.size() == this->pointers.size());
                return tmp1;
            }
            uint64_t reverse_psum([[maybe_unused]] uint64_t i) const
            {
                return 0;
            }
            uint64_t psum([[maybe_unused]] uint64_t i, [[maybe_unused]] uint64_t j) const
            {
                return 0;
            }
            uint8_t get_new_key(uint64_t pointer_to_linked_container) const
            {
                std::array<uint64_t, 4> occurrence_flag_array;
                occurrence_flag_array[0] = 0;
                occurrence_flag_array[1] = 0;
                occurrence_flag_array[2] = 0;
                occurrence_flag_array[3] = 0;


                int64_t size = this->pointers.size();
                for(int64_t i = 0; i < size; i++){
                    uint64_t pointer = this->pointers[i];                    
                    if (pointer == pointer_to_linked_container)
                    {
                        uint64_t idx = this->keys[i] / 64;
                        uint64_t geta = idx * 64;

                        occurrence_flag_array[idx] = occurrence_flag_array[idx] | (1LL << (63 - (this->keys[i] - geta)));
                    }

                }

                for (uint64_t i = 0; i < 4; i++)
                {
                    if (occurrence_flag_array[i] != UINT64_MAX)
                    {
                        uint64_t code_len = stool::LSBByte::get_code_length(~occurrence_flag_array[i]);
                        return (64 * i) + (64 - code_len);
                    }
                }
                throw std::invalid_argument("Error(PermutationIterator::get_new_key)");
            }

            void update_linked_tree(uint64_t ith, uint64_t leaf_index_of_this_container, uint64_t previous_leaf_index, Tree &linked_tree)
            {
                PermutationItem item = this->at(ith);
                assert(item.pointer < linked_tree.get_leaf_container_vector_size());
                PermutationContainer &container = linked_tree.get_leaf_container(item.pointer);
                int64_t idx = container.get_index(PermutationItem(previous_leaf_index, item.key));
                assert(idx != -1);
                uint8_t new_key = container.get_new_key(leaf_index_of_this_container);
                // std::cout << "##" << (uint64_t)this << "/" << previous_leaf_index << "/" << (int)item.key << " -> " << (uint64_t)new_key << "/" << leaf_index_of_this_container << std::endl;
                // this->print();
                // container.print();

                assert(idx < (int64_t)container.size());
                container.pointers.set_value(idx, leaf_index_of_this_container);
                container.keys.set_value(idx, new_key);
                this->keys.set_value(ith, new_key);

                assert(this->keys.size() == this->pointers.size());
            }
            void set_value(uint64_t ith, const PermutationItem &&item)
            {
                assert(ith < this->size());
                this->keys.set_value(ith, item.key);
                this->pointers.set_value(ith, item.pointer);

                assert(this->keys.size() == this->pointers.size());
            }

            static uint64_t get_byte_size(const PermutationContainer &item)
            {
                return NaiveFLCVector<false>::get_byte_size(item.keys) + NaiveFLCVector<false>::get_byte_size(item.pointers);
            }

            static uint64_t get_byte_size(const std::vector<PermutationContainer> &items)
            {
                uint64_t bytes = sizeof(uint64_t);
                for (auto &it : items)
                {
                    bytes += PermutationContainer::get_byte_size(it);
                }
                return bytes;
            }

            static void save(const PermutationContainer &item, std::vector<uint8_t> &output, uint64_t &pos)
            {
                NaiveFLCVector<false>::save(item.keys, output, pos);
                NaiveFLCVector<false>::save(item.pointers, output, pos);
            }
            static void save(const PermutationContainer &item, std::ofstream &os)
            {
                NaiveFLCVector<false>::save(item.keys, os);
                NaiveFLCVector<false>::save(item.pointers, os);
            }

            static void save(const std::vector<PermutationContainer> &items, std::vector<uint8_t> &output, uint64_t &pos)
            {
                uint64_t size = items.size();
                std::memcpy(output.data() + pos, &size, sizeof(size));
                pos += sizeof(size);

                for (auto &it : items)
                {
                    PermutationContainer::save(it, output, pos);
                }
            }
            static void save(const std::vector<PermutationContainer> &items, std::ofstream &os)
            {
                uint64_t size = items.size();
                os.write(reinterpret_cast<const char *>(&size), sizeof(size));
                for (auto &it : items)
                {
                    PermutationContainer::save(it, os);
                }
            }

            static PermutationContainer load(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                PermutationContainer r;
                NaiveFLCVector<false> tmp1 = NaiveFLCVector<false>::load(data, pos);
                NaiveFLCVector<false> tmp2 = NaiveFLCVector<false>::load(data, pos);
                r.keys.swap(tmp1);
                r.pointers.swap(tmp2);

                return r;
            }
            static PermutationContainer load(std::ifstream &ifs)
            {
                PermutationContainer r;
                NaiveFLCVector<false> tmp1 = NaiveFLCVector<false>::load(ifs);
                NaiveFLCVector<false> tmp2 = NaiveFLCVector<false>::load(ifs);
                r.keys.swap(tmp1);
                r.pointers.swap(tmp2);

                return r;
            }

            static std::vector<PermutationContainer> load_vector(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                uint64_t size;
                std::memcpy(&size, data.data() + pos, sizeof(size));
                pos += sizeof(size);

                std::vector<PermutationContainer> r;
                for (uint64_t i = 0; i < size; i++)
                {
                    r.push_back(PermutationContainer::load(data, pos));
                }
                return r;
            }
            static std::vector<PermutationContainer> load_vector(std::ifstream &ifs)
            {
                uint64_t size = 0;
                ifs.read(reinterpret_cast<char *>(&size), sizeof(size));

                std::vector<PermutationContainer> r;
                for (uint64_t i = 0; i < size; i++)
                {
                    r.push_back(PermutationContainer::load(ifs));
                }
                return r;
            }
        };
    }
}
