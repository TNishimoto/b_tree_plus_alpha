#pragma once
#include "../bp_tree/bp_tree.hpp"

namespace stool
{
    namespace bptree
    {
        ////////////////////////////////////////////////////////////////////////////////
        /// @brief      A dynamic data structure that maintains a sequence of 64-bit non-negative integers
        ///
        ////////////////////////////////////////////////////////////////////////////////
        class DynamicSequence64{
        public:

            using NodePointer = bptree::BPNodePointer<VLCDeque, uint64_t>;
            using Tree = bptree::BPTree<VLCDeque, uint64_t, false, false>;
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

            DynamicSequence64()
            {
                this->tree.initialize(Tree::DEFAULT_MAX_DEGREE_OF_INTERNAL_NODE);
            }
            void swap(DynamicSequence64 &item)
            {
                this->tree.swap(item.tree);
            }

            DynamicSequence64 &operator=(const DynamicSequence64 &) = delete;
            DynamicSequence64(DynamicSequence64 &&) noexcept = default;
            DynamicSequence64 &operator=(DynamicSequence64 &&) noexcept = default;

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

            static DynamicSequence64 build(const std::vector<uint64_t> &items, uint64_t tree_degree = Tree::DEFAULT_MAX_DEGREE_OF_INTERNAL_NODE)
            {
                DynamicSequence64 r;
                r.tree.initialize(tree_degree, tree_degree);
                r.tree.build(items);
                return r;
            }

            uint64_t size() const
            {
                return this->tree.size();
            }
            void push_back(int64_t value)
            {
                this->tree.push_back(value);
            }
            void push_front(int64_t value)
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

            uint64_t at(uint64_t pos) const
            {
                return this->tree.at(pos);
            }
            void set_value(uint64_t pos, uint64_t value){
                uint64_t v = this->at(pos);
                if(v < value){
                    this->increment(pos, value - v);
                }else if(v > value){
                    this->decrement(pos, v - value);
                }
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
                s += VLCDeque::name();
                s += ")";
                return s;
            }
            void push_many(const std::vector<uint64_t> &items)
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
                r.push_back(stool::Message::get_paragraph_string(message_paragraph)  + "=DynamicSequence64: " + std::to_string(this->size_in_bytes()) + " bytes =");
                for (std::string &s : log1)
                {
                    r.push_back(s);
                }
                r.push_back(stool::Message::get_paragraph_string(message_paragraph)  + "==");
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
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Statistics(DynamicSequence64):" << std::endl;
                this->tree.print_statistics(message_paragraph + 1);
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[END]" << std::endl;
            }

            static void save(DynamicSequence64 &item, std::vector<uint8_t> &output, uint64_t &pos)
            {
                item.tree.save(output, pos);
            }
            static void save(DynamicSequence64 &item, std::ofstream &os)
            {
                item.tree.save(os);
            }

            static DynamicSequence64 build_from_data(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                DynamicSequence64 r;
                r.tree.build_from_data(data, pos);
                return r;
            }

            static DynamicSequence64 build_from_data(std::ifstream &ifs)
            {
                DynamicSequence64 r;
                r.tree.build_from_data(ifs);
                return r;
            }
            uint64_t operator[](uint64_t n) const
            {
                return this->tree.at(n);
            }
            std::string to_string() const
            {
                std::stringstream ss;
                auto vec = this->to_vector();
                ss << stool::DebugPrinter::to_integer_string(vec);
                return ss.str();
            }

            void print() const
            {
                auto vec = this->to_vector();
                std::cout << "================== SPSI ==================" << std::endl;
                stool::DebugPrinter::print_integers(vec, "values");

                std::cout << "================== SPSI[END] =============" << std::endl;
            }


        };
    }
}
