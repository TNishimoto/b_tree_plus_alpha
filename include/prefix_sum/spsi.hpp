#pragma once
#include "./plain_spsi_container.hpp"
#include "stool/include/light_stool.hpp"

namespace stool
{
    namespace prefix_sum
    {

        ////////////////////////////////////////////////////////////////////////////////
        /// @class      SPSI
        /// @brief      A dynamic data structure supporting prefix-sum query
        ///
        ////////////////////////////////////////////////////////////////////////////////
        template <typename LEAF_CONTAINER>
        class SPSI
        {
        public:
            using NodePointer = bptree::BPNodePointer<LEAF_CONTAINER, uint64_t>;
            using Tree = bptree::BPTree<LEAF_CONTAINER, uint64_t, false, true>;

        private:
            Tree tree;

        public:
            typename Tree::ValueForwardIterator begin() const
            {
                return this->tree.get_value_forward_iterator_begin();
            }
            typename Tree::ValueForwardIterator end() const
            {
                return this->tree.get_value_forward_iterator_end();
            }

            SPSI()
            {
                this->tree.initialize(Tree::DEFAULT_MAX_DEGREE_OF_INTERNAL_NODE);
            }
            void swap(SPSI &item)
            {
                this->tree.swap(item.tree);
            }

            SPSI &operator=(const SPSI &) = delete;
            SPSI(SPSI &&) noexcept = default;
            SPSI &operator=(SPSI &&) noexcept = default;

        public:
            void set_degree(uint64_t degree)
            {
                this->tree.initialize(degree);
            }

            void clear()
            {
                this->tree.clear();
            }
            void verify() const
            {
                this->tree.verify();
            }
            Tree &get_tree()
            {
                return this->tree;
            }
            uint64_t get_degree() const
            {
                return this->tree.get_max_degree_of_internal_node();
            }

            static SPSI build(const std::vector<uint64_t> &items, uint64_t tree_degree = Tree::DEFAULT_MAX_DEGREE_OF_INTERNAL_NODE)
            {
                SPSI r;
                r.tree.initialize(tree_degree, tree_degree);
                r.tree.build(items);
                return r;
            }

            uint64_t size() const
            {
                return this->tree.size();
            }
            void push_back(uint64_t value)
            {
                this->tree.push_back(value);
            }
            void push_front(uint64_t value)
            {
                this->tree.push_front(value);
            }

            void insert(uint64_t pos, uint64_t value)
            {
                this->tree.insert(pos, value, value);
            }
            void remove(uint64_t pos)
            {
                this->tree.remove(pos);
            }
            std::vector<uint64_t> to_vector() const
            {
                return this->tree.to_value_vector();
            }

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

            /*
             * returns the sum of the first i values stored in this data structure.
             */
            uint64_t psum(uint64_t i) const
            {
                return this->tree.psum(i);
            }

            /*
             * returns the sum of the stored values.
             */
            uint64_t psum() const
            {
                return this->tree.psum();
            }
            /*
             * returns smallest i such that psum(i) >= x
             */
            int64_t search(uint64_t x) const
            {
                return this->tree.search(x);
            }
            uint64_t at(uint64_t pos) const
            {
                return this->tree.at(pos);
            }
            void increment(uint64_t i, int64_t delta)
            {
                this->tree.increment(i, delta);
            }
            void decrement(uint64_t i, int64_t delta)
            {
                this->tree.increment(i, -delta);
            }

            static std::string name()
            {
                std::string s;
                s += "SPSI(";
                s += LEAF_CONTAINER::name();
                s += ")";
                return s;
            }
            void push_many(const std::vector<uint64_t> &items)
            {
                this->tree.push_many(items);
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

            uint64_t size_in_bytes() const
            {
                return this->tree.size_in_bytes();
            }
            std::vector<std::string> get_memory_usage_info(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::vector<std::string> log1 = this->tree.get_memory_usage_info(message_paragraph + 1);

                std::vector<std::string> r;
                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "=SPSI: " + std::to_string(this->size_in_bytes()) + " bytes =");
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
            void print_statistics(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Statistics(SPSI):" << std::endl;
                this->tree.print_statistics(message_paragraph + 1);
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[END]" << std::endl;
            }

            static void save(SPSI &item, std::vector<uint8_t> &output, uint64_t &pos)
            {
                item.tree.save(output, pos);
            }
            static void save(SPSI &item, std::ofstream &os)
            {
                item.tree.save(os);
            }

            static SPSI build_from_data(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                SPSI r;
                r.tree.build_from_data(data, pos);
                return r;
            }

            static SPSI build_from_data(std::ifstream &ifs)
            {
                SPSI r;
                r.tree.build_from_data(ifs);
                return r;
            }
            uint64_t operator[](uint64_t n) const
            {
                return this->tree.at(n);
            }

            void print() const
            {
                auto vec = this->to_vector();
                std::cout << "================== SPSI ==================" << std::endl;
                stool::DebugPrinter::print_integers(vec, "values");

                std::cout << "================== SPSI[END] =============" << std::endl;
            }
        };

        using PlainSPSI = SPSI<PlainSPSIContainer>;
        // using GapEncodedSPSI = SPSI<GapEncodedContainer>;
        using VLCDequeSPSI = SPSI<VLCDeque>;
    }

}