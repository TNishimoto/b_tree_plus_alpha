#pragma once
#include "./bp_tree_template.hpp"

namespace stool
{
    namespace bptree
    {
        /**
         * @brief A dynamic data structure storing a permutation [Unchecked AI's Comment]
         * \ingroup PermutationClasses
         */
        class DynamicPermutation
        {
        public:
            using Tree = typename PermutationContainer::Tree;

            using BPIterator = bptree::BPPostorderIterator<PermutationContainer, PermutationItem, bptree::DEFAULT_MAX_DEGREE_OF_INTERNAL_NODE, false>;
            using PermIterator = PermutationContainer::PermutationIterator;
            using NodePointer = bptree::BPNodePointer<PermutationContainer, PermutationItem, bptree::DEFAULT_MAX_DEGREE_OF_INTERNAL_NODE, false>;

        private:
            Tree pi_tree;
            Tree inverse_pi_tree;


        public:
            DynamicPermutation()
            {
                this->pi_tree.set_linked_tree(&this->inverse_pi_tree);
                this->inverse_pi_tree.set_linked_tree(&this->pi_tree);

                //this->set_degree(Tree::DEFAULT_MAX_DEGREE_OF_INTERNAL_NODE);
            }

            ~DynamicPermutation()
            {
                this->clear();
            }
            DynamicPermutation &operator=(const DynamicPermutation &) = delete;
            DynamicPermutation(DynamicPermutation &&other) noexcept
            {
                this->pi_tree = std::move(other.pi_tree);
                this->inverse_pi_tree = std::move(other.inverse_pi_tree);
                this->pi_tree.set_linked_tree(&this->inverse_pi_tree);
                this->inverse_pi_tree.set_linked_tree(&this->pi_tree);
            };
            DynamicPermutation &operator=(DynamicPermutation &&other) noexcept
            {
                if (this != &other)
                {

                    this->pi_tree = std::move(other.pi_tree);
                    this->inverse_pi_tree = std::move(other.inverse_pi_tree);
                    this->pi_tree.set_linked_tree(&this->inverse_pi_tree);
                    this->inverse_pi_tree.set_linked_tree(&this->pi_tree);

                    // other->clear();
                }
                return *this;
            };

        public:
            /*
            void set_degree(uint64_t degree)
            {
                this->pi_tree.initialize(degree);
                this->inverse_pi_tree.initialize(degree);
            }
            */

            void swap(DynamicPermutation &perm)
            {
                // this->pi_tree.set_linked_tree(&perm.inverse_pi_tree);
                // perm.pi_tree.set_linked_tree(&this->inverse_pi_tree);

                // this->inverse_pi_tree.set_linked_tree(&perm.pi_tree);
                // perm.inverse_pi_tree.set_linked_tree(&this->pi_tree);

                this->pi_tree.swap(perm.pi_tree, false);
                this->inverse_pi_tree.swap(perm.inverse_pi_tree, false);
            }
            Tree &get_pi_tree()
            {
                return this->pi_tree;
            }
            Tree &get_inverse_pi_tree()
            {
                return this->inverse_pi_tree;
            }
            uint64_t get_max_degree() const
            {
                return this->pi_tree.get_max_degree_of_internal_node();
            }

            template <typename PI_ITERATOR_BEGIN, typename PI_ITERATOR_END>
            void build(PI_ITERATOR_BEGIN beg, [[maybe_unused]] PI_ITERATOR_END end, uint64_t pi_size, int message_paragraph = stool::Message::SHOW_MESSAGE)
            {

                if (message_paragraph >= 0 && pi_size > 0)
                {
                    std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Constructing Dynamic Permutation... " << "(input size = " << pi_size << ")" << std::endl;
                }
                std::chrono::system_clock::time_point st1, st2;
                st1 = std::chrono::system_clock::now();

                this->clear();
                // this->initialize(degree);
                // this->pi_tree.initialize(degree);
                // this->inverse_pi_tree.initialize(degree);
                this->pi_tree.set_linked_tree(nullptr);
                this->inverse_pi_tree.set_linked_tree(nullptr);

                PermutationItem default_item;
                default_item.key = 0;
                default_item.pointer = 0;
                this->pi_tree.resize(pi_size, default_item);
                this->inverse_pi_tree.resize(pi_size, default_item);

                // stool::Printer::print("items", pi);

                this->pi_tree.set_linked_tree(&this->inverse_pi_tree);
                this->inverse_pi_tree.set_linked_tree(&this->pi_tree);

                uint64_t message_counter = 10000;
                uint64_t processed_node_counter = 0;

                [[maybe_unused]] uint64_t i = 0;
                for (auto it = this->pi_tree.get_postorder_iterator_begin(); it != this->pi_tree.get_postorder_iterator_end(); ++it)
                {
                    message_counter++;
                    processed_node_counter++;
                    if (message_paragraph >= 0 && message_counter > 10000)
                    {

                        std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "Processing... [" << (processed_node_counter / 1000) << "KB] \r" << std::flush;
                        message_counter = 0;
                    }

                    if ((*it).is_leaf())
                    {
                        uint64_t pi_leaf_idx = (*it).get_leaf_container_index();
                        PermutationContainer &leaf = this->pi_tree.get_leaf_container(pi_leaf_idx);
                        std::unordered_map<uint64_t, uint64_t> mapper;
                        for (uint64_t j = 0; j < leaf.size(); j++)
                        {
                            uint64_t idx = this->inverse_pi_tree.compute_path_from_root_to_leaf(*beg);
                            ++beg;
                            const auto &path = this->inverse_pi_tree.get_temporary_path();
                            uint64_t inv_leaf_idx = path[path.size() - 1].get_leaf_container_index();
                            PermutationContainer &inv_leaf = this->inverse_pi_tree.get_leaf_container(inv_leaf_idx);

                            // uint64_t x1 = std::min(pi[i] / half_degree, pi_tree_vec_size - 1);
                            auto f = mapper.find(inv_leaf_idx);
                            if (f == mapper.end())
                            {
                                mapper[inv_leaf_idx] = 0;
                            }
                            uint64_t key = mapper[inv_leaf_idx];
                            leaf.set_value(j, PermutationItem(inv_leaf_idx, key));
                            inv_leaf.set_value(idx, PermutationItem(pi_leaf_idx, key));

                            mapper[inv_leaf_idx] = key + 1;
                            i++;
                        }
                    }
                }

                st2 = std::chrono::system_clock::now();

                if (message_paragraph >= 0 && pi_size > 0)
                {
                    uint64_t sec_time = std::chrono::duration_cast<std::chrono::seconds>(st2 - st1).count();
                    uint64_t ms_time = std::chrono::duration_cast<std::chrono::milliseconds>(st2 - st1).count();
                    uint64_t per_time = ((double)ms_time / (double)pi_size) * 1000000;
                    std::cout << std::endl;

                    std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[END], the number of nodes = " << processed_node_counter  << ", Elapsed Time: " << sec_time << " sec (" << per_time << " ms/MB)" << std::endl;
                }
            }

            void insert(int64_t pi_index, int64_t inverse_pi_index)
            {
                assert(pi_index <= (int64_t)this->pi_tree.size());
                assert(inverse_pi_index<= (int64_t)this->inverse_pi_tree.size());

                uint64_t _size1 = this->inverse_pi_tree.get_leaf_container_vector_size();
                uint64_t _size2 = this->pi_tree.get_leaf_container_vector_size();

                // std::vector<NodePointer>& path1 = const_cast<std::vector<NodePointer>&>(this->pi_tmp_path);
                // std::vector<NodePointer>& path2 = const_cast<std::vector<NodePointer>&>(this->inverse_pi_tmp_path);


                this->pi_tree.insert(pi_index, PermutationItem(_size1 + 1, UINT8_MAX), 0);
                this->inverse_pi_tree.insert(inverse_pi_index, PermutationItem(_size2 + 1, UINT8_MAX), 0);



                // std::vector<NodePointer> path1, path2;
                uint64_t value_idx1 = this->pi_tree.compute_path_from_root_to_leaf(pi_index);
                uint64_t value_idx2 = this->inverse_pi_tree.compute_path_from_root_to_leaf(inverse_pi_index);
                const std::vector<NodePointer> &path1 = this->pi_tree.get_temporary_path();
                const std::vector<NodePointer> &path2 = this->inverse_pi_tree.get_temporary_path();

                uint64_t idx1 = path1[path1.size() - 1].get_leaf_container_index();
                uint64_t idx2 = path2[path2.size() - 1].get_leaf_container_index();
                PermutationContainer &cont1 = this->pi_tree.get_leaf_container(idx1);
                PermutationContainer &cont2 = this->inverse_pi_tree.get_leaf_container(idx2);
                uint64_t key = cont1.get_new_key(idx2);

                cont1.set_value(value_idx1, PermutationItem(idx2, key));
                cont2.set_value(value_idx2, PermutationItem(idx1, key));


            }
            void erase(int64_t pi_index)
            {
                uint64_t idx1 = this->pi_tree.compute_path_from_root_to_leaf(pi_index);
                const std::vector<NodePointer> &path = const_cast<std::vector<NodePointer> &>(this->pi_tree.get_temporary_path());
                uint64_t inverse_pi_index = this->access(path, idx1);
                this->pi_tree.remove_using_path(path, idx1);
                assert(inverse_pi_index < this->inverse_pi_tree.size());
                this->inverse_pi_tree.remove(inverse_pi_index);
                assert(this->pi_tree.size() == this->inverse_pi_tree.size());
            }
            void clear()
            {

                this->pi_tree.set_linked_tree(nullptr);
                this->inverse_pi_tree.set_linked_tree(nullptr);
                this->pi_tree.clear();
                this->inverse_pi_tree.clear();
                this->pi_tree.set_linked_tree(&this->inverse_pi_tree);
                this->inverse_pi_tree.set_linked_tree(&this->pi_tree);
            }

            void move_pi_index(int64_t from, int64_t to)
            {
                int64_t inverse_pi_index = this->access(from);
                if (from < to)
                {
                    this->insert(to + 1, inverse_pi_index);
                    this->erase(from);
                }
                else if (from > to)
                {
                    this->erase(from);
                    this->insert(to, inverse_pi_index);
                }
            }
            void verify() const
            {
                assert(this->pi_tree.get_linked_tree() == &this->inverse_pi_tree);
                assert(this->inverse_pi_tree.get_linked_tree() == &this->pi_tree);
            }
            uint64_t size() const
            {
                return this->pi_tree.size();
            }

            int64_t access(const std::vector<NodePointer> &path, uint64_t position_to_leaf_index) const
            {
                const NodePointer &leaf_pointer = path[path.size() - 1];

                uint64_t leaf_index = leaf_pointer.get_leaf_container_index();
                const PermutationContainer &leaf1 = this->pi_tree.get_leaf_container(leaf_index);
                PermutationItem item = leaf1.at(position_to_leaf_index);

                const PermutationContainer &leaf2 = this->inverse_pi_tree.get_leaf_container(item.pointer);
                uint64_t idx2 = leaf2.get_index(PermutationItem(leaf_index, item.key));

                uint64_t result = this->inverse_pi_tree.get_value_index(item.pointer, idx2);

                return result;
            }

            int64_t access(int64_t pi_index) const
            {

                uint64_t idx1 = this->pi_tree.compute_path_from_root_to_leaf(pi_index);
                const std::vector<NodePointer> &path = const_cast<std::vector<NodePointer> &>(this->pi_tree.get_temporary_path());
                return this->access(path, idx1);
            }
            int64_t inverse(int64_t inverse_pi_index) const
            {

                // std::vector<NodePointer> path;
                uint64_t idx1 = this->inverse_pi_tree.compute_path_from_root_to_leaf(inverse_pi_index);
                const std::vector<NodePointer> &path = const_cast<std::vector<NodePointer> &>(this->inverse_pi_tree.get_temporary_path());

                assert(path.size() > 0);
                const NodePointer &leaf_pointer = path[path.size() - 1];
                uint64_t leaf_index = leaf_pointer.get_leaf_container_index();
                const PermutationContainer &leaf1 = this->inverse_pi_tree.get_leaf_container(leaf_index);
                PermutationItem item = leaf1.at(idx1);

                const PermutationContainer &leaf2 = this->pi_tree.get_leaf_container(item.pointer);
                uint64_t idx2 = leaf2.get_index(PermutationItem(leaf_index, item.key));

                return this->pi_tree.get_value_index(item.pointer, idx2);
            }
            std::string to_string() const
            {
                std::stringstream ss;
                auto vec = this->get_pi_vector();
                ss << stool::ConverterToString::to_integer_string(vec);
                return ss.str();
            }

            std::vector<uint64_t> get_pi_vector() const
            {
                std::vector<uint64_t> r;
                uint64_t size = this->size();
                for (uint64_t i = 0; i < size; i++)
                {
                    r.push_back(this->access(i));
                }

                return r;
            }
            std::vector<uint64_t> get_inverse_pi_vector() const
            {
                std::vector<uint64_t> r;
                uint64_t size = this->size();
                for (uint64_t i = 0; i < size; i++)
                {
                    r.push_back(this->inverse(i));
                }
                return r;
            }
            void print(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "========= DP ========" << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "PI: " << std::endl;
                auto pi_vec = this->get_pi_vector();
                stool::DebugPrinter::print_integers(pi_vec, stool::Message::get_paragraph_string(message_paragraph) + "pi_vector");
                this->pi_tree.print_tree();
                this->pi_tree.print_leaves();
                std::cout << std::endl;
                std::cout << "Inv_PI: " << std::endl;
                auto inv_pi_vec = this->get_inverse_pi_vector();
                stool::DebugPrinter::print_integers(inv_pi_vec, stool::Message::get_paragraph_string(message_paragraph) +"inverse_pi_vector");
                this->inverse_pi_tree.print_tree();
                this->inverse_pi_tree.print_internal_nodes();
                this->inverse_pi_tree.print_leaves();
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "=====================" << std::endl;
            }
            void print_information_about_performance(int message_paragraph = stool::Message::SHOW_MESSAGE) const{
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Performance Information (DynamicPermutation)[" << std::endl;
                this->pi_tree.print_information_about_performance(message_paragraph + 1);
                this->inverse_pi_tree.print_information_about_performance(message_paragraph + 1);
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "]" << std::endl;
            }

            uint64_t size_in_bytes() const
            {
                return this->pi_tree.size_in_bytes() + this->inverse_pi_tree.size_in_bytes();
            }
            std::vector<std::string> get_memory_usage_info(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::vector<std::string> log1 = this->pi_tree.get_memory_usage_info(message_paragraph + 1);
                std::vector<std::string> log2 = this->inverse_pi_tree.get_memory_usage_info(message_paragraph + 1);

                std::vector<std::string> r;
                r.push_back("=Dynamic Permutation: " + std::to_string(this->size_in_bytes()) + " bytes ( " + std::to_string(this->size_in_bytes() / this->size()) + " bytes per element) = ");
                for (std::string &s : log1)
                {
                    r.push_back(s);
                }
                for (std::string &s : log2)
                {
                    r.push_back(s);
                }
                r.push_back("==");
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

            void sort_leaf_containers()
            {
                this->pi_tree.sort_leaf_containers();
                // this->pi_tree.print_leaf_containers();
                // this->inverse_pi_tree.print_leaves();
                this->inverse_pi_tree.sort_leaf_containers();
            }
            void print_statistics(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Statistics(DynamicPermutation):" << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "The size of permutation: " << this->size() << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "Estimated memory usage: " << this->size_in_bytes() << " bytes" << std::endl;
                this->pi_tree.print_statistics(message_paragraph + 1);
                this->inverse_pi_tree.print_statistics(message_paragraph + 1);
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[END]" << std::endl;
            }
            void print_content(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Content(DynamicPermutation):" << std::endl;
                auto pi_vector = this->get_pi_vector();
                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "PI: \t" << stool::ConverterToString::to_integer_string(pi_vector) << std::endl;
                auto inverse_pi_vector = this->get_inverse_pi_vector();
                std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "Inverse PI: \t" << stool::ConverterToString::to_integer_string(inverse_pi_vector) << std::endl;
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[END]" << std::endl;
            }

            static void store_to_bytes(DynamicPermutation &dp, std::vector<uint8_t> &output, uint64_t &pos)
            {
                dp.pi_tree.sort_leaf_containers();
                dp.inverse_pi_tree.sort_leaf_containers();
                Tree::store_to_bytes(dp.pi_tree, output, pos);
                Tree::store_to_bytes(dp.inverse_pi_tree, output, pos);
            }
            static void store_to_file(DynamicPermutation &dp, std::ofstream &os)
            {
                Tree::store_to_file(dp.pi_tree, os);
                Tree::store_to_file(dp.inverse_pi_tree, os);
            }

            static DynamicPermutation load_from_bytes(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                DynamicPermutation r;
                r.pi_tree.load_from_bytes(data, pos);
                r.inverse_pi_tree.load_from_bytes(data, pos);
                return r;
            }

            static DynamicPermutation load_from_file(std::ifstream &ifs)
            {
                DynamicPermutation r;
                r.pi_tree.load_from_file(ifs);
                r.inverse_pi_tree.load_from_file(ifs);
                return r;
            }
        };
    }

}