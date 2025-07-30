#pragma once
#include "./dynamic_permutation.hpp"

namespace stool
{
    namespace bptree
    {
        ////////////////////////////////////////////////////////////////////////////////
        /// @class      DynamicPermutationBuilder
        /// @brief      A builder of DynamicPermutation
        ///
        ////////////////////////////////////////////////////////////////////////////////
        class DynamicPermutationBuilder
        {
            using PostorderIterator = DynamicPermutation::Tree::PostorderIterator;
            // PostorderIterator postorder_iterator_beg;
            // PostorderIterator postorder_iterator_end;
            int64_t i;
            int64_t j;
            int64_t inverse_leaf_index;
            DynamicPermutation *dp;
            DynamicPermutation::Tree *pi_tree;
            DynamicPermutation::Tree *inverse_pi_tree;
            std::unordered_map<uint64_t, uint64_t> mapper;

        public:
            void initialize(DynamicPermutation &perm, uint64_t pi_vector_size)
            {
                DynamicPermutation::Tree& pi_tree = perm.get_pi_tree();
                DynamicPermutation::Tree& inverse_pi_tree = perm.get_inverse_pi_tree();

                pi_tree.initialize();
                inverse_pi_tree.initialize();
                pi_tree.set_linked_tree(nullptr);
                inverse_pi_tree.set_linked_tree(nullptr);

                PermutationItem default_item;
                default_item.key = 0;
                default_item.pointer = 0;
                pi_tree.resize(pi_vector_size, default_item);
                inverse_pi_tree.resize(pi_vector_size, default_item);

                pi_tree.set_linked_tree(&inverse_pi_tree);
                inverse_pi_tree.set_linked_tree(&pi_tree);

                this->i = 0;
                this->dp = &perm;
                this->pi_tree = &perm.get_pi_tree();
                this->inverse_pi_tree = &perm.get_inverse_pi_tree();

                if (this->inverse_pi_tree->size() > 0)
                {
                    this->j = this->inverse_pi_tree->compute_path_from_root_to_leaf(this->inverse_pi_tree->size() - 1);
                    const auto &path = this->inverse_pi_tree->get_temporary_path();
                    this->inverse_leaf_index = path[path.size() - 1].get_leaf_container_index();
                    this->mapper.clear();
                }
                else
                {
                    this->j = -1;
                }
            }
            void push_front(uint64_t inverse_pi_value)
            {
                if (this->j == -1)
                {
                    this->j = this->inverse_pi_tree->compute_path_from_root_to_leaf(this->inverse_pi_tree->size() - 1 - this->i);
                    const auto &path = this->inverse_pi_tree->get_temporary_path();
                    this->inverse_leaf_index = path[path.size() - 1].get_leaf_container_index();
                    this->mapper.clear();
                }

                uint64_t idx = this->pi_tree->compute_path_from_root_to_leaf(inverse_pi_value);
                const auto &path = this->pi_tree->get_temporary_path();
                uint64_t pi_leaf_idx = path[path.size() - 1].get_leaf_container_index();
                PermutationContainer &pi_leaf = this->pi_tree->get_leaf_container(pi_leaf_idx);

                auto f = mapper.find(pi_leaf_idx);
                if (f == mapper.end())
                {
                    mapper[pi_leaf_idx] = 0;
                }
                uint64_t key = mapper[pi_leaf_idx];
                PermutationContainer &inv_leaf = this->inverse_pi_tree->get_leaf_container(this->inverse_leaf_index);

                pi_leaf.set_value(idx, PermutationItem(this->inverse_leaf_index, key));
                inv_leaf.set_value(this->j, PermutationItem(pi_leaf_idx, key));

                mapper[pi_leaf_idx] = key + 1;
                this->i++;
                this->j--;
            }
            void finish()
            {
                if (this->i != (int64_t)this->pi_tree->size())
                {
                    std::cout << "Error: i: " << (this->i) << "!= pi_tree_size: " << this->pi_tree->size() << std::endl;
                    throw std::runtime_error("Error: DeynamicPermutationBuilder");
                }
            }
        };
    }
}
