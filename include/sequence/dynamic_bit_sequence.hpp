#pragma once
#include "../bp_tree/bp_tree.hpp"
#include "./bit_container.hpp"
#include "./bit_forward_iterator.hpp"

namespace stool
{
    namespace sequence
    {
        // template <typename LEAF_CONTAINER, typename T>
        class DynamicBitSequence
        {
            using NodePointer = bptree::BPNodePointer<BitContainer, bool>;
            using T = uint64_t;
            using Tree = bptree::BPTree<BitContainer, bool, false, true>;

            static inline constexpr int DEFAULT_CONTAINER_DEGREE = 62;

        public:
            Tree tree;

            DynamicBitSequence(const DynamicBitSequence &) = delete;
            DynamicBitSequence()
            {
                this->set_degree(Tree::DEFAULT_MAX_DEGREE_OF_INTERNAL_NODE);
            }

            DynamicBitSequence &operator=(const DynamicBitSequence &) = delete;
            DynamicBitSequence(DynamicBitSequence &&) noexcept = default;
            DynamicBitSequence &operator=(DynamicBitSequence &&) noexcept = default;

        public:
            void set_degree(uint64_t degree)
            {
                this->tree.initialize(degree, DynamicBitSequence::DEFAULT_CONTAINER_DEGREE);
            }
            void swap(DynamicBitSequence &item)
            {
                this->tree.swap(item.tree);
            }

            void clear()
            {
                this->tree.clear();
            }

            static DynamicBitSequence build(const std::vector<bool> &items, uint64_t tree_degree)
            {
                DynamicBitSequence r;
                r.tree.initialize(tree_degree, DynamicBitSequence::DEFAULT_CONTAINER_DEGREE);
                r.tree.build(items);
                return r;
            }
            static DynamicBitSequence build(const std::vector<bool> &items)
            {
                return build(items, Tree::DEFAULT_MAX_DEGREE_OF_INTERNAL_NODE);
            }

            /*
            void build_from_data(const std::vector<uint64_t> &data)
            {
                throw -1;
            }
            */

            uint64_t size() const
            {
                return this->tree.size();
            }
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
            void push_front(bool value)
            {
                this->tree.push_front(value);
            }

            void insert(uint64_t pos, bool value)
            {
                this->tree.insert(pos, value, value);
            }
            void remove(uint64_t pos)
            {
                assert(pos < this->size());
                this->tree.remove(pos);
            }
            bool empty() const
            {
                return this->tree.empty();
            }

            BitForwardIterator get_bit_forward_iterator_begin() const
            {
                auto leaf_it = this->tree.get_leaf_forward_iterator_begin();
                return BitForwardIterator(&leaf_it, &this->tree);
            }
            BitForwardIterator get_leaf_forward_iterator_end() const
            {
                return BitForwardIterator(nullptr, &this->tree);
            }

            std::vector<bool> to_vector() const
            {
                uint64_t _size = this->size();
                std::vector<bool> r;
                r.resize(_size, false);
                uint64_t counter = 0;
                auto _end = this->get_leaf_forward_iterator_end();
                for (BitForwardIterator it = this->get_bit_forward_iterator_begin(); it != _end; ++it)
                {
                    uint64_t bits = *it;
                    uint64_t i = 0;
                    while (i < 64 && counter < _size)
                    {
                        bool b = stool::Byte::get_bit(bits, i);
                        r[counter] = b;
                        counter++;
                        i++;
                    }
                }
                assert(counter == _size);
                return r;
            }

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
            bool at(uint64_t pos) const
            {
                return this->tree.at(pos);
            }

            static std::string name()
            {
                std::string s;
                s += "DynamicSequence(";
                s += BitContainer::name();
                s += ")";
                return s;
            }
            void push_many(const std::vector<bool> &items)
            {
                this->tree.push_many(items);
            }

            uint64_t size_in_bytes() const
            {
                return this->tree.size_in_bytes();
            }
            std::vector<std::string> get_memory_usage_info(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::vector<std::string> log1 = this->tree.get_memory_usage_info(message_paragraph+1);

                std::vector<std::string> r;
                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "=DynamicBitSequence: " + std::to_string(this->size_in_bytes()) + " bytes =");
                for (std::string &s : log1)
                {
                    r.push_back(s);
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
            uint64_t psum(uint64_t x) const
            {
                return this->tree.psum(x);
            }
            uint64_t psum() const
            {
                return this->tree.psum();
            }
            int64_t search(uint64_t sum) const
            {
                return this->tree.search(sum);
            }
            std::string to_string() const
            {
                std::string s;
                std::vector<uint64_t> bits = this->to_value_vector();
                s.push_back('[');
                for (uint64_t i = 0; i < bits.size(); i++)
                {
                    s.push_back(bits[i] >= 1 ? '1' : '0');
                }
                s.push_back(']');
                return s;
            }

            int64_t rank1(uint64_t i) const
            {
                if (i == 0)
                {
                    return 0;
                }
                else if (i <= this->size())
                {
                    return this->tree.psum(i - 1);
                }
                else
                {
                    assert(false);
                    throw std::range_error("Error: DynamicBitSequence::rank1()");
                }
            }
            int64_t rank0(uint64_t i) const
            {
                return i - this->rank1(i);
            }
            int64_t rank(uint64_t i, bool c) const
            {
                return c ? this->rank1(i) : this->rank0(i);
            }

            int64_t select1(uint64_t i) const
            {
                return this->tree.search(i + 1);
            }
            int64_t select0(uint64_t i) const
            {
                return this->tree.select0(i);
            }
            int64_t select(uint64_t i, bool c) const
            {
                return c ? this->select1(i) : this->select0(i);
            }
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

            bool operator[](uint64_t n) const
            {
                return this->at(n);
            }
            void print_statistics(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Statistics(DynamicBitSequence):" << std::endl;
                this->tree.print_statistics(message_paragraph + 1);
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[END]" << std::endl;
            }
            void print_content(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Content(DynamicBitSequence):" << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph+1) << "Bits: " << this->to_string() << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[END]" << std::endl;
            }
            static void save(DynamicBitSequence &item, std::vector<uint8_t> &output, uint64_t &pos)
            {
                item.tree.save(output, pos);
            }
            static void save(DynamicBitSequence &item, std::ofstream &os)
            {
                item.tree.save(os);
            }

            static DynamicBitSequence build_from_data(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                DynamicBitSequence r;
                r.tree.build_from_data(data, pos);
                return r;
            }

            static DynamicBitSequence build_from_data(std::ifstream &ifs)
            {
                DynamicBitSequence r;
                r.tree.build_from_data(ifs);
                return r;
            }

            void sort_leaf_containers()
            {
                bool b = this->tree.check_if_leaf_container_vec_is_sorted();
                if (!b)
                {
                    std::cout << "unsorted" << std::endl;
                    this->tree.sort_leaf_containers();
                }
            }
        };

        // template <typename T>
        // using VLCDequeSeq = DynamicSequence<VLCDeque, T>;
    }

}
